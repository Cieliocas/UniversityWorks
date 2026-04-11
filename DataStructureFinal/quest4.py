import time
import re
from collections import deque, defaultdict, namedtuple
import matplotlib.pyplot as plt
import numpy as np 

# --- Configurações ---
PALAVRAS_EXCLUIR = [
    'Lisbon', 'NASA', 'Kyunghee', 'Konkuk', 'Sogang', 
    'momentarily', 'rubella', 'vaccinations', 'government', 'Authorities'
]
NUM_EXECUCOES = 5  # Número de repetições para o cálculo da média
# NamedTuple adicionado à lista de estruturas a serem testadas
ESTRUTURAS_TESTADAS = ['list', 'set', 'dict', 'deque', 'defaultdict', 'namedtuple'] 

# Definição simples de um NamedTuple (usada para representar a classe)
# Embora o namedtuple não seja ideal para esta quantidade de dados, o teste
# simula a operação de remoção de uma estrutura imutável.
NamedWord = namedtuple('NamedWord', ['word'])


# --- Funções Auxiliares ---
def ler_arquivo(file):
    """Lê o arquivo de texto e retorna uma lista de palavras."""
    try:
        with open(file, 'r', encoding='utf-8') as arq:
            arquivo = arq.read()
            padrao = r"[ \t\n,.;!?()\"]+" 
        palavras = [p for p in re.split(padrao, arquivo) if p]
        return palavras
    except FileNotFoundError:
        print(f"ERRO: Arquivo '{file}' não encontrado. Certifique-se de que está na mesma pasta.")
        return []

def inicializar_colecao(words, estrutura):
    """Inicializa e preenche a coleção com todas as palavras (140k)."""
    if estrutura == 'list':
        return list(words)
    elif estrutura == 'set':
        return set(words)
    elif estrutura == 'dict':
        return {w: i for i, w in enumerate(words)}
    elif estrutura == 'deque':
        return deque(words)
    elif estrutura == 'defaultdict':
        colecao = defaultdict(list)
        for w in words:
            colecao[w].append(1)
        return colecao
    elif estrutura == 'namedtuple':
        # Namedtuple é imutável. A inicialização é via conversão da lista
        # para a estrutura subjacente (tuple).
        return tuple(words)
    return None 

def medir_tempo_exclusao(colecao_base, estrutura):
    """Mede o tempo total para excluir as 10 palavras na estrutura. 
    A colecao_base é copiada (ou reconstruída) para manter a original intacta."""
    
    # Faz uma cópia inicial da coleção para as estruturas mutáveis
    if estrutura == 'list':
        colecao = colecao_base[:]
    elif estrutura == 'set':
        colecao = colecao_base.copy()
    elif estrutura in ['dict', 'defaultdict']:
        colecao = colecao_base.copy()
    elif estrutura == 'deque':
        colecao = colecao_base.copy()
    elif estrutura == 'namedtuple':
        # Para namedtuple/tuple (imutável), a exclusão envolve reconstrução total (O(n))
        colecao = colecao_base # A variável será reconstruída a cada remoção
    else:
        return 0.0

    total_tempo = 0.0
    num_exclusoes = 0

    for w in PALAVRAS_EXCLUIR:
        try:
            init = time.time()
            
            if estrutura in ['list', 'deque']:
                colecao.remove(w) 
            elif estrutura == 'set':
                colecao.discard(w) 
            elif estrutura in ['dict', 'defaultdict']:
                del colecao[w]
            elif estrutura == 'namedtuple':
                # Remove um item de uma estrutura imutável: Lento!
                # 1. Converte para lista (O(n))
                temp_list = list(colecao)
                # 2. Busca e remove o item (O(n))
                temp_list.remove(w)
                # 3. Converte de volta para tuple (O(n))
                colecao = tuple(temp_list)
                
            end = time.time()
            total_tempo += (end - init)
            num_exclusoes += 1
            
        except (ValueError, KeyError, IndexError):
            # A palavra não estava na coleção
            pass

    return total_tempo / num_exclusoes if num_exclusoes > 0 else 0.0


# --- Execução Principal (Questão 4) ---
print("--- [ Questão 4 ] Avaliação Temporal: Exclusão ---")

# 1. Carregar as palavras do arquivo
word_list = ler_arquivo('leipzig100k.txt')
if not word_list:
    exit()
print(f"Total de palavras carregadas: {len(word_list)}")

# 2. Inicializar as coleções base
colecoes_base = {}
for estrutura in ESTRUTURAS_TESTADAS:
    colecoes_base[estrutura] = inicializar_colecao(word_list, estrutura)

# 3. Medir e Calcular o Tempo Médio
tempos_medios_finais = {}
print(f"\nRealizando {NUM_EXECUCOES} execuções para calcular a média...")

for estrutura in ESTRUTURAS_TESTADAS:
    tempos_execucoes = []
    
    for _ in range(NUM_EXECUCOES):
        tempo_total = medir_tempo_exclusao(colecoes_base[estrutura], estrutura)
        tempos_execucoes.append(tempo_total)
    
    tempo_medio = sum(tempos_execucoes) / len(tempos_execucoes)
    tempos_medios_finais[estrutura] = tempo_medio

# 4. Exibir as Informações no Terminal (Estruturado)
print("\n" + "="*75)
print("  RESULTADO: TEMPO MÉDIO DE EXCLUSÃO (10 Palavras)")
print("="*75)
print(f"{'Estrutura':<15} | {'Tempo Médio (s)':<25} | {'Complexidade Esperada':<30}")
print("-" * 75)

for estrutura, tempo in tempos_medios_finais.items():
    if estrutura in ['set', 'dict', 'defaultdict']:
        complexidade = "O(1) - Muito Rápido (Hash)"
    elif estrutura == 'deque':
        complexidade = "O(n) - Sequencial/Lento (Remove)"
    elif estrutura in ['list', 'namedtuple']:
        complexidade = "O(n) - Lento (Busca + Deslocamento/Reconstrução)"
    else:
        complexidade = "-"
    
    # Exibe o tempo com 8 casas decimais para clareza
    print(f"{estrutura:<15} | {tempo:<25.8f} | {complexidade:<30}")

print("-" * 75)

# 5. Geração do Gráfico Aprimorado (com Escala Logarítmica)
estruturas_grafico = list(tempos_medios_finais.keys())
valores_grafico = list(tempos_medios_finais.values())
cores = ['#2ECC71', '#3498DB', '#9B59B6', '#E74C3C', '#F39C12', '#7F8C8D'] # Nova cor para namedtuple

plt.figure(figsize=(14, 8))
barras = plt.bar(estruturas_grafico, valores_grafico, color=cores)

# Aplica a escala logarítmica ao eixo Y (solução para dados insignificantes)
plt.yscale('log')

# Adiciona os valores nas barras (rótulos)
for bar in barras:
    yval = bar.get_height()
    # Usa notação científica para valores pequenos (< 10^-4)
    if yval < 0.0001 and yval > 0:
        label = f'{yval:.2e}s' 
        plt.text(bar.get_x() + bar.get_width()/2, 
                 yval * 1.05, label, 
                 ha='center', va='bottom', fontsize=10)
    elif yval > 0:
        label = f'{yval:.6f}s'
        plt.text(bar.get_x() + bar.get_width()/2, 
                 yval * 1.05, label, 
                 ha='center', va='bottom', fontsize=10)

plt.xlabel('Estrutura de Dados (Container)', fontsize=14)
plt.ylabel('Tempo Médio de Exclusão (segundos) - ESCALA LOGARÍTMICA', fontsize=14)
plt.title(f'Comparativo de Desempenho: Exclusão de 10 Palavras (Média de {NUM_EXECUCOES} Execuções)', fontsize=16, pad=20)
plt.xticks(rotation=15, ha='right', fontsize=12)
plt.grid(axis='y', linestyle='--', alpha=0.7) 
plt.tight_layout()
plt.show()