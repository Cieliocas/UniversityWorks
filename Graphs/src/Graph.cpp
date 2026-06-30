#include "Graph.h"

#include <algorithm>
#include <numeric>
#include <queue>

// ---------------------------------------------------------------------------
// Graph
// ---------------------------------------------------------------------------

Graph::Graph(int n) : n_(n), m_(0), adj_(static_cast<size_t>(n)) {}

void Graph::addEdge(int u, int v) {
    if (u == v) return;  // ignora laços
    adj_[u].push_back(v);
    adj_[v].push_back(u);
    ++m_;
}

bool Graph::hasEdge(int u, int v) const {
    const auto& nu = adj_[u];
    return std::find(nu.begin(), nu.end(), v) != nu.end();
}

double Graph::density() const {
    if (n_ < 2) return 0.0;
    const double maxEdges = static_cast<double>(n_) * (n_ - 1) / 2.0;
    return static_cast<double>(m_) / maxEdges;
}

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
            if (!visited[u]) {
                visited[u] = 1;
                ++seen;
                q.push(u);
            }
        }
    }
    return seen == n_;
}

// ---------------------------------------------------------------------------
// GraphGenerator
// ---------------------------------------------------------------------------

namespace GraphGenerator {

Graph generate(int n, double p, std::mt19937& rng) {
    Graph g(n);
    if (n <= 1) return g;

    // Matriz de incidência auxiliar (apenas durante a geração) para evitar
    // arestas duplicadas em O(1). Liberada ao final do escopo.
    std::vector<char> edgeExists(static_cast<size_t>(n) * n, 0);
    auto markEdge = [&](int u, int v) {
        edgeExists[static_cast<size_t>(u) * n + v] = 1;
        edgeExists[static_cast<size_t>(v) * n + u] = 1;
    };
    auto edgeSet = [&](int u, int v) {
        return edgeExists[static_cast<size_t>(u) * n + v] != 0;
    };

    // Etapa 1: árvore geradora aleatória -> garante conexidade.
    // Cada vértice perm[i] (i>=1) é ligado a um vértice já inserido perm[0..i-1].
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);
    for (int i = 1; i < n; ++i) {
        std::uniform_int_distribution<int> pickEarlier(0, i - 1);
        int j = pickEarlier(rng);
        g.addEdge(perm[i], perm[j]);
        markEdge(perm[i], perm[j]);
    }

    // Etapa 2: modelo Erdős–Rényi G(n, p) sobre os pares restantes.
    std::uniform_real_distribution<double> coin(0.0, 1.0);
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (edgeSet(u, v)) continue;       // já é aresta da árvore
            if (coin(rng) < p) {
                g.addEdge(u, v);
                markEdge(u, v);
            }
        }
    }

    // Garantia de NÃO-COMPLETO: se por acaso o grafo ficou completo, remove a
    // primeira aresta encontrada (situação praticamente impossível com p<1 e
    // n grande, mas mantida por robustez).
    const long long maxEdges = static_cast<long long>(n) * (n - 1) / 2;
    if (g.numEdges() == maxEdges && maxEdges > 0) {
        // Reconstrói o grafo sem a aresta {0, 1}.
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
