
/*

 This is the arduino end of the kit created for operation of an open-source syringe
 pump designed and built by me, Vinicius. The package also includes a full-stepping library (Vinstepper.h)
 created by modifing the original Stepper library that ships with the Arduino IDE.
 This version is specific for use with the 28BYJ-48 5V stepper motor and ULN2003 driver.
 
 28BYJ-48 Data:
 Steps per revolution: 32 (Wave drive) or 64 (full-stepping, which is used here)
 Gear Ratio: 63.683950617
 Steps per revolution of outer shaft: 2037.886419753
 Max RPM of motor: aprox. 890 RPM
 Max RPM of outer shaft: 14 RPM
 
 Based on "Stepper Motor Control - one revolution" sketch and Stepper v0.4 library, by Tom Igoe (2009)
 
 Created 5 May 2015
 by Vinicius Carvalho
 
 */

#include <Vinstepper.h>

float gearRatio = 63.683950617;
const int stepsPerRevolution = 64;  // number of steps/rev in drive mode used (wave drive = 32, full-stepping = 64)
float realRevSteps = gearRatio * stepsPerRevolution; //take into account the gear ratio
float realRpm = 0; //rpm of rotor (rotor RPM * gearRatio, input from Processing)
unsigned long timer = 0; //timer
unsigned long last_timer = 0; //timer last state
int dir = 1;
int serbyte;
//char handshake;


// initialize the stepper library on pins 8 through 11:
Vinstepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  myStepper.setSpeed(realRpm); // set the speed at rpm calculated by var realRpm:
  Serial.begin(9600);  // initialize the serial port:
}

void loop() {
  
  //==================== SERIAL READING AND MESSAGE IDENTIFYING ====================
  
  if (Serial.available > 0){  //waits for serial data available
    serbyte = Serial.read();  //reads incoming serial byte and assigns it to serbyte 
    Serial.flush();           //flushes serial buffer
    if (serbyte != -1){       //checks if serbyte is not null
      Serial.print(9999);     //prints confirmatin of message received -REVIEW MESSAGE FOR ONE BYTE - ASCII 6 = ACK
      if (serbyte == 83){     //check for Handshake // Handshake 83 = S = start/stop boolean
        nextMsg = 'S';        // S for start/stop
        Serial.print ("Receiving START/STOP command");
      } else if (serbyte == 82){         //check for Handshake // Handshake 68 = D = direction boolean
        nextMsg = 'R'                    // R for RPM
        Serial.print ("Receiving SET SPEED command");
      } else  if (serbyte == 68){      //check for Handshake // Handshake 68 = D = direction boolean
        nextMsg = 'D';                  // D for DIRECTION
        Serial.print ("Receiving DIRECTION command");
      } else  if (serbyte == 80){      //check for Handshake // Handshake 80 = P = pause boolean
        nextMsg = 'P';                 //P for PAUSE
        Serial.print ("Receiving PAUSE command");
      } else  if (serbyte == 84){      //check for Handshake // Handshake 84 = T = time int
        nextMsg = 'T';                 //T for TIME
        Serial.print ("Receiving OPERATING TIME command");
      } else decode(serbyte);          //Process any data that is not a handshake
    }
  } 
     
  //==================== MOTOR OPERATION ====================   
    
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(realRevSteps);

  //debug: timing of one rev
  timer = millis() - last_timer;
  last_timer = millis();
  Serial.println(timer / 1000.00);

  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-realRevSteps);

  //debug: timing of one rev
  timer = millis() - last_timer;
  last_timer = millis();
  Serial.println(timer / 1000.00);

  delay(500);
  
}

  //++++++++ DECODING/APPLYING FUNCTION
  
void decode(int msg){
  

  if(nextMsg == 'R'){      //check command type and command value
    myStepper.setSpeed(long(serbyte);
    Serial.print();          //send a message back for debugging
  }else if(nextMsg == 'D'){      //check command type and command value
    myStepper.direction = serbyte;
    Serial.print();          //send a message back for debugging
  }else if(nextMsg == 'T'){      //check command type and command value
    timer = serbyte;
    Serial.print();          //send a message back for debugging
  }else if(nextMsg == 'S'){      //check command type and command value
    if(myStepper.running == 0) {
      myStepper.step(realRevSteps); //before starting sets how long the motor will run
      running = 1;         //turns motor on
      else {
        running = 0;       //if already running, turns motor off
        myStepper.step(0);  //resets the steps_to_move in the stepper object step() function
      }
    Serial.print();          //send a message back for debugging
  }else if(nextMsg == 'P'){      //check command type and command value
    running != running;
    if(myStepper.running == 1) {
      myStepper.step(realRpm)
      Serial.print();          //send a message back for debugging
    }
  } else if(nextMsg == 'b'){
    //do what you would need to do with a message of this type
    sendMsg(120);         //send a message back for debugging
  }
  nextMsg = 'x'; //char variable cannot be empty, so we use 'x' to define the lack of type specification
}

