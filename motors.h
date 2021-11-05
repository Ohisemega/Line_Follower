// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _MOTORS_H
#define _MOTORS_H

#define FWD LOW
#define REV HIGH

#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN 9
#define R_DIR_PIN 15

// Class to operate the motor(s).
class Motors_c {
 public:
  #define P5 5
    // Constructor, must exist.
    Motors_c() {

    } 

    // Use this function to 
    // initialise the pins and 
    // state of your motor(s).
  void initialise() {
    pinMode(L_PWM_PIN, OUTPUT);
    pinMode(R_PWM_PIN, OUTPUT);  
    pinMode(L_DIR_PIN, OUTPUT);
    pinMode(R_DIR_PIN, OUTPUT); 

    // Set initial direction (HIGH/LOW)
    // for the direction pins.
    digitalWrite(L_DIR_PIN, FWD);
    digitalWrite(R_DIR_PIN, FWD);

    // Set initial values for the PWM
    // Pins.
    analogWrite(L_PWM_PIN, 0);
    analogWrite(R_PWM_PIN, 0);
  }
  
  //true boolean for FORWARD direction
  //false boolean for REVERSE direction
  void setRightMotorDirection(bool dir){
    if(dir == true){
      digitalWrite(R_DIR_PIN, FWD);
    }else{
      digitalWrite(R_DIR_PIN, REV);
    }
  }

  //true boolean for FORWARD direction
  //false boolean for REVERSE direction
  void setLeftMotorDirection(bool dir){
    if(dir == true){
      digitalWrite(L_DIR_PIN, FWD);
    }else{
      digitalWrite(L_DIR_PIN, REV);
    }
  }

  void rotate(bool dir){
    setLeftMotorDirection(dir);
    setRightMotorDirection(!dir);
    analogWrite(L_PWM_PIN, 20);
    analogWrite(R_PWM_PIN, 20);
  }

  void fwdDrive(){
    setLeftMotorDirection(true);
    setRightMotorDirection(true);
    analogWrite(L_PWM_PIN, 20);
    analogWrite(R_PWM_PIN, 20);
  }

  void fullRotate(){
    setLeftMotorDirection(FWD);
    setRightMotorDirection(REV);
    analogWrite(L_PWM_PIN, 20);
    analogWrite(R_PWM_PIN, 20);
  }

  void arcTurnLeft(){
    setLeftMotorDirection(true);
    setRightMotorDirection(true);
    analogWrite(L_PWM_PIN, 0);
    analogWrite(R_PWM_PIN, 25);
  }

  void stopMovement(){
    setMotorsPwm(0, 0);
    setLeftMotorDirection(FWD);
    setRightMotorDirection(FWD);
    analogWrite(L_PWM_PIN, 0);
    analogWrite(R_PWM_PIN, 0);
  }
  
  void setMotorsDirection(bool dir1, bool dir2){
    setLeftMotorDirection(dir1);
    setRightMotorDirection(dir2);
  }

  //FALSE boolean to rotate LEFT continuously
  //TRUE boolean direction to rotate RIGHT continuously
  
  void setRightMotorPwm(float pwm){
    Right_PWM = (int)pwm;
  }
  
   void setLeftMotorPwm(float pwm){
    Left_PWM = (int)pwm;
  } 

  void setMotorsPwm(float pwm1, float pwm2){
   setRightMotorPwm(pwm2);
   setLeftMotorPwm(pwm1);
  } 

  void turnLeft(){
    Left_PWM = Left_PWM * 0.5;
    setLeftMotorPwm(Left_PWM);
    powerBothMotors();
  }

  void turnRight(){
    Right_PWM = Right_PWM * 0.5;
    setRightMotorPwm(Right_PWM);
    powerBothMotors();
  }
  
  void powerRightMotor() {
    if(Right_PWM >= 0 && Right_PWM < 256){
      digitalWrite(R_DIR_PIN, FWD);
      analogWrite( R_PWM_PIN, Right_PWM);
    }else if(Right_PWM < 0 && Right_PWM > -256){
      digitalWrite(R_DIR_PIN, REV);
      Right_PWM = (-1) * Right_PWM;
      analogWrite( R_PWM_PIN, Right_PWM);
    }else if(Right_PWM > 255){
      digitalWrite(R_DIR_PIN, FWD);
      Right_PWM = 255;
      analogWrite(R_PWM_PIN, Right_PWM);
    }else{
      digitalWrite(R_DIR_PIN, REV);
      Right_PWM = 255;
    analogWrite(R_PWM_PIN, Right_PWM);
    }
  }

  void powerLeftMotor() {
    if(Left_PWM >= 0 && Left_PWM < 256){
      digitalWrite(L_DIR_PIN, FWD);
      analogWrite( L_PWM_PIN, Left_PWM);
    }else if(Left_PWM < 0 && Left_PWM > -256){
      digitalWrite(L_DIR_PIN, REV);
      Left_PWM = (-1) * Left_PWM;
      analogWrite( L_PWM_PIN, Left_PWM);
    }else if(L_PWM_PIN > 255){
      digitalWrite(L_DIR_PIN, FWD);
      Left_PWM = 255;
      analogWrite( L_PWM_PIN, Left_PWM);
    }else{
      digitalWrite(L_DIR_PIN, REV);
      Left_PWM = 255;
      analogWrite( L_PWM_PIN, Left_PWM);
    }
  }

  void powerBothMotors(){
    powerLeftMotor();
    powerRightMotor();
  }
 private:
  int Left_PWM;
  int Right_PWM;
};

#endif
