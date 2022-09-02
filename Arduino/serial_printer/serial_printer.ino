#include "TypeWriter.h"
#define charsInRow 29
#define pressDelay 200

const int StepPinout[] = {13,12,11,10};
const int brailleDots[] = {2,3,4,5,6,7,8};  // 2 - is for space because it's index is 0 and other ones are then indexed like normally

TypeWriter BrailleTypeWriter(brailleDots, StepPinout);

void setup(){
  Serial.begin(9600);
  BrailleTypeWriter.setParameters(charsInRow, pressDelay);
  BrailleTypeWriter.test();
}

void loop(){
  String input = getInput();
  BrailleTypeWriter.print(input);
}

String getInput(){
  while(Serial.available() == 0){
    return Serial.readString();
  }
}





//-------------------------------------------------Method without class---------------





/*typedef struct{
  char key;
  int *value;
  int count;    // Temporary
  } brailleChar;

#define brailleDLength 26
const brailleChar brailleDict[] = {{'a', new int[1]{1}, 1},
                                    {'b', new int[2]{1,2}, 2},
                                    {'c', new int[2]{1,4}, 2},
                                    {'d', new int[3]{1,4,5}, 3},
                                    {'e', new int[2]{1,5}, 2},
                                    {'f', new int[3]{1,2,4}, 3},
                                    {'g', new int[4]{1,2,4,5}, 4},
                                    {'h', new int[3]{1,2,5}, 3},
                                    {'i', new int[2]{2,4}, 2},
                                    {'j', new int[3]{2,4,5}, 3},
                                    {'k', new int[2]{1,3}, 2},
                                    {'l', new int[3]{1,2,3}, 3},
                                    {'m', new int[3]{1,3,4}, 3},
                                    {'n', new int[4]{1,3,4,5}, 4},
                                    {'o', new int[3]{1,3,5}, 3},
                                    {'p', new int[4]{1,2,3,4}, 4},
                                    {'q', new int[5]{1,2,3,4,5}, 5},
                                    {'r', new int[4]{1,2,3,5}, 4},
                                    {'s', new int[3]{2,3,4}, 3},
                                    {'t', new int[4]{2,3,4,5}, 4},
                                    {'u', new int[3]{1,3,6}, 3},
                                    {'v', new int[4]{1,2,3,6}, 4},
                                    {'w', new int[4]{2,4,5,6}, 4},
                                    {'x', new int[4]{1,3,4,6}, 4},
                                    {'y', new int[5]{1,3,4,5,6}, 5},
                                    {'z', new int[4]{1,3,5,6}, 4},
                                    {' ', new int[1]{0}, 1}
                                    };

const int brailleDots[] = {2,3,4,5,6,7,8};  // 2 - is for space because it's index is 0 and other ones are then indexed like normally
#define brailleDotsCount 7
#define charsInRow 29
#define pressDelay 150

#define STEP_PIN_1 13
#define STEP_PIN_2 12
#define STEP_PIN_3 11
#define STEP_PIN_4 10

void setup() {
  // For some reason setting up pins in for loop is not working properly because the setup just cannot run for loop properly  // For some reason setting up pins in for loop is not working properly because the setup just cannot run for loop properly
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  // Pinmode for stepper motor
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);

  Serial.begin(9600);

  //testSequence();
  //Serial.println("Done test sequence");
}

void testSequence(){
  for(int i = 0; i< sizeof(brailleDots) / sizeof(int); i++){
    pinMode(brailleDots[i], HIGH);
    delay(500);
  }
  for(int i = 0; i< sizeof(brailleDots) / sizeof(int); i++){
    pinMode(brailleDots[i], LOW);
    delay(500);
  }
}

void loop() {
  String input = getInput();
  //Serial.println(input);
  braillePrint(input);
}

String getInput(){
  while(Serial.available() == 0){
    return Serial.readString();
  }
}

void braillePrint(String message){
  if(message == NULL) return;

  int rowPos = 0;
  for(int i = 0; i < message.length(); i++){
    for(int j = 0; j < brailleDLength; j++){
      if(brailleDict[j].key == message[i]){

        // When on the end of line it will go to new line
        if(rowPos >= charsInRow){
          Serial.print("\n");
          newLine();
          rowPos = 0;
        }

        // Presses every corresponding solenoid to the character
        for(int k = 0; k < brailleDict[j].count; k++){
          Serial.print(brailleDict[j].value[k]);
          digitalWrite(brailleDots[brailleDict[j].value[k]], HIGH);
        }
        
        Serial.print(":");
        delay(pressDelay);

        // Releases all of those solenoids
        for(int k = 0; k < brailleDict[j].count; k++){
          digitalWrite(brailleDots[brailleDict[j].value[k]], LOW);
        }

        delay(pressDelay);
        rowPos++;
        continue;
      }
    }
  }
  Serial.println("\ndone");
}

void newLine(){
  for(int i = 0; i < 2048 / 12; i++){
    OneStep(true);
    delay(2);
  }
}

int step_number = 0;
void OneStep(bool dir){
  if(dir){
    switch(step_number){
      case 0:
        digitalWrite(STEP_PIN_1, HIGH);
        digitalWrite(STEP_PIN_2, LOW);
        digitalWrite(STEP_PIN_3, LOW);
        digitalWrite(STEP_PIN_4, LOW);
        break;

      case 1:
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, HIGH);
        digitalWrite(STEP_PIN_3, LOW);
        digitalWrite(STEP_PIN_4, LOW);
        break;

      case 2:
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, LOW);
        digitalWrite(STEP_PIN_3, HIGH);
        digitalWrite(STEP_PIN_4, LOW);
        break;

      case 3:
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, LOW);
        digitalWrite(STEP_PIN_3, LOW);
        digitalWrite(STEP_PIN_4, HIGH);
        break;
    }      
  }
  else{
    switch(step_number){
      case 0:
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, LOW);
        digitalWrite(STEP_PIN_3, LOW);
        digitalWrite(STEP_PIN_4, HIGH);
        break;

      case 1:
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, LOW);
        digitalWrite(STEP_PIN_3, HIGH);
        digitalWrite(STEP_PIN_4, LOW);
        break;

      case 2:
        digitalWrite(STEP_PIN_1, LOW);
        digitalWrite(STEP_PIN_2, HIGH);
        digitalWrite(STEP_PIN_3, LOW);
        digitalWrite(STEP_PIN_4, LOW);
        break;

      case 3:
        digitalWrite(STEP_PIN_1, HIGH);
        digitalWrite(STEP_PIN_2, LOW);
        digitalWrite(STEP_PIN_3, LOW);
        digitalWrite(STEP_PIN_4, LOW);
        break;
    }   
  }

  step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}*/












// -----------AS FOOL I DIDN'T GOOGLE HOW TO DYNAMIC ARRAY IN C++ BEFORE DOING ANYTHING-------------------
// https://forum.arduino.cc/t/separating-a-comma-delimited-string/682868/5
/*void braillePrint(String text){

  if(text == NULL) return;

  Serial.println(text);

  char *ptr = NULL;
  for(int i = 0; i < text.length(); i++){
    for(int j = 0; j < 8; j++){
      if(brailleDict[j].key == text[i]){
        char **dots;
        byte index = 0;
        char c_text[20];
        brailleDict[j].value.toCharArray(c_text, 20);
        ptr = strtok(c_text, ",");
        while(ptr != NULL){
          dots[index] = ptr;
          index++;
          ptr = strtok(NULL, ",");
        }

        for(int i = 0; i < index; i++){
          Serial.println(dots[i]);
        }
      }
    }
  }
}


String messageCommaCount = String(brailleDict[j].value);

        // Getting the cound of commas for allocating memory for int array corresponding to individual indexes of commas
        int size = 0;
        while(messageCommaCount.indexOf(',') != -1){
          Serial.println(messageCommaCount);
          size++;
          messageCommaCount.remove(messageCommaCount.indexOf(','));
        }

        int dotPos[size];

        // Getting the values from string
        for(int i = 0; i < size; i++){
          dotPos[i] = text.substring(text.indexOf(',') - 1).toInt();
        }*/
