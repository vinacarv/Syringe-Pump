
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
float rpm = 1;
float realRevSteps = gearRatio * stepsPerRevolution; //take into account the gear ratio
float realRpm = rpm*gearRatio; //rpm of rotor (rotor RPM * gearRatio, input from Processing)
unsigned long timer = 0; //timer
unsigned long last_timer = 0; //timer last state
int dir = 1;
int serbyte;
char nextMsg;

String hsStart = "0010";
int hsSpeed = 0020;
int hsDir = 0030;
int hsPause = 0040;
int hsTime = 0050;

int start0 = 0011;
int start1 = 0012;
int speedSet = 0021;
int dir0 = 0031;
int dir1 = 0032;
int pause0 = 0041;
int pause1 = 0042;
int timeSet = 0051;

// initialize the stepper library on pins 8 through 11:
Vinstepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  myStepper.setSpeed(realRpm); // set the speed at rpm calculated by var realRpm:
  Serial.begin(9600);  // initialize the serial port:
}

void loop() {

  //==================== SERIAL READING AND MESSAGE IDENTIFYING ====================

  if (Serial.available() > 0) { 
    //serbyte = Serial.read();
    String _serbyte = Serial.readString();
    
    Serial.println (_serbyte);
    serbyte = Serial.readString().toInt();  //Reads the string of 4 chars in the buffer and converts it to Int
    //String message = Serial.readString();  //waits for serial data available
    //serbyte = message.toInt();
    Serial.println(serbyte);
    //Serial.flush();           //flushes serial buffer
    if (serbyte != -1) {      //checks if serbyte is not null
      Serial.println(9999);     //prints confirmatin of message received -REVIEW MESSAGE FOR ONE BYTE - ASCII 6 = ACK
      if (serbyte == 83 || serbyte == 'S') {    //check for Handshake // Handshake 83 = S = start/stop boolean
        nextMsg = 'S';        // S for start/stop
        Serial.println (hsStart);
      } 
      else if (serbyte == 82) {        //check for Handshake // Handshake 68 = D = direction boolean
        nextMsg = 'R';                    // R for RPM
        Serial.println (hsSpeed);
      } 
      else  if (serbyte == 68) {     //check for Handshake // Handshake 68 = D = direction boolean
        nextMsg = 'D';                  // D for DIRECTION
        Serial.println (hsDir);
      } 
      else  if (serbyte == 80) {     //check for Handshake // Handshake 80 = P = pause boolean
        nextMsg = 'P';                 //P for PAUSE
        Serial.println (hsPause);
      } 
      else  if (serbyte == 84) {     //check for Handshake // Handshake 84 = T = time int
        nextMsg = 'T';                 //T for TIME
        Serial.println (hsTime);
      } 
      else {
        serbyte = serbyte - '0';
        decode(serbyte);          //Process any data that is not a handshake
      }
    }
  }
}

//++++++++ DECODING/APPLYING FUNCTION

void decode(int msg) {

  if (nextMsg == 'R') {    //check what handshake it is
    myStepper.setSpeed(serbyte);
    Serial.print(speedSet);
    sendMsg(serbyte);    //send a message back for debugging
  } 
  else if (nextMsg == 'D') {    //check what handshake it is
    myStepper.direction = serbyte;
    if (myStepper.direction == 1)    Serial.println(dir1);
    if (myStepper.direction == 0)    Serial.println(dir0);    //send a message back for debugging
  } 
  else if (nextMsg == 'T') {    //check what handshake it is
    realRevSteps = serbyte;       //ou timer = serbyte;
    Serial.println(timeSet);     //send a message back for debugging
    sendMsg(serbyte);  
  } 
  else if (nextMsg == 'S') {    //check what handshake it is
    if (serbyte == 1) {
      myStepper.step(realRevSteps); //before starting sets how long the motor will run
      myStepper.running = 1;         //turns motor on
      Serial.println(start1);          //send a message back for debugging
    } 
    else if (serbyte == 0) {
      myStepper.running = 0;       //if already running, turns motor off
      myStepper.step(0);  //resets the steps_to_move in the stepper object step() function
      Serial.println(start0);          //send a message back for debugging
    }
  } 
  else if (nextMsg == 'P') {    //check what handshake it is
    if (serbyte == 1) {
      myStepper.running = 0;
      Serial.println(pause1);
      Serial.flush();
    } 
    else if  (serbyte == 0) {
      myStepper.running = 1;
      Serial.println(pause0);
      Serial.flush();
    }
  }

  nextMsg = 'x'; //char variable cannot be empty, so we use 'x' to define the lack of type specification

}

void sendMsg(int msg){
  /* Processing uses Serial.buffer(4) to read the messages it receives,
   * meaning that messages of 4 bytes long should be sent.
   * Arduino sends integers as Strings, so to ensure 4 characters are sent
   * for each message, I check the size of the integer to send and add zeroes
   * as needed (zeroes ensure that the received message can easily be cast
   * to an integer by Processing, which would not be the case with other, non-numerical, characters)
   */
  if(Serial.available() > 0) Serial.flush();
  if(msg < 1000) Serial.print(0);
  if(msg < 100) Serial.print(0);
  if(msg < 10) Serial.print(0);
  Serial.print(msg);
}





