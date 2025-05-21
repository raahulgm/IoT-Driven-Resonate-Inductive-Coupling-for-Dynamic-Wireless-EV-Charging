#include <AFMotor.h>   // Library for L293D Motor Shield

// Define motors
AF_DCMotor motor1(1);   // Motor 1 (M1)
AF_DCMotor motor2(2);   // Motor 2 (M2)
AF_DCMotor motor3(3);   // Motor 3 (M3)
AF_DCMotor motor4(4);   // Motor 4 (M4)

void setup() {
  Serial.begin(9600);
  
}
void loop() {
  // Move motors forward at different speeds
  Serial.println("Moving Forward...");
  motor1.setSpeed(150);  
  motor2.setSpeed(150);  
  motor3.setSpeed(150);  
  motor4.setSpeed(150);

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(5000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(500);
}
