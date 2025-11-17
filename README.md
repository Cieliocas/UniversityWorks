# 📚 UFPI - Estruturas de Dados: Atividade Extra (2025.2)

## Tabela de Símbolos: Implementações e Análise Comparativa

Este repositório contém a resolução da Atividade Extra da disciplina de
Estruturas de Dados, implementada na linguagem **Go (Golang)**. O
objetivo é implementar uma Tabela de Símbolos usando três estruturas de
dados distintas para demonstrar os *trade-offs* de cada abordagem.

------------------------------------------------------------------------

## 🚀 Como Executar o Projeto

O projeto está organizado no pacote `main` e é projetado para ser
executado como um pacote Go único.

1.  **Pré-requisito:** Certifique-se de ter o Go instalado em sua
    máquina.
2.  **Estrutura:** Mantenha todos os arquivos `.go` (incluindo
    `main.go`, `q1_...go`, `q2_...go`, etc.) no mesmo diretório.
3.  **Execução:** Utilize o comando `go run .` no diretório raiz do
    projeto.

O arquivo `main.go` atua como um **arquivo de controle**, chamando
sequencialmente as funções de teste (`testarQuestao1`, `testarQuestao2`,
etc.) de cada questão.

``` bash
# Navegue até o diretório do projeto
cd [caminho_do_seu_diretorio]

# Executa todas as questões em sequência
go run .
```

------------------------------------------------------------------------

# 1. 🔗 Questão 1: Tabela de Símbolos em Lista Encadeada

## Conceito

A Tabela de Símbolos é implementada como uma **Lista Encadeada
Simples**.

Esta estrutura é flexível em termos de alocação, mas possui custo de
acesso sequencial.

------------------------------------------------------------------------

## Design em Go

  ----------------------------------------------------------------------------------
  Componente                Tipo Go                        Propósito
  ------------------------- ------------------------------ -------------------------
  **Elemento**              `struct No`                    Armazena chave, valor e o
                                                           ponteiro `proximo *No`.

  **Tabela**                `struct TabelaSimbolosLista`   Armazena a referência
                                                           para a `cabeca *No` da
                                                           lista.
  ----------------------------------------------------------------------------------

------------------------------------------------------------------------

## Complexidade das Operações

  -------------------------------------------------------------------------
  Operação              Complexidade                Observação
  --------------------- --------------------------- -----------------------
  **inserir**           O(N)                        Verifica duplicidade
                                                    percorrendo a lista.

  **buscar / remover**  O(N)                        Busca linear.

  **listarOrdenados**   O(N log N)                  Copia para slice
                                                    temporário e ordena.
  -------------------------------------------------------------------------

------------------------------------------------------------------------

# 2. 🗂️ Questão 2: Tabela de Símbolos em Vetor (Slice) com Redimensionamento

## Conceito

Implementa um **Array Dinâmico** usando slices (`[]Par`). O acesso é
direto por índice, porém redimensionamentos têm custo elevado.

------------------------------------------------------------------------

## Design em Go

  ----------------------------------------------------------------------------------
  Componente                Tipo Go                        Propósito
  ------------------------- ------------------------------ -------------------------
  **Elemento**              `struct Par`                   Armazena chave e valor.

  **Tabela**                `struct TabelaSimbolosVetor`   Gerencia o slice,
                                                           quantidade e capacidade.
  ----------------------------------------------------------------------------------

------------------------------------------------------------------------

## Destaque da Implementação

### 🔄 Redimensionamento

-   Usa `copy()` para mover elementos para um array maior.
-   Capacidade dobra quando o fator de carga (\~75%) é atingido.
-   Custo: **O(N)**.

### 🗑 Remoção

-   Usa `copy()` para deslocar elementos.
-   Custo: **O(N)**.

------------------------------------------------------------------------

# 3. 🧩 Questão 3: Tabela Hash com Lista Encadeada (Encadeamento Separado)

## Conceito

Combina a velocidade de acesso do array com a flexibilidade das listas
para lidar com colisões.

------------------------------------------------------------------------

## Design em Go

  Componente    Tipo Go           Propósito
  ------------- ----------------- ------------------------
  **Tabela**    `[5]*NoHash`      Array fixo de buckets.
  **Colisão**   `struct NoHash`   Nó da lista encadeada.

------------------------------------------------------------------------

## Destaque da Implementação

### 🔢 Função Hash

Mapeia chave → índice (`0 a 4`) usando:

    soma dos caracteres % 5

### ⚡ Inserção e Busca

-   Índice calculado em O(1)
-   Buscar/inserir ocorre apenas na lista do bucket correspondente.
-   Custo médio **O(1)**.

### 📄 listar()

Exibe:

-   Os 5 buckets
-   Cadeias de colisões
-   Elementos presentes
