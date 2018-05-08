#include <Speaker.h>
#include <RFID_Reader.h>
#define LED RED_LED

#define INFRAM __attribute__((section(".text")))

RFID_Reader testreader(10);
Speaker mySpeak = Speaker();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT); 
  
}

uint16_t counter = 0;
int tests_run = 0;
int tests_passed = 0;
int assertions = 0;


static void testInit(int num){
  Serial.println("Initializing RFID reader...");
  testreader = RFID_Reader(num);
  Serial.println("RFID reader initialized");
}

static void testcheckTagID()
{
  Serial.println("Checking tag ID...");
  
  testreader.checkTagID("1E009A4067A3");
  int* checked = testreader.getChecked();
  int* unchecked = testreader.getUnchecked();

  if(unchecked[0] == -1 && checked[0] == 0){
    Serial.println("CheckTag test 1 success");
  }
  else{
    Serial.println("CheckTag test 1 failed");
  }

  testreader.checkTagID("1E009A4067A3");
  checked = testreader.getChecked();
  unchecked = testreader.getUnchecked();

  if(unchecked[0] == 0 && checked[0] == -1){
    Serial.println("CheckTag test 2 success");
  }
  else{
    Serial.println("CheckTag test 2 failed");
  }

  testreader.checkTagID("01023C000E31");
  checked = testreader.getChecked();
  unchecked = testreader.getUnchecked();

  if(unchecked[4] == -1 && checked[4] == 4){
    Serial.println("CheckTag test 3 success");
  }
  else{
    Serial.println("CheckTag test 3 failed");
  }
  
  Serial.println("Tag ID tests completed");
}

static void testrunCheck(){

  boolean success = testreader.runCheck();

  if(!success){
    Serial.println("runCheck test 1 success");
  }else{
    Serial.println("runCheck test 1 failed");
  }


  testreader.checkTagID("1E009A4067A3");

  success = testreader.runCheck();
  
  if(!success){
    Serial.println("runCheck test 2 success");
  }else{
    Serial.println("runCheck test 2 failed");
  }

  testreader.checkTagID("010230F28243");
  testreader.checkTagID("01023C013A04");
  testreader.checkTagID("01023C0A4376");
  testreader.checkTagID("01023C000E31");
  testreader.checkTagID("01023C0A3207");
  testreader.checkTagID("1A004116317C");
  testreader.checkTagID("1E009A81F9FC");
  testreader.checkTagID("1A004162261F");
  testreader.checkTagID("1A004162261Z");

  success = testreader.runCheck();

  if(success){
    Serial.println("runCheck test 3 success");
  }else{
    Serial.println("runCheck test 3 failed");
  }

}

/*Require the reader to test */
static void testResetReader(){
  char* tagString;
  char* newtagString;
  while(Serial.available() <= 0){
    
  }
  if(Serial.available() > 0){
    tagString = testreader.processTag();
  }
  testreader.resetReader();
  while(Serial.available() <= 0){
    
  }
  if(Serial.available() > 0){
    newtagString = testreader.processTag();
  }

  if(strcmp(newtagString,tagString) == 0){
    Serial.println("Reader was reset and rescanned");
  }
}


static void testsuccessNotify(){
  testreader.successNotify();
  Serial.println("RFID Reading LED reader activates");
}

/* Can be ran with the RFID reader or simulated with second launchpad*/
static void testprocessTagSimulationRX(){
  char* tagString;
  while(Serial.available() <= 0){};
  
  if(Serial.available() > 0){
    tagString = testreader.processTag();
  }

  if(strcmp(tagString, "1E009A4067A3") == 0){
    Serial.println("processTag simulation 1 success");
  }
  else{
    Serial.println("processTag simulation 1 failed");
  }


  while(Serial.available() <= 0){};
  
  if(Serial.available() > 0){
    tagString = testreader.processTag();
  }

  if(strcmp(tagString, "01023C0A3207") == 0){
    Serial.println("processTag simulation 2 success");
  }
  else{
    Serial.println("processTag simulation 2 failed");
  }
}

/* This function needs to be uploaded to second launchpad to simulated reader */
static void testprocessTagSimulationTx(){
  Serial.write(2);
  Serial.print("1E009A4067A3");
  Serial.write(3);
  Serial.println("Sending first tag..");

  delay(10000);
  Serial.println("Done.");

  Serial.println("Sending second tag..");
  Serial.write(2);
  Serial.write("01023C0A3207");
  Serial.write(3);

  delay(10000);
  Serial.println("Done.");
}

static void testInit(){
  Serial.println("Initiating speaker...");
  mySpeak = Speaker();
  Serial.println("Speaker initiated");
}

static void testPos(){
  Serial.println("Playing positive tone...");
  mySpeak.playPositive();
  mySpeak.sleepSpeaker();
  Serial.println("Positive tone played");
}

static void testNeg(){
  Serial.println("Playing negative tone...");
  mySpeak.playNegative();
  mySpeak.sleepSpeaker();
  Serial.println("Negative tone played");
}

static void testFreq(int freq, int elapsedTime){
  Serial.print("Playing frequency ");
  Serial.print(freq);
  Serial.print(" for ");
  Serial.print(elapsedTime);
  Serial.println(" milliseconds...");
  mySpeak.playFreq(freq, elapsedTime);
  Serial.print("Frequency ");
  Serial.print(freq);
  Serial.print("played for ");
  Serial.print(elapsedTime);
  Serial.println(" millisecond");
}

static void testSleep(){
  mySpeak.playPositive();
  Serial.println("Putting speaker into lowpower mode...");
  mySpeak.sleepSpeaker();
  Serial.println("Speaker put into lowpower mode");
}


void loop() {
 Serial.println("Starting RFID Tests...");
 
 testInit(10);
 /* Uncomment to run the reader simulations. These require an RFID reader or a second
 launch pad running the TX simulation! */
 //testInit(10);        
 //testResetReader();
 //testprocessTagSimulationRX();

 /* Uncomment to run the reader simulation on second launch pad*/
 while(1){
 testprocessTagSimulationTx();
 }
 
 delay(5000);
 Serial.println("~~~ starting speaker test ~~~");
  delay(5000);
  testInit();
delay(5000);
  testPos();
delay(5000);
  testFreq(10000, 1000);
delay(5000);
  testFreq(10000, 5000);
delay(5000);
  testFreq(20000, 1000);
delay(5000);
  testFreq(20000, 5000);
delay(5000);
  testSleep();
delay(5000);
  Serial.println("~~~ speaker test completed, examine serial printer for errors ~~~");
  Serial.println();
  delay(5000);

  suspend();
  
}
