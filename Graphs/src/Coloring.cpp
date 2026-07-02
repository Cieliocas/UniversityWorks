// ===========================================================================
// Coloring.cpp
// ---------------------------------------------------------------------------
// Implementacao das tres heuristicas de coloracao de vertices declaradas em
// Coloring.h: Sequencial (Gulosa), Welsh-Powell e DSATUR (Brelaz), alem das
// funcoes auxiliares de validacao e contagem de cores.
//
// Convencao de cores: inteiros 0, 1, 2, ... (0-indexados e contiguos). O
// numero de cores usado e sempre (maior_cor + 1). Duas cores adjacentes nunca
// recebem o mesmo rotulo (coloracao propria).
// ===========================================================================
#include "Coloring.h"

#include <algorithm>  // std::sort, std::max
#include <numeric>    // std::iota

namespace Coloring {

// ---------------------------------------------------------------------------
// countColors: conta quantas cores distintas foram usadas.
// Como os rotulos sao 0-indexados e contiguos, basta achar o maior rotulo e
// somar 1. Ex.: se a maior cor e 4, foram usadas as cores {0,1,2,3,4} = 5.
// ---------------------------------------------------------------------------
int countColors(const std::vector<int>& colors) {
    int maxColor = -1;
    for (int c : colors) maxColor = std::max(maxColor, c);
    return maxColor + 1;  // -1 (grafo vazio) vira 0 cores
}

// ---------------------------------------------------------------------------
// isValid: verifica se 'colors' e uma coloracao PROPRIA de 'g'.
// Percorre cada vertice v e cada vizinho u; se algum par adjacente compartilha
// a mesma cor, a coloracao e invalida. Tambem rejeita vertices sem cor (-1).
// Custo O(V + E). Usada como rede de seguranca no benchmark.
// ---------------------------------------------------------------------------
bool isValid(const Graph& g, const std::vector<int>& colors) {
    const int n = g.numVertices();
    for (int v = 0; v < n; ++v) {
        if (colors[v] < 0) return false;          // vertice nao colorido
        for (int u : g.neighbors(v)) {
            if (colors[u] == colors[v]) return false;  // conflito de adjacencia
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// 1) HEURISTICA SEQUENCIAL (GULOSA)
// ---------------------------------------------------------------------------
// Ideia: percorre os vertices na ordem natural 0..n-1 e da a cada vertice a
// MENOR cor que nenhum de seus vizinhos ja coloridos esteja usando.
//
// Detalhe de implementacao (o "truque do sentinela"): em vez de recriar, para
// cada vertice, um vetor booleano de cores proibidas, usamos um unico vetor
// 'forbidden' de inteiros. Marcamos forbidden[c] = v para dizer "a cor c foi
// proibida pelo vertice v". Assim, ao colorir v, uma cor c esta livre se, e
// somente se, forbidden[c] != v. Isso evita reinicializar o vetor a cada passo
// e mantem o custo total em O(V + E).
// ---------------------------------------------------------------------------
ColoringResult greedySequential(const Graph& g) {
    const int n = g.numVertices();
    std::vector<int> color(n, -1);      // -1 = ainda sem cor
    std::vector<int> forbidden(n, -1);  // forbidden[c] == v  =>  cor c proibida para v

    for (int v = 0; v < n; ++v) {
        // Marca como proibidas todas as cores ja usadas pelos vizinhos de v.
        for (int u : g.neighbors(v)) {
            if (color[u] != -1) forbidden[color[u]] = v;
        }
        // Procura a menor cor c que nao foi proibida por nenhum vizinho de v.
        int c = 0;
        while (c < n && forbidden[c] == v) ++c;
        color[v] = c;
    }

    return ColoringResult{color, countColors(color)};
}

// ---------------------------------------------------------------------------
// 2) ALGORITMO DE WELSH-POWELL
// ---------------------------------------------------------------------------
// Ideia: colorir primeiro os vertices "dificeis" (de maior grau). Ordena-se a
// lista por grau decrescente e, para cada cor corrente, varre-se a lista
// pintando todo vertice ainda sem cor que NAO seja vizinho de nenhum vertice
// ja pintado com aquela cor. Repete-se com a proxima cor ate colorir todos.
// ---------------------------------------------------------------------------
ColoringResult welshPowell(const Graph& g) {
    const int n = g.numVertices();

    // 'order' = indices dos vertices ordenados por grau decrescente.
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);          // preenche 0,1,...,n-1
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return g.degree(a) > g.degree(b);              // maior grau primeiro
    });

    std::vector<int> color(n, -1);
    int current = 0;   // cor sendo distribuida nesta passada
    int colored = 0;   // quantos vertices ja receberam cor

    // Cada iteracao do 'while' distribui uma unica cor (current) ao maior
    // numero possivel de vertices independentes entre si.
    while (colored < n) {
        for (int idx = 0; idx < n; ++idx) {
            int v = order[idx];
            if (color[v] != -1) continue;              // ja colorido: pula

            // v pode receber 'current' se nenhum vizinho ja usa essa cor.
            bool conflito = false;
            for (int u : g.neighbors(v)) {
                if (color[u] == current) { conflito = true; break; }
            }
            if (!conflito) {
                color[v] = current;
                ++colored;
            }
        }
        ++current;  // proxima passada usa a proxima cor
    }

    return ColoringResult{color, countColors(color)};
}

// ---------------------------------------------------------------------------
// 3) ALGORITMO DSATUR (BRELAZ, 1979)
// ---------------------------------------------------------------------------
// Ideia: a ordem de coloracao NAO e fixa; ela e decidida a cada passo. Define-
// se o GRAU DE SATURACAO de um vertice como o numero de cores DISTINTAS ja
// presentes em sua vizinhanca. A cada iteracao, escolhe-se o vertice ainda sem
// cor de maior saturacao (desempate: maior grau no subgrafo nao colorido) e
// atribui-se a ele a menor cor disponivel. Ao colorir um vertice, atualiza-se
// a saturacao dos seus vizinhos.
//
// Estruturas auxiliares (por vertice):
//   - neighborColorUsed[v][c]: true se a cor c ja aparece na vizinhanca de v
//     (bitmap que evita contar a mesma cor duas vezes ao medir a saturacao);
//   - saturation[v]: grau de saturacao (numero de cores distintas em torno de v);
//   - uncoloredDegree[v]: quantos vizinhos de v ainda estao sem cor (desempate).
// Custo O(V^2), adequado para as dimensoes deste trabalho (V <= 1000).
// ---------------------------------------------------------------------------
ColoringResult dsatur(const Graph& g) {
    const int n = g.numVertices();
    std::vector<int> color(n, -1);

    std::vector<std::vector<char>> neighborColorUsed(n, std::vector<char>(n, 0));
    std::vector<int> saturation(n, 0);
    std::vector<int> uncoloredDegree(n);
    for (int v = 0; v < n; ++v) uncoloredDegree[v] = g.degree(v);

    // Exatamente n iteracoes: cada uma colore um vertice.
    for (int step = 0; step < n; ++step) {
        // (a) Seleciona o vertice nao colorido de maior saturacao; em caso de
        //     empate, o de maior grau no subgrafo ainda nao colorido.
        int best = -1;
        for (int v = 0; v < n; ++v) {
            if (color[v] != -1) continue;
            if (best == -1 || saturation[v] > saturation[best] ||
                (saturation[v] == saturation[best] &&
                 uncoloredDegree[v] > uncoloredDegree[best])) {
                best = v;
            }
        }

        // (b) Atribui a 'best' a menor cor que ainda nao aparece a sua volta.
        int c = 0;
        while (c < n && neighborColorUsed[best][c]) ++c;
        color[best] = c;

        // (c) Propaga o efeito para os vizinhos ainda sem cor: um deles perdeu
        //     um vizinho nao colorido e pode ter ganho uma nova cor vizinha
        //     (aumentando sua saturacao).
        for (int u : g.neighbors(best)) {
            if (color[u] == -1) {
                --uncoloredDegree[u];
                if (!neighborColorUsed[u][c]) {   // cor c e nova para u?
                    neighborColorUsed[u][c] = 1;
                    ++saturation[u];
                }
            }
        }
    }

    return ColoringResult{color, countColors(color)};
}

}  // namespace Coloring
