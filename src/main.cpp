//////////////////////////////////////////////////////////////////////
//                                                                  //
//   Program for displaying temperature on LCD using Arduino.       //
//                                                                  //
//   Hardware Used:                                                 //
//    - Keyestudio Mega 2560 R3 (SBC)                               //
//    - 1602A                   (LCD)                               //
//    - NTC-MF52 3950           (Temperature Sensor)                //
//                                                                  //
//   Software Used:                                                 //
//    - Visual Code    (IDE)                                        //
//    - PlatformIO     (Platform)                                   //
//    - NTC_Thermistor (Library)                                    //
//    - LiquidCrystal  (Library)                                    //
//                                                                  //
//                           Copyright (c) 2023, André Jakobsson.   //
//                                                                  //
//////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <NTC_Thermistor.h>
#include <LiquidCrystal.h>

// Common Constants
#define SERIAL_BAUD            9600       // Speed of Serial Data
#define DELAY_MS               1000       // Milliseconds to wait between loops

// Thermometer Constants
#define SENSOR_PIN             A0         // Analog Input
#define REFERENCE_RESISTANCE   10000      // 10K Resistor
#define NOMINAL_RESISTANCE     10000      // 10K Resistor
#define NOMINAL_TEMPERATURE    25         // 25c at 10K+10K Resistance
#define B_VALUE                3950       // Beta Value (it's complicated!)

// LCD Constants
#define LCD_RS                 7          // Digital RS Signal
#define LCD_EN                 6          // Digital Enable LCD Signal
#define LCD_DATA0              13         // Digital Data
#define LCD_DATA1              12         // Digital Data
#define LCD_DATA2              11         // Digital Data
#define LCD_DATA3              10         // Digital Data
#define LCD_CONTRAST           A7         // Analog Contrast

// Global variables and references
double temperature;
Thermistor* thermistor = NULL;
LiquidCrystal* lcd = NULL;

// Bootstrap
void setup() {
  // Print to Serial Port that we're doing something
  Serial.begin(SERIAL_BAUD);
  Serial.println("Started");

  // Define direction of specific PINs (INPUT is default)
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LCD_CONTRAST, OUTPUT);

  // Thermometer instance
  thermistor = new NTC_Thermistor(
    SENSOR_PIN,
    REFERENCE_RESISTANCE,
    NOMINAL_RESISTANCE,
    NOMINAL_TEMPERATURE,
    B_VALUE
  );

  // Display instance
  lcd = new LiquidCrystal(
    LCD_RS,
    LCD_EN,
    LCD_DATA0,
    LCD_DATA1,
    LCD_DATA2,
    LCD_DATA3
  );
  lcd->begin(16, 2);              // Our display is 16x2
  lcd->clear();

  analogWrite(LCD_CONTRAST, 1);   // Set contrast to highest (hard to see otherwise)
}

void loop() {
  // Turn on built-in LED to show we're working
  digitalWrite(LED_BUILTIN, HIGH);

  // Read Temperature and print to LCD
  const double celsius = thermistor->readCelsius();
  lcd->print("  Temperature   ");
  lcd->setCursor(0, 1);   // Advance to second row
  lcd->print("    [");
  lcd->print(celsius);
  lcd->print("]        ");

  // Turn of built-in LED and sleep for about 1 second. This is to not overload any components
  digitalWrite(LED_BUILTIN, LOW);
  delay(DELAY_MS);
}

