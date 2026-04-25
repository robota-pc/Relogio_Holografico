import requests
from PIL import Image
import numpy as np
from io import BytesIO
from google.colab import files

# Importa as bibliotecas necessárias

# Faz o download da imagem de uma URL e realiza manipulações

# Define a URL da imagem que será baixada
url_imagem = "https://static.mundoeducacao.uol.com.br/mundoeducacao/2023/02/mapa-mundi-em-fundo-azul.jpg"



caminho_imagem = '/content/robota_relogio.PNG'

img = Image.open(caminho_imagem)

# Converter para escala de cinza (opcional)
img = img.convert('RGB')  # Se preferir trabalhar com RGB, use 'RGB' ao invés de 'L'



# Faz o download da imagem da URL especificada utilizando a biblioteca requests
response = requests.get(url_imagem)

# Abre a imagem a partir do conteúdo da resposta utilizando a biblioteca PIL
imagem = Image.open(BytesIO(response.content))

imagem = img
# Define as dimensões de largura e altura desejadas para a imagem redimensionada
largura = 20
altura = 20

# Redimensiona a imagem utilizando o método resize da biblioteca PIL
imagem_redimensionada = imagem.resize((largura, altura))

# Converte a imagem redimensionada em um array numpy
array_pixels = np.asarray(imagem_redimensionada)


# Converter a imagem para um array NumPy
array_pixels_1 = np.array(imagem_redimensionada)

# Criar uma máscara para identificar pixels verdes (aproximados)
mascara_verde = (array_pixels_1[:, :, 0] < 100) & (array_pixels_1[:, :, 1] > 150) & (array_pixels_1[:, :, 2] < 100)
# Criar uma máscara para identificar pixels vermelhos (aproximados)
mascara_vermelha = (array_pixels_1[:, :, 0] > 150) & (array_pixels_1[:, :, 1] < 100) & (array_pixels_1[:, :, 2] < 100)

# Substituir os pixels conforme especificado
array_pixels_1[mascara_verde] = [255, 25, 0]  # Para pixels verdes
array_pixels_1[mascara_vermelha] = [75, 0, 130]  # Para pixels vermelhos
array_pixels_1[~(mascara_verde | mascara_vermelha)] = [0, 0, 0]  # Qualquer outro pixel


# Itera sobre cada linha e cada pixel do array de pixels da imagem
for linha in array_pixels_1:
    for pixel in linha:
        # Imprime cada pixel na forma de um dicionário com os valores RGB
        print("{" + ",".join(map(str, pixel)) + "}", end=",")

# Converter o array de volta para uma imagem
nova_imagem = Image.fromarray(array_pixels_1)

# Salvar ou exibir a nova imagem
nova_imagem.save('imagem_transformada.png')
nova_imagem.show()
