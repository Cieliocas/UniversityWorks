#ifndef GRAPH_H
#define GRAPH_H

#include <random>
#include <vector>

/**
 * @file Graph.h
 * @brief Grafo não-orientado representado por Lista de Adjacências e um
 *        gerador de instâncias aleatórias (conexas e não-completas).
 *
 * Trabalho final — Teoria e Aplicação de Grafos — Coloração de Grafos.
 */

/**
 * @class Graph
 * @brief Grafo simples, não-orientado, armazenado como Lista de Adjacências.
 *
 * A representação por lista de adjacências (adj_[v] = vizinhos de v) é eficiente
 * em memória para grafos esparsos e densos não-completos, e fornece iteração
 * direta sobre a vizinhança de cada vértice — operação central das heurísticas
 * de coloração.
 */
class Graph {
public:
    /// Constrói um grafo com @p n vértices (rotulados 0..n-1) e sem arestas.
    explicit Graph(int n);

    /**
     * @brief Insere a aresta não-orientada {u, v}.
     *
     * Ignora laços (u == v). Por desempenho NÃO verifica duplicatas: cabe a
     * quem chama garantir unicidade (o gerador já garante). Isso evita um
     * custo O(grau) por inserção, mantendo a geração eficiente.
     */
    void addEdge(int u, int v);

    /// Retorna true se a aresta {u, v} existe. Custo O(grau(u)).
    bool hasEdge(int u, int v) const;

    /// Número de vértices.
    int numVertices() const { return n_; }

    /// Número de arestas (não-orientadas).
    long long numEdges() const { return m_; }

    /// Grau do vértice @p v.
    int degree(int v) const { return static_cast<int>(adj_[v].size()); }

    /// Lista de vizinhos do vértice @p v (somente leitura).
    const std::vector<int>& neighbors(int v) const { return adj_[v]; }

    /**
     * @brief Densidade do grafo em [0, 1].
     *
     * Razão entre o número de arestas existentes e o máximo possível
     * n(n-1)/2 de um grafo simples.
     */
    double density() const;

    /// Verifica conexidade por busca em largura (BFS). Custo O(V + E).
    bool isConnected() const;

private:
    int n_;                                ///< número de vértices
    long long m_;                          ///< número de arestas
    std::vector<std::vector<int>> adj_;    ///< lista de adjacências
};

/**
 * @namespace GraphGenerator
 * @brief Geração de grafos aleatórios para o benchmark.
 */
namespace GraphGenerator {

/**
 * @brief Gera um grafo não-orientado, CONEXO e NÃO-COMPLETO com @p n vértices.
 *
 * Estratégia em duas etapas:
 *   1. Árvore geradora aleatória (random spanning tree): liga todos os
 *      vértices com n-1 arestas, garantindo que o grafo seja CONEXO.
 *   2. Modelo Erdős–Rényi G(n, p): para cada par de vértices ainda não
 *      ligado, adiciona a aresta com probabilidade @p p. Como p < 1, o grafo
 *      permanece NÃO-COMPLETO com altíssima probabilidade; um ajuste final
 *      remove uma aresta caso, por acaso, o grafo fique completo.
 *
 * @param n   número de vértices (V).
 * @param p   probabilidade de aresta (densidade-alvo), 0 < p < 1.
 * @param rng gerador Mersenne Twister (semente controlada → reprodutível).
 */
Graph generate(int n, double p, std::mt19937& rng);

}  // namespace GraphGenerator

#endif  // GRAPH_H
