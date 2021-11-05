// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _KINEMATICS_H
#define _KINEMATICS_H

//RECALL DIMENSIONS OF LENGTH ARE IN MM
#define encoder_counts_per_comp_rot 358.32
//#define encoder_counts_per_comp_rot 360
#define wheel_radius 16 //mm
#define wheel_perimeter 100.57428f//mm
#define centre_to_wheel 43//mm

//i radian is equal to 57.028 encoder_counts

// Class to track robot position.
class Kinematics_c {
  public:
    // Constructor, must exist.
    Kinematics_c() {
      
    } 

    // Use this function to update
    // your kinematics
    void initialise(){
      right_Angular_Velocity = 0;
      left_Angular_Velocity = 0;
      rotation_contribution = 0;
      rotation = 0;
      x_axis_contribution = 0;
      x_axis = 0;
      y_axis = 0;
      xHome = 0;
      yHome = 0;
      thetaHome = 0;
    }
    
    void angular_speed(long left_encoder_Counts, long right_encoder_Counts){
      float revs  = static_cast<float>(right_encoder_Counts)/encoder_counts_per_comp_rot;
      right_Angular_Velocity = revs;
      revs = static_cast<float>(left_encoder_Counts)/encoder_counts_per_comp_rot;
      left_Angular_Velocity = revs;
      calc_rotation_contribution();
      calc_x_axis_contribution();
      calc_y_axis_contribution();
      Serial.print("count_Right: ");
      Serial.print(right_encoder_Counts);
      Serial.print("\n");
      Serial.print("count_Left: ");
      Serial.print(left_encoder_Counts);
      Serial.print("\n");
    }
    
    void xAndy(){
      Serial.print("X and Y is: ");
      Serial.print(x_axis);
      Serial.print(" ");
      Serial.print(y_axis);
      Serial.print("\n");
      Serial.print("THETA: ");
      Serial.print(rotation);
      Serial.print("\n");
    }

    float returnTheta(){
      return rotation;
    }

    float returnThetaHomeRadian(){
      return thetaHome * 2 * PI;
    }
    
    float returnThetaRadian(){
      return rotation * 2 * PI;
    }

    float returnXaxis(){
      return x_axis;
    }

    float returnXhome(){
      return xHome;
    }

    float returnYaxis(){
      return y_axis;
    }

    float returnLeftRotation(){
      return left_Angular_Velocity * encoder_counts_per_comp_rot;
    }
    
    float returnRightRotation(){
      return right_Angular_Velocity * encoder_counts_per_comp_rot;
    }
    
    float distanceCovered(){
      return sqrt((x_axis*x_axis) + (y_axis*y_axis));
    }
    
  private: 
    float right_Angular_Velocity;
    float left_Angular_Velocity;
    float rotation_contribution;
    float rotation;
    float x_axis_contribution;
    float x_axis;
    float y_axis;
    float xHome;
    float yHome;
    float thetaHome;
    void calc_rotation_contribution(){
      rotation_contribution = static_cast<float>(wheel_radius * (left_Angular_Velocity - right_Angular_Velocity))/(2 * centre_to_wheel);
      rotation += rotation_contribution;
    }
    void calc_x_axis_contribution(){
      x_axis_contribution = static_cast<float>(wheel_radius * 2 * PI  * (left_Angular_Velocity + right_Angular_Velocity))/2;
      float x_axis_contribution_cos = x_axis_contribution * cos(rotation);
      x_axis += x_axis_contribution_cos;
      Serial.print("\nThe X_value is: ");
      Serial.print(x_axis);
      Serial.print("\n");
    }
    void calc_y_axis_contribution(){
      float y_axis_contribution_sin = x_axis_contribution * sin(rotation);
      y_axis += y_axis_contribution_sin;
    }
};



#endif
