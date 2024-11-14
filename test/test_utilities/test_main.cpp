#include <Arduino.h>
#include <AUnit.h>
#include "../include/utilities.h"

test(filtroTest) {
  unsigned long y0 = 1000;
  unsigned long y1 = 1100;
  unsigned long expected = 0.9 * y0 + 0.1 * y1;
  assertEqual(filtro(y0, y1), expected);
}

test(parseStringToIntMatrixTest_Success) {
  String input = "{1,2,3,4,5,6,7,8}";
  int imagem[2][4];
  bool result = parseStringToIntMatrix(input, imagem);
  assertTrue(result);
  assertEqual(imagem[0][0], 1);
  assertEqual(imagem[0][1], 2);
  assertEqual(imagem[0][2], 3);
  assertEqual(imagem[0][3], 4);
  assertEqual(imagem[1][0], 5);
  assertEqual(imagem[1][1], 6);
  assertEqual(imagem[1][2], 7);
  assertEqual(imagem[1][3], 8);
}

test(parseStringToIntMatrixTest_Failure) {
  String input = "{1,2,three,4}";
  int imagem[1][4];
  bool result = parseStringToIntMatrix(input, imagem);
  assertFalse(result);
}
