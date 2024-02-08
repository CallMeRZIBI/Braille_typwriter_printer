#ifndef TypeWriter_h
#define TypeWriter_h
#include <Arduino.h>
#include <ctype.h>
#include "A4988.h"

class TypeWriter
{
public:
  TypeWriter(int solenoidPins[7], int stepperPins[5], int motorPins[2], int endPos, int paperInserted);
  void setUp(int rowLength, int rowCount, int pressDelay, double degrees);
  void print(String &message);
  void endPrint();
  void test();

private:
  int _endPos;
  int _paperInserted;
  int _motorPins[2];
  int _rowPos = 0;
  int _linePos = 0;
  int _brailleDots[7];
  A4988 _stepper;
  int _stepperSleep;
  int _MUnT = 40;
  int _pressDelay = 150;
  int _rowLength = 24;
  int _rowCount = 24; // idk how many
  double _degrees = 30;
  int _brailleDotsCount = 7;
  static const int _brailleDLength = 70;

  void printChar(int *value, int length, bool display = false);
  bool checkForNewLine(int rowPos, String word);
  void newLine();
  void Split(String message, String **words, int *count);

  // "Dictionary"
  typedef struct
  {
    char key;
    int *value;
    int count; // Temporary
  } _brailleChar;

  const int _numberChar[4] = {3, 4, 5, 6};
  const int _upperCase[1] = {6};
  const int _upperCaseWord[2] = {5, 6};

  // I really don't know why was this the problem (probably because this is a header file)
  // but I had to add size to _brailledict to compile
  // This is now Czech braille alphabet
  const _brailleChar _brailleDict[_brailleDLength] = {{'a', new int[1]{1}, 1},
                                                      {'b', new int[2]{1, 2}, 2},
                                                      {'c', new int[2]{1, 4}, 2},
                                                      {'d', new int[3]{1, 4, 5}, 3},
                                                      {'e', new int[2]{1, 5}, 2},
                                                      {'f', new int[3]{1, 2, 4}, 3},
                                                      {'g', new int[4]{1, 2, 4, 5}, 4},
                                                      {'h', new int[3]{1, 2, 5}, 3},
                                                      {'i', new int[2]{2, 4}, 2},
                                                      {'j', new int[3]{2, 4, 5}, 3},
                                                      {'k', new int[2]{1, 3}, 2},
                                                      {'l', new int[3]{1, 2, 3}, 3},
                                                      {'m', new int[3]{1, 3, 4}, 3},
                                                      {'n', new int[4]{1, 3, 4, 5}, 4},
                                                      {'o', new int[3]{1, 3, 5}, 3},
                                                      {'p', new int[4]{1, 2, 3, 4}, 4},
                                                      {'q', new int[5]{1, 2, 3, 4, 5}, 5},
                                                      {'r', new int[4]{1, 2, 3, 5}, 4},
                                                      {'s', new int[3]{2, 3, 4}, 3},
                                                      {'t', new int[4]{2, 3, 4, 5}, 4},
                                                      {'u', new int[3]{1, 3, 6}, 3},
                                                      {'v', new int[4]{1, 2, 3, 6}, 4},
                                                      {'w', new int[4]{2, 4, 5, 6}, 4},
                                                      {'x', new int[4]{1, 3, 4, 6}, 4},
                                                      {'y', new int[5]{1, 3, 4, 5, 6}, 5},
                                                      {'z', new int[4]{1, 3, 5, 6}, 4},
                                                      {' ', new int[1]{0}, 1}, // End of standard
                                                      {0b11111111111111111111111111100001, new int[2]{1, 6}, 2},          // á
                                                      {0b11111111111111111111111111101000, new int[3]{1, 4, 6}, 3},       // č
                                                      {0b11111111111111111111111111101111, new int[4]{1, 4, 5, 6}, 4},    // ď
                                                      {0b11111111111111111111111111101001, new int[3]{3, 4, 5}, 3},       // é
                                                      {0b11111111111111111111111111101100, new int[3]{1, 2, 6}, 3},       // ě
                                                      {0b11111111111111111111111111101101, new int[2]{3, 4}, 2},          // í
                                                      {0b11111111111111111111111111110010, new int[4]{1, 2, 4, 6}, 4},    // ň
                                                      {0b11111111111111111111111111110011, new int[3]{2, 4, 6}, 3},       // ó
                                                      {0b11111111111111111111111111111000, new int[4]{2, 4, 5, 6}, 4},    // ř
                                                      {0b11111111111111111111111110111001, new int[3]{1, 5, 6}, 3},       // š
                                                      {0b11111111111111111111111110111011, new int[4]{1, 2, 5, 6}, 4},    // ť
                                                      {0b11111111111111111111111111111001, new int[3]{3, 4, 6}, 3},       // ů
                                                      {0b11111111111111111111111111111010, new int[5]{2, 3, 4, 5, 6}, 5}, // ú
                                                      {0b11111111111111111111111111111101, new int[5]{1, 2, 3, 4, 6}, 5}, // ý
                                                      {0b11111111111111111111111110111110, new int[4]{2, 3, 4, 6}, 4},    // ž 
                                                      // End of Czech extend in bytes, because I cannot send those characters using UTF8 and this is the most
                                                      // reliable way I discovered of translating it to the right characters
                                                      {'1', new int[1]{1}, 1},          // Numbers
                                                      {'2', new int[2]{1, 2}, 2},
                                                      {'3', new int[2]{1, 4}, 2},
                                                      {'4', new int[3]{1, 4, 5}, 3},
                                                      {'5', new int[2]{1, 5}, 2},
                                                      {'6', new int[3]{1, 2, 4}, 3},
                                                      {'7', new int[4]{1, 2, 4, 5}, 4},
                                                      {'8', new int[3]{1, 2, 5}, 3},
                                                      {'9', new int[2]{2, 4}, 2},
                                                      {'0', new int[3]{2, 4, 5}, 3}, // End of numbers
                                                      {'.', new int[1]{3}, 1},       // Special characters
                                                      {',', new int[1]{2}, 1},
                                                      {'!', new int[3]{2, 3, 5}, 3},
                                                      {'?', new int[2]{2, 6}, 2},
                                                      {':', new int[2]{2, 4}, 2},
                                                      {';', new int[2]{2, 3}, 2},
                                                      {'(', new int[3]{2, 3, 6}, 3},
                                                      {')', new int[3]{3, 5, 6}, 3},
                                                      {'+', new int[3]{2, 5, 6}, 3},
                                                      {'-', new int[2]{3, 6}, 2},
                                                      {'=', new int[4]{2, 3, 5, 6}, 4},
                                                      {'<', new int[3]{1, 2, 6}, 3},
                                                      {'>', new int[3]{3, 4, 5}, 3},
                                                      {'"', new int[4]{2, 3, 5, 6}, 4},
                                                      {'*', new int[2]{3, 5}, 2},
                                                      {'/', new int[5]{1, 2, 4, 5, 6}, 5},
                                                      {'|', new int[3]{4, 5, 6}, 3},
                                                      {'\'', new int[1]{4}, 1}};
};

#endif
