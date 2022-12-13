#include "TypeWriter.h"
#define charsInRow 29
#define pressDelay 200
#define degrees 30

const int StepPinout[] = {13,12,11,10,9};
const int brailleDots[] = {2,3,4,5,6,7,8};  // 2 - is for space because it's index is 0 and other ones indexes are same as brailleDict

TypeWriter BrailleTypeWriter(brailleDots, StepPinout);

void setup(){
  Serial.begin(9600);
  BrailleTypeWriter.setParameters(charsInRow, pressDelay, degrees);
  //BrailleTypeWriter.test();
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
