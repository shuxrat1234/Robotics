#include <NewPing.h>

#define TRIG_PIN A4 
#define ECHO_PIN A5
#define MAX_DISTANCE 200
int m1p1 = 4;
int m1p2 = 6;
int m3p1 = 7;
int m3p2 = 2;

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

int distance = 100;

void setup() {
  pinMode(m1p1, OUTPUT);
  pinMode(m1p2, OUTPUT);
  pinMode(m3p1, OUTPUT);
  pinMode(m3p2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  distance = readPing();
  analogWrite(5, 130);
  analogWrite(3, 100);
  if (distance < 30) {
    turnRight();
  }
  else moveForward();
  Serial.println(distance);
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  move(m1p1, m1p2, 0);
  move(m3p1, m3p2, 0);
}
  
void moveForward() {
  move(m1p1, m1p2, 1);
  move(m3p1, m3p2, 1);    
}

void moveBackward() {
  move(m1p1, m1p2, -1);
  move(m3p1, m3p2, -1);
  
}  

void turnRight() {
  moveStop(); 
  delay(1000);
  moveBackward();
  delay(1000);
  move(m1p1, m1p2, 1);
  move(m3p1, m3p2, -1);
  delay(1000);
  moveForward(); 
} 
 
void turnLeft() {
 move(m1p1, m1p2, -1);
  move(m3p1, m3p2, 1);  
  delay(500);
  move(m1p1, m1p2, 1);
  move(m3p1, m3p2, 1);   
}  

void move(int m1p1, int m1p2, int direction) {
  if (direction == 1) {
    digitalWrite(m1p1, HIGH);
    digitalWrite(m1p2, LOW);
  }
  else if (direction == 0) {
    digitalWrite(m1p1, LOW);
    digitalWrite(m1p2, LOW);
  }
  else if (direction == -1) {
    digitalWrite(m1p1, LOW);
    digitalWrite(m1p2, HIGH);
  }
}