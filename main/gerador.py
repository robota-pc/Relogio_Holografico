import requests
from PIL import Image
import numpy as np
from io import BytesIO
from google.colab import files

# Importa as bibliotecas necessárias

# Faz o download da imagem de uma URL e realiza manipulações

# Define a URL da imagem que será baixada
url_imagem = "https://static.mundoeducacao.uol.com.br/mundoeducacao/2023/02/mapa-mundi-em-fundo-azul.jpg"

# Faz o download da imagem da URL especificada utilizando a biblioteca requests
response = requests.get(url_imagem)

# Abre a imagem a partir do conteúdo da resposta utilizando a biblioteca PIL
imagem = Image.open(BytesIO(response.content))

# Define as dimensões de largura e altura desejadas para a imagem redimensionada
largura = 72
altura = 36

# Redimensiona a imagem utilizando o método resize da biblioteca PIL
imagem_redimensionada = imagem.resize((largura, altura))

# Converte a imagem redimensionada em um array numpy
array_pixels = np.asarray(imagem_redimensionada)

# Itera sobre cada linha e cada pixel do array de pixels da imagem
for linha in array_pixels:
    for pixel in linha:
        # Imprime cada pixel na forma de um dicionário com os valores RGB
        print("{" + ",".join(map(str, pixel)) + "}", end=",")

# Cria uma nova imagem a partir do array de pixels RGB
image = Image.fromarray(array_pixels.astype('uint8'))

# Salva a imagem em um arquivo chamado "imagem.png"
image.save('imagem.png')
