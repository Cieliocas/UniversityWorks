// Q3 - Tabela Hash com Lista Encadeada
// ARQUIVO: q3_tabela_hash.go
// Implementa uma Tabela Hash de tamanho 5, usando lista encadeada para colisões.

package main

import (
	"fmt"
)

// =============================================================================
// CONSTANTES E ESTRUTURAS
// =============================================================================

const TAMANHO_TABELA = 5

// NoHash é o nó da lista encadeada dentro de cada "bucket" da Hash.
type NoHash struct {
	chave   string
	valor   int
	proximo *NoHash // Ponteiro para o próximo nó na lista de colisão
}

// TabelaHash é a estrutura principal. Usa um array de 5 posições.
// Cada posição guarda um ponteiro para a cabeça da lista de colisão.
type TabelaHash struct {
	tabela [TAMANHO_TABELA]*NoHash 
}

// NewTabelaHash é o CONSTRUTOR.
func NewTabelaHash() *TabelaHash {
	// Em Go, um array de ponteiros é inicializado com nil por padrão.
	return &TabelaHash{} 
}

// =============================================================================
// MÉTODOS (FUNÇÕES DA TABELA)
// =============================================================================

// hash: FUNÇÃO CENTRAL. Mapeia a string para um índice de 0 a 4.
func (t *TabelaHash) hash(chave string) int {
	hashValue := 0
	// char é o rune (código Unicode) do caractere
	for _, char := range chave { 
		hashValue = (hashValue + int(char))
	}
	// O operador % (módulo) garante que o resultado esteja entre 0 e 4
	return hashValue % TAMANHO_TABELA 
}

// inserir adiciona um novo par (chave, valor) à tabela.
func (t *TabelaHash) inserir(chave string, valor int) {
	indice := t.hash(chave)
	atual := t.tabela[indice]

	// 1. Percorre a lista de colisão para verificar se a chave já existe
	for atual != nil {
		if atual.chave == chave {
			fmt.Printf("[Inserir Hash] \"%s\": chave já existente\n", chave)
			return
		}
		atual = atual.proximo
	}

	// 2. Adiciona o novo nó no INÍCIO da lista encadeada do bucket
	novoNo := &NoHash{chave: chave, valor: valor, proximo: t.tabela[indice]}
	t.tabela[indice] = novoNo // Atualiza a cabeça do bucket
	fmt.Printf("[Inserir Hash] Sucesso: (%s, %d) na posição %d\n", chave, valor, indice)
}

// buscar retorna o valor associado à chave.
func (t *TabelaHash) buscar(chave string) (int, bool) {
	indice := t.hash(chave)
	atual := t.tabela[indice]

	// Percorre a lista de colisão do índice
	for atual != nil {
		if atual.chave == chave {
			return atual.valor, true
		}
		atual = atual.proximo
	}

	fmt.Printf("[Buscar Hash] \"%s\": chave não for encontrada\n", chave)
	return 0, false
}

// listar mostra todos os elementos, indicando a posição (bucket).
func (t *TabelaHash) listar() {
	fmt.Println("\n================ LISTAGEM (Tabela Hash - Tamanho 5) ================")

	for i := 0; i < TAMANHO_TABELA; i++ {
		cabeca := t.tabela[i]
		elementos := ""
		atual := cabeca

		// Percorre a lista encadeada em cada índice 'i'
		for atual != nil {
			elementos += fmt.Sprintf("(%s, %d) -> ", atual.chave, atual.valor)
			atual = atual.proximo
		}

		if len(elementos) > 0 {
			// Remove o último " -> "
			fmt.Printf("Posição [%d]: %s\n", i, elementos[:len(elementos)-4])
		} else {
			fmt.Printf("Posição [%d]: nulo\n", i) // Requisito: mostrar "nulo"
		}
	}
	fmt.Println("=====================================================================")
}

// remover remove o par (chave, valor) da tabela.
func (t *TabelaHash) remover(chave string) {
	indice := t.hash(chave)
	cabeca := t.tabela[indice]

	if cabeca == nil {
		fmt.Printf("[Remover Hash] \"%s\": chave não for encontrada.\n", chave)
		return
	}

	// Caso 1: Remove a CABEÇA da lista de colisão
	if cabeca.chave == chave {
		t.tabela[indice] = cabeca.proximo
		fmt.Printf("[Remover Hash] Sucesso: \"%s\"\n", chave)
		return
	}

	// Caso 2: Remove nó no meio/fim da lista
	anterior := cabeca
	atual := cabeca.proximo

	for atual != nil {
		if atual.chave == chave {
			anterior.proximo = atual.proximo // O nó anterior "pula" o nó atual
			fmt.Printf("[Remover Hash] Sucesso: \"%s\"\n", chave)
			return
		}
		anterior = atual
		atual = atual.proximo
	}

	fmt.Printf("[Remover Hash] \"%s\": chave não for encontrada.\n", chave)
}

// =============================================================================
// FUNÇÃO DE TESTE DA QUESTÃO 3
// =============================================================================

func main() {
	fmt.Println("\n\n==========================================================")
	fmt.Println("           TESTE Q3: TABELA HASH (Tamanho 5)           ")
	fmt.Println("==========================================================")

	tabela := NewTabelaHash()

	// Inserções com colisões (ex: HASH("A") = 0, HASH("F") = 0)
	tabela.inserir("A", 10)
	tabela.inserir("F", 16) 
	tabela.inserir("B", 20)
	tabela.inserir("G", 27) 
	tabela.inserir("C", 30)
	tabela.inserir("H", 38)
	
	tabela.inserir("A", 99) 

	// Teste de Listagem
	tabela.listar()

	// Teste de Busca
	valor, encontrado := tabela.buscar("G")
	if encontrado {
		fmt.Printf("[Resultado Busca \"G\"]: %d\n", valor)
	}
	tabela.buscar("Z") 

	// Teste de Remoção
	tabela.remover("A") 
	tabela.remover("F") 
	tabela.remover("C") 
	tabela.remover("Z") 

	// Teste Final
	tabela.listar()
}