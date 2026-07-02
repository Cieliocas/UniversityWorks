// ===========================================================================
// Graph.cpp
// ---------------------------------------------------------------------------
// Implementacao da classe Graph (grafo nao-orientado em lista de adjacencias)
// e do gerador de instancias aleatorias GraphGenerator, ambos declarados em
// Graph.h.
// ===========================================================================
#include "Graph.h"

#include <algorithm>  // std::find, std::shuffle
#include <numeric>    // std::iota
#include <queue>      // std::queue (usada na BFS de conexidade)

// ===========================================================================
// Classe Graph
// ===========================================================================

// Constroi um grafo com n vertices (0..n-1) e nenhuma aresta. O vetor de
// listas de adjacencia ja nasce com n listas vazias.
Graph::Graph(int n) : n_(n), m_(0), adj_(static_cast<size_t>(n)) {}

// Insere a aresta nao-orientada {u, v}: registra v na lista de u e u na lista
// de v. Ignora lacos (u == v). Por desempenho NAO checa duplicatas — quem
// chama garante unicidade (o gerador faz isso), evitando um custo O(grau) por
// insercao que tornaria a geracao de grafos densos muito lenta.
void Graph::addEdge(int u, int v) {
    if (u == v) return;          // laco: ignorado
    adj_[u].push_back(v);
    adj_[v].push_back(u);
    ++m_;                        // conta a aresta uma unica vez
}

// Verifica se {u, v} existe procurando v na lista de adjacencia de u.
// Custo O(grau(u)).
bool Graph::hasEdge(int u, int v) const {
    const auto& nu = adj_[u];
    return std::find(nu.begin(), nu.end(), v) != nu.end();
}

// Densidade = arestas existentes / arestas possiveis. Para um grafo simples
// com n vertices, o maximo e n(n-1)/2. Retorna valor em [0, 1].
double Graph::density() const {
    if (n_ < 2) return 0.0;
    const double maxEdges = static_cast<double>(n_) * (n_ - 1) / 2.0;
    return static_cast<double>(m_) / maxEdges;
}

// Testa a conexidade por busca em largura (BFS) a partir do vertice 0.
// Se, ao final, o numero de vertices visitados for igual a n, o grafo e
// conexo (existe caminho de 0 a todos os demais). Custo O(V + E).
bool Graph::isConnected() const {
    if (n_ == 0) return true;
    std::vector<char> visited(static_cast<size_t>(n_), 0);
    std::queue<int> q;
    q.push(0);
    visited[0] = 1;
    int seen = 1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u : adj_[v]) {
            if (!visited[u]) {       // primeira vez que alcancamos u
                visited[u] = 1;
                ++seen;
                q.push(u);
            }
        }
    }
    return seen == n_;
}

// ===========================================================================
// GraphGenerator: geracao de grafos aleatorios conexos e nao-completos
// ===========================================================================
namespace GraphGenerator {

// Gera um grafo com n vertices em duas etapas (ver explicacao em Graph.h):
//   1) arvore geradora aleatoria  -> garante que o grafo seja CONEXO;
//   2) modelo Erdos–Renyi G(n, p) -> adiciona as demais arestas com prob. p.
// Uma matriz auxiliar 'edgeExists' (n x n) marca os pares ja ligados em O(1),
// evitando arestas duplicadas sem pagar o custo de varrer listas de adjacencia.
Graph generate(int n, double p, std::mt19937& rng) {
    Graph g(n);
    if (n <= 1) return g;  // 0 ou 1 vertice: nada a conectar

    // Matriz de incidencia auxiliar, usada SO durante a geracao (liberada ao
    // fim da funcao). edgeExists[u*n + v] != 0 significa que {u, v} ja existe.
    std::vector<char> edgeExists(static_cast<size_t>(n) * n, 0);
    auto markEdge = [&](int u, int v) {
        edgeExists[static_cast<size_t>(u) * n + v] = 1;
        edgeExists[static_cast<size_t>(v) * n + u] = 1;
    };
    auto edgeSet = [&](int u, int v) {
        return edgeExists[static_cast<size_t>(u) * n + v] != 0;
    };

    // --- Etapa 1: arvore geradora aleatoria (garante conexidade) ---
    // Embaralha os vertices e liga cada vertice perm[i] (i >= 1) a um vertice
    // ja inserido perm[0..i-1] escolhido ao acaso. Isso forma uma arvore com
    // n-1 arestas que toca todos os vertices.
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);
    for (int i = 1; i < n; ++i) {
        std::uniform_int_distribution<int> pickEarlier(0, i - 1);
        int j = pickEarlier(rng);
        g.addEdge(perm[i], perm[j]);
        markEdge(perm[i], perm[j]);
    }

    // --- Etapa 2: modelo Erdos–Renyi G(n, p) ---
    // Para cada par (u, v) ainda nao ligado, sorteia uma moeda viciada e
    // adiciona a aresta com probabilidade p. Como p < 1, o grafo tende a
    // permanecer nao-completo.
    std::uniform_real_distribution<double> coin(0.0, 1.0);
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (edgeSet(u, v)) continue;   // ja e aresta (da arvore)
            if (coin(rng) < p) {
                g.addEdge(u, v);
                markEdge(u, v);
            }
        }
    }

    // --- Garantia de NAO-COMPLETO ---
    // Situacao praticamente impossivel com p < 1 e n grande, mas mantida por
    // robustez: se o grafo saiu completo, reconstroi-o sem a aresta {0, 1}.
    const long long maxEdges = static_cast<long long>(n) * (n - 1) / 2;
    if (g.numEdges() == maxEdges && maxEdges > 0) {
        Graph reduced(n);
        for (int u = 0; u < n; ++u) {
            for (int v : g.neighbors(u)) {
                if (u < v && !(u == 0 && v == 1)) reduced.addEdge(u, v);
            }
        }
        return reduced;
    }

    return g;
}

}  // namespace GraphGenerator
