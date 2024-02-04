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

void setup()
{
  Serial.begin(9600);
  BrailleTypeWriter.setUp(charsInRow, numberOfRows, pressDelay, degrees);
  //BrailleTypeWriter.test();
}

void loop()
{
  String input = getInput();
  input = convert(input);
  
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

String convert(String input){
  // Converting input from ISO 8859-2 -- almost working
  for(int i = 0; i < input.length(); i++){
    switch(input[i]){
      // á
      case 0b11111111111111111111111111100001:
        input[i] = 'á';
        break;
      // č
      case 0b11111111111111111111111111101000:
        input[i] = 'č';
        break;
      // ď
      case 0b11111111111111111111111111101111:
        input[i] = 'ď';
        break;
      // é
      case 0b11111111111111111111111111101001:
        input[i] = 'é';
        break;
      // ě
      case 0b11111111111111111111111111101100:
        input[i] = 'ě';
        break;
      // í
      case 0b11111111111111111111111111101101:
        input[i] = 'í';
        break;
      // ň
      case 0b11111111111111111111111111110010:
        input[i] = 'ň';
        break;
      // ó
      case 0b11111111111111111111111111110011:
        input[i] = 'ó';
        break;
      // ř
      case 0b11111111111111111111111111111000:
        input[i] = 'ř';
        break;
      // š
      case 0b11111111111111111111111110111001:
        input[i] = 'š';
        break;
      // ť
      case 0b11111111111111111111111110111011:
        input[i] = 'ť';
        break;
      // ů
      case 0b11111111111111111111111111111001:
        input[i] = 'ů';
        break;
      // ú
      case 0b11111111111111111111111111111010:
        input[i] = 'ú';
        break;
      // ý
      case 0b11111111111111111111111111111101:
        input[i] = 'ý';
        break;
      // ž
      case 0b11111111111111111111111110111110:
        input[i] = 'ž';
        break;
      default:
        break;
    }  
  }

  return input;
}
