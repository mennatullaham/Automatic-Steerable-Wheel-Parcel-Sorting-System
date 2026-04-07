// the code integrates the whole system including (dc motor, stepper, ir sensor, qr code scanner) //
#include <Stepper.h>
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);//in1,3,2,4

const int irSensorPin = 3;

// dc motor pins //
const int in1=6;
const int in2=7;
const int enA=5;

int DC_speed=200;

void setup() {
  Serial.begin(9600);   
  Serial1.begin(9600);  // for scanner serial comm
  
  pinMode(irSensorPin, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);


  myStepper.setSpeed(15); // motor rpm
}

void loop() {
  // starting with obj detection
  if (digitalRead(irSensorPin) == LOW) {
    
    Serial.println("Object Detected! Waiting for QR Scan...");

    //waiting until scanner reads a code
    while (!Serial1.available()) {
    }

    if (Serial1.available()) {
      String c = Serial1.readStringUntil('\n');
      Serial.println(c);
      c.trim();

      if (c == "123") {
        Serial.println("Action: Moving RIGHT");
        startDC();
        moveStepper(512); 
        delay(2000); // time for obj to have passed completely
        moveStepper(-512); //back to original position
        stopDC();
      } 
      else if (c == "789") {
        Serial.println("Action: Moving LEFT");
        startDC();
        moveStepper(-512); 
        delay(2000);
        moveStepper(512);
        stopDC();
      } 
      else {
        Serial.println("Staying STRAIGHT"); //only forward motion, no directional motion = no stepper needed
        startDC();
        delay(2000);
        stopDC();
      }
    }
    delay(1000);
  }
}
void moveStepper(int steps) {
  myStepper.step(steps);
}
void startDC(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(enA,DC_speed);
}
void stopDC(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(enA,0);
}
