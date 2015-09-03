/*
  Vinstepper.h - - Stepper library for Wiring/Arduino - Version 0.1
  
  Original library     (0.1) by Tom Igoe.
  Full-step modifications (0.1) by Vinicius Carvalho

  Drives a unipolar or bipolar stepper motor using 4 wires in full-step drive mode

  The sequence of control signals for 4 control wires is as follows:

  Step C0 C1 C2 C3
     1  0  0  0  1
     2  0  0  1  1
     3  0  0  1  0
     4  0  1  1  0
     5  0  1  0  0
     6  1  1  0  0
     7  1  0  0  0
     8  1  0  0  1
defaut  0  0  0  0

 
*/

// ensure this library description is only included once
#ifndef Vinstepper_h
#define Vinstepper_h

// library interface description
class Vinstepper {
  public:
    // constructors:
    //Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2);
    Vinstepper(int number_of_steps, int IN1, int IN2, int IN3, int IN4);

    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);

    int running;
    int direction;        // Direction of rotation
    int speed;          // Speed in RPMs

  private:
    void stepMotor(int this_step);
    unsigned long step_delay;    // delay between steps, in ms, based on speed
    int number_of_steps;      // total number of steps this motor takes to make a revolution
    int pin_count;        // whether you're driving the motor with 2 or 4 pins
    int step_number;        // which step the motor is on
    
    
    // motor pin numbers:
    int IN1;
    int IN2;
    int IN3;
    int IN4;
    
    long last_step_time;      // time stamp in ms of when the last step was taken
};

#endif

