#ifndef TypeWriter_h
#define TypeWriter_h
#include <Arduino.h>

class TypeWriter{
  public:
    TypeWriter(int solenoidPins[7], int stepperPins[4]);
    void setParameters(int rowLength, int pressDelay);
    void print(String message);
    void test();

  private:
    int _brailleDots[7];
    int _stepperPins[4];
    int _pressDelay;
    int _rowLength;
    int _brailleDotsCount = 7;
    int _stepNumber = 0;
    static const int _brailleDLength = 27;

    void newLine();
    void OneStep(bool dir);
    void Split(String message, String **words, int *count);

    // "Dictionary"
    typedef struct{
      char key;
      int *value;
      int count;    // Temporary
    } _brailleChar;

    // I really don't know why was this the problem (probably because this is a header file)
    // but I had to add size to _brailledict to compile
    const _brailleChar _brailleDict[_brailleDLength] = {{'a', new int[1]{1}, 1},
                                       {'b', new int[2]{1,2}, 2},
                                       {'c', new int[2]{1,4}, 2},
                                       {'d', new int[3]{1,4,5}, 3},
                                       {'e', new int[2]{1,5}, 2},
                                       {'f', new int[3]{1,2,4}, 3},
                                       {'g', new int[4]{1,2,4,5}, 4},
                                       {'h', new int[3]{1,2,5}, 3},
                                       {'i', new int[2]{2,4}, 2},
                                       {'j', new int[3]{2,4,5}, 3},
                                       {'k', new int[2]{1,3}, 2},
                                       {'l', new int[3]{1,2,3}, 3},
                                       {'m', new int[3]{1,3,4}, 3},
                                       {'n', new int[4]{1,3,4,5}, 4},
                                       {'o', new int[3]{1,3,5}, 3},
                                       {'p', new int[4]{1,2,3,4}, 4},
                                       {'q', new int[5]{1,2,3,4,5}, 5},
                                       {'r', new int[4]{1,2,3,5}, 4},
                                       {'s', new int[3]{2,3,4}, 3},
                                       {'t', new int[4]{2,3,4,5}, 4},
                                       {'u', new int[3]{1,3,6}, 3},
                                       {'v', new int[4]{1,2,3,6}, 4},
                                       {'w', new int[4]{2,4,5,6}, 4},
                                       {'x', new int[4]{1,3,4,6}, 4},
                                       {'y', new int[5]{1,3,4,5,6}, 5},
                                       {'z', new int[4]{1,3,5,6}, 4},
                                       {' ', new int[1]{0}, 1}};
};

#endif
