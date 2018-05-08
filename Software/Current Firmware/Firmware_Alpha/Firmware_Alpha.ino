#include <RFID_Reader.h>

#include <Speaker.h>
#include <RFID_Reader.h>
#include <string.h>

#define RFIDResetPin P4_3
#define LED RED_LED
#define G_LED GREEN_LED

const int buttonPin = PUSH2;
int buttonState = 0;
int button_delay = 0;

RFID_Reader reader(10);
Speaker speaker = Speaker();

void setup(){
Serial.begin(9600);

pinMode(RFIDResetPin, OUTPUT);
digitalWrite(RFIDResetPin, HIGH);

pinMode(buttonPin, INPUT_PULLUP);
pinMode(LED, OUTPUT);
digitalWrite(LED, LOW);
pinMode(G_LED, OUTPUT);
digitalWrite(G_LED, LOW);

Serial.println("Starting RFID reader...");
reader.resetReader();
}

void loop(){
  char* tagString;

  buttonState = digitalRead(buttonPin);   // Record initial button state
  reader.resetReader();                   // Reset the reader for rescanning.
  button_delay = 0;                       // Reset button hold delay

  /* Read RFID information and process the information depending on current mode selected*/
  if(Serial.available() > 0 && reader.isModeRead()){
    tagString = reader.processTag();  
    Serial.println(tagString);       

    //Check that the string is the correct length of a correct tag ID  and checkin/checkout
    if(strlen(tagString) == 12){  
      int action = reader.checkTagID(tagString);     
      if(action == -1){
        speaker.playFreq(349, 375);
      }else if (action == 1){
       speaker.playFreq(1000, 375);
      }
      delay(1000);  //Next read delay
    }
  } else if (Serial.available() > 0 && reader.isModeEdit()){  //In edit mode, can add or remove tags up to a limit of 10
    tagString = reader.processTag();
    Serial.println(tagString);                            

     //Check that the string is the correct length of a correct tag ID and register/unregister
    if(strlen(tagString) == 12){  
      int action = reader.addOrRemove(tagString);
      if(action == -1){
        speaker.playFreq(349, 375);
      }else{
        speaker.playFreq(1000, 375);
      } 
      delay(1000); //Next read delay
    } 
  }
 
  /*Button is being pushed and held for 15 delays counts. Is therefore, a push and held action
    Then switch modes.
  */
  while (buttonState == LOW){         
    button_delay++;
    delay(100);
    buttonState = digitalRead(buttonPin);
    if(button_delay > 15){
      reader.changeMode();
      if(reader.isModeRead()){          
          speaker.playReadMode();
      }else{
          speaker.playEditMode();
      }
      delay(1000);
      break;
    }
  } 

  /* Button checks if the button has been pressed and release. Is therefore, a push action.
      Then check backpack completeness.
  */
  if(buttonState == HIGH && button_delay > 1 && button_delay < 15) {               
    boolean complete = reader.runCheck(); 
    if(complete == false){     //Signify incomplete backpack   
      digitalWrite(LED, HIGH);     
      delay(300);               
      digitalWrite(LED, LOW);    
      delay(300);       
      speaker.playNegative();
    }else {                     //Signify complete backpack
      digitalWrite(G_LED, HIGH);   
      delay(300);               
      digitalWrite(G_LED, LOW);    
      delay(300);          
      speaker.playPositive();
    }
  }

  /*If the reader is in edit mode provide second modality to signify the state has changed */
  if(reader.isModeEdit()){
    digitalWrite(LED, HIGH); 
    delay(300);              
    digitalWrite(LED, LOW);   
    delay(300);               
  }
}
