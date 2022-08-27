#include "TypeWriter.h"

TypeWriter::TypeWriter(int solenoidPins[7], int stepperPins[4]){
  // Setting up pins for solenoids -- for some reason this doesnt work in for loop so I have to set them up like that
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

void TypeWriter::setParameters(int rowLength, int pressDelay){
  _rowLength = rowLength;
  _pressDelay = pressDelay;
}

// Write flawless printing algorithm that isn't creating
// new line in middle of a word etc.
/*void TypeWriter::print(String message){
  if(message == NULL) return;

  int rowPos = 0;
  for(int i = 0; i < message.length(); i++){
    for(int j = 0; j < _brailleDLength; j++){
      if(_brailleDict[j].key == message[i]){

        // When on the end of line it will go to new line
        if(rowPos >= _rowLength){
          Serial.print("\n");
          newLine();
          rowPos = 0;
        }

        // Presses every corresponding solenoid to the character
        for(int k = 0; k < _brailleDict[j].count; k++){
          Serial.print(_brailleDict[j].value[k]);
          digitalWrite(_brailleDots[_brailleDict[j].value[k]], HIGH);
        }

        Serial.print(":");
        delay(_pressDelay);

        // Releases all of those solenoids
        for(int k = 0; k < _brailleDict[j].count; k++){
          digitalWrite(_brailleDots[_brailleDict[j].value[k]], LOW);
        }

        delay(_pressDelay);
        rowPos++;
        continue;
      }
    }
  }
  Serial.println("\ndone");
}*/

void TypeWriter::print(String message){
  if(message == NULL) return;

  // Split message into words
  int wCount = 0;
  // Counting all spaces for array size declaration
  for(int i = 0; i < message.length(); i++){
    if(message[i] == ' ') wCount++;
    if(i == message.length() - 1) wCount++;
  }

  String words[wCount];

  // Adding words to array
  String s = "";
  int count = 0;
  for(int i = 0; i < message.length(); i++){
    if(message[i] == ' ' || i == message.length() - 1){
      if(i == message.length()) words[count] = s + message[i];
      else words[count] = s + " ";
      s = "";
      count++;
      continue;
    }

    s += message[i];
  }

  for(int i = 0; i < wCount; i++){
    //for(int j = 0; j < words[i].length(); j++)
    Serial.print(words[i]);
  }

  // Printing
  bool onNewLine = false;
  int rowPos = 0;
  // Looping through every word
  for(int i = 0; i < wCount; i++){
    // Looping through every character of word -- TODO: it ain't looping through that :(
    for(int j = 0; j < words[i].length(); i++){
      for(int k = 0; k < _brailleDLength; k++){
        if(words[i][j] == _brailleDict[k].key){
          // Checking if the next word won't need new line, if so,
          // after this row it will jump on new one
          if(rowPos + words[wCount + 1].length() > _rowLength && !onNewLine){
            onNewLine = true;
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
  Serial.println("\ndone");
}

void TypeWriter::newLine(){
  for(int i = 0; i < 2048 / 12; i++){
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
