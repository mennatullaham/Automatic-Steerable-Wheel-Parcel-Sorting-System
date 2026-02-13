#include <AccelStepper.h>

const int pirPin = 10; 

// stepper motor (swivel Unit)
const int stepPin = 8;
const int dirPin  = 9;
const int en1     = 3;
//paths
int leftPath  = 1.8 * -20;
int straightPath  = 1.8 * 0;
int rightPath = 1.8 * 20;


// DC motor (conveyor)
const int dc2_pin1 = 6; 
const int dc2_pin2 = 7; 
const int en2 = 5;   

bool motionActive = false;

// stepper configuration
const float stepAngle = 1.8; // degrees per step
const int stepsPerRev = 200;  // full steps per revolution

// AccelStepper object.
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void setup() {
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);

  digitalWrite(en1, LOW);    // enable stepper
  digitalWrite(en2, HIGH);   // enable DC motor

  pinMode(pirPin, INPUT);

  pinMode(dc2_pin1, OUTPUT);
  pinMode(dc2_pin2, OUTPUT);

  Serial.begin(9600);

  // Configure stepper speed/acceleration
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);

  runDC2();  
  Serial.println("System Ready. Conveyor belt is running...");
}

void loop() {

  int pirState = digitalRead(pirPin);

  if (pirState == HIGH && !motionActive) {

    motionActive = true;

    Serial.println("Motion detected!");
    stopDC2();
    Serial.println("Conveyor stopped. Waiting for Scanner...");

    while (!Serial.available()); //until recieving from qr scanner
    char qrCode = Serial.read(); // data type will be changed based on the qr code type

    float targetAngle;

    if (qrCode == '1') {
      stepper.moveTo(rightPath);
      Serial.println("RIGHT PATH");
    } 
    else if (qrCode == '2') {
      stepper.moveTo(leftPath);
      Serial.println("LEFT PATH");
    } 
    else if (qrCode == '3'){
      stepper.moveTo(straightPath);
      Serial.println("STRAIGHT PATH");
    }
    else {Serial.println("invalid path");}

    
    Serial.println("Stepper movement done.");

    runDC2();     
    Serial.println("Conveyor running again...");
  }

  if (pirState == LOW) {
    motionActive = false;
  }
}

// -------- Conveyor DC motor functions --------//
void runDC2() {
  digitalWrite(dc2_pin1, HIGH);
  digitalWrite(dc2_pin2, LOW);
}

void stopDC2() {
  digitalWrite(dc2_pin1, LOW);
  digitalWrite(dc2_pin2, LOW);
}
