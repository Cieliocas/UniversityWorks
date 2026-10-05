"""
Atividade Prática 01 - Processamento Digital de Imagens (2026.2)
Equalização e especificação de histograma em imagens em tons de cinza.

Aluno: Franciélio Evangelista dos Santos Castro
Matrícula: 20249050551
"""

import os

import cv2
import matplotlib.pyplot as plt
import numpy as np

L = 256  # número de níveis de intensidade (8 bits)

IMAGEM_ORIGINAL = "polen.png"
IMAGEM_REFERENCIA = "lena_gray.bmp"
IMAGEM_CONTRAEXEMPLO = "einstein.jpg"
PASTA_SAIDA = "resultados"


def carregar_cinza(caminho):
    """Lê a imagem do disco e a converte para tons de cinza (uint8)."""
    img = cv2.imread(caminho, cv2.IMREAD_GRAYSCALE)
    if img is None:
        raise FileNotFoundError(f"Não foi possível ler a imagem: {caminho}")
    return img


def calcular_histograma(img):
    """Conta quantos pixels possuem cada intensidade de 0 a 255."""
    hist = np.zeros(L, dtype=np.int64)
    for valor in img.ravel():
        hist[valor] += 1
    return hist


def calcular_cdf(hist):
    """Função de distribuição acumulada normalizada no intervalo [0, 1]."""
    pdf = hist / hist.sum()
    return np.cumsum(pdf)


def equalizar(img):
    """Equalização: s_k = round((L - 1) * sum_{j=0}^{k} p_r(r_j))."""
    hist = calcular_histograma(img)
    cdf = calcular_cdf(hist)
    tabela = np.round((L - 1) * cdf).astype(np.uint8)
    return tabela[img], tabela


def especificar(img, ref):
    """Especificação (casamento) do histograma de img com o de ref.

    1. T(r) = (L - 1) * CDF_original(r)
    2. G(z) = (L - 1) * CDF_referencia(z)
    3. Para cada r, escolhe o menor z tal que G(z) >= T(r),
       o que aproxima z = G^-1(T(r)).
    """
    t = np.round((L - 1) * calcular_cdf(calcular_histograma(img)))
    g = np.round((L - 1) * calcular_cdf(calcular_histograma(ref)))
    tabela = np.zeros(L, dtype=np.uint8)
    for r in range(L):
        z = np.searchsorted(g, t[r], side="left")
        tabela[r] = min(z, L - 1)
    return tabela[img], tabela


def desenhar_histograma(eixo, img, titulo, cor="gray"):
    """Desenha o histograma de intensidades no intervalo 0 a 255."""
    hist = calcular_histograma(img)
    eixo.bar(np.arange(L), hist, width=1.0, color=cor)
    eixo.set_xlim(0, L - 1)
    eixo.set_title(titulo)
    eixo.set_xlabel("Intensidade")
    eixo.set_ylabel("Número de pixels")


def mostrar_imagem(eixo, img, titulo):
    eixo.imshow(img, cmap="gray", vmin=0, vmax=255)
    eixo.set_title(titulo)
    eixo.axis("off")


def salvar(fig, nome):
    fig.tight_layout()
    fig.savefig(os.path.join(PASTA_SAIDA, nome), dpi=150)


def estatisticas(nome, img):
    hist = calcular_histograma(img)
    niveis = int(np.count_nonzero(hist))
    print(f"{nome:<28} min={img.min():3d} max={img.max():3d} "
          f"media={img.mean():6.1f} desvio={img.std():5.1f} niveis={niveis}")


def main():
    os.makedirs(PASTA_SAIDA, exist_ok=True)

    # Passo 1: leitura e histograma da imagem original
    original = carregar_cinza(IMAGEM_ORIGINAL)
    fig, eixos = plt.subplots(1, 2, figsize=(11, 4))
    mostrar_imagem(eixos[0], original, "Imagem original")
    desenhar_histograma(eixos[1], original, "Histograma da imagem original")
    salvar(fig, "passo1_original.png")

    # Passo 2: equalização
    equalizada, tabela_eq = equalizar(original)
    fig, eixos = plt.subplots(2, 2, figsize=(11, 8))
    mostrar_imagem(eixos[0, 0], original, "Imagem original")
    mostrar_imagem(eixos[0, 1], equalizada, "Imagem equalizada")
    desenhar_histograma(eixos[1, 0], original, "Histograma original")
    desenhar_histograma(eixos[1, 1], equalizada, "Histograma equalizado")
    salvar(fig, "passo2_equalizacao.png")

    fig, eixo = plt.subplots(figsize=(7, 4))
    eixo.plot(np.arange(L), tabela_eq, color="black")
    eixo.set_xlim(0, L - 1)
    eixo.set_ylim(0, L - 1)
    eixo.set_title("Função de transformação da equalização")
    eixo.set_xlabel("Intensidade de entrada r")
    eixo.set_ylabel("Intensidade de saída s")
    salvar(fig, "passo2_transformacao.png")

    # Contraexemplo: equalização de uma imagem que já possui boa distribuição
    contraexemplo = carregar_cinza(IMAGEM_CONTRAEXEMPLO)
    contraexemplo_eq, _ = equalizar(contraexemplo)
    fig, eixos = plt.subplots(2, 2, figsize=(11, 8))
    mostrar_imagem(eixos[0, 0], contraexemplo, "Imagem original")
    mostrar_imagem(eixos[0, 1], contraexemplo_eq, "Imagem equalizada")
    desenhar_histograma(eixos[1, 0], contraexemplo, "Histograma original")
    desenhar_histograma(eixos[1, 1], contraexemplo_eq, "Histograma equalizado")
    salvar(fig, "passo2_contraexemplo.png")

    # Passo 3: especificação
    referencia = carregar_cinza(IMAGEM_REFERENCIA)
    especificada, _ = especificar(original, referencia)
    fig, eixos = plt.subplots(2, 3, figsize=(15, 8))
    mostrar_imagem(eixos[0, 0], original, "Imagem original")
    mostrar_imagem(eixos[0, 1], referencia, "Imagem de referência")
    mostrar_imagem(eixos[0, 2], especificada, "Imagem resultante")
    desenhar_histograma(eixos[1, 0], original, "Histograma original")
    desenhar_histograma(eixos[1, 1], referencia, "Histograma de referência")
    desenhar_histograma(eixos[1, 2], especificada, "Histograma resultante")
    salvar(fig, "passo3_especificacao.png")

    fig, eixo = plt.subplots(figsize=(7, 4))
    eixo.plot(np.arange(L), calcular_cdf(calcular_histograma(original)),
              label="Original", color="0.6", linestyle=":")
    eixo.plot(np.arange(L), calcular_cdf(calcular_histograma(referencia)),
              label="Referência", color="black")
    eixo.plot(np.arange(L), calcular_cdf(calcular_histograma(especificada)),
              label="Resultante", color="0.4", linestyle="--")
    eixo.set_xlim(0, L - 1)
    eixo.set_title("Distribuições acumuladas")
    eixo.set_xlabel("Intensidade")
    eixo.set_ylabel("Frequência acumulada")
    eixo.legend()
    salvar(fig, "passo3_cdfs.png")

    # Estatísticas usadas na análise
    estatisticas("Original (polen)", original)
    estatisticas("Equalizada (polen)", equalizada)
    estatisticas("Referência (lena)", referencia)
    estatisticas("Especificada (polen)", especificada)
    estatisticas("Original (einstein)", contraexemplo)
    estatisticas("Equalizada (einstein)", contraexemplo_eq)

    cdf_ref = calcular_cdf(calcular_histograma(referencia))
    cdf_esp = calcular_cdf(calcular_histograma(especificada))
    print(f"Maior diferença entre CDFs (referência x resultante): "
          f"{np.max(np.abs(cdf_ref - cdf_esp)):.4f}")

    plt.show()


if __name__ == "__main__":
    main()
