// Simple piece of code to implement a state machine 
// using function pointer to turn LED/Pin13 On or Off
// adapted from: https://codeandlife.com/2013/10/06/tutorial-state-machines-with-c-callbacks/

typedef void *(*StateFunc)();

// States
void *led_on();
void *led_off();

// State pointer
StateFunc statefunc;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial port:
  Serial.begin(9600);  
  statefunc = led_on;
}

void loop() {
      
    if (Serial.available()) {
      int inByte = Serial.read();
      statefunc = (StateFunc)(*statefunc)();
    }  

}

void *led_on(){
  digitalWrite(LED_BUILTIN, HIGH);
  return led_off;
}

void *led_off(){
  digitalWrite(LED_BUILTIN, LOW);
  return led_on;
}