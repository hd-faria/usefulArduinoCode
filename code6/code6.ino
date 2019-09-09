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
    byte reserved5    : 1; // uint8_t  
    byte reserved4    : 1; // uint8_t
    byte reserved3    : 1; // uint8_t
    byte reserved2    : 1; // uint8_t
    byte reserved1    : 1; // uint8_t
    byte reserved0    : 1; // uint8_t
    byte is_Ready     : 1; // uint8_t    
    byte is_Emergency : 1; // uint8_t
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
void emergencyCall();
void emergencyRelease();

// State pointer
StateFunc statefunc;

void setup() {
    // AVR pin config.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(EMERGENCY_STOP_SSR, INPUT);
    pinMode(START_UP_SSR, OUTPUT);  
    attachInterrupt(digitalPinToInterrupt(EMERGENCY_STOP_SSR), emergencyRelease, RISING);
    attachInterrupt(digitalPinToInterrupt(EMERGENCY_STOP_SSR), emergencyCall, FALLING);
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
    Serial.println(">>> Emergency called. <<<");
    statefunc = stopped;
}

void emergencyRelease(){
    system_state.is_Emergency = 0;
    Serial.println("### Emergency released. ###");
}

void *ledOn(){
    noInterrupts();
    digitalWrite(LED_BUILTIN, HIGH);
    interrupts();

    if(Serial.available()) {
        // Reconsider if it`s convenient to report state here
        stateReport;
    }  
    
    // Select next state 
    if(!system_state.is_Emergency){
        Serial.println("ledOn -> startingUp");
        return startingUp;
    }
    else {
        Serial.println("ledOn -> stopped");
        return stopped;
    }
}

void *ledOff(){
    noInterrupts();
    digitalWrite(LED_BUILTIN, LOW);
    interrupts();

    delay(500);

    if (Serial.available()) {
        // Reconsider if it`s convenient to report state here        
        stateReport;
    } 
    
    Serial.println("ledOff -> ledOn");
    return ledOn;
}

void *startingUp(){
    // pre-check for emergency condition
    if(system_state.is_Emergency){
        Serial.println("startingUp -> stopped(Emergency)");
        return stopped;
    }
    
    //Setups
    noInterrupts();
    last_current_measure = analogRead(ACS1);
    if((495<=last_current_measure)&&(last_current_measure < 516)){ //calculado 511<=last_current_measure<516
        system_state.is_Ready = 1;
        }
    else{
        system_state.is_Ready = 0;
    }
    interrupts();

    if(!system_state.is_Emergency && !system_state.is_Ready){
                
        if (Serial.available()) {
            Serial.println("Starting up...");
            Serial.println(last_current_measure);
            // Reconsider if it`s convenient to report state here
            stateReport;
        } 
        
        delay(250); // verify the minimum intervalbetween analog readings

        //Serial.println("startingUp -> startingUp");
        return startingUp;
    }

    if(!system_state.is_Emergency && system_state.is_Ready){
        digitalWrite(START_UP_SSR, HIGH);
        Serial.println("startingUp -> running");
        return running;
    }
    else{ // safety
        Serial.println("startingUp -> stopped(safety condition)");
        return stopped; 
    }    
}

void *stopped(){
    
    noInterrupts();
    digitalWrite(START_UP_SSR, LOW);
    interrupts();
    if(digitalRead(EMERGENCY_STOP_SSR)){
        emergencyRelease();
    }

    if(Serial.available()) {
        Serial.println("Stopped!");
        // Reconsider if it`s convenient to report state here
        stateReport;
    } 

    delay(500);    

    // next State
    Serial.println("stopped -> ledOff");
    return ledOff;
}

// in development...
void *running(){
    // pre-check for emergency condition
    if(system_state.is_Emergency){
            Serial.println("running -> stopped(Emergency)");
            return stopped;
        }

    
    if (Serial.available()) {
        Serial.println("Running...");
        // Reconsider if it`s convenient to report state here
        stateReport;
    } 

    if(!system_state.is_Emergency && system_state.is_Ready){
        //Serial.println("running -> running");
        return running;
    }
    else{ // Safety
        Serial.println("running -> stopped(safety condition)");
        return stopped;
    }
}



// Not used, yet...
void stateReport(){
    while (Serial.available())
    {
        int trashbin = Serial.read();
    }
    
    Serial.print("Estado: 0b");
    Serial.print(system_state.reserved5);
    Serial.print(system_state.reserved4);
    Serial.print(system_state.reserved3);
    Serial.print(system_state.reserved2);
    Serial.print(system_state.reserved1);
    Serial.print(system_state.reserved0);
    Serial.print(system_state.is_Emergency);
    Serial.println(system_state.is_Ready);    
}