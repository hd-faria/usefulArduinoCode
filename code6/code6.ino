// Application of state Machiene impementation, besed on
// https://codeandlife.com/2013/10/06/tutorial-state-machines-with-c-callbacks/
// but using a struct to hold some values of interest. Sadly I still have to 
// find a way of define de struct locally and pass it to the functions that represent 
// transitions
// Author: Henrique 

#define EMERGENCY_STOP_SSR   12
#define START_UP_SSR         11

//State
struct state {
    uint8_t                 : 1;
    uint8_t                 : 1;  
    uint8_t                 : 1;
    uint8_t                 : 1;
    uint8_t                 : 1;
    uint8_t                 : 1;
    uint8_t is_ON           : 1;
    uint8_t is_Ready        : 1;    
    uint8_t is_Emergency : 1;
    /* data */
} systemState;

typedef void *(*StateFunc)();

// State transition
void *led_on();
void *led_off();
void *starting_up();
void *stopped();

// State pointer
StateFunc statefunc;

void setup() {
    // AVR pin config.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(EMERGENCY_STOP_SSR, OUTPUT);
    pinMode(START_UP_SSR, OUTPUT);  
    // initialize serial port:
    Serial.begin(9600);
    systemState.is_ON=0;
    systemState.is_Ready=0;
    systemState.is_Emergency=0;  
    statefunc = led_on;
}

void loop() {
    /*  
    if (Serial.available()) {
      int inByte = Serial.read();
      statefunc = (StateFunc)(*statefunc)();
    }  */
    statefunc = (StateFunc)(*statefunc)();
}

void *led_on(){
    systemState.is_ON = 1;
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Estado: ");
    Serial.println(systemState.is_ON);
    
    if (!systemState.is_Emergency){
        return starting_up;
    }
    else {
        return stopped;
    }
}

void *led_off(){
  systemState.is_ON = 0;
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Estado: ");
  Serial.println(systemState.is_ON);
  return led_on;
}

void *starting_up(){
    Serial.println("Starting up...");
    return stopped;
}

void *stopped(){
    return stopped;
}