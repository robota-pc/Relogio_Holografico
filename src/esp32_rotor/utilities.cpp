#include "utilities.h"
#include "config.h"
#include <string>
#include <iostream>
#include "esp_log.h"

static const char *TAG = "utilities";

/**
 * @brief Função de filtro com coeficientes.
 *
 * @param y0 Valor anterior.
 * @param y1 Valor atual.
 * @return Resultado filtrado.
 */
uint64_t filtro(uint64_t y0, uint64_t y1) {
  // Aplicação de filtro simples
  uint64_t y2 = anterior * y0 + novo * y1;
  return y2;
}

/**
 * @brief Função para converter uma string em uma matriz de inteiros.
 *
 * @param inputString String de entrada.
 * @param imagem Matriz de inteiros de saída.
 * @return true se a conversão for bem-sucedida, false caso contrário.
 */
bool parseStringToIntMatrix(std::string inputString, int imagem[1080][4]) {
  // Verificação inicial da string
  if (inputString.length() < 2) {
    ESP_LOGE(TAG, "String de entrada inválida.");
    return false;
  }

  // Remover '{' e '}'
  if (inputString.front() == '{') inputString.erase(0, 1);
  if (inputString.back() == '}') inputString.pop_back();

  int rowIndex = 0;
  int colIndex = 0;
  std::string token;
  size_t pos = 0;
  
  while (!inputString.empty() && rowIndex < 1080) {
    pos = inputString.find(',');
    if (pos != std::string::npos) {
      token = inputString.substr(0, pos);
      inputString.erase(0, pos + 1);
    } else {
      token = inputString;
      inputString = "";
    }

    // Converter o token para int com tratamento de erro (sem exceções)
    int value = 0;
    char *endptr = nullptr;
    long parsed = strtol(token.c_str(), &endptr, 10);
    if (endptr == token.c_str() && token != "0") {
        ESP_LOGE(TAG, "Erro ao converter token para inteiro: %s", token.c_str());
        return false;
    }
    value = (int)parsed;
 
    imagem[rowIndex][colIndex] = value;
    colIndex++;
    if (colIndex >= 4) {
      colIndex = 0;
      rowIndex++;
    }
  }

  if (rowIndex >= 1080) {
    ESP_LOGE(TAG, "A matriz de imagem excedeu o tamanho máximo.");
    return false;
  }

  return true;
}
