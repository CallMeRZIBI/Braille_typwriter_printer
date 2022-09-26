#include "TypeWriter.h"

TypeWriter::TypeWriter(int solenoidPins[7], int stepperPins[4]){
  // Setting up pins for solenoids -- for some reason this doesnt work in for loop so I have to set them up like that
  // TODO: try in in for loop cause it's not called from setup function of arduino so it should work
  pinMode(solenoidPins[0], OUTPUT);
  pinMode(solenoidPins[1], OUTPUT);
  pinMode(solenoidPins[2], OUTPUT);
  pinMode(solenoidPins[3], OUTPUT);
  pinMode(solenoidPins[4], OUTPUT);
  pinMode(solenoidPins[5], OUTPUT);
  pinMode(solenoidPins[6], OUTPUT);

  _brailleDots[0] = solenoidPins[0];
  _brailleDots[1] = solenoidPins[1];
  _brailleDots[2] = solenoidPins[2];
  _brailleDots[3] = solenoidPins[3];
  _brailleDots[4] = solenoidPins[4];
  _brailleDots[5] = solenoidPins[5];
  _brailleDots[6] = solenoidPins[6];

  // Setting up pins for stepper Motor
  pinMode(stepperPins[0], OUTPUT);
  pinMode(stepperPins[1], OUTPUT);
  pinMode(stepperPins[2], OUTPUT);
  pinMode(stepperPins[3], OUTPUT);

  _stepperPins[0] = stepperPins[0];
  _stepperPins[1] = stepperPins[1];
  _stepperPins[2] = stepperPins[2];
  _stepperPins[3] = stepperPins[3];
}

void TypeWriter::setParameters(int rowLength, int pressDelay, double degrees){
  _rowLength = rowLength;
  _pressDelay = pressDelay;
  _degrees = degrees;
}

void TypeWriter::test(){
  Serial.println("Running test sequence");

  for(int i = 0; i < sizeof(_brailleDots) / sizeof(int); i++){
    digitalWrite(_brailleDots[i], HIGH);
    delay(_pressDelay);
  }
  for(int i = 0; i < sizeof(_brailleDots) / sizeof(int); i++){
    digitalWrite(_brailleDots[i], LOW);
    delay(_pressDelay);
  }

  Serial.println("done");
}

void TypeWriter::print(String message){
  if(message == NULL) return;

  int count;
  String *words;
  Split(message, &words, &count);

  // Printing
  bool onNewLine = false;
  int rowPos = 0;

  // Looping through every word
  for(int i = 0; i < count; i++){
    bool AllUpperCase = false;

    // Looping through every character of word
    for(int j = 0; j < words[i].length(); j++){
      for(int k = 0; k < _brailleDLength; k++){
        if(tolower(words[i][j]) == _brailleDict[k].key){

          // Checking if the next word won't need new line, if so,
          // after this row it will jump on new one
          if(rowPos + (i+1 > count ? 0 : words[i + 1].length()) > _rowLength && !onNewLine){
            onNewLine = true;
          }

          // Checking if character is first digit in digit sequence so it prints digit char
          if(isdigit(words[i][j])){
            if( j == 0 || (j != 0 && !isdigit(words[i][j-1]))){
              for(int l = 0; l < sizeof(_numberChar) / sizeof(int); l++){
                Serial.print(_numberChar[l]);
                digitalWrite(_brailleDots[_numberChar[l]], HIGH);
              }

              Serial.print(":");
              delay(_pressDelay);

              for(int l = 0; l < sizeof(_numberChar) / sizeof(int); l++){
                digitalWrite(_brailleDots[_numberChar[l]], LOW);
              }

              delay(_pressDelay);
            }
          }

          // Checking if character is uppercase to add upperCase character
          // Adding two upperCase characters if the whole word is upper case
          if(isUpperCase(words[i][j]) && !AllUpperCase){
            int upperChars = 1;

            // Check if it's first upperCase in word -- when there's no space after the all upper word than it's just messing up
            if(j == 0){
              int l = 0;
              int upper = 0;

              // Loops through all the chars of the word to check if they're all upper case
              do{
                // Idk anymore
                if(words[i][l] == ' '){
                  break;
                }

                upper += isUpperCase(words[i][l]);
                l++;
              }while(l < words[i].length());

              Serial.println(l);
              Serial.println(upper);

              if(l == upper){ // All characters of the word are upper case
                upperChars = 2;
                AllUpperCase = true;
              }
            }

            // Printing the uppercase character, twice it it's all uppercase
            for(int c = 0; c < upperChars; c++){
              Serial.print("6");
              digitalWrite(_brailleDots[6], HIGH);

              Serial.print(":");
              delay(_pressDelay);

              digitalWrite(_brailleDots[6], LOW);
            }
          }

          // Presses every corresponding solenoid to the character
          for(int l = 0; l < _brailleDict[k].count; l++){
            Serial.print(_brailleDict[k].value[l]);
            digitalWrite(_brailleDots[_brailleDict[k].value[l]], HIGH);
          }

          Serial.print(":");
          delay(_pressDelay);

          // Releases all of those solenoids
          for(int l = 0; l < _brailleDict[k].count; l++){
            digitalWrite(_brailleDots[_brailleDict[k].value[l]], LOW);
          }

          delay(_pressDelay);
          rowPos++;
          continue;
        }
      }
    }
    if(onNewLine){
      Serial.print("\n");
      newLine();
      rowPos = 0;
      onNewLine = false;
    }
  }
  delete[] words;
  Serial.println("\ndone");
}

void TypeWriter::Split(String message, String **words, int *count){
  // Split message into words
  int wCount = 0;
  // Counting all spaces for array size declaration
  for(int i = 0; i < message.length(); i++){
    if(message[i] == ' ') wCount++;
    if(i == message.length() - 1) wCount++;
  }

  String *wordsArr = new String[wCount];

  // Adding words to array
  String s = "";
  int Count = 0;
  for(int i = 0; i < message.length(); i++){
    if(message[i] == ' ' || i == message.length() - 1){
      if(i == message.length()) wordsArr[Count] = s + message[i];
      else wordsArr[Count] = s + " ";
      s = "";
      Count++;
      continue;
    }

    s += message[i];
  }

  *words = wordsArr;
  *count = Count;
}

void TypeWriter::newLine(){
  // Dividing 360 degrees by the degrees to spin and then dividing 2048
  // (one spin) by te result of previous division, by that it will spin
  // by wanted degrees
  for(int i = 0; i < 2048 / (360 / _degrees); i++){
    OneStep(true);
    delay(2);
  }
}

void TypeWriter::OneStep(bool dir){
  if(dir){
    switch(_stepNumber){
      case 0:
        digitalWrite(_stepperPins[0], HIGH);
        digitalWrite(_stepperPins[1], LOW);
        digitalWrite(_stepperPins[2], LOW);
        digitalWrite(_stepperPins[3], LOW);
        break;

      case 1:
        digitalWrite(_stepperPins[0], LOW);
        digitalWrite(_stepperPins[1], HIGH);
        digitalWrite(_stepperPins[2], LOW);
        digitalWrite(_stepperPins[3], LOW);
        break;

      case 2:
        digitalWrite(_stepperPins[0], LOW);
        digitalWrite(_stepperPins[1], LOW);
        digitalWrite(_stepperPins[2], HIGH);
        digitalWrite(_stepperPins[3], LOW);
        break;

      case 3:
        digitalWrite(_stepperPins[0], LOW);
        digitalWrite(_stepperPins[1], LOW);
        digitalWrite(_stepperPins[2], LOW);
        digitalWrite(_stepperPins[3], HIGH);
        break;
    }
  }
  else{
    switch(_stepNumber){
      case 0:
        digitalWrite(_stepperPins[0], LOW);
        digitalWrite(_stepperPins[1], LOW);
        digitalWrite(_stepperPins[2], LOW);
        digitalWrite(_stepperPins[3], HIGH);
        break;

      case 1:
        digitalWrite(_stepperPins[0], LOW);
        digitalWrite(_stepperPins[1], LOW);
        digitalWrite(_stepperPins[2], HIGH);
        digitalWrite(_stepperPins[3], LOW);
        break;

      case 2:
        digitalWrite(_stepperPins[0], LOW);
        digitalWrite(_stepperPins[1], HIGH);
        digitalWrite(_stepperPins[2], LOW);
        digitalWrite(_stepperPins[3], LOW);
        break;

      case 3:
        digitalWrite(_stepperPins[0], HIGH);
        digitalWrite(_stepperPins[1], LOW);
        digitalWrite(_stepperPins[2], LOW);
        digitalWrite(_stepperPins[3], LOW);
        break;
    }
  }

  _stepNumber++;
  if(_stepNumber > 3){
    _stepNumber = 0;
  }
}
