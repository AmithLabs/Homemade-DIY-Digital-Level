/*
  ---------------------------------------------------------
  Project         : Digital Spirit Level using MPU6050 + OLED
  Youtube Channel : @AmithLabs
  Author          : Amith Wijekoon
  ---------------------------------------------------------

  This ESP8266 project demonstrates a simple Digital
  Spirit Level (Digital Speed Level) using an MPU6050
  gyroscope/accelerometer sensor and a 0.96" OLED display.

  The MPU6050 reads the tilt angle of the device and
  calculates the X-axis angle in degrees.

  The angle value is then displayed in large text on
  the OLED screen for easy reading.

  This project is ideal for learning:
  - I2C communication
  - MPU6050 sensor interfacing
  - OLED display control
  - Angle calculation using accelerometer data
  - ESP8266 NodeMCU projects

  ---------------------------------------------------------
  Hardware Connections
  ---------------------------------------------------------

  NodeMCU ESP8266 + OLED Display (SSD1306)

    OLED GND  -> GND
    OLED VCC  -> 3V3
    OLED SCL  -> D1 (GPIO5)
    OLED SDA  -> D2 (GPIO4)

  NodeMCU ESP8266 + MPU6050 Sensor

    MPU GND   -> GND
    MPU VCC   -> 3V3
    MPU SCL   -> D1 (GPIO5)
    MPU SDA   -> D2 (GPIO4)

  Note:
  OLED and MPU6050 both use the same I2C bus
  (same SDA and SCL lines)

  ---------------------------------------------------------
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#include <math.h>

// OLED display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// OLED reset pin (-1 means no reset pin used)
#define OLED_RESET -1

// Create OLED display object
Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// Create MPU6050 sensor object
MPU6050 mpu;

void setup() {

  // Start serial monitor for debugging
  Serial.begin(115200);

  // Start I2C communication
  // SDA = D2
  // SCL = D1
  Wire.begin(D2, D1);

  /*
    -----------------------------------------------------
    Initialize OLED Display
    -----------------------------------------------------
  */

  // Start OLED with I2C address 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("OLED not found");

    // Stop program if OLED is not detected
    while (true);
  }

  /*
    -----------------------------------------------------
    Initialize MPU6050 Sensor
    -----------------------------------------------------
  */

  // Start MPU6050
  mpu.initialize();

  // Check sensor connection
  if (mpu.testConnection()) {

    Serial.println("MPU6050 Connected Successfully!");

  } else {

    Serial.println("MPU6050 Connection Failed!");

    // Stop program if MPU6050 is not detected
    while (true);
  }

  // Clear OLED screen at startup
  display.clearDisplay();
  display.display();
}

void loop() {

  /*
    -----------------------------------------------------
    Read Accelerometer Values
    -----------------------------------------------------
  */

  int16_t ax, ay, az;

  // Read acceleration values from MPU6050
  mpu.getAcceleration(&ax, &ay, &az);

  /*
    -----------------------------------------------------
    Calculate X-Axis Angle
    -----------------------------------------------------

    Formula:

    angleX = atan2(ay, az) * 180 / PI

    This converts accelerometer values into
    angle in degrees.
  */

  float angleX = atan2(ay, az) * 180 / PI;

  /*
    -----------------------------------------------------
    Display Data on OLED
    -----------------------------------------------------
  */

  // Clear previous screen
  display.clearDisplay();

  // Set text color
  display.setTextColor(WHITE);

  /*
    Display Title
  */

  display.setTextSize(2);
  display.setCursor(40, 0);
  display.print("LEVEL");

  /*
    Display Large Angle Value
  */

  display.setTextSize(3);
  display.setCursor(10, 25);
  display.print(angleX, 1);

  /*
    Display Degree Symbol
  */

  display.setTextSize(2);
  display.setCursor(110, 25);

  // Character 247 looks similar to degree symbol
  display.print((char)247);

  // Update OLED screen
  display.display();

  // Small refresh delay
  delay(200);
}