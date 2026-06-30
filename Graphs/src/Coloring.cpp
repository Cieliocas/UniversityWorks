#include "Coloring.h"

#include <algorithm>
#include <numeric>

namespace Coloring {

int countColors(const std::vector<int>& colors) {
    int maxColor = -1;
    for (int c : colors) maxColor = std::max(maxColor, c);
    return maxColor + 1;  // rótulos 0-indexados e contíguos
}

bool isValid(const Graph& g, const std::vector<int>& colors) {
    const int n = g.numVertices();
    for (int v = 0; v < n; ++v) {
        if (colors[v] < 0) return false;  // vértice não colorido
        for (int u : g.neighbors(v)) {
            if (colors[u] == colors[v]) return false;  // conflito de adjacência
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// 1) Heurística Sequencial (Gulosa)
// ---------------------------------------------------------------------------
ColoringResult greedySequential(const Graph& g) {
    const int n = g.numVertices();
    std::vector<int> color(n, -1);

    // forbidden[c] == v  =>  a cor c já foi vista entre os vizinhos de v.
    // Truque clássico que evita realocar um vetor de proibições por vértice.
    std::vector<int> forbidden(n, -1);

    for (int v = 0; v < n; ++v) {
        for (int u : g.neighbors(v)) {
            if (color[u] != -1) forbidden[color[u]] = v;
        }
        int c = 0;
        while (c < n && forbidden[c] == v) ++c;  // menor cor disponível
        color[v] = c;
    }

    return ColoringResult{color, countColors(color)};
}

// ---------------------------------------------------------------------------
// 2) Algoritmo de Welsh-Powell
// ---------------------------------------------------------------------------
ColoringResult welshPowell(const Graph& g) {
    const int n = g.numVertices();

    // Ordena os índices dos vértices por grau decrescente.
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return g.degree(a) > g.degree(b);
    });

    std::vector<int> color(n, -1);
    int current = 0;       // cor corrente
    int colored = 0;       // total já colorido

    while (colored < n) {
        // Percorre a lista (ordem de grau) atribuindo a cor corrente a todo
        // vértice sem cor que não conflite com vértices já pintados com ela.
        for (int idx = 0; idx < n; ++idx) {
            int v = order[idx];
            if (color[v] != -1) continue;

            bool conflito = false;
            for (int u : g.neighbors(v)) {
                if (color[u] == current) { conflito = true; break; }
            }
            if (!conflito) {
                color[v] = current;
                ++colored;
            }
        }
        ++current;
    }

    return ColoringResult{color, countColors(color)};
}

// ---------------------------------------------------------------------------
// 3) Algoritmo DSATUR (Brélaz, 1979)
// ---------------------------------------------------------------------------
ColoringResult dsatur(const Graph& g) {
    const int n = g.numVertices();
    std::vector<int> color(n, -1);

    // Para cada vértice: bitmap de cores presentes na vizinhança (saturação),
    // o grau de saturação (nº de cores distintas) e o grau no subgrafo ainda
    // não colorido (usado como critério de desempate).
    std::vector<std::vector<char>> neighborColorUsed(
        n, std::vector<char>(n, 0));
    std::vector<int> saturation(n, 0);
    std::vector<int> uncoloredDegree(n);
    for (int v = 0; v < n; ++v) uncoloredDegree[v] = g.degree(v);

    for (int step = 0; step < n; ++step) {
        // Seleciona o vértice não-colorido de maior saturação; desempata pelo
        // maior grau no subgrafo não colorido.
        int best = -1;
        for (int v = 0; v < n; ++v) {
            if (color[v] != -1) continue;
            if (best == -1 || saturation[v] > saturation[best] ||
                (saturation[v] == saturation[best] &&
                 uncoloredDegree[v] > uncoloredDegree[best])) {
                best = v;
            }
        }

        // Atribui a menor cor não presente na vizinhança de 'best'.
        int c = 0;
        while (c < n && neighborColorUsed[best][c]) ++c;
        color[best] = c;

        // Atualiza saturação e grau-não-colorido dos vizinhos de 'best'.
        for (int u : g.neighbors(best)) {
            if (color[u] == -1) {
                --uncoloredDegree[u];
                if (!neighborColorUsed[u][c]) {
                    neighborColorUsed[u][c] = 1;
                    ++saturation[u];
                }
            }
        }
    }

    return ColoringResult{color, countColors(color)};
}

}  // namespace Coloring
