#include <Servo.h>
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define DHTPIN 3      
#define DHTTYPE DHT11 
// To enter password
const int buttonPin = 2;
const int shortPressTime = 50;
const unsigned long longPressTime = 1000;
const String password = "10110010";
String enteredPassword = "124";
int buttonState = HIGH;
unsigned long pressStartTime = 0;
bool longPressDetected = false;
bool flag = 0;
bool access = 0;

LiquidCrystal_I2C lcd(0x27,20,4);
NewPing sonar(TRIG_PIN, ECHO_PIN, 200); 
DHT dht(DHTPIN, DHTTYPE);
Servo servo1;


void setup() {
  Serial.begin(9600);
  dht.begin();          
  lcd.init();
  lcd.backlight();
  pinMode(6, OUTPUT);
  servo1.attach(9);
  pinMode(2, INPUT_PULLUP);
  lcd.clear();
}

void loop() {
  checkButton();
  if (access) {
    int s = readPing();
    float humidity = dht.readHumidity();    
    float temperatureC = dht.readTemperature();   
    if (isnan(humidity) || isnan(temperatureC)) {
      Serial.println("Error reading data!");
      return;
    }

    if (analogRead(A2) < 300) {
      analogWrite(6, 200);
    }
    else {
      analogWrite(6, 0);
    }

    if (s < 20) {
      servo1.write(0);
    } 
    else {
      servo1.write(90);
    }


    lcd.setCursor(0,0);
    lcd.print("Temp: " + String(temperatureC));
    lcd.setCursor(0,1);
    lcd.print("Humidity: " + String(humidity));

    delay(100);
  }
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  
  return cm;
}

// void checkButton() {
//   while (enteredPassword != password) { // Infinite loop for checking the button
//     int reading = digitalRead(buttonPin);
//     static unsigned long lastDebounceTime = 0;

//     if (reading != buttonState) {
//       lastDebounceTime = millis();
//     }

//     if ((millis() - lastDebounceTime) > debounceDelay) {
//       if (reading != buttonState) {
//         buttonState = reading;

//         if (buttonState == LOW) {
//           // Button is pressed
//           pressStartTime = millis();
//           longPressDetected = false;  // Reset long press flag
//           Serial.println("Button Pressed");

//         } else {
//           // Button is released
//           unsigned long pressDuration = millis() - pressStartTime;

//           if (pressDuration < longPressThreshold) {
//             // Short Press
//             Serial.println("Short Press Detected");
//             enteredPassword += "0";
//             // Example: digitalWrite(13, LOW); // Turn off LED
//           } else if (longPressDetected) {
//             // Long Press (already handled)
//             // no need to do anything here as it was done in a previous step
//           }
//         }
//       } else if (buttonState == LOW && (millis() - pressStartTime) >= longPressThreshold && !longPressDetected) {
//         // Check for Long Press while the button is being pressed
//         Serial.println("Long Press Detected");
//         longPressDetected = true;  // Set the flag

//         enteredPassword += "1";
//         digitalWrite(13, HIGH); // Example: Turn on LED
//       }
//     }
    
//     delay(1);  // Small delay to avoid excessive CPU usage
//   }
// }
void checkButton() {
  while (!access) {  // Infinite loop
    int buttonState = digitalRead(buttonPin);
    
    // Button is pressed (LOW because we're using INPUT_PULLUP)
    if (buttonState == LOW) {
      if (flag == false) {
        flag = true;
        pressStartTime = millis();
      }
    }
    else if (buttonState == HIGH && flag == true) {
      if (millis() - pressStartTime < longPressTime) {
          if (millis() - pressStartTime > shortPressTime) {
            enteredPassword += "0";  // SHORT PRESS
          }
      }
      else if ((millis() - pressStartTime > longPressTime) && (flag == true)) {
        enteredPassword += "1";   //LONG PRESS DETECTED
        flag = false;
        // Wait for button release
        while (digitalRead(buttonPin) == LOW) {
          // Do nothing until button is released
        }
      }
      flag = false;
    }
    lcd.setCursor(0,0);
    lcd.print("EnteredPassword:");
    lcd.setCursor(0,1);
    lcd.print(enteredPassword);
    if (enteredPassword.length() == password.length() && enteredPassword != password) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INCORRECT");
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("Try Again");
      delay(1000);
      lcd.print("EnteredPassword:");
      enteredPassword = "";
    }
    else if (enteredPassword == password) {
      // lcd.clear();
      // lcd.setCursor(0, 0);
      // lcd.print("Access Granted!");
      access = 1;
      return;
      // break;
    }
    // Small delay to prevent bouncing (optional, hardware debouncing is better)
    delay(10);
  }
  return;
}