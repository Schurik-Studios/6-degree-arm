#include <Servo.h>
#include <Arduino.h>

#define Servo1 7
#define Servo2 6
#define Servo3 5
#define Servo4 4
#define Servo5 3
#define Servo6 8

#define SW 2
#define y1 A0
#define y2 A2
#define x1 A1
#define x2 A3

Servo myServo1, myServo2, myServo3, myServo4, myServo5, myServo6;

unsigned long lastSerialTime = 0;

int targetAngle1 = 90;
int targetAngle2 = 90;
int targetAngle3 = 90;
int targetAngle4 = 90;
int targetAngle5 = 90;
int targetAngle6 = 90;

int currentAngle1 = targetAngle1;
int currentAngle2 = targetAngle2;
int currentAngle3 = targetAngle3;
int currentAngle4 = targetAngle4;
int currentAngle5 = targetAngle5;
int currentAngle6 = targetAngle6;

const int smoothStep = 3;
const int smoothDelay = 15;

int maxAngle(int angle) {
  if (angle < 0) return 0;
  if (angle > 180) return 180;
  return angle;
}


int increment(int val) {
  int center = 511;     
  int deadzone = 70;    

  if (val > center + deadzone) {
    if (val > center + 350) {
      return 2;
    } else {
      return 1;
    }
  }

  if (val < center - deadzone) {
    if (val < center - 350) {
      return -2;
    } else {
      return -1;
    }
  }

  return 0; 
}



void smoothMove(Servo &servo, int &current, int target) {
  if (current < target) 
    current += smoothStep;
  else if (current > target)
    current -= smoothStep;
  servo.write(current);
}

void setup() {
  pinMode(SW, INPUT);

  myServo1.attach(Servo1);
  myServo2.attach(Servo2);
  myServo3.attach(Servo3);
  myServo4.attach(Servo4);
  myServo5.attach(Servo5);
  myServo6.attach(Servo6);

  Serial.begin(9600);
}

void loop() {
  int xVal1 = analogRead(x1);
  int yVal1 = analogRead(y1);
  int xVal2 = analogRead(x2);
  int yVal2 = analogRead(y2);
  int sw = digitalRead(SW);

  unsigned long now = millis();
  if (now - lastSerialTime >= 1000) {
    lastSerialTime = now;
    Serial.print("SW: "); Serial.print(sw);
    Serial.print(" X1: "); Serial.print(xVal1);
    Serial.print(" Y1: "); Serial.print(yVal1);
    Serial.print(" X2: "); Serial.print(xVal2);
    Serial.print(" Y2: "); Serial.println(yVal2);
    Serial.print("Angle1: "); Serial.println(targetAngle1);
    Serial.print("Angle2: "); Serial.println(targetAngle2);
    Serial.print("Angle3: "); Serial.println(targetAngle3);
    Serial.print("Angle4: "); Serial.println(targetAngle4);
    Serial.print("Angle5: "); Serial.println(targetAngle5);
    Serial.print("Angle6: "); Serial.println(targetAngle6);
  }

  int inc1 = increment(yVal1);
  int inc2 = increment(xVal1);
  targetAngle1 = maxAngle(targetAngle1 + inc1);
  targetAngle2 = maxAngle(targetAngle2 + inc2);

  
  int inc3 = increment(yVal2);
  int inc4 = increment(xVal2);

  if (sw == 0) {
   
    targetAngle3 = maxAngle(targetAngle3 + inc3);
    targetAngle4 = maxAngle(targetAngle4 + inc4);
    
  } else {
    
    targetAngle5 = maxAngle(targetAngle5 + inc3);
    targetAngle6 = maxAngle(targetAngle6 + inc4);
    
  }
  smoothMove(myServo1, currentAngle1, targetAngle1);
  smoothMove(myServo2, currentAngle2, targetAngle2);
  smoothMove(myServo3, currentAngle3, targetAngle3);
  smoothMove(myServo4, currentAngle4, targetAngle4);
  smoothMove(myServo5, currentAngle5, targetAngle5);
  smoothMove(myServo6, currentAngle6, targetAngle6);

  delay(smoothDelay);
}
