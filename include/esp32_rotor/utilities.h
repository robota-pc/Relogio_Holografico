#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <stdint.h>

uint64_t filtro(uint64_t y0, uint64_t y1);
bool parseStringToIntMatrix(std::string inputString, int imagem[1080][4]);

#endif
