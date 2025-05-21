/**
*@file espCode1_0.ino
*@author Chandana R
*@brief Main Code to Arduino UNO for 6 Coils
*@version 2.0
*@date of revision 2025-03-28
*/

/**
Version History:
0.1---> Initial Draft
1.0---> The relay module previously was connected to the analog and IR to the digital pins 
        now, the relay module is connected to the digital sensors and IR module is connected to analog + digital pins
2.0---> The relay module previously connected are completely turned to digital, since they are available, 8 IR connected
        digital pins 8, 9, 10, 11, 12, 13 and for relay digital pins 2, 3, 4, 5, 6, 7. Note: To have efficient switching between
		coils this version of the code must be used. Only for loop based firmware works efficiently. To have smooth switching,
		when one relay are ON they are kept at that state for atlest 3 seconds (3000 ms).
		NOTE: Relay will be in ON state when its put to LOW state, and IR trigger is also in LOW state.
*/
/** 
*Components:
-Arduino UNO X 1
-IR Sensors X 6
-5v 8Channel Relay X 1
-Copper coil 150 turns each of ~22 gauge
-Connecting Wire
*/

// Pin arrays for IR sensors and coils
int irPins[] = {8, 9, 10, 11, 12, 13};   // IR sensor pins
int coilPins[] = {2, 3, 4, 5, 6, 7};     // Corresponding coil pins
const int NUM_COILS = 6;

unsigned long coilTimers[NUM_COILS] = {0};   // Stores the ON time for each coil
const unsigned long coilOnDuration = 3000;   // 3 seconds ON time

void setup() {
  Serial.begin(115200);   // Initialize Serial Monitor

  // Configure IR sensors and coils
  for (int i = 0; i < NUM_COILS; i++) {
    pinMode(irPins[i], INPUT);       // Set IR pins as INPUT
    pinMode(coilPins[i], OUTPUT);    // Set coil pins as OUTPUT
    digitalWrite(coilPins[i], HIGH); // Ensure coils are initially OFF
  }
}

void loop() {
  unsigned long currentMillis = millis();   // Get current time

  for (int i = 0; i < NUM_COILS; i++) {
    if (digitalRead(irPins[i]) == LOW) {
      // IR triggered, activate the coil
      Serial.print("COIL ");
      Serial.print(i + 1);
      Serial.println(" IS ON");

      digitalWrite(coilPins[i], LOW);            // Turn ON coil
      coilTimers[i] = currentMillis;             // Set the coil's ON time
    }

    // Check if 3 seconds have passed since the coil was turned ON
    if (currentMillis - coilTimers[i] >= coilOnDuration) {
      digitalWrite(coilPins[i], HIGH);           // Turn OFF the coil
    }
  }

  delay(10);   // Stability delay
}
