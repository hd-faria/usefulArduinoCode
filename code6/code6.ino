// Application of state Machiene impementation, besed on
// https://codeandlife.com/2013/10/06/tutorial-state-machines-with-c-callbacks/
// but using a struct to hold some values of interest. Sadly I still have to 
// find a way of define de struct locally and pass it to the functions that represent 
// transitions
// Author: Henrique 

#define EMERGENCY_STOP_SSR   2
#define START_UP_SSR         11 //4 if using nano

//State
struct state {
    uint8_t reserved5    : 1;  
    uint8_t reserved4    : 1;
    uint8_t reserved3    : 1;
    uint8_t reserved2    : 1;
    uint8_t reserved1    : 1;
    uint8_t is_ON        : 1;
    uint8_t is_Ready     : 1;    
    uint8_t is_Emergency : 1;
} systemState;

int getStateByte();
typedef void *(*StateFunc)();

// State transition
void *led_on();   //Dummy state just to control arduino led
void *led_off();  //Dummy state just to control arduino led
void *starting_up();
void *ready();
void *running();
void *stopped();

// State pointer
StateFunc statefunc;

void setup() {
    // AVR pin config.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(EMERGENCY_STOP_SSR, INPUT);
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
    Serial.print(systemState.is_Emergency);
    Serial.print(systemState.is_ON);
    Serial.println(systemState.is_Ready);
    
    // Select next state 
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
    Serial.print(systemState.is_Emergency);
    Serial.print(systemState.is_ON);
    Serial.println(systemState.is_Ready);
  
    return stopped;
}

void *starting_up(){
    Serial.println("Starting up...");
    return stopped;
}

void *ready(){
    Serial.println("Ready to run.");
    return stopped;
}
void *running(){
    Serial.println("Running...");
    return stopped;
}

void *stopped(){
    Serial.print("Estado: ");
    return stopped;
}

int getStateByte(){
    return ((systemState.reserved5<<7)|(systemState.reserved4<<6)|(systemState.reserved3<<5)|(systemState.reserved2<<4)|(systemState.reserved1<<3)|(systemState.is_Ready<<2)|(systemState.is_ON<<1)|(systemState.is_Emergency<<0));
}