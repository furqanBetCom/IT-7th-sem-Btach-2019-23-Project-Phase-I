/* Get tilt angles on X and Y, and rotation angle on Z
 * Angles are given in degrees
 * 
 * License: MIT
 */

// Include the BluetoothSerial library for communication over Bluetooth
#include "BluetoothSerial.h"

// Check if Bluetooth is enabled in the configuration
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Create a BluetoothSerial object for communication
BluetoothSerial SerialBT;

// Include the LiquidCrystal library for the LCD display
#include <LiquidCrystal.h> 

// Create an LCD object with the specified pins
LiquidCrystal lcd(15,16,17,18,19,23);

// Include the Wire library for communication with the MPU6050
#include "Wire.h"

// Include the MPU6050_light library for reading data from the MPU6050
#include <MPU6050_light.h>

// Initialize variables for storing the number of steps and a flag for tracking step transitions
int x=0,strp=0;

// Create an MPU6050 object using the Wire object
MPU6050 mpu(Wire);

// Initialize a timer for controlling the data output frequency
unsigned long timer = 0;

// Initialize a counter for sending data over Bluetooth
int timecount=0;

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);

  // Initialize the Wire library for communication with the MPU6050
  Wire.begin();

  // Initialize Bluetooth communication with the specified name
  SerialBT.begin("ESP32test");

  // Attempt to connect to the MPU6050
  byte status = mpu.begin();

  // Print the status of the connection
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);

  // If the MPU6050 could not be connected to, stop the program
  while(status!=0){ }

  // Print a message to the serial monitor and LCD display
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  lcd.begin(16, 2); // Initialize the LCD display with 16 columns and 2 rows
  lcd.print("do not move MPU6050"); // Display a message on the LCD
  delay(1000); // Wait for 1 second

  // Uncomment the following line if the MPU6050 is mounted upside-down
  // mpu.upsideDownMounting = true; 

  // Calculate the gyro and accelerometer offsets
  mpu.calcOffsets(); 

  // Print a message to the serial monitor
  Serial.println("Done!\n");

  // Clear the LCD display and print a message
  lcd.clear();
  lcd.print("PEDOMETER"); // Display a message on the LCD
  delay(3000); // Wait for 3 seconds
  lcd.clear(); // Clear the LCD display
}

void loop() {
  // Update the data from the MPU6050
  mpu.update();