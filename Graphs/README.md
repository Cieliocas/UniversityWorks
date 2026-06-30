# Coloração de Grafos — Heurísticas e Benchmark

Trabalho final da disciplina **Teoria e Aplicação de Grafos**. Implementa e
compara três heurísticas de **coloração de vértices** sobre grafos aleatórios de
dimensões crescentes, medindo **esforço computacional** (tempo) e **qualidade da
solução** (número de cores).

## Heurísticas implementadas

| Heurística | Ideia central | Complexidade |
|---|---|---|
| **Sequencial (Gulosa)** | menor cor livre, em ordem fixa | O(V + E) |
| **Welsh-Powell** | colore por grau decrescente | O(V²) |
| **DSATUR (Brélaz)** | maior grau de saturação a cada passo | O(V²) |

## Estrutura do projeto

```
ColoracaoEmGrafos/
├── CMakeLists.txt          # Build (C++17, Release -O2)
├── README.md
├── RELATORIO_BASE.md       # Esqueleto do relatório final
├── include/
│   ├── Graph.h             # Grafo (lista de adjacências) + gerador
│   └── Coloring.h          # Heurísticas + validação
├── src/
│   ├── Graph.cpp
│   ├── Coloring.cpp
│   └── main.cpp            # Benchmark + exportação do CSV
└── scripts/
    └── plot_results.py     # Gera os gráficos PNG
```

Artefatos gerados em tempo de execução: `resultados.csv`, `graficos/`.

## Requisitos

- **C++17** (g++/clang++) e **CMake ≥ 3.15**
- **Python 3** com `pandas`, `matplotlib`, `seaborn` (apenas para os gráficos)

## Como compilar

```bash
cmake -S . -B build
cmake --build build
```

O executável é gerado em `build/coloracao`.

## Como executar o benchmark

```bash
./build/coloracao
```

Isso gera `resultados.csv` com as médias (e desvios-padrão) de tempo e de cores
para cada dimensão e heurística.

### Opções de linha de comando

```bash
./build/coloracao \
    --sizes 50,100,200,500,1000 \
    --instances 10 \
    --density 0.5 \
    --seed 42 \
    --output resultados.csv
```

| Opção | Padrão | Descrição |
|---|---|---|
| `--sizes` | `50,100,200,500,1000` | dimensões V a testar |
| `--instances` | `10` | instâncias por dimensão |
| `--density` | `0.5` | probabilidade de aresta (0 < p < 1) |
| `--seed` | `42` | semente base (reprodutibilidade) |
| `--output` | `resultados.csv` | arquivo CSV de saída |

## Como gerar os gráficos

```bash
pip install pandas matplotlib seaborn      # se necessário
python3 scripts/plot_results.py            # lê resultados.csv
```

Gera `graficos/grafico_tempo.png` (esforço computacional) e
`graficos/grafico_cores.png` (qualidade da solução).

## Metodologia

- Grafos **não-orientados**, **conexos** (árvore geradora aleatória) e
  **não-completos** (Erdős–Rényi com p < 1), em **lista de adjacências**.
- Tempo medido com `std::chrono` cronometrando **apenas** o algoritmo de coloração.
- Toda coloração é **validada** antes de ser contabilizada.
- Resultados consolidados como **média ± desvio-padrão** de 10 instâncias.
