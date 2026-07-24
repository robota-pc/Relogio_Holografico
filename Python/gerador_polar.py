import os
import requests
from PIL import Image, ImageEnhance
import numpy as np
from io import BytesIO

def removedor_fundo_inteligente(img, threshold=200):
    """
    Remove o fundo baseado na luminosidade.
    Pixels muito claros (brancos/fundos) ficam com brilho (Alpha) 0.
    """
    img = img.convert("RGBA")
    data = np.array(img)
    
    # Pega os canais RGB
    rgb = data[:,:,:3]
    # Calcula luminosidade aproximada
    luminosidade = np.mean(rgb, axis=2)
    
    # Onde a luminosidade for maior que o threshold, zeramos o Alpha (Transparente/Apagado)
    # Onde for menor (imagem), deixamos o Alpha em 255
    data[:,:,3] = np.where(luminosidade > threshold, 0, 255)
    
    return Image.fromarray(data)

def gerar_matriz_polar(img, num_setores=40, led_count=36):
    """
    Mapeia a imagem para coordenadas polares.
    O centro da imagem vira o eixo do motor.
    """
    img = img.convert("RGBA")
    largura, altura = img.size
    centro_x = largura / 2
    centro_y = altura / 2
    
    # O raio máximo que podemos pegar é metade da menor dimensão da imagem
    raio_max = min(centro_x, centro_y)
    
    array_saida = []
    
    # Para cada setor (ângulo) no giro
    for setor in range(num_setores):
        # O ângulo de 0 a 2*PI
        # Dependendo da rotação (horário ou anti-horário), pode ser necessário inverter o sinal
        angulo = (setor / num_setores) * 2 * np.pi
        
        # Para cada LED (0 é o centro, led_count-1 é a borda)
        for led in range(led_count):
            # Transforma a posição do LED em um raio (0 a raio_max)
            raio_atual = (led / led_count) * raio_max
            
            # Converte Polar para Cartesiano
            x = int(centro_x + raio_atual * np.cos(angulo))
            y = int(centro_y - raio_atual * np.sin(angulo)) # -sin porque o eixo Y cresce pra baixo
            
            # Limita dentro da imagem pra não dar erro
            x = max(0, min(x, largura - 1))
            y = max(0, min(y, altura - 1))
            
            pixel = img.getpixel((x, y))
            r, g, b, a = pixel
            
            array_saida.append(f"{{{r},{g},{b},{a}}}")
            
    return array_saida

def main():
    # URL de exemplo (Substitua pela sua ou use caminho local)
    url = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcS-YnW3X4abWz2NJVOIUhCosBsfEgIaZoe1pc9yT2D5zzXSn6ulByQ6m8jy&s=10"
    
    print("Baixando imagem...")
    try:
        response = requests.get(url)
        img = Image.open(BytesIO(response.content))
    except Exception as e:
        print(f"Erro ao baixar, tentando usar arquivo local 'robota_relogio.PNG': {e}")
        if os.path.exists('robota_relogio.PNG'):
            img = Image.open('robota_relogio.PNG')
        else:
            print("Nenhuma imagem encontrada.")
            return

    # 1. Aumento de contraste (Opcional, bom para LEDs)
    enhancer = ImageEnhance.Contrast(img)
    img = enhancer.enhance(1.5)

    # 2. Remoção de Fundo
    img = removedor_fundo_inteligente(img, threshold=200)
    
    # Para visualizar como a imagem ficou após a remoção do fundo:
    # img.show()

    # 3. Gerar a matriz polar {R, G, B, Alpha/Brilho_Original}
    # num_setores é a largura da sua matriz original no ESP32.
    # led_count é a quantidade de LEDs da sua fita.
    num_setores = 40
    led_count = 36
    
    matriz_text = gerar_matriz_polar(img, num_setores=num_setores, led_count=led_count)
    
    # 4. Salvar em arquivo txt
    arquivo_saida = "imagem_base_polar.txt"
    with open(arquivo_saida, "w") as f:
        f.write(",".join(matriz_text))
        
    print(f"Sucesso! {len(matriz_text)} pixels gerados e salvos em {arquivo_saida}")
    print(f"Formato: {num_setores} setores x {led_count} LEDs = {num_setores*led_count} pixels.")
    print("Use os dados dentro do .txt para alimentar seu ESP32.")

if __name__ == "__main__":
    main()
