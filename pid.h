// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _PID_H
#define _PID_H

// Class to contain generic PID algorithm.
class PID_c {
  public:
  
    // Constructor, must exist.
    PID_c() {

    } 

    void initialise(double pgain, double igain, double dgain){
      pgain_const = pgain;
      igain_const = igain;
      dgain_const = dgain;
      prev_error = 0;
      iterm = 0;
      p_term = 0;
      i_term = 0;
    }
    double update_pid(double demand, double measurement, unsigned long timeLapse){
      double error_signal = demand - measurement;
      p_term = pgain_const * error_signal;
      iterm = static_cast<double> (igain_const * error_signal * timeLapse);
      i_term += iterm;
      d_term = static_cast<double>(dgain_const * ((error_signal - prev_error)/timeLapse));
      prev_error = error_signal;
      return p_term + i_term + d_term;
    }
    
    double iterm;
  private:
    double p_term;
    double i_term;
    double d_term;
    double prev_error;
    double pgain_const;
    double igain_const;
    double dgain_const;
};



#endif
