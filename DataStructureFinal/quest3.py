import time
import re
from collections import deque, namedtuple, defaultdict
import matplotlib.pyplot as plt

#Ler aquivo
def ler_arquivo(file):
    with open(file, 'r', encoding='utf-8') as arq:
        arquivo = arq.read()
        padrao = r"[ \t\n,.;!?()\"]+"  

    palavras = [p for p in re.split(padrao, arquivo) if p]
    return palavras

palavras_consulta = [
    'Lisbon', 'NASA', 'Kyunghee', 'Konkuk', 'Sogang',
    'momentarily', 'rubella', 'vaccinations', 'government', 'Authorities'
 ]
#Calcula o tempo da inserção de cada collections:
def insercao(words, estrutura):
    if estrutura == 'list':
        tipoEstrutura = list()
        for w in words:
            tipoEstrutura.append(w)
        return tipoEstrutura

    elif estrutura == 'set':
        tipoEstrutura = set()
        for w in words:
            tipoEstrutura.add(w)
        return tipoEstrutura
    
    elif estrutura == 'dict':
        tipoEstrutura = dict()
        for i, w in enumerate(words):
            tipoEstrutura[w] = i
        return tipoEstrutura
    
    elif estrutura == 'deque':
        tipoEstrutura = deque()
        for w in words:
            tipoEstrutura.append(w)
        return tipoEstrutura
    
    elif estrutura == 'defaultdict':
        tipoEstrutura = defaultdict(int)
        for w in words:
            tipoEstrutura[w] += 1
        return tipoEstrutura
    
    elif estrutura == 'namedtuple':
        tipoEstrutura = list()
        tipo = namedtuple('Estrutura', ['Valor'])
        for w in words:
            obj = tipo(w)
            tipoEstrutura.append(obj)
        return tipoEstrutura
    
def tempo_busca(words, estrutura, words_consults):
    struct = insercao(words, estrutura)

    init = time.perf_counter()
    if estrutura == 'list':
        flag = False
        for w in words_consults:
            if w in struct:
                flag =  True

    elif estrutura == 'dict':
        flag = False
        for w in words_consults:
            if w in struct:
                flag = True

    elif estrutura == 'set':
        flag = False
        for w in words_consults:
            if w in struct:
                flag = True

    elif estrutura == 'deque':
        flag = False
        for w in words_consults:
            if w in struct:
                flag = True

    elif estrutura == 'defaultdict':
        flag = False
        for w in words_consults:
            if w in struct:
                flag = True
    
    elif estrutura == 'namedtuple':
        for w in words_consults:
            flag = False
            for obj in struct:
                if obj.Valor == w:
                    flag = True
                    break
    
    end = time.perf_counter()
    return f"{(end - init) / len(words_consults):.3f}"


word = ler_arquivo('leipzig100k.txt')
estruturas = ['list', 'set', 'dict', 'deque', 'defaultdict', 'namedtuple']
tempos = dict()

for estrutura in estruturas:
    tempo = tempo_busca(word, estrutura, palavras_consulta)
    tempos[estrutura] = float(tempo)

print(tempos)

estruturas = list(tempos.keys())
valores = list(tempos.values())

plt.figure(figsize=(10, 6))
plt.bar(estruturas, valores)
plt.xlabel('Estrutura de Dados')
plt.ylabel('Tempo (segundos)')
plt.title('Média do Tempo de Busca das Palavras Consultadas:')
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()
