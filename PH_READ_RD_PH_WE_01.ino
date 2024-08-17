#include <SoftwareSerial.h>
#include <Wire.h>

// Define RS485 pins for RE and DE to switch between transmit and receive mode
#define RS485_RE 4
#define RS485_DE 2

// Modbus RTU request for reading PH value
const byte readPH[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};

// A byte array to store the response data
byte responseData[7];

// SoftwareSerial object to communicate with the RS485 module
SoftwareSerial modbus(16, 17); // RX, TX

void setup() {
  // Start serial communication with the computer
  Serial.begin(9600);

  // Start serial communication with the RS485 module
  modbus.begin(9600);

  // Set RS485 pins as outputs
  pinMode(RS485_RE, OUTPUT);
  pinMode(RS485_DE, OUTPUT);

  // Turn off RS485 receiver and transmitter initially
  digitalWrite(RS485_RE, LOW);
  digitalWrite(RS485_DE, LOW);

  // Wait for the RS485 module to initialize
  delay(500);
}

void loop() {
  // Read PH value and print it to the serial monitor
  float pHValue = readPHValue();
  Serial.print("pH Value: ");
  Serial.println(pHValue);
  // Wait for 2 seconds before reading the value again
  delay(2000);
}

// Sends a Modbus RTU request and reads the response to get the PH value
float readPHValue() {
  // Set RS485 module to transmit mode
  digitalWrite(RS485_RE, HIGH);
  digitalWrite(RS485_DE, HIGH);

  // Send Modbus RTU request to the device
  modbus.write(readPH, sizeof(readPH));

  // Set RS485 module to receive mode
  digitalWrite(RS485_RE, LOW);
  digitalWrite(RS485_DE, LOW);

  // Wait for the response to be received
  delay(10);

  // Read the response into the responseData array
  byte responseLength = modbus.available();
  for (byte i = 0; i < responseLength; i++) {
    responseData[i] = modbus.read();
  }

  // Check if the response is valid and contains the PH value
  if (responseLength == 7 &&
      responseData[0] == 0x01 &&
      responseData[1] == 0x03 &&
      responseData[2] == 0x02) {
    // Calculate the PH value from the response data
    int rawPHValue = (responseData[3] << 8) | responseData[4];
    float pHValue = static_cast<float>(rawPHValue) / 100.0;
    Serial.println(rawPHValue);

    return pHValue;
  } else {
    // Invalid response or PH value not found
    return -1.0; // Return a negative value to indicate an error
  }
}




//CodeByAbdulrhmanElfeky_afeky3@gmail.com
