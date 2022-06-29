typedef struct{
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
const int brailleDotsCount = 7;

void setup() {
  for(int i = 0; i < brailleDotsCount; i++){
    pinMode(brailleDots[i], OUTPUT);
  }

  Serial.begin(9600);

  testSequence();
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

  for(int i = 0; i < message.length(); i++){
    for(int j = 0; j < brailleDLength; j++){
      if(brailleDict[j].key == message[i]){
        for(int k = 0; k < brailleDict[j].count; k++){
          Serial.print(brailleDict[j].value[k]);
          pinMode(brailleDots[brailleDict[j].value[k]], HIGH);
        }
        Serial.print(":");
        delay(1000);
        for(int k = 0; k < brailleDict[j].count; k++){
          pinMode(brailleDots[brailleDict[j].value[k]], LOW);
        }
        delay(1000);
        continue;
      }
    }
  }
  Serial.println("\ndone");
}














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
