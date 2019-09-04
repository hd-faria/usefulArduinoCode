// Application of state Machiene impementation, besed on
// https://codeandlife.com/2013/10/06/tutorial-state-machines-with-c-callbacks/
// but using a struct to hold some values of interest. Sadly I still have to 
// find a way of define de struct locally and pass it to the functions that represent 
// transitions
// Author: Henrique 

#define EMERGENCY_STOP_SSR   2
#define START_UP_SSR         11 //4 if using nano
#define ACS1                 A0

//State
struct state {
    uint8_t reserved5    : 1;  
    uint8_t reserved4    : 1;
    uint8_t reserved3    : 1;
    uint8_t reserved2    : 1;
    uint8_t reserved1    : 1;
    uint8_t reserved0    : 1;
    uint8_t is_Ready     : 1;    
    uint8_t is_Emergency : 1;
} system_state;
int last_current_measure;

void stateReport();
typedef void *(*StateFunc)();

// State transition
void *ledOn();   //Dummy state just to control arduino led
void *ledOff();  //Dummy state just to control arduino led
void *startingUp();
void *running();
void *stopped();

// State pointer
StateFunc statefunc;

void setup() {
    // AVR pin config.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(EMERGENCY_STOP_SSR, INPUT);
    pinMode(START_UP_SSR, OUTPUT);  
    attachInterrupt(digitalPinToInterrupt(EMERGENCY_STOP_SSR), emergencyCall, FALLING);
    attachInterrupt(digitalPinToInterrupt(EMERGENCY_STOP_SSR), emergencyCall, RISING);
    // initialize serial port:
    Serial.begin(9600);
    system_state.is_Ready=0;
    system_state.is_Emergency=0;  
    statefunc = ledOn;
}

void loop() {
    statefunc = (StateFunc)(*statefunc)();
}

void emergencyCall(){
    system_state.is_Emergency = 1;
}

void emergencyRelease(){
    system_state.is_Emergency = 0;
}

void *ledOn(){
    noInterrupts();
    digitalWrite(LED_BUILTIN, HIGH);
    interrupts();

    // Reconsider if it`s convenient to report state here
    Serial.print("Estado: 0b");
    stateReport;
    
    // Select next state 
    if (!system_state.is_Emergency){
        return startingUp;
    }
    else {
        return stopped;
    }
}

void *ledOff(){
    noInterrupts();
    digitalWrite(LED_BUILTIN, LOW);
    interrupts();

    delay(500);

    // Reconsider if it`s convenient to report state here
    Serial.print("Estado: 0b");
    stateReport;
  
    return ledOn;
}

void *startingUp(){
    // pre-check for emergency condition
    if(system_state.is_Emergency){
        return stopped;
    }
    
    //Setups
    noInterrupts();
    last_current_measure = analogRead(ACS1);
    if(last_current_measure < 512){
        system_state.is_Ready = 1;
        }
    else{
        system_state.is_Ready = 0;
    }
    interrupts();

    if(!system_state.is_Emergency && !system_state.is_Ready){
        Serial.println("Starting up...");
        Serial.println(last_current_measure);
        
        // Reconsider if it`s convenient to report state here
        Serial.print("Estado: 0b");
        stateReport;
        
        delay(100); // verify the minimum intervalbetween analog readings

        return startingUp;
    }

    if(!system_state.is_Emergency && system_state.is_Ready){
        digitalWrite(START_UP_SSR, HIGH);
        return running;
    }
    else{ // safety
        return stopped; 
    }    
}

// in development...
void *running(){
    // pre-check for emergency condition
    if(system_state.is_Emergency){
            return stopped;
        }

    Serial.println("Running...");
    Serial.print("Estado: 0b");
    stateReport;

    if(!system_state.is_Emergency && system_state.is_Ready){
        return running;
    }
    else{ // Safety
        return stopped;
    }
}

void *stopped(){
    
    noInterrupts();
    digitalWrite(START_UP_SSR, LOW);
    interrupts();

    Serial.println("Stopped!");
    Serial.print("Estado: 0b");
    stateReport;
    delay(500);    

    // next State
    return ledOff;
}

// Not used, yet...
void stateReport(){
    Serial.print(system_state.reserved5);
    Serial.print(system_state.reserved4);
    Serial.print(system_state.reserved3);
    Serial.print(system_state.reserved2);
    Serial.print(system_state.reserved1);
    Serial.print(system_state.reserved0);
    Serial.print(system_state.is_Emergency);
    Serial.println(system_state.is_Ready);    
}