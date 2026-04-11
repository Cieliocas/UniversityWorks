// Q2 - Tabela de Símbolos em Vetor com Redimensionamento
// ARQUIVO: q2_vetor_redimensionamento.go
// Implementa uma Tabela de Símbolos usando um Slice (Array dinâmico) com redimensionamento manual.

package main

import (
	"fmt"
	"sort"
)

// =============================================================================
// CONSTANTES E ESTRUTURAS
// =============================================================================

const (
	CAPACIDADE_INICIAL  = 4
	FATOR_CARGA_MAXIMO  = 0.75
)

// Par (similar ao Par do TS) representa a entrada no nosso vetor.
type Par struct {
	chave string
	valor int
}

// TabelaSimbolosVetor gerencia o slice que armazena os Pars.
type TabelaSimbolosVetor struct {
	array      []Par // O slice (vetor dinâmico) que armazena os dados
	tamanho    int   // Capacidade total do slice
	quantidade int   // Número de elementos válidos inseridos
}

// NewTabelaSimbolosVetor é o CONSTRUTOR. Usa make para criar o slice com
// a capacidade inicial.
func NewTabelaSimbolosVetor() *TabelaSimbolosVetor {
	return &TabelaSimbolosVetor{
		array:      make([]Par, CAPACIDADE_INICIAL),
		tamanho:    CAPACIDADE_INICIAL,
		quantidade: 0,
	}
}

// =============================================================================
// MÉTODOS (FUNÇÕES DA TABELA)
// =============================================================================

// redimensionar cria um novo slice e copia os dados existentes para o novo local.
func (t *TabelaSimbolosVetor) redimensionar(novaCapacidade int) {
	novoArray := make([]Par, novaCapacidade)
	// copy é uma função nativa do Go que copia elementos entre slices.
	// Copiamos apenas os elementos [0 até t.quantidade] para o novoArray.
	copy(novoArray, t.array[:t.quantidade]) 
	
	t.array = novoArray
	t.tamanho = novaCapacidade
	fmt.Printf("[Redimensionamento] Nova capacidade = %d\n", t.tamanho)
}

// inserir adiciona um novo par (chave, valor) à tabela.
func (t *TabelaSimbolosVetor) inserir(chave string, valor int) {
	// 1. Verifica se a chave já existe
	for i := 0; i < t.quantidade; i++ {
		if t.array[i].chave == chave {
			fmt.Printf("[Inserir] \"%s\": chave já existente\n", chave)
			return
		}
	}

	// 2. Verifica a condição de Redimensionamento (Fator de Carga)
	if float64(t.quantidade) >= float64(t.tamanho)*FATOR_CARGA_MAXIMO {
		t.redimensionar(t.tamanho * 2) // Dobra o tamanho
	}

	// 3. Adiciona na próxima posição disponível
	t.array[t.quantidade] = Par{chave: chave, valor: valor}
	t.quantidade++
	fmt.Printf("[Inserir] Sucesso: (%s, %d)\n", chave, valor)
}

// buscar retorna o valor associado à chave.
func (t *TabelaSimbolosVetor) buscar(chave string) (int, bool) {
	for i := 0; i < t.quantidade; i++ {
		if t.array[i].chave == chave {
			return t.array[i].valor, true
		}
	}
	fmt.Printf("[Buscar] \"%s\": chave não for encontrada\n", chave)
	return 0, false
}

// listar mostra a quantidade, capacidade e todos os elementos válidos.
func (t *TabelaSimbolosVetor) listar() {
	fmt.Println("\n================ LISTAGEM (Vetor com Redim.) ================")
	fmt.Printf("Quantidade de elementos: %d (Capacidade: %d)\n", t.quantidade, t.tamanho)

	if t.quantidade == 0 {
		fmt.Println("Elementos: Tabela vazia.")
		return
	}

	elementos := ""
	for i := 0; i < t.quantidade; i++ {
		elementos += fmt.Sprintf("(%s, %d) | ", t.array[i].chave, t.array[i].valor)
	}
	fmt.Printf("Elementos (ordem de inserção): %s\n", elementos[:len(elementos)-3])
	fmt.Println("=============================================================")
}

// listarOrdenados copia os dados, ordena e exibe por chave alfabética.
func (t *TabelaSimbolosVetor) listarOrdenados() {
	fmt.Println("\n================ LISTAGEM ORDENADA (Vetor com Redim.) ================")

	if t.quantidade == 0 {
		fmt.Println("Tabela vazia.")
		return
	}

	// 1. Copia o slice de elementos válidos para um slice temporário
	elementosValidos := make([]Par, t.quantidade)
	copy(elementosValidos, t.array[:t.quantidade])

	// 2. Ordena o slice
	sort.Slice(elementosValidos, func(i, j int) bool {
		return elementosValidos[i].chave < elementosValidos[j].chave
	})

	// 3. Exibe
	saida := ""
	for _, par := range elementosValidos {
		saida += fmt.Sprintf("(%s, %d) | ", par.chave, par.valor)
	}
	fmt.Printf("Elementos Ordenados: %s\n", saida[:len(saida)-3])
	fmt.Println("=====================================================================")
}

// remover remove o par, deslocando os elementos subsequentes (shift).
func (t *TabelaSimbolosVetor) remover(chave string) {
	indiceRemover := -1

	// 1. Encontra o índice da chave
	for i := 0; i < t.quantidade; i++ {
		if t.array[i].chave == chave {
			indiceRemover = i
			break
		}
	}

	if indiceRemover == -1 {
		fmt.Printf("[Remover] \"%s\": chave não for encontrada.\n", chave)
		return
	}

	// 2. Desloca (shift): move os elementos [indice+1] para [indice]
	// Isso sobrescreve o elemento a ser removido.
	copy(t.array[indiceRemover:], t.array[indiceRemover+1:t.quantidade])
	t.quantidade--
	
	// 3. Redimensionamento para menor (se o array estiver muito vazio)
	if t.tamanho > CAPACIDADE_INICIAL && t.quantidade <= t.tamanho/4 {
		t.redimensionar(t.tamanho / 2)
	}

	fmt.Printf("[Remover] Sucesso: \"%s\"\n", chave)
}

// =============================================================================
// FUNÇÃO DE TESTE DA QUESTÃO 2
// =============================================================================

func main() {
	fmt.Println("\n\n========================================================")
	fmt.Println("        TESTE Q2: VETOR COM REDIMENSIONAMENTO        ")
	fmt.Println("========================================================")

	tabela := NewTabelaSimbolosVetor()

	// Inserções que forçam o redimensionamento
	tabela.inserir("Gato", 1)
	tabela.inserir("Cachorro", 2)
	tabela.inserir("Elefante", 3)
	tabela.inserir("Peixe", 4) // -> FORÇA REDIMENSIONAMENTO

	tabela.inserir("Gato", 99) 

	// Teste de Listagem
	tabela.listar()
	tabela.listarOrdenados()

	// Teste de Busca
	valor, encontrado := tabela.buscar("Cachorro")
	if encontrado {
		fmt.Printf("[Resultado Busca \"Cachorro\"]: %d\n", valor)
	}
	tabela.buscar("Zebra") 

	// Teste de Remoção
	tabela.remover("Elefante")
	tabela.remover("Gato")
	tabela.remover("Macaco") 

	// Teste Final
	tabela.listar()
	tabela.listarOrdenados()
}