#include "TypeWriter.h"
#define charsInRow 29
#define numberOfRows 24
#define pressDelay 200
#define degrees 45

const int StepPinout[] = {13, 12, 9, 10, 11};
const int brailleDots[] = {2, 3, 4, 5, 6, 7, 8}; // 2 - is for space because it's index is 0 and other ones indexes are same as brailleDict

TypeWriter BrailleTypeWriter(brailleDots, StepPinout);

void setup()
{
  Serial.begin(9600);
  BrailleTypeWriter.setUp(charsInRow, numberOfRows, pressDelay, degrees);
  // BrailleTypeWriter.test();
}

void loop()
{
  String input = getInput();
  BrailleTypeWriter.print(input);
}

String getInput()
{
  while (Serial.available() == 0)
  {
    return Serial.readString();
  }
}
