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



#include "Arduino.h"
#include "Vinstepper.h"

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 
Stepper::Stepper(int number_of_steps, int IN1, int IN2)
{
  this->step_number = 0;      // which step the motor is on
  this->speed = 0;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps for this motor
  
  // Arduino pins for the motor control connection:
  this->IN1 = IN1;
  this->IN2 = IN2;

  // setup the pins on the microcontroller:
  pinMode(this->IN1, OUTPUT);
  pinMode(this->IN2, OUTPUT);
  
  // When there are only 2 pins, set the other two to 0:
  this->IN3 = 0;
  this->IN4 = 0;
  
  // pin_count is used by the stepMotor() method:
  this->pin_count = 2;
}
*/

//=======================================================================================================

/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */

 Vinstepper::Vinstepper(int number_of_steps, int IN1, int IN2, int IN3, int IN4)
 {
  this->step_number = 0;      // which step the motor is on
  this->speed = 0;        // the motor speed, in revolutions per minute
  this->direction = 0;      // motor direction
  this->running = 0; 
  this->last_step_time = 0;    // time stamp in ms of the last step taken
  this->number_of_steps = number_of_steps;    // total number of steps per revolution for this motor
  
  // Arduino pins for the motor control connection:
  this->IN1 = IN1;
  this->IN2 = IN2;
  this->IN3 = IN3;
  this->IN4 = IN4;

  // setup the pins on the microcontroller:
  pinMode(this->IN1, OUTPUT);
  pinMode(this->IN2, OUTPUT);
  pinMode(this->IN3, OUTPUT);
  pinMode(this->IN4, OUTPUT);

  // pin_count is used by the stepMotor() method:  
  this->pin_count = 4;  
}

/*
  Sets the speed in revs per minute

*/
  void Vinstepper::setSpeed(long whatSpeed)
  {
    this->step_delay = 60L * 1000L / this->number_of_steps / whatSpeed;
  }

/*
  Moves the motor steps_to_move steps.  If the number is negative, 
   the motor moves in the reverse direction.
 */
   void Vinstepper::step(int steps_to_move)
   {  
  int steps_left = abs(steps_to_move);  // how many steps to take
  
  // determine direction based on whether steps_to_mode is + or -:
  //if (steps_to_move > 0) {this->direction = 1;}
  //if (steps_to_move < 0) {this->direction = 0;}

  if (this->running == 1){

    if(this->direction == 1){
      while(steps_left > 0) {     // decrement the number of steps, moving one step each time:
        if (millis() - this->last_step_time >= this->step_delay) {   // move only if the appropriate delay has passed:
           this->last_step_time = millis();  // get the timeStamp of when you stepped:
           this->step_number++;  // increment or decrement the step number,depending on direction
           if (this->step_number == this->number_of_steps) {
            this->step_number = 0; //resets step number to continue spinning
          }
        }
      }
    } else if(this->direction == 0) {
      if (this->step_number == 0) {
        this->step_number = this->number_of_steps;
      }
      this->step_number--;
    }

      steps_left--;  // decrement the steps left:
      stepMotor(this->step_number % 8); // step the motor to step number 0, 1, 2, or 3:
  } else if (this-> running == 0) {         //if var running is not 1 (true) 
      stepMotor(9);//do nothing
    }
  }


/*
 * Moves the motor forward or backwards.
 */
 void Vinstepper::stepMotor(int thisStep)
 {
  /*
  if (this->pin_count == 2) {
    switch (thisStep) {
      case 0: // 01 
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      break;
      case 1: // 11 
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      break;
      case 2: // 10
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      break;
      case 3: // 00 
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      break;
    } 
  }
  */
  
  if (this->pin_count == 4) {
    switch (thisStep) {
      case 0:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
      case 1:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH);
      break;
      case 2:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
      case 3:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
      case 4:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
      case 5:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
      case 6:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
      case 7:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
      default:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    } 
  }
}

/*
  version() returns the version of the library:
*/
  int Vinstepper::version(void)
  {
    return 1;
  }
