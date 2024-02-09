#include "TypeWriter.h"
#define charsInRow 29
#define numberOfRows 24
#define pressDelay 200
#define degrees 360/9

// INPUTS
const int stopPosBtn = A3;
const int paperInsertedBtn = A4;

// OUTPTUS
const int motorPins[] = {A1, A2};
const int StepPinout[] = {13, 12, 9, 10, 11, A0};
const int brailleDots[] = {2, 3, 4, 5, 6, 7, 8}; // 2 - is for space because it's index is 0 and other ones indexes are same as brailleDict

TypeWriter BrailleTypeWriter(brailleDots, StepPinout, motorPins, stopPosBtn, paperInsertedBtn);

bool started = false;

void setup()
{
  Serial.begin(9600);
  BrailleTypeWriter.setUp(charsInRow, numberOfRows, pressDelay, degrees);
  //BrailleTypeWriter.test();
}

void loop()
{
  String input = getInput();

  // If the recieved message is "\\\r\n" then it initialises the print 
  // and the second time it recieves this message it ends the print
  if(input == "\\\r\n"){
    if(!started){
      BrailleTypeWriter.initPrint();
      started = true;
    }else{
      BrailleTypeWriter.endPrint();
      Serial.println("Ending print");
      started = false;
    }
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
