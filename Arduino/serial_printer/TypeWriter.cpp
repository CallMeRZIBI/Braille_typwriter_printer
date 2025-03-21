#include "TypeWriter.h"

TypeWriter::TypeWriter(int solenoidPins[7], int stepperPins[6], int motorPins[2], int endPos, int paperInserted)
    : _stepper(200, (short)stepperPins[0], (short)stepperPins[1], (short)stepperPins[2], (short)stepperPins[3], (short)stepperPins[4]) // The 200 is steps per revolution
{
  // Setting pin for A4988 drivers sleep pin
  // sleep pin is connected straight to MCU and reset must be connected to logical HIGH to be disbaled all the time
  _stepperSleep = stepperPins[5];
  pinMode(_stepperSleep, OUTPUT);
  digitalWrite(_stepperSleep, LOW);

  // Setting pin for motor, for horizontal movement
  _motorPins[0] = motorPins[0];
  _motorPins[1] = motorPins[1];
  pinMode(_motorPins[0], OUTPUT);
  pinMode(_motorPins[1], OUTPUT);

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
  _brailleDots[1] = solenoidPins[3]; // These three are flipped because on Pichťák it's like this: 321 456:
  _brailleDots[2] = solenoidPins[2];
  _brailleDots[3] = solenoidPins[1];
  _brailleDots[4] = solenoidPins[4];
  _brailleDots[5] = solenoidPins[5];
  _brailleDots[6] = solenoidPins[6];

  // Initializing stop position button pin
  _endPos = endPos;
  pinMode(_endPos, INPUT);

  // Initializing paper inserted button pin
  _paperInserted = paperInserted;
  pinMode(_paperInserted, INPUT);
}

void TypeWriter::setUp(int rowLength, int rowCount, int pressDelay, double degrees)
{
  // Stepper initialisation is here, because you cannot initialise it before Arduinos setup() method
  // Maybye I also should put there pinModes, because on this forum: https://forum.arduino.cc/t/expected-initializer-before-with-my-own-library/537053/11 it is said that it's bad practise to put
  // it before the setup() method
  _stepper.begin(50, 16);

  digitalWrite(_motorPins[0], LOW);
  digitalWrite(_motorPins[1], LOW);

  _rowLength = rowLength;
  _rowCount = rowCount;
  _pressDelay = pressDelay;
  _degrees = degrees;
}

void TypeWriter::test()
{
  Serial.println("Running test sequence");
  int testSequence[7] = {0, 1, 2, 3, 4, 5, 6};

  printChar(testSequence, 7, true);

  for(int i = 0; i < 7; i++){
    int actualSolenoid[] = {testSequence[i]};
    printChar(actualSolenoid, 1, true);
  }

  Serial.println("done");
}

void TypeWriter::print(String &message)
{
  if (message == NULL)
    return;

  int count;
  String *words;
  Split(message, &words, &count);

  // Printing
  // Looping through every word
  for (int i = 0; i < count; i++)
  {
    // Check if this word is longer than remaining space, if so go to new line and then print the word
    if (checkForNewLine(_rowPos, words[i]))
    {
      newLine();
    }

    bool AllUpperCase = false;

    // Looping through every character of word
    for (int j = 0; j < words[i].length(); j++)
    {
      // Checking for special characters in word
      if(words[i][j] == '\r'){
        newLine();
        // Skipping to next character if this character is special character
        continue;  
      }
      
      // Looping through every character of dictionary
      for (int k = 0; k < _brailleDLength; k++)
      {
        
        // Comparing character of word with character from dictionary
        if (tolower(words[i][j]) == _brailleDict[k].key)
        {
          
          // Checking if character is first digit in digit sequence so it prints digit char
          if (isdigit(words[i][j]))
          {
            if (j == 0 || (j != 0 && !isdigit(words[i][j - 1])))
            {
              int numberCharLen = sizeof(_numberChar) / sizeof(_numberChar[0]);
              printChar(_numberChar, numberCharLen, true);
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
              int upperCaseWordLen = sizeof(_upperCaseWord) / sizeof(_upperCaseWord[0]);
              printChar(_upperCaseWord, upperCaseWordLen, true);
            }
            else
            { // Otherwise printing just uppercase character
              printChar(_upperCase, 1, true);
            }
          }

          // Write character
          printChar(_brailleDict[k].value, _brailleDict[k].count, true);
          _rowPos++;
          continue;
        }
      }
    }

    // Space after every word, cause the word separator is also space
    printChar(_brailleDict[26].value, _brailleDict[26].count, true);
    _rowPos++;
  }
  delete[] words;
  Serial.println("\ndone");
}

void TypeWriter::printChar(int *value, int length, bool display)
{
  // Presses every corresponding solenoid to the character
  for (int i = 0; i < length; i++)
  {
    // Send keys to serial only if display = true
    if (display)
      Serial.print(value[i]);
    digitalWrite(_brailleDots[value[i]], HIGH);
  }

  if (display)
    Serial.print(":");

  unwind();

  // Waiting for a bit because of the mechanism of typewriter
  delay(_pressDelay - _MUnT);

  // Releases all of those solenoids
  for (int i = 0; i < length; i++)
  {
    digitalWrite(_brailleDots[value[i]], LOW);
  }

  delay(_pressDelay);
}

// Reseting row and line positions after the whole print, because I'm sending text by chunks,
// wich are not the same size, so it's not by lines
void TypeWriter::endPrint()
{
  // Getting the holder on the starting position, so that it can be rotated
  while (!digitalRead(_endPos))
  {
    digitalWrite(_motorPins[0], HIGH);
    digitalWrite(_motorPins[1], LOW);
  }
  digitalWrite(_motorPins[0], LOW);
  digitalWrite(_motorPins[1], LOW);
  
  // Returning the paper by rotating it the number of left lines
  digitalWrite(_stepperSleep, HIGH); // Waking up controller
  delay(1);
  _stepper.rotate((int)_degrees * 4 * (-1) * (_rowCount - _linePos));
  digitalWrite(_stepperSleep, LOW);

  _rowPos = 0;
  _linePos = 0;
}

// This method is just for inserting paper on the start of each print
// It uses the insertPaper and unwind methods
void TypeWriter::initPrint()
{
  insertPaper();
  unwind();
  Serial.println("loaded paper");
}

bool TypeWriter::checkForNewLine(int rowPos, String word)
{
  // Checking if the next word won't need new line
  // "ThIsSS" is actually going to make it look like this:
  // SpecialChar - T - h - SpecialChar - I - s - SpecialChar - S - SpecialChar - S
  // Which is 3 characters longer, and that's a short example

  // Removing last character if it's CR
  int wordLen = word.length();
  if(word.indexOf(char(13)) > 0)  wordLen--;

  int specialChars = 0;

  // Check if it's digit
  int numChars = 0;

  for (int i = 0; i < wordLen; i++)
  {
    if (isdigit(word[i]))
    {
      if (i == 0 || (i != 0 && !isdigit(word[i - 1])))
      {
        numChars++;
      }
    }
  }

  // Check for upperCase characters
  bool allUpper = false;
  int upperChars = 0;

  // Check if it's first upperCase in word -- when there's no space after the all upper word then it's just messing up
  if (isUpperCase(word[0]))
  {
    allUpper = true;

    // Looping through all characters in word
    for (auto &c : word)
    {
      if (!isUpperCase(static_cast<unsigned char>(c)) && (int)c != 13)
      { // For some reason when there's just a word without space
        // after it it adds CR character which ASCII value is 13
        allUpper = false;
        break;
      }
    }
  }

  // If it's all uppercase there's just one special character, otherwise add special character for every single uppercase letter
  if (allUpper)
    upperChars = 1;
  else
  {
    for (int i = 0; i < wordLen; i++)
    {
      if (isUpperCase(word[i]))
        upperChars++;
    }
  }

  specialChars = numChars + upperChars;
  return ((specialChars + rowPos + wordLen) > _rowLength) ? true : false;
}

// Unwind a bit of string, so there is no resistance when printing new letter
void TypeWriter::unwind()
{
  digitalWrite(_motorPins[0], LOW);
  digitalWrite(_motorPins[1], HIGH);

  delay(_MUnT);

  digitalWrite(_motorPins[0], LOW);
  digitalWrite(_motorPins[1], LOW);
}

// Waiting for user to insert paper
void TypeWriter::insertPaper()
{
    Serial.println("Insert paper");
    
    // Wait for button to get pressed
    while (!digitalRead(_paperInserted)){}

    // First move horizontally the paper on the start position
    while (!digitalRead(_endPos))
    {
      digitalWrite(_motorPins[0], HIGH);
      digitalWrite(_motorPins[1], LOW);
    }
    digitalWrite(_motorPins[0], LOW);
    digitalWrite(_motorPins[1], LOW);

    // Loading the paper by rotating it the number of lines
    digitalWrite(_stepperSleep, HIGH); // Waking up controller
    delay(1);
    _stepper.rotate((int)_degrees * 4 * _rowCount);
    digitalWrite(_stepperSleep, LOW);

    Serial.print("\n");
}

// Moving on new line
void TypeWriter::newLine()
{
  Serial.print("\n");
  
  _linePos++;
  _rowPos = 0;

  // If the new line would be out of the range of paper, then it ejects paper and waits for user to insert new one
  if (_linePos > _rowCount)
  {
    // Eject the paper and wait for new one to be inserted
    endPrint();
    insertPaper();
  }

  // First move horizontally the paper on the start position
  while (!digitalRead(_endPos))
  {
    digitalWrite(_motorPins[0], HIGH);
    digitalWrite(_motorPins[1], LOW);
  }
  digitalWrite(_motorPins[0], LOW);
  digitalWrite(_motorPins[1], LOW);

  // Then move vertically paper on new line
  // Method with Nema 17 stepper motor and A4988 controller
  digitalWrite(_stepperSleep, HIGH); // Waking up controller
  delay(1);
  _stepper.rotate((int)_degrees * 4 * (-1));
  digitalWrite(_stepperSleep, LOW);

  unwind();
}

void TypeWriter::Split(String message, String **words, int *count)
{
  // Split message into words
  int wCount = 0;
  // Counting all spaces for array size declaration
  for (int i = 0; i < message.length(); i++)
  {
    if (message[i] == ' ' || (message[i] == '\r' && message[i+1] == '\n'))
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
    // Removing spaces and last two characters wich are CR LR
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
