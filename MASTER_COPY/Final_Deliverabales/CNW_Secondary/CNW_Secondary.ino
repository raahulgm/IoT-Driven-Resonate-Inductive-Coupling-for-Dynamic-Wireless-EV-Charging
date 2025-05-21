/**
 * @file espCode2.ino
 * @author Chandana R
 * @brief ESP8266 with Battery Monitoring, DHT11, and LCD Display
 * @version 2.0
 * @date 2025-03-30
 */
 /**
Version History:
0.1---> Initial Draft

1.0---> Battery Status updated using 12 V battery insted of 3.3 V, and also voltage divider circuit is also upgraded accordingly.
        NOTE: R1 & R2 is the new Resistance value i.e. 20K and 10K respectively for voltage devider circuit ((R1 + R2)/ R2), to 
		know Battery Stats
		
2.0---> The is deisgned to display both battery voltage and coil voltage, and also battery percentage and coil persecntage seperately
        with the help of switch (SWITCH_PIN 5  Yellow Switch in Hardware) for trigger it to fetch stats of Coil during this period
		the IoT  features is turned OFF[1 in Coil Mode, 0 in Battery Mode]. Only when yellow switch is in 0 state the data is pushed
		to IoT.
		
*/
 /*
 * ThingSpeak Settings
 * field1- Battery Voltage
 * field2-Battery Percentage
 * field3- Temperature
 * field4- Humidity
 
* Analog Pin for Battery to D34 (ESP32 GPIO34)
* Analog Pin for Coil to D35 (ESP32 GPIO35)
* DHT11 Sensor to D4 (ESP32 GPIO4)
* SDA (LCD) → D21 (ESP32 GPIO21)
* SCL (LCD) → D22 (ESP32 GPIO22)
* Switch → D5 (ESPP32 GPIO5)

 */
#include <Wire.h>                // I2C Communication
#include <LiquidCrystal_I2C.h>   // Library for 16x2 LCD with I2C
#include <DHT.h>                 // DHT11 Sensor Library
#include <HTTPClient.h>         // HTTP Client Library
#include <WiFi.h>                // WiFi Library for ESP32

// Hardware Pins
#define ANALOG_BAT 34         // ADC pin for battery voltage monitoring (ESP32-WROOM)
#define ANALOG_COIL 35       // ADC pin coil votage
#define DHT_PIN 4                // DHT11 data pin
#define DHT_TYPE DHT11           // DHT sensor type
#define SWITCH_PIN 5             // Switch pin (ON/OFF control)
#define SWITCH_PIN 14      //ONGOING

const char* ssid = "Chandana_R";
const char* password = "Chandu2002@";

// ThingSpeak API Information
const char* server = "http://api.thingspeak.com/update";
const char* apikey = "ASET8PVPCZQL68MR";  // Replace with Write API Key


// LCD and DHT Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);  

// Calibration & Resistor Values
float calibration1 = 2.39;         // Add offset if needed Battery
float calibration2 = 0;         // Add offset if needed Coil
float R1 = 20.0;                  // Resistor 1 (20kΩ)
float R2 = 10.0;                  // Resistor 2 (10kΩ)

void setup() {
    Serial.begin(115200);
    
    // Initialize LCD, DHT11, and Switch
    lcd.init();
    lcd.backlight();
    dht.begin();

    // Set switch pin as input with internal pull-down
    pinMode(SWITCH_PIN, INPUT_PULLDOWN);

    // Display startup message
    lcd.setCursor(0, 0);
    lcd.print("Team-XX");
    lcd.setCursor(0, 1);
    lcd.print("Jain University");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting WiFi...");
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi Connected!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected!");
    delay(1000);
}

// Function to map float values
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
    // Read the switch state
    bool switchState = digitalRead(SWITCH_PIN);

    if (switchState == HIGH) {
        // Switch ON → Run monitoring operations
        Serial.println("Battery Status");
      
        
        // Read Battery Voltage
        int sensorValue = analogRead(ANALOG_BAT);
        float batteryVoltage = ((sensorValue * 3.3) / 4095) * ((R1 + R2) / R2) + calibration1;

        // Map Battery Voltage to Percentage for 3-cell Li-ion (9.0V → 0%, 12.6V → 100%)
        int batteryPercentage = mapFloat(batteryVoltage, 9.0, 12.6, 0, 100);
        batteryPercentage = constrain(batteryPercentage, 0, 100);

        // Read DHT11 Sensor Data
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        // Print to Serial Monitor
        Serial.println("--------------------------");
        Serial.printf("Analog Value: %d\n", sensorValue);
        Serial.printf("Battery Voltage: %.2fV\n", batteryVoltage);
        Serial.printf("Battery Percentage: %d%%\n", batteryPercentage);
        Serial.printf("Temperature: %.2f°C\n", temperature);
        Serial.printf("Humidity: %.2f%%\n", humidity);
        Serial.println("--------------------------");

        // Display on LCD
        lcd.clear();
        
        // Row 1: Temperature and Humidity
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temperature, 1);
        lcd.print("C H:");
        lcd.print(humidity, 1);
        lcd.print("%");

        // Row 2: Battery Voltage and Percentage
        lcd.setCursor(0, 1);
        lcd.print("Bat:");
        lcd.print(batteryVoltage, 2);
        lcd.print("V ");
        lcd.print(batteryPercentage);
        lcd.print("%");
         if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(server) + "?api_key=" + apikey +
                     "&field1=" + String(batteryVoltage) +
                     "&field2=" + String(batteryPercentage) +
                     "&field3=" + String(temperature) +
                     "&field4=" + String(humidity);
        
        http.begin(url);
        int httpCode = http.GET(); // Send GET request
        
        if (httpCode > 0) {
            Serial.printf("Data Sent! HTTP Response: %d\n", httpCode);
        } else {
            Serial.printf("Error Sending Data: %s\n", http.errorToString(httpCode).c_str());
        }
        
        http.end(); // End HTTP connection
    } else {
        Serial.println("WiFi Disconnected! Reconnecting...");
        WiFi.reconnect();
    }

    delay(5000); // Update every 5 seconds

    } else {
        // Switch OFF → Display "SWITCH OFF"

        Serial.println("--------------------------");
        Serial.println("Coil Mode Requeseted");
       Serial.println("Coil Status");
    

        // Read Battery Voltage
        int sensorValue = analogRead(ANALOG_COIL);
        float coilVoltage = ((sensorValue * 3.3) / 4095) * ((R1 + R2) / R2) + calibration2;

        // Map Battery Voltage to Percentage for 3-cell Li-ion (9.0V → 0%, 12.6V → 100%)
        int coilPercentage = mapFloat(coilVoltage, 9.0, 10.5, 0, 100);
        coilPercentage = constrain(coilPercentage, 0, 100);

        // Read DHT11 Sensor Data
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        // Print to Serial Monitor
        Serial.println("--------------------------");
        Serial.printf("Analog Value: %d\n", sensorValue);
        Serial.printf("Coil Voltage: %.2fV\n", coilVoltage);
        Serial.printf("Coil Percentage: %d%%\n", coilPercentage);
        Serial.printf("Temperature: %.2f°C\n", temperature);
        Serial.printf("Humidity: %.2f%%\n", humidity);

        // Display on LCD
        lcd.clear();
        
        // Row 1: Temperature and Humidity
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temperature, 1);
        lcd.print("C H:");
        lcd.print(humidity, 1);
        lcd.print("%");

        // Row 2: Battery Voltage and Percentage
        lcd.setCursor(0, 1);
        lcd.print("Coil:");
        lcd.print(coilVoltage, 2);
        lcd.print("V ");
        lcd.print(coilPercentage);
        lcd.print("%");
    }

    delay(2000);  // Update every 2 seconds
}
