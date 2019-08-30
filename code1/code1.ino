// Simple piece of code to read serial 
// port and turn LED/Pin13 On or Off:
//#include "/home/liza/Downloads/arduino-1.8.9-linux64/arduino-1.8.9/hardware/arduino/avr/libraries/SoftwareSerial/SoftwareSerial"
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial port:
  Serial.begin(9600);
}

void loop() {
  // read from port 0:
  if (Serial.available()) {
    int inByte = Serial.read();
    switch (inByte)
    {
    case 'l':
      // Turn LED/Pin13 On:
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    case 'd':
      // Turn LED/Pin13 Off:
      digitalWrite(LED_BUILTIN, LOW);
      break;
    
    default:
      //Do nothing
      break;
    }
  
  }
}
