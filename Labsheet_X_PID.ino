#include "motors.h"
#include "linesensor.h"
#include "encoders.h"
#include "kinematics.h"
#include "pid.h"

// Global definition of the time interval
#define LINE_SENSOR_UPDATE 100
#define KINEMATICS_UPDATE 10
#define PI_UPDATE 20
#define LINE_FOLLOW 0
#define ROTATION_VELOCITY_LAPSE 50
#define HALF_ROTATE_TIME 1750
#define FWD_SPD 22
#define GAIN_K 7

//FINITE STATE DETAILS
#define STATE_UPDATE 50
unsigned long state_update;
float angle = 0;
#define INITIAL_STATE 0
#define DRIVE_FORWARD_STATE 1
#define LINE_FOUND 2
#define JOIN_LINE 3
#define END_OF_LINE 7
#define BUZZER 6
int state;
bool value;
//FINITE STATE DETAILS

//velocity estimation variables and functions
void calculateWheelsVel();
unsigned long elapsed_t = 0;
unsigned long current_ts;
unsigned long state_ts = 0;
unsigned long ks_ts = 0;
double rotation_velocity1 = 0;
double rotation_velocity2 = 0; 
float angleRad;
double feedback_signal1 = 0;
double feedback_signal2 = 0;
double heading_signal = 0;
//estimation variables


unsigned long ls_ts = 0;
double pwm = 0;
double rpwm = 0;
double lpwm = 0;
angleRad = (float)(PI/2);

Motors_c motors;
LineSensor_c lineSensors;
Kinematics_c kinematics;
PID_c heading_controller;
PID_c pid1;
PID_c pid2;

void calibration(){
  motors.fullRotate();
  lineSensors.Calibrate_Line_Sensors();
  motors.stopMovement();
}

void setup() {
  // put your setup code here, to run once:
  pid1.initialise(1, 0.00004, 2);
  pid2.initialise(1, 0.00004, 2);
  heading_controller.initialise(7, 0, 3);
  motors.initialise();
  lineSensors.initialise();
  calibration();
  setupEncoder0();
  setupEncoder1();
  kinematics.initialise();
  state = INITIAL_STATE;
  delay(3000);
}

void loop(){
//IMPLEMENT FINAL STATE
  current_ts = millis();
  elapsed_t = current_ts - state_ts;
  if(elapsed_t >= STATE_UPDATE){
        updateState(state);
        state_ts = millis();
  }
  elapsed_t = current_ts - ks_ts;
  if(elapsed_t >= KINEMATICS_UPDATE){
    kinematics.angular_speed(left_wheel_count, right_wheel_count);
    if(state == DRIVE_FORWARD_STATE) {
      Serial.print("\nDRIVE FWD state: ");
      Serial.println( state );
      motors.fwdDrive();    
    } else if(state == LINE_FOUND) {
      Serial.print("\nLINE FOUND state: ");
      Serial.println( state );
      joinLine();
    } else if(state == JOIN_LINE){
      Serial.print("\nJOIN LINE state: ");
      Serial.println( state );
      followLine();
    }else if(state == END_OF_LINE){
      Serial.print("\nEND OF LINE state: ");
      Serial.println( state );
      goHome();
    }else{
      motors.stopMovement();
      Serial.print("System Error, Unknown state: ");
      Serial.println( state );
    }
    //update state variable on everyIteration
    right_wheel_count = 0;
    left_wheel_count = 0;
    
    ks_ts = millis();
  }  
}



void updateState(int& stateVariable){
  if(stateVariable == INITIAL_STATE){
    stateVariable = DRIVE_FORWARD_STATE;
  }else if(stateVariable == DRIVE_FORWARD_STATE){
      motors.fwdDrive(); 
      value = lineSensors.checkForLine();
      if(value == true){
        stateVariable = LINE_FOUND;
        motors.stopMovement();   
      }
  }else if(stateVariable  == LINE_FOUND){
    stateVariable = JOIN_LINE;
  }else if(stateVariable == JOIN_LINE){
      value = lineSensors.checkForLine();
    if(value == false && kinematics.returnXaxis() >= 1450){
      followLine();
      delay(1500);
      value = lineSensors.checkForLine();
      if(value == false){
          motors.stopMovement();
         stateVariable = END_OF_LINE;
         angle = atan2(kinematics.returnYaxis(), kinematics.returnXaxis());
         kinematics.initialise();
      }
    }else if(value == false){
        motors.fwdDrive();
     }
   }
}

void joinLine(){
  motors.stopMovement();
  motors.arcTurnLeft();
  delay(1200);
  motors.stopMovement();
  motors.fwdDrive(); 
  delay(2000);
  motors.stopMovement();
  motors.fullRotate();
  delay(HALF_ROTATE_TIME);
  motors.stopMovement();
  motors.fwdDrive(); 
  delay(1000);
  motors.stopMovement();
}

void followLine(){
      lineSensors.Read_Line_Sensor();
      pwm = lineSensors.getErrorLine();//btween -1 and 1
      pwm *= GAIN_K;
      Serial.print("PWM: ");
      Serial.print(pwm);
      heading_signal = heading_controller.update_pid(LINE_FOLLOW, pwm, elapsed_t);
      feedback_signal1 = pid1.update_pid(heading_signal, rotation_velocity1, elapsed_t);
      feedback_signal2 = pid2.update_pid(heading_signal, rotation_velocity2, elapsed_t);
      lpwm = FWD_SPD + feedback_signal2;
      rpwm = FWD_SPD - feedback_signal1;
      motors.setMotorsPwm(lpwm, rpwm);
      motors.powerBothMotors();
      calculateWheelsVel();
}

void goHome(){
  unsigned long home_time = 0;
  Serial.print("angle: ");
  Serial.print(angle);
  while(abs(kinematics.returnThetaRadian()) < angleRad ){
    Serial.print("angle: ");
    Serial.print(angle);
    kinematics.xAndy();
    motors.rotate((angle/abs(angle)) * true);
    delay(500);
    kinematics.angular_speed(left_wheel_count, right_wheel_count);
    right_wheel_count = 0;
    left_wheel_count = 0;
  }
  motors.stopMovement();
  delay(200);
  home_time = millis();
  motors.setMotorsPwm(20, 21);
  motors.setMotorsDirection(true, true);
  while(millis() - home_time < 13000){
    motors.powerBothMotors();
  }
  while(1){
    tone(BUZZER, 8, 500);
    motors.stopMovement();
  }
}

//void angularRotation(float ang){
//  if()
//}

void calculateWheelsVel(){
    rotation_velocity1 = static_cast<double>(kinematics.returnLeftRotation())/((double)(elapsed_t)/10);
    rotation_velocity2 = static_cast<double>(kinematics.returnRightRotation())/((double)(elapsed_t)/10);
}
