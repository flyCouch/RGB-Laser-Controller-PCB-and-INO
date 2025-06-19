/*
  Copyright 2024 Ron Lyttle.
*/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int OLED_RESET = -1;
#define OLED_SDA 4
#define OLED_SCL 5
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

int i = 0;
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;
int redDial = A0;
int blueDial = A2;
int greenDial = A1;
int pauseDial = A3;
int menuDial = A6;
const int menuPin = 4;
const int firePin = 2;
int redDialValue = 0;
int greenDialValue = 0;
int blueDialValue = 0;
int presetDialValue = 0;
int menuDialValue = 0;
byte redBrightness = 0;
byte greenBrightness = 0;
byte blueBrightness = 0;
byte pauseUse = 0;

bool menuBool = LOW;
bool fireBool = LOW;

int menuDialSwitcher;
int presetValue = 0;
int pauseRead = 0;
int speakerPin = 9;
int volume = 0;
int volumeValue = 0;
byte pulseWidthAmount = 1;
byte pulseWidthValue = 1;

struct RGB {
  byte red;
  byte green;
  byte blue;
  String name;
};

// Create an array of 8 RGB values with color names
RGB colors[8] = {
  {255, 0, 0, "Red"},
  {0, 255, 0, "Green"},
  {0, 0, 255, "Blue"},
  {255, 255, 0, "Yellow"},
  {0, 255, 255, "Cyan"},
  {255, 0, 255, "Magenta"},
  {128, 128, 128, "Gray"},
  {255, 255, 255, "White"}
};

void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 15);
  display.println("Lyttle ReSearch");
  display.display();
  delay(3000);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(firePin, INPUT);
  pinMode(menuPin, INPUT);
  pinMode(redDial, INPUT);
  pinMode(greenDial, INPUT);
  pinMode(blueDial, INPUT);
  pinMode(pauseDial, INPUT);
  pinMode(menuDial, INPUT);
}

void loop() {
  menuDialValue = analogRead(menuDial);
  menuDialSwitcher = map(menuDialValue, 1023, 0, 1, 4);
  switch (menuDialSwitcher) {
    case 1:
      // Code to execute for case 1
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(15, 25);
      display.print("Dials Laser");
      display.display();
      if (digitalRead(menuPin) == HIGH) {
        delay(200); // Debounce delay
        menuBool = LOW;
        dialsLaser();
      }
      break;

    case 2:
      // Code to execute for case 2
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(15, 25);
      display.print("Presets Laser");
      display.display();
      if (digitalRead(menuPin) == HIGH) {
        delay(200); // Debounce delay
        menuBool = LOW;
        presetsLaser();
      }
      break;

    case 3:
      // Code to execute for case 3
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(15, 25);
      display.print("Pulse Width");
      display.display();
      if (digitalRead(menuPin) == HIGH) {
        delay(200); // Debounce delay
        menuBool = LOW;
        pulseWidth();
      }
      break;

    case 4:
      // Code to execute for case 4
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(15, 25);
      display.print("Volume");
      display.display();
      if (digitalRead(menuPin) == HIGH) {
        delay(200); // Debounce delay
        menuBool = LOW;
        sound();
      }
      break;
  }
  delay(200); // General delay to manage loop timing
}

void dialsLaser() {
  // Read dial values
  redDialValue = analogRead(redDial);
  greenDialValue = analogRead(greenDial);
  blueDialValue = analogRead(blueDial);
  pauseRead = analogRead(pauseDial);

  // Map dial values to brightness
  redBrightness = map(redDialValue, 0, 1023, 255, 0);
  greenBrightness = map(greenDialValue, 0, 1023, 255, 0);
  blueBrightness = map(blueDialValue, 0, 1023, 255, 0);
  pauseUse = map(pauseRead, 0, 1023, 100, 0);

  // Display RGB values on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(80, 15);
  display.print("Dials");
  display.setCursor(0, 5);
  display.print("Red:    ");
  display.print(redBrightness);
  display.setCursor(0, 20);
  display.print("Green:  ");
  display.print(greenBrightness);
  display.setCursor(0, 35);
  display.print("Blue    ");
  display.print(blueBrightness);
  display.setCursor(0, 50);
  display.print("Bright %");
  display.print(pauseUse);
  display.display();

  // Check if menu button is pressed
  menuBool = digitalRead(menuPin);
  if (menuBool == HIGH) {
    delay(200);  // Debounce delay
    return;      // Exit function if menu button is pressed
  }

  fireLaser();  // Call fireLaser function
  dialsLaser();
}

void presetsLaser() {
  presetDialValue = analogRead(menuDial);
  presetValue = map(presetDialValue, 0, 1023, 7, 0);
  pauseRead = analogRead(pauseDial);
  pauseUse = map(pauseRead, 0, 1023, 100, 0);
  redBrightness = (colors[presetValue].red);
  greenBrightness = (colors[presetValue].green);
  blueBrightness = (colors[presetValue].blue);

  // Display RGB values on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(80, 15);
  display.print("Presets");
  display.setCursor(80, 35);
  display.print(colors[presetValue].name);
  display.setCursor(0, 5);
  display.print("Red:    ");
  display.print(redBrightness);
  display.setCursor(0, 20);
  display.print("Green:  ");
  display.print(greenBrightness);
  display.setCursor(0, 35);
  display.print("Blue    ");
  display.print(blueBrightness);
  display.setCursor(0, 50);
  display.print("Bright %");
  display.print(pauseUse);
  display.display();

  // Check if menu button is pressed
  menuBool = digitalRead(menuPin);
  if (menuBool == HIGH) {
    delay(200);  // Debounce delay
    return;      // Exit function if menu button is pressed
  }

  fireLaser();  // Call fireLaser function
  presetsLaser();
}

void pulseWidth() {
  // Read the pulse width value from a potentiometer or other input
  int pulseWidthValue = analogRead(menuDial);

  // Map the input value to a suitable PWM range (adjust as needed)
  pulseWidthAmount = map(pulseWidthValue, 0, 1023, 255, 0);

  // Display the pulse width value on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 15);
  display.print("Pulse Width Amount");
  display.setCursor(10, 35);
  display.setTextSize(2);
  display.print(pulseWidthAmount);
  display.display();

  // Check if menu button is pressed
  menuBool = digitalRead(menuPin);
  if (menuBool == HIGH) {
    delay(200);  // Debounce delay
    return;      // Exit function if menu button is pressed
  }
  delay(200);
  pulseWidth();
}

void sound() {
  // Read the pulse width value from a potentiometer or other input
  int volumeValue = analogRead(menuDial);

  // Map the input value to a suitable PWM range (adjust as needed)
  volume = map(volumeValue, 0, 1023, 255, 0);

  // Display the pulse width value on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 15);
  display.print("Volume");
  display.setCursor(20, 35);
  display.setTextSize(2);
  display.print(volume);
  display.display();

  // Check if menu button is pressed
  menuBool = digitalRead(menuPin);
  if (menuBool == HIGH) {
    delay(200);  // Debounce delay
    return;      // Exit function if menu button is pressed
  }
  delay(200);
  sound();
}

void fireLaser() {
  fireBool = digitalRead(firePin);  // Read the fire button state
  // Check if fire button is pressed
  if (fireBool == HIGH) {
    fireBool = LOW;  // Set Fire to LOW

    for ( i = 1; i <= 20; i++) {
      analogWrite(speakerPin, volume);
      analogWrite(redPin, (redBrightness * pauseUse / 100));
      analogWrite(greenPin, (greenBrightness * pauseUse / 100));
      analogWrite(bluePin, (blueBrightness * pauseUse / 100));
      delay(pulseWidthAmount);

      analogWrite(speakerPin, 0);
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      delay(pulseWidthAmount);
    }
  }
}
