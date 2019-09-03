// A state Machiene impementation, besed on
// https://codeandlife.com/2013/10/06/tutorial-state-machines-with-c-callbacks/
// but using a struct to hold some values of interest. Sadly I still have to 
// find a way of define de struct locally and pass it to the functions that represent 
// transitions


//State
struct state {
    uint8_t       : 1;
    uint8_t       : 1;  
    uint8_t       : 1;
    uint8_t       : 1;
    uint8_t       : 1;
    uint8_t       : 1;
    uint8_t       : 1;
    uint8_t       : 1;    
    uint8_t is_ON : 1;
    /* data */
} systemState;

typedef void *(*StateFunc)();

// State transition
void *led_on();
void *led_off();

// State pointer
StateFunc statefunc;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial port:
  Serial.begin(9600);
  systemState.is_ON=0;  
  statefunc = led_on;
}

void loop() {
      
    if (Serial.available()) {
      int inByte = Serial.read();
      statefunc = (StateFunc)(*statefunc)();
    }  

}

void *led_on(){
  systemState.is_ON = 1;
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("Estado: ");
  Serial.println(systemState.is_ON);
  return led_off;
}

void *led_off(){
  systemState.is_ON = 0;
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Estado: ");
  Serial.println(systemState.is_ON);
  return led_on;
}