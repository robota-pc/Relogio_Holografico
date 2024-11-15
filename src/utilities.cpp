#include "../include/utilities.h"
#include <Arduino.h>
#include "../include/config.h"

/**
 * @brief Função de filtro com coeficientes.
 *
 * @param y0 Valor anterior.
 * @param y1 Valor atual.
 * @return Resultado filtrado.
 */
unsigned long filtro(unsigned long y0, unsigned long y1) {
  // Aplicação de filtro simples
  unsigned long y2 = anterior * y0 + novo * y1;
  return y2;
}

/**
 * @brief Função para converter uma string em uma matriz de inteiros.
 *
 * @param inputString String de entrada.
 * @param imagem Matriz de inteiros de saída.
 * @return true se a conversão for bem-sucedida, false caso contrário.
 */
bool parseStringToIntMatrix(String inputString, int imagem[1080][4]) {
  // Verificação inicial da string
  if (inputString.length() < 2) {
    Serial.println("String de entrada inválida.");
    return false;
  }

  // Remover '{' e '}'
  inputString.remove(0, 1);
  inputString.remove(inputString.length() - 1, 1);

  int rowIndex = 0;
  int colIndex = 0;
  String token;
  while (inputString.length() > 0 && rowIndex < 1080) {
    int commaIndex = inputString.indexOf(',');
    if (commaIndex != -1) {
      token = inputString.substring(0, commaIndex);
      inputString.remove(0, commaIndex + 1);
    } else {
      token = inputString;
      inputString = "";
    }

    // Converter o token para int com tratamento de erro
    int value = token.toInt();
    if (value == 0 && token != "0") {
      Serial.println("Erro ao converter token para inteiro: " + token);
      return false;
    }
 
    imagem[rowIndex][colIndex] = value;
    colIndex++;
    if (colIndex >= 4) {
      colIndex = 0;
      rowIndex++;
    }
  }

  if (rowIndex >= 1080) {
    Serial.println("A matriz de imagem excedeu o tamanho máximo.");
    return false;
  }

  return true;
}
