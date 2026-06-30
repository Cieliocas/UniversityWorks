#!/usr/bin/env python3
"""Gera os graficos academicos do benchmark de coloracao de grafos.

Le 'resultados.csv' (produzido pelo executavel C++) e gera dois graficos PNG
com design academico (linhas claras, legendas, grids e rotulos nos eixos):

  * grafico_tempo.png  -> Esforco computacional (Tempo medio x Dimensao V),
                          grafico de linha com marcadores, uma linha por
                          heuristica (eixo Y em escala log).
  * grafico_cores.png  -> Qualidade da solucao (Cores medias x Dimensao V),
                          barras agrupadas com barras de erro (desvio-padrao).

Bibliotecas: pandas, matplotlib, seaborn.

Uso:
    python3 scripts/plot_results.py [caminho_csv] [dir_saida]
"""

import os
import sys

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
from matplotlib.container import BarContainer
from matplotlib.ticker import ScalarFormatter

# Ordem e cores fixas das heuristicas (consistentes entre os dois graficos).
HEURISTICAS = ["Sequencial", "Welsh-Powell", "DSATUR"]


def carregar_dados(caminho_csv: str) -> pd.DataFrame:
    """Le o CSV e valida as colunas esperadas."""
    if not os.path.isfile(caminho_csv):
        sys.exit(
            f"Arquivo '{caminho_csv}' nao encontrado. "
            "Compile e rode o executavel C++ primeiro para gerar o CSV."
        )
    df = pd.read_csv(caminho_csv)
    colunas = {"V", "heuristica", "tempo_medio_ms", "cores_media"}
    faltando = colunas - set(df.columns)
    if faltando:
        sys.exit(f"Colunas ausentes no CSV: {faltando}")
    # Mantem apenas heuristicas conhecidas, na ordem definida.
    df = df[df["heuristica"].isin(HEURISTICAS)].copy()
    df["heuristica"] = pd.Categorical(
        df["heuristica"], categories=HEURISTICAS, ordered=True
    )
    return df.sort_values(["V", "heuristica"])


def configurar_estilo() -> dict:
    """Aplica o tema academico do seaborn e devolve a paleta por heuristica."""
    sns.set_theme(style="whitegrid", context="talk")
    plt.rcParams["axes.titlesize"] = 16
    plt.rcParams["axes.labelsize"] = 13
    plt.rcParams["font.family"] = "DejaVu Sans"
    cores = sns.color_palette("Set2", len(HEURISTICAS))
    return dict(zip(HEURISTICAS, cores))


def grafico_tempo(df: pd.DataFrame, paleta: dict, dir_saida: str) -> None:
    """Grafico 1: tempo medio de execucao por dimensao (linha + marcadores)."""
    plt.figure(figsize=(10, 6))
    marcadores = {"Sequencial": "o", "Welsh-Powell": "s", "DSATUR": "^"}

    for heur in HEURISTICAS:
        sub = df[df["heuristica"] == heur]
        if sub.empty:
            continue
        plt.plot(
            sub["V"], sub["tempo_medio_ms"],
            marker=marcadores.get(heur, "o"), markersize=9,
            linewidth=2.2, color=paleta[heur], label=heur,
        )

    ax = plt.gca()
    if (df["tempo_medio_ms"] > 0).all():
        ax.set_yscale("log")
        ylabel = "Tempo medio de execucao (ms) — escala log"
    else:
        ylabel = "Tempo medio de execucao (ms)"

    ax.set_xlabel("Dimensao do grafo (numero de vertices V)")
    ax.set_ylabel(ylabel)
    ax.set_title("Esforco Computacional das Heuristicas de Coloracao")
    ax.set_xticks(sorted(df["V"].unique()))
    ax.get_xaxis().set_major_formatter(ScalarFormatter())
    ax.grid(True, which="both", linestyle="--", linewidth=0.5, alpha=0.7)
    plt.legend(title="Heuristica", frameon=True)
    plt.tight_layout()

    destino = os.path.join(dir_saida, "grafico_tempo.png")
    plt.savefig(destino, dpi=300)
    plt.close()
    print(f"Salvo: {destino}")


def grafico_cores(df: pd.DataFrame, paleta: dict, dir_saida: str) -> None:
    """Grafico 2: cores medias por dimensao (barras agrupadas + erro)."""
    pivot = df.pivot(index="V", columns="heuristica", values="cores_media")
    pivot = pivot.reindex(columns=HEURISTICAS)

    erros = None
    if "cores_dp" in df.columns:
        erros = df.pivot(index="V", columns="heuristica", values="cores_dp")
        erros = erros.reindex(columns=HEURISTICAS)

    ax = pivot.plot(
        kind="bar",
        yerr=erros,
        capsize=4,
        figsize=(10, 6),
        width=0.8,
        color=[paleta[h] for h in HEURISTICAS],
        edgecolor="black",
        linewidth=0.6,
    )

    ax.set_xlabel("Dimensao do grafo (numero de vertices V)")
    ax.set_ylabel("Media de cores utilizadas")
    ax.set_title("Qualidade da Solucao: Cores Utilizadas por Heuristica")
    ax.tick_params(axis="x", rotation=0)
    ax.grid(True, axis="y", linestyle="--", linewidth=0.5, alpha=0.7)
    ax.legend(title="Heuristica", frameon=True)

    # Rotulos numericos sobre cada barra (ignora containers de barras de erro).
    for container in ax.containers:
        if isinstance(container, BarContainer):
            ax.bar_label(container, fmt="%.0f", padding=2, fontsize=9)

    plt.tight_layout()
    destino = os.path.join(dir_saida, "grafico_cores.png")
    plt.savefig(destino, dpi=300)
    plt.close()
    print(f"Salvo: {destino}")


def main() -> None:
    caminho_csv = sys.argv[1] if len(sys.argv) > 1 else "resultados.csv"
    dir_saida = sys.argv[2] if len(sys.argv) > 2 else "graficos"
    os.makedirs(dir_saida, exist_ok=True)

    df = carregar_dados(caminho_csv)
    paleta = configurar_estilo()
    grafico_tempo(df, paleta, dir_saida)
    grafico_cores(df, paleta, dir_saida)
    print("Concluido.")


if __name__ == "__main__":
    main()
