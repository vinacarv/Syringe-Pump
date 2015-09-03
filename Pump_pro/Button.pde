class Button {
  
  PVector bPos;
    int size, state, ref;   //position and size and  state (1 or 0);
  color active = color(20, 150, 255);      //blue color while on
  color inactive = color(127, 140, 150);   //bluish grey while off


  Button (float _px, float _py, int _s, int _st, int _ref) {
    bPos.set(_px, _py, 0);
    size = _s;
    state = _st; //0 or 1
    ref = _ref;

    qtyButtons++; //raise the button count by one when is created
    //buttons.append()  
  }

  void run() {
    display();
  }


  void display() {
    if (state == 1) {
      strokeWeight(size/2);
      stroke(active, 100);  
      fill(active);
    } else if (state == 0) {
      noStroke();
      fill(inactive);
    }
    ellipse(bPos.x, bPos.y, size, size);
  }

  void update() {
  }

  void notifyArd(){
    queueMessage(ref);      //handshake
    queueMessage(state);     //data
  }
}

