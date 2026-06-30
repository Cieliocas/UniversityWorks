#ifndef COLORING_H
#define COLORING_H

#include <vector>

#include "Graph.h"

/**
 * @file Coloring.h
 * @brief Heurísticas de coloração de vértices abordadas nos slides da
 *        disciplina: Sequencial (Gulosa), Welsh-Powell e DSATUR (Brélaz).
 *
 * Em todas as heurísticas as cores são inteiros 0, 1, 2, ... (0-indexadas).
 * Duas cores adjacentes nunca recebem o mesmo rótulo. O objetivo é minimizar
 * o número de cores — uma aproximação do número cromático χ(G), que é
 * NP-difícil de calcular exatamente (Garey & Johnson, 1974).
 */

/**
 * @struct ColoringResult
 * @brief Resultado de uma coloração: o rótulo de cada vértice e o total de
 *        cores distintas utilizadas.
 */
struct ColoringResult {
    std::vector<int> colors;  ///< colors[v] = cor atribuída ao vértice v
    int numColors = 0;        ///< quantidade de cores distintas utilizadas
};

namespace Coloring {

/**
 * @brief Heurística Sequencial (Gulosa).
 *
 * Percorre os vértices na ordem natural 0..n-1 e atribui a cada um o MENOR
 * inteiro (cor) ainda não usado por seus vizinhos já coloridos. Garante o
 * limitante superior χ(G) ≤ Δ + 1. Custo O(V + E).
 */
ColoringResult greedySequential(const Graph& g);

/**
 * @brief Algoritmo de Welsh-Powell.
 *
 * 1. Ordena os vértices em ordem DECRESCENTE de grau.
 * 2. Para a cor corrente, percorre a lista colorindo todo vértice ainda sem
 *    cor que não seja adjacente a nenhum vértice já pintado com essa cor.
 * 3. Incrementa a cor e repete até todos os vértices estarem coloridos.
 *
 * Tende a usar menos cores que a sequencial pura por priorizar vértices de
 * alto grau.
 */
ColoringResult welshPowell(const Graph& g);

/**
 * @brief Algoritmo DSATUR (Brélaz, 1979).
 *
 * A ordem de coloração é construída dinamicamente: a cada passo escolhe o
 * vértice não-colorido de MAIOR grau de saturação (número de cores distintas
 * já presentes em sua vizinhança), desempatando pelo maior grau no subgrafo
 * ainda não colorido. Atribui-lhe a menor cor disponível. Em geral produz as
 * melhores soluções entre as três heurísticas.
 */
ColoringResult dsatur(const Graph& g);

/**
 * @brief Verifica se @p colors é uma coloração própria de @p g.
 * @return true se nenhum par de vértices adjacentes compartilha a mesma cor.
 */
bool isValid(const Graph& g, const std::vector<int>& colors);

/// Conta o número de cores distintas em @p colors (rótulos 0-indexados).
int countColors(const std::vector<int>& colors);

}  // namespace Coloring

#endif  // COLORING_H
