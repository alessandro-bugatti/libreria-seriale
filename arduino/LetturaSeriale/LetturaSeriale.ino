int i = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

void loop() {
  Serial.print("Hello world!");
  Serial.println(i++);
  delay(1000);
}
