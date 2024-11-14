# Projeto: Relógio Holográfico com ESP32 e NeoPixel

## Sumário

- [Introdução](#introdução)
- [Descrição do Projeto](#descrição-do-projeto)
- [Estrutura do Código](#estrutura-do-código)
- [Configurações e Variáveis Importantes](#configurações-e-variáveis-importantes)
- [Como Utilizar o Código](#como-utilizar-o-código)
- [Testes Unitários](#testes-unitários)
  - [Como Executar os Testes](#como-executar-os-testes)
- [Considerações sobre a Organização do Código](#considerações-sobre-a-organização-do-código)
- [Licença](#licença)

---

## Introdução

Este projeto implementa um relógio holográfico utilizando um ESP32, LEDs NeoPixel e um sensor para detecção de movimento. O objetivo é criar uma visualização holográfica de imagens pré-definidas ou enviadas via interface web, sincronizando a exibição dos LEDs com a rotação do dispositivo.

## Descrição do Projeto

O sistema consiste em:

- **ESP32**: Microcontrolador responsável por gerenciar a lógica do sistema, controle dos LEDs e comunicação Wi-Fi.
- **NeoPixel LEDs**: Uma matriz de LEDs RGB controlados individualmente para exibir as imagens.
- **Sensor**: Utilizado para detectar a posição do dispositivo e sincronizar a exibição dos LEDs.
- **Servidor Web**: Permite controlar o dispositivo remotamente, ligar/desligar LEDs e enviar novas imagens.

O código lê o sensor para determinar o tempo de rotação, calcula o tempo por setor (`t_arco`) e atualiza os LEDs para exibir a imagem desejada sincronizada com a rotação.

## Estrutura do Código

O código foi modularizado para melhorar a organização, manutenção e escalabilidade. A estrutura de pastas é a seguinte:

```
- projeto/
  - main/
    - main.cpp
  - src/
    - sensor_control.cpp
    - utilities.cpp
    - web_server_handlers.cpp
  - include/
    - config.h
    - image_data.h
    - sensor_control.h
    - utilities.h
    - web_server_handlers.h
  - test/
    - test_sensor_control.cpp
    - test_utilities.cpp
    - test_web_server_handlers.cpp
```

### Descrição dos Arquivos

- **main.cpp**: Contém as funções `setup()` e `loop()`, e inicia as configurações iniciais do sistema.
- **config.h**: Armazena constantes e variáveis globais que são utilizadas em múltiplos arquivos.
- **sensor_control.h** e **sensor_control.cpp**: Gerenciam a leitura do sensor e o cálculo dos tempos de rotação.
- **utilities.h** e **utilities.cpp**: Contêm funções utilitárias, como o filtro e o parsing de strings.
- **web_server_handlers.h** e **web_server_handlers.cpp**: Definem as funções relacionadas ao servidor web e suas rotas.
- **image_data.h**: Armazena os dados da imagem `imagem1` a ser exibida.
- **test/**: Contém os testes unitários para os diferentes módulos do código.

## Configurações e Variáveis Importantes

### Configurações de Wi-Fi

- **`ssid`**: Nome da rede Wi-Fi que o ESP32 irá criar ou conectar.
- **`password`**: Senha da rede Wi-Fi.

### Configurações do Servidor Web

- **`serverPort`**: Porta em que o servidor web irá operar (padrão é 80).

### Configurações do NeoPixel

- **`LED_PIN`**: Pino do ESP32 conectado aos LEDs NeoPixel.
- **`LED_COUNT`**: Número total de LEDs no projeto.

### Configurações do Sensor

- **`SENSOR_PIN`**: Pino do ESP32 conectado ao sensor.
- **`NUM_SETORES`**: Número de setores em que o círculo é dividido para sincronização (calculado como `3 * largura`).

### Variáveis Globais Importantes

- **`detect`**: Contador de detecções do sensor.
- **`N_giro`**: Número de giros contabilizados.
- **`tempoSensor`**: Tempo em microssegundos desde a última detecção do sensor.
- **`tempo`**: Tempo base para sincronização.
- **`t_giro[]`**: Array que armazena os tempos dos últimos giros.
- **`t_arco`**: Tempo calculado para cada setor.
- **`M_giro_antes`** e **`M_giro_atual`**: Utilizados no cálculo do tempo médio de giro, aplicando o filtro.

## Como Utilizar o Código

### Pré-requisitos

- **Hardware**:
  - ESP32.
  - LEDs NeoPixel (mínimo de 36 LEDs).
  - Sensor conectado ao pino especificado em `SENSOR_PIN`.

- **Software**:
  - Arduino IDE ou PlatformIO.
  - Bibliotecas:
    - **Adafruit NeoPixel**: Para controle dos LEDs.
    - **WiFi.h**: Para conexão Wi-Fi.
    - **WebServer.h**: Para criar o servidor web.

### Passos para Utilização

1. **Clone o Repositório**: Faça o download ou clone o repositório para sua máquina local.

2. **Configuração do Wi-Fi**:
   - Edite o arquivo `config.h` localizado na pasta `include/`.
   - Altere as constantes `ssid` e `password` para corresponder à sua rede Wi-Fi.

3. **Montagem do Hardware**:
   - Conecte os LEDs NeoPixel ao pino definido em `LED_PIN`.
   - Conecte o sensor ao pino definido em `SENSOR_PIN`.
   - Certifique-se de que todos os componentes estão alimentados corretamente.

4. **Compilação e Upload**:
   - Abra o projeto no Arduino IDE ou no seu ambiente preferido.
   - Certifique-se de que todas as bibliotecas necessárias estão instaladas.
   - Compile e faça o upload do código para o ESP32.

5. **Operação**:
   - Após o upload, o ESP32 irá se conectar à rede Wi-Fi especificada.
   - Abra o monitor serial para visualizar mensagens de depuração.
   - Anote o endereço IP exibido no monitor serial.

6. **Interação via Servidor Web**:
   - Abra um navegador web e acesse `http://<endereço_ip_do_esp32>/`.
   - Utilize as opções disponíveis para ligar/desligar os LEDs ou enviar uma nova imagem.

### Exemplo de Uso do Servidor Web

- **Ligar LEDs**: Acesse `http://<endereço_ip_do_esp32>/led/on`.
- **Desligar LEDs**: Acesse `http://<endereço_ip_do_esp32>/led/off`.
- **Enviar Imagem**:
  - Navegue até a página principal.
  - Utilize o formulário para enviar uma string representando a imagem.
  - A string deve estar no formato `{R1,G1,B1,A1,R2,G2,B2,A2,...}`.

## Testes Unitários

Os testes unitários foram criados utilizando o framework **AUnit** para Arduino. Eles permitem verificar o correto funcionamento das funções e módulos do código.

### Estrutura dos Testes

- **`test_sensor_control.cpp`**: Testes para o módulo de controle do sensor.
- **`test_utilities.cpp`**: Testes para as funções utilitárias.
- **`test_web_server_handlers.cpp`**: Testes para as rotas e funções do servidor web.

### Como Executar os Testes

1. **Instalar o Framework AUnit**:
   - Abra o Arduino IDE.
   - Vá em **Sketch** > **Incluir Biblioteca** > **Gerenciar Bibliotecas**.
   - Pesquise por **AUnit** e instale.

2. **Configurar o Projeto para Testes**:
   - Abra o arquivo de teste que deseja executar (por exemplo, `test_utilities.cpp`).
   - Certifique-se de que os caminhos para os arquivos `.h` estão corretos.

3. **Compilar e Carregar**:
   - Selecione a placa correta (ESP32).
   - Compile e faça o upload do código de teste para o ESP32.

4. **Visualizar os Resultados**:
   - Abra o monitor serial.
   - O resultado dos testes será exibido, indicando quais passaram e quais falharam.

### Observações Importantes

- **Mocks e Simulações**: Como o ESP32 interage com hardware real, os testes utilizam mocks para simular o comportamento do hardware.
- **Isolamento dos Testes**: Cada teste é independente para garantir a confiabilidade dos resultados.
- **Limitações de Memória**: Certifique-se de que o ESP32 possui memória suficiente para executar os testes.

## Considerações sobre a Organização do Código

A organização do código em múltiplos arquivos e pastas tem como objetivo:

- **Modularidade**: Facilitar a manutenção e compreensão separando funcionalidades distintas em módulos.
- **Reutilização de Código**: Funções utilitárias e configurações globais podem ser reutilizadas em diferentes partes do projeto.
- **Escalabilidade**: Permitir a adição de novas funcionalidades sem tornar o código monolítico.
- **Testabilidade**: Facilitar a criação de testes unitários para cada módulo.

### Organização em Pastas

- **`main/`**: Contém o ponto de entrada do programa (`main.cpp`).
- **`src/`**: Implementações das funções e classes.
- **`include/`**: Arquivos de cabeçalho com declarações e definições.
- **`test/`**: Arquivos relacionados aos testes unitários.

### Por que Essa Estrutura?

- **Clareza**: Separar o código em pastas lógicas torna mais fácil para outros desenvolvedores entenderem o projeto.
- **Manutenção**: Facilita a localização e correção de bugs ou adição de novas funcionalidades.
- **Colaboração**: Em equipes, uma estrutura organizada permite que múltiplas pessoas trabalhem em diferentes partes do projeto simultaneamente.

