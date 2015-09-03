import processing.serial.*;

float gearRatio = 63.683950617;
int stepsPerRevolution = 64;  // number of steps/rev in drive mode used (wave drive = 32, full-stepping = 64)
float realRevSteps = gearRatio * stepsPerRevolution; //take into account the gear ratio
float rpm = 2; //rpm of motor shaft (rotor RPM * gearRatio, calculated in Processing)
//float realRpm = rpm * gearRatio; //rpm of outer shaft


Serial myPort;
PFont font;



int MSG_TYPE_S = 'S';
int MSG_TYPE_R = 'R';
int MSG_TYPE_D = 'D';
int MSG_TYPE_P = 'P';
int MSG_TYPE_T = 'T';

//handshakes with arduino
int msgArrived = 9999;
int hsStart = 0010;
int hsSpeed = 0020;
int hsDir = 0030;
int hsPause = 0040;
int hsTime = 0050;

//codes for state indications from arduino
int start0 = 0011;
int start1 = 0012;
int speedSet = 0021;
int dir0 = 0031;
int dir1 = 0032;
int pause0 = 0041;
int pause1 = 0042;
int timeSet = 0051;

//Message manaintance
int msgQueue[];             //the message queue
boolean msgLock;            //message lock, active until last message is confirmed
int lstMsg;                 //last message sent
int lstMsgType;             //if last message wasn't a type identifier, type is specified here
int msgTimer;               //time since last message was sent
int MSG_TIME_OUT = 2000;    //timeout in msec after which a non-received message is sent again
int ttl = 2;                //time to live - # of times a message is resent before being put at the end of the queue

//GUI
int qtyButtons = 0;         //count of how many buttons exist in the sketch
//Button[] buttons = new Button[5];    //array to index the buttons
ArrayList<Button> buttons = new ArrayList<Button>();
PVector mPos;                //mousePosition vector
PImage bg;                   //Background image                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             


void setup() {
  size(800, 400);
  background(0);
  

  //GUI

  font = createFont("Verdana", 14);
  imageMode(CENTER);
  rectMode(CENTER);
  bg = loadImage("bg_prot.png");
  
  buttons.add(new Button(x, y, size, state, ref)); //ADICIONAR TODOS OS BOTOES NECESSÁRIOS E LINKAR REFS COM ARDUINO
  
  //inserir construtores de botoes individuais aqui, para atualizar o qtyButtons usado abaixo
  

  


  //SERIAL
  msgQueue = new int[0];

  println(Serial.list());
  myPort = new Serial(this, Serial.list()[Serial.list().length - 1], 9600); //the highest connected COM port is always my Arduino
  myPort.buffer(4); //buffer 4 bytes of data before calling serialEvent()
}




void draw() {
  background(0);
  image (bg, width/2, height/2); //Display the BG right from the corner 




  textFont(font, 14);
  text("Press S for message of type S\nPress 1 to send a value 1", 25, 25);
  parseQueue();
}




void keyPressed() {
  if (int(key) ==  'S') {
    queueMessage(MSG_TYPE_S); //send a message that specifies the next message as being of type A
    //queueMessage('S'); //just a random numerical message
  } else if (int(key) == '1') {
    //queueMessage(MSG_TYPE_B); //send a message that specifies the next message as being of type B
    queueMessage('1'); //just a random numerical message
  }
}


//check for clicks and determine what button is being clicked. 
void mouseClicked() {
  mPos.set(mouseX, mouseY, 0);
  for (int i = 0; i < buttons.size(); i++) {      //run through array and chegck each button distance to mouse 
    Button b = buttons.get(i);
    if (mPos.dist(b.bPos) <= b.size/2) {    
      if (b.state == 1) b.state = 0; 
      else if (b.state == 0) b.state = 1;       //change state of button
      b.notify();                                //notify arduino of change in state
    } else {
      println("no button was clicked");
    }
  }
}

/* serialEvent(Serial myPort)
 * called when the amount of bytes specified in myPort.buffer()
 * have been transmitted, converts serial message to integer,
 * then sets this value in the chair object
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void serialEvent(Serial myPort) {
  if (myPort.available() > 0) {
    //String message = myPort.readString(); //read serial buffer
    //int msg = int(message); //convert message to integer
    int msg = myPort.read();
    //println(msg);
    //myPort.clear(); //clear whatever might be left in the serial buffer

    if (msg == msgArrived) {
      println("...x");
      msgLock = false;
    } else if (msg == hsStart) {
      println("Receiving START/STOP command");
      msgLock = false;
    } else if (msg == 0020) {
      println("Receiving SET SPEED command");
      msgLock = false;
    } else if (msg == 0030) {
      println("Receiving DIRECTION command");
      msgLock = false;
    } else if (msg == 0040) {
      println("Receiving PAUSE command");
      msgLock = false;
    } else if (msg == 0050) {
      println("Receiving OPERATING TIME command");
      msgLock = false;
    } else if (msg == 0011) {
      println("Pump stoped");
      msgLock = false;
    } else if (msg == 0012) {
      println("Pump started");
      msgLock = false;
    } else if (msg == 0021) {
      println("Rotational Speed Set");
      delay(10);
      // if (myPort.read > 0) {    
      //  String s = myPort.readString();
      //   println(s);
      // }
      msgLock = false;
    } else if (msg == 0031) {
      println("Counter-Clockwise rotation selected");
      msgLock = false;
    } else if (msg == 0032) {
      println("Clockwise rotation selected");
      msgLock = false;
    } else if (msg == 0041) {
      println("Pump unpaused");
      msgLock = false;
    } else if (msg == 0042) {
      println("Pump paused");
      msgLock = false;
    } else if (msg == 0051) {
      println("Time has been set");
      delay(10);
      // if (myPort.read > 0)     println(myPort.readString());
      msgLock = false;
    } else {
      println(int(msg));
    }
  }
}

private void writeSerial(int msg) {
  if (myPort.available() > 0) myPort.clear(); //empty serial buffer before sending
  myPort.write(msg);
}

public void queueMessage(int msg) {
  msgQueue = append(msgQueue, msg);
}

private void parseQueue() {

  if (msgQueue.length > 0 && !msgLock)
  {
    msgLock = true; //lock queue, preventing new messages from being sent
    lstMsg = msgQueue[0]; //queue the first message on the stack
    if (lstMsg > 10) lstMsgType = lstMsg; //message greater than 10 specify message types
    writeSerial(lstMsg);
    msgTimer = millis();
    println("writing message: " + lstMsg);
    msgQueue = subset(msgQueue, 1);
  }
  /* a timeout prevents unreceived messages from clogging up the system
   * and effectively stop the message flow. after the timeout,
   * a message is resent until it is received and confirmed by the Arduino
   */
  else if (msgLock && millis() - msgTimer > MSG_TIME_OUT) {
    if (ttl > 0) {
      writeSerial(lstMsg);
      msgTimer = millis();
      println("resending unconfirmed message...");
      ttl--;
    } else {
      //in case of message type identifier, requeue this and next message
      if (lstMsg > 10) {
        queueMessage(lstMsg);
        queueMessage(msgQueue[0]);
        msgQueue = subset(msgQueue, 1);
      }
      //in case of regular message, queue last used type identifier
      // and this message
      else {
        queueMessage(lstMsgType);
        queueMessage(lstMsg);
      }  
      //resume parsing the queue
      msgLock = false;
      ttl = 2;
    }
  }
}

