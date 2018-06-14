/*
  Echo seriale
 Language: C

 This program receives one byte (possibly an ASCII character) from serial
 and send it back to serial.

 Derived from Tom Igoe SerialCallResponse

 Created 14 June 2018
 by Alessandro Bugatti
 
 This example code is in the public domain.

 */

int inByte = 0;         // incoming serial byte

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    Serial.write(inByte);
    }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print("Waiting...\n");   // send a message
    delay(1000);
  }
}

