// Simple piece of code to implement a state machine 
// port and turn LED/Pin13 On or Off:

void updateStateMachine(int);
enum stateMachine {OFF, ON} state;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial port:
  Serial.begin(9600);
  state = OFF;
}

void loop() {
  // read from port 0:

    if (Serial.available()) {
      int inByte = Serial.read();
      updateStateMachine(inByte);
    }

}

void updateStateMachine(int cmd){
    switch (state)
    {
      case ON:
        switch (cmd)
        {
          case 'l':
            Serial.println("Already turned on");
            break;
          case 'd':
            state = OFF;
            if (state == OFF){
              digitalWrite(LED_BUILTIN, LOW);            
              Serial.println("Turned off.");
            }
            else {
              Serial.println("Couldn\'t turn off.");
            }
            break;
          case 'o':
            Serial.println(state);
            break;             
          default:
            Serial.println("No valid command.");
            break;
        }
        break;

      case OFF:
        switch (cmd)
        {
          case 'd':
            Serial.println("Already turned off.");
            break;
          case 'l':
            state = ON;
            if (state == ON){
              digitalWrite(LED_BUILTIN, HIGH);            
              Serial.println("Turned on.");
            }
            else {
              Serial.println("Couldn\'t turn on.");
            }
            break;
          case 'o':
            Serial.println(state);
            break;            
          default:
            Serial.println("No valid command.");
            break;
        }          
        break;

      default:
        Serial.println("Fault!");
        break;
    }
}


