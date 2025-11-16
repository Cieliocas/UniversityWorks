// Q1 - Tabela de Símbolos em Lista Encadeada
// ARQUIVO: q1_lista_encadeada.go
// Este programa implementa uma Tabela de Símbolos usando Lista Encadeada Simples em Go.

package main

import (
	"fmt"
	"sort"
)

// =============================================================================
// ESTRUTURA DE DADOS: No (Nó da Lista)
// =============================================================================

// No representa um elemento na lista encadeada.
// Contém o par (chave, valor) e um ponteiro para o próximo elemento.
type No struct {
	chave   string
	valor   int
	proximo *No // Ponteiro: Aponta para a próxima struct No na lista.
}

// =============================================================================
// ESTRUTURA DA TABELA DE SÍMBOLOS
// =============================================================================

// TabelaSimbolosLista gerencia a lista encadeada.
type TabelaSimbolosLista struct {
	cabeca     *No // A "cabeça" da lista (início). Se for nil, a lista está vazia.
	quantidade int // Contador para o número total de elementos.
}

// =============================================================================
// MÉTODOS (FUNÇÕES DA TABELA)
// =============================================================================

// inserir adiciona um novo par (chave, valor) à tabela.
// Se a chave já existir, imprime uma mensagem de erro.
func (t *TabelaSimbolosLista) inserir(chave string, valor int) {
	atual := t.cabeca

	// 1. Percorre a lista para verificar se a chave já existe (busca linear)
	for atual != nil {
		if atual.chave == chave {
			fmt.Printf("[Inserir] \"%s\": chave já existente\n", chave)
			return
		}
		atual = atual.proximo // Avança para o próximo nó
	}

	// 2. Cria o novo nó. O & cria um ponteiro para a nova struct.
	novoNo := &No{chave: chave, valor: valor, proximo: t.cabeca}

	// 3. Atualiza a cabeça para apontar para o novo nó (Inserção no início)
	t.cabeca = novoNo
	t.quantidade++
	fmt.Printf("[Inserir] Sucesso: (%s, %d)\n", chave, valor)
}

// buscar retorna o valor associado à chave. Retorna o valor e um booleano
// indicando se a chave foi encontrada.
func (t *TabelaSimbolosLista) buscar(chave string) (int, bool) {
	atual := t.cabeca
	for atual != nil {
		if atual.chave == chave {
			return atual.valor, true // Chave encontrada, retorna valor e true
		}
		atual = atual.proximo
	}
	fmt.Printf("[Buscar] \"%s\": chave não for encontrada\n", chave)
	return 0, false // Chave não encontrada, retorna valor zero e false
}

// listar mostra a quantidade e todos os elementos da tabela na ordem atual.
func (t *TabelaSimbolosLista) listar() {
	fmt.Println("\n================ LISTAGEM (Lista Encadeada) ================")
	fmt.Printf("Quantidade de elementos: %d\n", t.quantidade)

	if t.cabeca == nil {
		fmt.Println("Elementos: Tabela vazia.")
		return
	}

	elementos := ""
	atual := t.cabeca
	for atual != nil {
		elementos += fmt.Sprintf("(%s, %d) -> ", atual.chave, atual.valor)
		atual = atual.proximo
	}
	// Limpa o último " -> " e imprime
	fmt.Printf("Elementos (ordem de inserção): %s\n", elementos[:len(elementos)-4])
	fmt.Println("=========================================================")
}

// listarOrdenados mostra todos os elementos da tabela em ordem alfabética de chave.
func (t *TabelaSimbolosLista) listarOrdenados() {
	fmt.Println("\n================ LISTAGEM ORDENADA (Lista Encadeada) ================")

	if t.cabeca == nil {
		fmt.Println("Tabela vazia.")
		return
	}

	// 1. Transfere todos os nós para um slice temporário para facilitar a ordenação
	elementos := []*No{}
	atual := t.cabeca
	for atual != nil {
		elementos = append(elementos, atual)
		atual = atual.proximo
	}

	// 2. Ordena o slice usando sort.Slice e uma função de comparação (lambda-like)
	sort.Slice(elementos, func(i, j int) bool {
		// Ordena em ordem alfabética (chave i < chave j)
		return elementos[i].chave < elementos[j].chave
	})

	// 3. Exibe os elementos ordenados
	saida := ""
	for _, no := range elementos {
		saida += fmt.Sprintf("(%s, %d) -> ", no.chave, no.valor)
	}
	fmt.Printf("Elementos Ordenados: %s\n", saida[:len(saida)-4])
	fmt.Println("==================================================================")
}

// remover remove o par (chave, valor) da tabela.
func (t *TabelaSimbolosLista) remover(chave string) {
	if t.cabeca == nil {
		fmt.Printf("[Remover] \"%s\": Tabela vazia.\n", chave)
		return
	}

	// Caso 1: O nó a ser removido é a CABEÇA
	if t.cabeca.chave == chave {
		t.cabeca = t.cabeca.proximo // A cabeça agora aponta para o segundo nó
		t.quantidade--
		fmt.Printf("[Remover] Sucesso: \"%s\"\n", chave)
		return
	}

	// Caso 2: O nó está no meio ou fim
	anterior := t.cabeca
	atual := t.cabeca.proximo

	for atual != nil {
		if atual.chave == chave {
			anterior.proximo = atual.proximo // O nó anterior "pula" o nó atual
			t.quantidade--
			fmt.Printf("[Remover] Sucesso: \"%s\"\n", chave)
			return
		}
		anterior = atual
		atual = atual.proximo
	}

	fmt.Printf("[Remover] \"%s\": chave não for encontrada.\n", chave)
}

// =============================================================================
// FUNÇÃO DE TESTE DA QUESTÃO 1
// =============================================================================

func main() {
	fmt.Println("=========================================================")
	fmt.Println("          TESTE Q1: LISTA ENCADEDA                       ")
	fmt.Println("=========================================================")

	tabela := TabelaSimbolosLista{} 

	// Teste de Inserção
	tabela.inserir("Banana", 10)
	tabela.inserir("Abacaxi", 5)
	tabela.inserir("Pera", 15)
	tabela.inserir("Uva", 8)
	tabela.inserir("Abacaxi", 20) 

	// Teste de Listagem
	tabela.listar()
	tabela.listarOrdenados()

	// Teste de Busca
	valor, encontrado := tabela.buscar("Pera")
	if encontrado {
		fmt.Printf("[Resultado Busca \"Pera\"]: %d\n", valor)
	}
	tabela.buscar("Manga") 

	// Teste de Remoção
	tabela.remover("Abacaxi")
	tabela.remover("Pera")
	tabela.remover("Manga") 

	// Teste Final
	tabela.listar()
	tabela.listarOrdenados()
}