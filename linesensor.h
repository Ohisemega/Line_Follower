// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LS_EXT_LEFT_IN_PIN 12
#define LS_LEFT_IN_PIN 18
#define LS_CENTRE_IN_PIN 20
#define LS_RIGHT_IN_PIN 21
#define LS_EXT_RIGHT_IN_PIN 22
#define GAIN_CONST 150
#define NB_LS_PIN 5

#define EMIT_PIN 11
#define TIME_OUT 4000
#define CALIBRATION_TIME 3700000 
#define BLACK_LINE 1700
// Class to operate the linesensor(s).
class LineSensor_c {
  public:
  
    // Constructor, must exist.
    LineSensor_c() {

    }

    void initialise(){
      for(int i = 0; i < NB_LS_PIN; ++i) pinMode(ls_pin[i], INPUT);
      pinMode(EMIT_PIN, OUTPUT);
      digitalWrite(EMIT_PIN, HIGH);
      Serial.println("***The setup is Complete");
      delay(2000);
    }

    void Calibrate_Line_Sensors(){
      unsigned long temp_elapsed_time;
      PT_Charge();
      while(done == false){
        for(int Which_Sensor = 0; Which_Sensor < NB_LS_PIN; ++Which_Sensor){
          temp_elapsed_time = micros() - start_time;         
          if(digitalRead(ls_pin[Which_Sensor]) == LOW || temp_elapsed_time > TIME_OUT){
            if(max_elapsed_time[Which_Sensor] < temp_elapsed_time && temp_elapsed_time < TIME_OUT) max_elapsed_time[Which_Sensor] = temp_elapsed_time;
            if(min_elapsed_time[Which_Sensor] > temp_elapsed_time && temp_elapsed_time > 550) min_elapsed_time[Which_Sensor] = temp_elapsed_time;
          }
        }
        if(temp_elapsed_time > CALIBRATION_TIME){
          done = true;     
          calculateScalingFactor();
        }
      }
    }

    bool checkForLine(){
      PT_Charge();
      unsigned long temp_elapsed_time;
      while(1){    
        for(int Which_Sensor = 0; Which_Sensor < NB_LS_PIN; ++Which_Sensor){
          temp_elapsed_time = micros() - start_time;         
          if((digitalRead(ls_pin[Which_Sensor]) == LOW || temp_elapsed_time > TIME_OUT) && !read_end[Which_Sensor]){
            elapsed_time[Which_Sensor] = temp_elapsed_time;
            read_end[Which_Sensor] = true;
          }
        }
        if(read_end[0] && read_end[1] && read_end[2] && read_end[3] && read_end[4]){
          if(elapsed_time[0] >= BLACK_LINE || elapsed_time[1] >= BLACK_LINE || elapsed_time[2] >=  BLACK_LINE || elapsed_time[3] >= BLACK_LINE || elapsed_time[4] >= BLACK_LINE){
            done = true;
          }
          break;
        }
      }
      return done;
    }
    
    void Read_Line_Sensor(){
      PT_Charge();
      unsigned long temp_elapsed_time;
      while(done == false){    
        for(int Which_Sensor = 0; Which_Sensor < NB_LS_PIN; ++Which_Sensor){
          temp_elapsed_time = micros() - start_time;         
          if((digitalRead(ls_pin[Which_Sensor]) == LOW || temp_elapsed_time > TIME_OUT) && !read_end[Which_Sensor]){
            elapsed_time[Which_Sensor] = temp_elapsed_time;
            read_end[Which_Sensor] = true;
          }
        }
        if(elapsed_time[0] != 0 && elapsed_time[1] !=  0 && elapsed_time[2] != 0 && elapsed_time[3] != 0 && elapsed_time[4] != 0){
          done = true;
//          Display_Lapsed_Time();
          getSensorCalibrationValue();
        }
      }
    }
        
    void getSensorCalibrationValue(){
      for(int i = 0; i < NB_LS_PIN; ++i){
        if(elapsed_time[i] < min_elapsed_time[i]){
          min_elapsed_time[i] = elapsed_time[i];
          calculateScalingFactor();
        }
        sensors_measured_condition[i] = (elapsed_time[i] - min_elapsed_time[i]) * sensors_calibration_factor[i];
      }
    }

    double getErrorLine(){
      return ((sensors_measured_condition[0] + sensors_measured_condition[1])  - (sensors_measured_condition[3] + sensors_measured_condition[4]));
    }
    
    unsigned long Sensor_Time_Elapsed(int sensor){
      return elapsed_time[sensor];
    }

    void Display_Lapsed_Time(){
      Serial.print("\n\nFINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      Serial.print("\nLine sensor FINAL VALUES!: ");
      Serial.print(elapsed_time[0]);
      Serial.print(", ");
      Serial.print(elapsed_time[1]);
      Serial.print(", ");
      Serial.print(elapsed_time[2]);
      Serial.print("\n");
    }
    
  private:
    int ls_pin[NB_LS_PIN] = {LS_EXT_LEFT_IN_PIN, LS_LEFT_IN_PIN, LS_CENTRE_IN_PIN, LS_RIGHT_IN_PIN, LS_EXT_RIGHT_IN_PIN};
    bool read_end[NB_LS_PIN];
    bool done;
    unsigned long start_time; //t_1
    unsigned long elapsed_time[NB_LS_PIN] = { 0 }; //to store elapsed time
    unsigned long max_elapsed_time[NB_LS_PIN] = { 0 }; //to store max discharge time duting Calibration
    unsigned long min_elapsed_time[NB_LS_PIN] = { TIME_OUT, TIME_OUT, TIME_OUT, TIME_OUT, TIME_OUT }; //to store min discharge time during Calibration
    double sensors_calibration_factor[NB_LS_PIN] = { 0 };
    float sensors_measured_condition[NB_LS_PIN] = { 0 };
    float calibration_factor;
    void Start_Time(){
      start_time = micros(); 
    }

    //function to perform charge and start discharge time
    void PT_Charge(){
      for(int k = 0; k < NB_LS_PIN; ++k) elapsed_time[k] = 0;
      for(int k = 0; k < NB_LS_PIN; ++k) read_end[k] = false;
      for(int i = 0; i < NB_LS_PIN; ++i) pinMode(ls_pin[i], OUTPUT);
      for(int j = 0; j < NB_LS_PIN; ++j) digitalWrite(ls_pin[j], HIGH);
      delayMicroseconds(30);
      done = false;
      for(int i = 0; i < NB_LS_PIN; ++i) pinMode(ls_pin[i], INPUT);
      Serial.print("\nDone Charging and starts Discharging\n");
      unsigned long temp_elapsed_time;
      Start_Time();
    }

    void calculateScalingFactor(){
      for(int i = 0; i < NB_LS_PIN; ++i){
        sensors_calibration_factor[i] = ((double)1 / (max_elapsed_time[i] - min_elapsed_time[i]));
      }
    }
};
#endif
