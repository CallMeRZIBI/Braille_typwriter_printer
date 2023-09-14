#include "TypeWriter.h"

TypeWriter::TypeWriter(int solenoidPins[7], int stepperPins[5])
    : _stepper(200, (short)stepperPins[0], (short)stepperPins[1], (short)stepperPins[2], (short)stepperPins[3], (short)stepperPins[4])
{ // The 200 is steps per revolution
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
}

void TypeWriter::setUp(int rowLength, int pressDelay, double degrees)
{
  // Stepper initialisation is here, because you cannot initialise it before Arduinos setup() method
  // Maybye I also should put there pinModes, because on this forum: https://forum.arduino.cc/t/expected-initializer-before-with-my-own-library/537053/11 it is said that it's bad practise to put
  // it before the setup() method
  _stepper.begin(1, 1);

  _rowLength = rowLength;
  _pressDelay = pressDelay;
  _degrees = degrees;
}

void TypeWriter::test()
{
  Serial.println("Running test sequence");

  for (int i = 0; i < sizeof(_brailleDots) / sizeof(int); i++)
  {
    digitalWrite(_brailleDots[i], HIGH);
    delay(_pressDelay);
  }
  for (int i = 0; i < sizeof(_brailleDots) / sizeof(int); i++)
  {
    digitalWrite(_brailleDots[i], LOW);
    delay(_pressDelay);
  }

  Serial.println("done");
}

void TypeWriter::print(String message)
{
  if (message == NULL)
    return;

  int count;
  String *words;
  Split(message, &words, &count);

  // Printing
  bool onNewLine = false;
  int rowPos = 0;

  // Looping through every word
  for (int i = 0; i < count; i++)
  {
    bool AllUpperCase = false;

    // Looping through every character of word
    for (int j = 0; j < words[i].length(); j++)
    {
      for (int k = 0; k < _brailleDLength; k++)
      {
        if (tolower(words[i][j]) == _brailleDict[k].key)
        {

          // Checking if the next word won't need new line, if so,
          // after this row it will jump on new one
          if (rowPos + (i + 1 > count ? 0 : words[i + 1].length()) > _rowLength && !onNewLine)
          {
            onNewLine = true;
          }

          // Checking if character is first digit in digit sequence so it prints digit char
          if (isdigit(words[i][j]))
          {
            if (j == 0 || (j != 0 && !isdigit(words[i][j - 1])))
            {
              for (int l = 0; l < sizeof(_numberChar) / sizeof(int); l++)
              {
                Serial.print(_numberChar[l]);
                digitalWrite(_brailleDots[_numberChar[l]], HIGH);
              }

              Serial.print(":");
              delay(_pressDelay);

              for (int l = 0; l < sizeof(_numberChar) / sizeof(int); l++)
              {
                digitalWrite(_brailleDots[_numberChar[l]], LOW);
              }

              delay(_pressDelay);
            }
          }

          // Checking if character is uppercase to add upperCase character or upperCase character for whole word
          if (isUpperCase(words[i][j]) && !AllUpperCase)
          {
            // Check if it's first upperCase in word -- when there's no space after the all upper word then it's just messing up
            if (j == 0)
            {
              AllUpperCase = true;

              for (auto &c : words[i])
              {
                if (!isUpperCase(static_cast<unsigned char>(c)) && (int)c != 13)
                { // For some reason when there's just a word without space
                  // after it it adds CR character which ASCII value is 13
                  AllUpperCase = false;
                }
              }
            }

            // Printing the uppercase character either for whole word or just one letter
            if (AllUpperCase)
            {
              for (int l = 0; l < sizeof(_upperCaseWord) / sizeof(_upperCaseWord[0]); l++)
              {
                Serial.print(_upperCaseWord[l]);
                digitalWrite(_upperCaseWord[l], HIGH);
              }

              Serial.print(":");
              delay(_pressDelay);

              for (int l = 0; l < sizeof(_upperCaseWord) / sizeof(_upperCaseWord[0]); l++)
              {
                digitalWrite(_upperCaseWord[l], LOW);
              }

              delay(_pressDelay);
            }
            else
            { // Otherwise printing just uppercase character
              Serial.print(_upperCase[0]);
              digitalWrite(_upperCase[0], HIGH);
              Serial.print(":");
              delay(_pressDelay);
              digitalWrite(_upperCase[0], LOW);
              delay(_pressDelay);
            }
          }

          // Presses every corresponding solenoid to the character
          for (int l = 0; l < _brailleDict[k].count; l++)
          {
            Serial.print(_brailleDict[k].value[l]);
            digitalWrite(_brailleDots[_brailleDict[k].value[l]], HIGH);
          }

          Serial.print(":");
          delay(_pressDelay);

          // Releases all of those solenoids
          for (int l = 0; l < _brailleDict[k].count; l++)
          {
            digitalWrite(_brailleDots[_brailleDict[k].value[l]], LOW);
          }

          delay(_pressDelay);
          rowPos++;
          continue;
        }
      }
    }

    // Space after every word, cause the word separator is also space
    Serial.print("0:");
    digitalWrite(_brailleDots[_brailleDict[26].value[0]], HIGH);
    delay(_pressDelay);
    digitalWrite(_brailleDots[_brailleDict[26].value[0]], LOW);
    delay(_pressDelay);

    // New Line
    if (onNewLine)
    {
      Serial.print("\n");
      newLine();
      rowPos = 0;
      onNewLine = false;
    }
  }
  delete[] words;
  Serial.println("\ndone");
}

void TypeWriter::Split(String message, String **words, int *count)
{
  // Split message into words
  int wCount = 0;
  // Counting all spaces for array size declaration
  for (int i = 0; i < message.length(); i++)
  {
    if (message[i] == ' ')
      wCount++;
    if (i == message.length() - 1)
      wCount++;
  }

  String *wordsArr = new String[wCount];

  // Adding words to array
  String s = "";
  int Count = 0;
  for (int i = 0; i < message.length(); i++)
  {
    if (message[i] == ' ' || i == message.length() - 1)
    {
      if (i == message.length())
        wordsArr[Count] = s + message[i];
      else
        wordsArr[Count] = s; //+ " ";
      s = "";
      Count++;
      continue;
    }

    s += message[i];
  }

  *words = wordsArr;
  *count = Count;
}

void TypeWriter::newLine()
{
  // Method with Nema 17 stepper motor and A4988 controller
  _stepper.rotate((int)_degrees);
}
