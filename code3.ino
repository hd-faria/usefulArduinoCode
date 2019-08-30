// Simple piece of code to implement a state machine 
// port and turn LED/Pin13 On or Off:

void updateStateMachie(void);
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
      switch (inByte)
      {
      case 'l':
        // Turn LED/Pin13 On:
        // digitalWrite(LED_BUILTIN, HIGH);
        state = ON;
        updateStateMachie;
        break;
      case 'd':
        // Turn LED/Pin13 Off:
        // digitalWrite(LED_BUILTIN, LOW);
        state = OFF;
        updateStateMachie;
        break;
    
      default:
        //Do nothing
        break;
      }

    }

}

void updateStateMachie(void){
    switch (state)
    {
    case ON:
        digitalWrite(LED_BUILTIN, HIGH);
        break;

    case OFF:
        digitalWrite(LED_BUILTIN, LOW);
        break;
    
    default:
        /* code */
        break;
    }
}


