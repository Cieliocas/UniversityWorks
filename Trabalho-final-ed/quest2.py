import time
import re
from collections import deque, namedtuple, defaultdict
import matplotlib.pyplot as plt
import numpy as np # Importado para futuros ajustes de gráfico

# --- Configurações ---
NUM_EXECUCOES = 5  # Número de repetições para o cálculo do tempo médio
ESTRUTURAS_TESTADAS = ['list', 'set', 'dict', 'deque', 'defaultdict', 'namedtuple'] 

# Definição do NamedTuple (Para simular a inserção de objetos)
NamedWord = namedtuple('NamedWord', ['Valor'])

# --- Funções Auxiliares ---
def ler_arquivo(file):
    """Lê o arquivo de texto e retorna uma lista de palavras."""
    try:
        with open(file, 'r', encoding='utf-8') as arq:
            arquivo = arq.read()
            # Padrão de separação: espaço, tab, quebra de linha, pontuação, etc.
            padrao = r"[ \t\n,.;!?()\"]+"  
        palavras = [p for p in re.split(padrao, arquivo) if p]
        return palavras
    except FileNotFoundError:
        print(f"ERRO: Arquivo '{file}' não encontrado. Certifique-se de que está na mesma pasta.")
        return []

def tempo_insercao(words, estrutura):
    """Calcula o tempo total para inserir todas as palavras na estrutura."""
    init = time.perf_counter()

    if estrutura == 'list':
        tipoEstrutura = list()
        for w in words:
            tipoEstrutura.append(w)

    elif estrutura == 'set':
        tipoEstrutura = set()
        for w in words:
            tipoEstrutura.add(w)

    elif estrutura == 'dict':
        tipoEstrutura = dict()
        for i, w in enumerate(words):
            tipoEstrutura[w] = i # Palavra como chave, índice como valor

    elif estrutura == 'deque':
        tipoEstrutura = deque()
        for w in words:
            tipoEstrutura.append(w) # Inserção no final

    elif estrutura == 'defaultdict':
        # Simula a contagem de frequência ou agrupamento
        tipoEstrutura = defaultdict(int)
        for w in words:
            tipoEstrutura[w] += 1

    elif estrutura == 'namedtuple':
        # Simula a criação de um registro NamedTuple para cada palavra e 
        # a inserção em uma lista (operação O(n))
        tipoEstrutura = list()
        for w in words:
            obj = NamedWord(w)
            tipoEstrutura.append(obj)
            
    # Para 'tuple', a inserção seria uma recriação O(n^2), que é impraticável para 140k palavras
    # Por isso, ele não está incluído.

    end = time.perf_counter()
    # Retorna o valor float bruto (sem arredondamento)
    return end - init


# --- Execução Principal (Questão 2) ---
print("--- [ Questão 2 ] Avaliação Temporal: Inclusão ---")

word_list = ler_arquivo('leipzig100k.txt')
if not word_list:
    exit()
print(f"Total de palavras carregadas: {len(word_list)}")

tempos_medios_finais = {}
print(f"\nRealizando {NUM_EXECUCOES} execuções para calcular o tempo médio de inclusão...")

for estrutura in ESTRUTURAS_TESTADAS:
    tempos_execucoes = []
    
    # Repetir o teste NUM_EXECUCOES vezes
    for _ in range(NUM_EXECUCOES):
        tempo_total = tempo_insercao(word_list, estrutura)
        tempos_execucoes.append(tempo_total)
    
    # Calcula o tempo médio das execuções
    tempo_medio = sum(tempos_execucoes) / len(tempos_execucoes)
    tempos_medios_finais[estrutura] = tempo_medio

# 4. Exibir as Informações no Terminal (Estruturado e Aprimorado)
print("\n" + "="*80)
print("  RESULTADO: TEMPO MÉDIO DE INCLUSÃO (140.000 Palavras)")
print("="*80)
# Aumentando o espaço para o tempo para exibir mais precisão
print(f"{'Estrutura':<15} | {'Tempo Médio (s)':<30} | {'Complexidade Esperada':<25}")
print("-" * 80)

for estrutura, tempo in tempos_medios_finais.items():
    if estrutura in ['set', 'dict', 'defaultdict']:
        # Inclusão com hashing, embora seja O(n) pelo loop, a operação é O(1)
        complexidade = "O(n) - Rápido (Hash)" 
    elif estrutura in ['list', 'deque', 'namedtuple']:
        # Inclusão sequencial (O(n) pelo loop)
        complexidade = "O(n) - Rápido (Append ou Extensão)"
    else:
        complexidade = "-"
    
    # Formato de precisão ajustado para 9 casas decimais para clareza no terminal
    tempo_formatado = f"{tempo:.9f} s"
    
    print(f"{estrutura:<15} | {tempo_formatado:<30} | {complexidade:<25}")

print("-" * 80)


# 5. Geração do Gráfico Aprimorado (Com Rótulos)
estruturas_grafico = list(tempos_medios_finais.keys())
valores_grafico = list(tempos_medios_finais.values())
cores = ['#2ECC71', '#3498DB', '#9B59B6', '#E74C3C', '#F39C12', '#7F8C8D']

plt.figure(figsize=(12, 7))
barras = plt.bar(estruturas_grafico, valores_grafico, color=cores)

# Adiciona os valores (rótulos) em cima de cada barra com 6 casas decimais
for bar in barras:
    yval = bar.get_height()
    # Rótulos com precisão de 6 casas e notação científica para valores pequenos
    if yval < 0.001:
        label = f'{yval:.3e}s'
    else:
        label = f'{yval:.6f}s' 
        
    plt.text(bar.get_x() + bar.get_width()/2, 
             yval * 1.01, # Ajuste ligeiro para que o texto fique acima da barra
             label, 
             ha='center', va='bottom', fontsize=9)

plt.xlabel('Estrutura de Dados (Container)', fontsize=12)
plt.ylabel('Tempo Médio de Inclusão (segundos)', fontsize=12)
plt.title(f'Tempo Médio de Inclusão de 140.000 Palavras (Média de {NUM_EXECUCOES} Execuções)', fontsize=14)
plt.xticks(rotation=45, ha='right', fontsize=10)
plt.grid(axis='y', linestyle='--', alpha=0.7) 
plt.tight_layout()
plt.show()