#include "TypeWriter.h"
#define charsInRow 29
#define numberOfRows 24
#define pressDelay 200
#define degrees 360/9

// INPUTS
const int stopPosBtn = A2;

// OUTPTUS
const int motorPin = A1;
const int StepPinout[] = {13, 12, 9, 10, 11, A0};
const int brailleDots[] = {2, 3, 4, 5, 6, 7, 8}; // 2 - is for space because it's index is 0 and other ones indexes are same as brailleDict

TypeWriter BrailleTypeWriter(brailleDots, StepPinout, motorPin, stopPosBtn);

void setup()
{
  Serial.begin(9600);
  BrailleTypeWriter.setUp(charsInRow, numberOfRows, pressDelay, degrees);
  // BrailleTypeWriter.test();
}

void loop()
{
  String input = getInput();
  if(input == "\\\r\n"){
    BrailleTypeWriter.endPrint();
    Serial.println("Ending print");
  }else{
    BrailleTypeWriter.print(input);
  }
}

String getInput()
{
  while (Serial.available() == 0)
  {
    return Serial.readString();
  }
}
