#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

unsigned long filtro(unsigned long y0, unsigned long y1);
bool parseStringToIntMatrix(String inputString, int imagem[1080][4]);

#endif
