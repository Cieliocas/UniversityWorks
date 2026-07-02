#!/usr/bin/env python3
"""Gera o relatorio final em PDF no padrao ABNT (UFPI).

Capa com a logo da UFPI, folha de rosto, resumo, sumario automatico, secoes
numeradas, tabelas e figuras (graficos do benchmark) e referencias.

Dependencias: reportlab, matplotlib (apenas para localizar as fontes DejaVu,
que cobrem os simbolos gregos/matematicos usados no texto).

Uso:
    python3 scripts/gerar_relatorio_pdf.py
"""

import os

import matplotlib
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_JUSTIFY, TA_LEFT
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib.units import cm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import (BaseDocTemplate, Frame, Image, KeepTogether,
                                NextPageTemplate, PageBreak, PageTemplate,
                                Paragraph, Spacer, Table, TableStyle)
from reportlab.platypus.tableofcontents import TableOfContents

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LOGO = os.path.join(BASE, "assets", "ufpi-logo.png")
FIG_TEMPO = os.path.join(BASE, "graficos", "grafico_tempo.png")
FIG_CORES = os.path.join(BASE, "graficos", "grafico_cores.png")
SAIDA = os.path.join(BASE, "docs", "Relatorio_Coloracao_Grafos_ABNT.pdf")

# --------------------------------------------------------------------------
# Fontes (DejaVu Serif, tipo Times, com cobertura Unicode completa)
# --------------------------------------------------------------------------
FONTDIR = os.path.join(matplotlib.get_data_path(), "fonts", "ttf")
pdfmetrics.registerFont(TTFont("ABNT", os.path.join(FONTDIR, "DejaVuSerif.ttf")))
pdfmetrics.registerFont(TTFont("ABNT-Bold", os.path.join(FONTDIR, "DejaVuSerif-Bold.ttf")))
pdfmetrics.registerFont(TTFont("ABNT-Italic", os.path.join(FONTDIR, "DejaVuSerif-Italic.ttf")))
pdfmetrics.registerFont(TTFont("ABNT-BoldItalic", os.path.join(FONTDIR, "DejaVuSerif-BoldItalic.ttf")))
pdfmetrics.registerFontFamily("ABNT", normal="ABNT", bold="ABNT-Bold",
                              italic="ABNT-Italic", boldItalic="ABNT-BoldItalic")

# --------------------------------------------------------------------------
# Estilos
# --------------------------------------------------------------------------
BODY = ParagraphStyle("Body", fontName="ABNT", fontSize=12, leading=18,
                      alignment=TA_JUSTIFY, firstLineIndent=1.25 * cm, spaceAfter=6)
BODY_NOINDENT = ParagraphStyle("BodyNI", parent=BODY, firstLineIndent=0)
H1 = ParagraphStyle("H1", fontName="ABNT-Bold", fontSize=12, leading=18,
                    spaceBefore=18, spaceAfter=12, alignment=TA_LEFT)
H1C = ParagraphStyle("H1C", parent=H1, alignment=TA_CENTER)
H2 = ParagraphStyle("H2", fontName="ABNT-Bold", fontSize=12, leading=18,
                    spaceBefore=12, spaceAfter=6, alignment=TA_LEFT)
FRONT_HEAD = ParagraphStyle("FrontHead", fontName="ABNT-Bold", fontSize=12,
                            leading=18, alignment=TA_CENTER, spaceAfter=12)
CENTER = ParagraphStyle("Center", fontName="ABNT", fontSize=12, leading=18,
                        alignment=TA_CENTER)
CENTER_BOLD = ParagraphStyle("CenterBold", parent=CENTER, fontName="ABNT-Bold")
TITLE = ParagraphStyle("Title", fontName="ABNT-Bold", fontSize=14, leading=20,
                       alignment=TA_CENTER)
NOTA = ParagraphStyle("Nota", fontName="ABNT", fontSize=11, leading=14,
                      alignment=TA_JUSTIFY, leftIndent=7 * cm)
CAP = ParagraphStyle("Cap", fontName="ABNT", fontSize=11, leading=14,
                     alignment=TA_CENTER, spaceBefore=6, spaceAfter=2)
FONTE = ParagraphStyle("Fonte", fontName="ABNT", fontSize=10, leading=12,
                       alignment=TA_CENTER, spaceAfter=10)
REF = ParagraphStyle("Ref", fontName="ABNT", fontSize=12, leading=14,
                     alignment=TA_LEFT, spaceAfter=10)
RESUMO_BODY = ParagraphStyle("Resumo", fontName="ABNT", fontSize=12, leading=18,
                             alignment=TA_JUSTIFY, firstLineIndent=0, spaceAfter=12)


# --------------------------------------------------------------------------
# Documento com sumario automatico e numeracao de paginas ABNT
# --------------------------------------------------------------------------
class ReportDoc(BaseDocTemplate):
    def afterFlowable(self, flowable):
        if flowable.__class__.__name__ == "Paragraph":
            style = flowable.style.name
            text = flowable.getPlainText()
            if style in ("H1", "H1C"):
                self.notify("TOCEntry", (0, text, self.page - 1))
            elif style == "H2":
                self.notify("TOCEntry", (1, text, self.page - 1))


def draw_page_number(canvas, doc):
    """Numero de pagina no canto superior direito (exclui a capa da contagem)."""
    canvas.saveState()
    canvas.setFont("ABNT", 10)
    canvas.drawRightString(A4[0] - 2 * cm, A4[1] - 1.7 * cm, str(canvas.getPageNumber() - 1))
    canvas.restoreState()


def build_tables():
    """Tabelas de tempo e de cores (dados do resultados.csv)."""
    estilo = TableStyle([
        ("FONTNAME", (0, 0), (-1, -1), "ABNT"),
        ("FONTNAME", (0, 0), (-1, 0), "ABNT-Bold"),
        ("FONTSIZE", (0, 0), (-1, -1), 11),
        ("ALIGN", (0, 0), (-1, -1), "CENTER"),
        ("VALIGN", (0, 0), (-1, -1), "MIDDLE"),
        ("LINEABOVE", (0, 0), (-1, 0), 1.0, colors.black),
        ("LINEBELOW", (0, 0), (-1, 0), 0.6, colors.black),
        ("LINEBELOW", (0, -1), (-1, -1), 1.0, colors.black),
        ("TOPPADDING", (0, 0), (-1, -1), 5),
        ("BOTTOMPADDING", (0, 0), (-1, -1), 5),
    ])
    tempo = [["V", "Sequencial", "Welsh-Powell", "DSATUR"],
             ["50", "0,0018", "0,0054", "0,0118"],
             ["100", "0,0041", "0,0175", "0,0382"],
             ["200", "0,0122", "0,0726", "0,1210"],
             ["500", "0,0653", "0,5936", "0,6349"],
             ["1000", "0,2481", "3,3291", "2,5344"]]
    cores = [["V", "Sequencial", "Welsh-Powell", "DSATUR"],
             ["50", "13,5", "12,3", "11,3"],
             ["100", "21,9", "20,0", "18,7"],
             ["200", "36,0", "34,0", "30,8"],
             ["500", "71,9", "69,8", "65,1"],
             ["1000", "126,3", "123,4", "115,6"]]
    larguras = [2.5 * cm, 3.8 * cm, 3.8 * cm, 3.8 * cm]
    t1 = Table(tempo, colWidths=larguras, hAlign="CENTER")
    t1.setStyle(estilo)
    t2 = Table(cores, colWidths=larguras, hAlign="CENTER")
    t2.setStyle(estilo)
    return t1, t2


def main():
    story = []

    def P(txt, style=BODY):
        story.append(Paragraph(txt, style))

    def gap(h):
        story.append(Spacer(1, h * cm))

    # ===================== CAPA =====================
    story.append(Image(LOGO, width=3.14 * cm, height=3.6 * cm, hAlign="CENTER"))
    gap(0.2)
    P("UNIVERSIDADE FEDERAL DO PIAUÍ", CENTER_BOLD)
    P("CENTRO DE CIÊNCIAS DA NATUREZA", CENTER_BOLD)
    P("CURSO DE BACHARELADO EM CIÊNCIA DA COMPUTAÇÃO", CENTER_BOLD)
    gap(3.2)
    for nome in ["Diogo Rangel Lopes Gonçalves",
                 "Franciélio Evangelista dos Santos Castro",
                 "Gustavo de Sousa Carvalho",
                 "Pedro Luz Lima"]:
        P(nome, CENTER)
    gap(3.6)
    P("COLORAÇÃO DE GRAFOS: UMA ANÁLISE COMPARATIVA DAS HEURÍSTICAS "
      "SEQUENCIAL, WELSH-POWELL E DSATUR", TITLE)
    gap(4.6)
    P("TERESINA", CENTER)
    P("2026", CENTER)
    story.append(NextPageTemplate("front"))
    story.append(PageBreak())

    # ===================== FOLHA DE ROSTO =====================
    for nome in ["Diogo Rangel Lopes Gonçalves",
                 "Franciélio Evangelista dos Santos Castro",
                 "Gustavo de Sousa Carvalho",
                 "Pedro Luz Lima"]:
        P(nome, CENTER)
    gap(4.0)
    P("COLORAÇÃO DE GRAFOS: UMA ANÁLISE COMPARATIVA DAS HEURÍSTICAS "
      "SEQUENCIAL, WELSH-POWELL E DSATUR", TITLE)
    gap(1.0)
    P("Trabalho apresentado à disciplina Teoria e Aplicação de Grafos do Curso "
      "de Bacharelado em Ciência da Computação do Centro de Ciências da Natureza "
      "da Universidade Federal do Piauí, como requisito parcial para avaliação, "
      "sob a orientação do Prof. Antonio Costa de Oliveira.", NOTA)
    gap(6.6)
    P("TERESINA", CENTER)
    P("2026", CENTER)
    story.append(PageBreak())

    # ===================== RESUMO =====================
    P("RESUMO", FRONT_HEAD)
    P("A coloração de vértices é um problema clássico da Teoria dos Grafos cujo "
      "objetivo é atribuir cores aos vértices de um grafo de modo que vértices "
      "adjacentes recebam cores distintas, minimizando o total de cores. Como a "
      "determinação do número cromático χ(G) é NP-difícil, heurísticas de tempo "
      "polinomial são amplamente empregadas. Este trabalho implementa, em C++, três "
      "heurísticas — Sequencial (Gulosa), Welsh-Powell e DSATUR (Brélaz) — e as "
      "compara empiricamente sobre grafos aleatórios conexos de densidade fixa "
      "(p = 0,5) e dimensões crescentes (V = 50, 100, 200, 500 e 1000), com dez "
      "instâncias por dimensão. Avaliam-se o esforço computacional (tempo médio de "
      "execução) e a qualidade da solução (número médio de cores). Os resultados "
      "mostram que a heurística Sequencial é a mais rápida em todas as dimensões, "
      "enquanto o DSATUR produz consistentemente as melhores colorações, utilizando "
      "cerca de 8,5% menos cores que a Sequencial na maior instância; o Welsh-Powell "
      "ocupa posição intermediária na qualidade e torna-se o mais lento nas maiores "
      "instâncias. Conclui-se que a escolha da heurística deve ponderar o "
      "compromisso entre custo computacional e qualidade da solução.", RESUMO_BODY)
    P("<b>Palavras-chave:</b> Coloração de grafos. Número cromático. Heurísticas. "
      "DSATUR. Welsh-Powell.", RESUMO_BODY)
    story.append(PageBreak())

    # ===================== SUMÁRIO =====================
    P("SUMÁRIO", FRONT_HEAD)
    toc = TableOfContents()
    toc.levelStyles = [
        ParagraphStyle("TOC0", fontName="ABNT-Bold", fontSize=12, leading=22),
        ParagraphStyle("TOC1", fontName="ABNT", fontSize=12, leading=20, leftIndent=1 * cm),
    ]
    story.append(toc)
    story.append(NextPageTemplate("body"))
    story.append(PageBreak())

    # ===================== 1 INTRODUÇÃO =====================
    P("1 INTRODUÇÃO", H1)
    P("A coloração de grafos consiste em atribuir rótulos, tradicionalmente "
      "chamados de “cores”, aos elementos de um grafo sujeitos a determinadas "
      "restrições. Na coloração de vértices — foco deste trabalho — a restrição "
      "fundamental é que nenhum par de vértices adjacentes compartilhe a mesma cor. "
      "O menor número de cores que torna possível uma coloração própria de um grafo "
      "G é denominado número cromático, denotado por χ(G).")
    P("Embora o problema seja simples de enunciar, determinar o número cromático de "
      "um grafo arbitrário é NP-difícil (GAREY; JOHNSON, 1979). Esse fato motiva o "
      "uso de heurísticas de complexidade polinomial, capazes de fornecer boas "
      "soluções — ainda que não necessariamente ótimas — em tempo viável, mesmo para "
      "instâncias de grande porte.")
    P("O objetivo deste trabalho é implementar e comparar empiricamente três "
      "heurísticas clássicas de coloração de vértices — a Sequencial (Gulosa), o "
      "algoritmo de Welsh-Powell e o algoritmo DSATUR (Brélaz) — quanto a duas "
      "métricas: o esforço computacional, medido pelo tempo médio de execução, e a "
      "qualidade da solução, medida pelo número médio de cores utilizadas. O texto "
      "está organizado da seguinte forma: a Seção 2 apresenta a fundamentação "
      "teórica; a Seção 3 descreve as heurísticas; a Seção 4 reúne aplicações "
      "práticas; a Seção 5 detalha a metodologia experimental; a Seção 6 discute os "
      "resultados; e a Seção 7 traz as conclusões.")

    # ===================== 2 FUNDAMENTAÇÃO =====================
    P("2 FUNDAMENTAÇÃO TEÓRICA", H1)
    P("2.1 Coloração de vértices e número cromático", H2)
    P("Uma coloração própria de um grafo G é uma atribuição de cores aos seus "
      "vértices tal que vértices adjacentes recebam cores distintas. O número "
      "cromático χ(G) é a menor quantidade de cores para a qual existe uma coloração "
      "própria; quando uma coloração utiliza exatamente χ(G) cores, ela é dita ótima. "
      "O processo é sempre possível, pois um grafo com n vértices pode, no limite, "
      "ser colorido com n cores distintas.")
    P("2.2 Limitantes para o número cromático", H2)
    P("Diversos limitantes auxiliam na análise de χ(G). Pela coloração sequencial, "
      "tem-se χ(G) ≤ Δ(G) + 1, em que Δ(G) é o grau máximo do grafo: ao colorir um "
      "vértice, seus vizinhos já coloridos utilizam no máximo Δ cores, restando "
      "sempre ao menos uma das cores entre 1 e Δ + 1. O Teorema de Brooks (1941) "
      "refina esse limite: se G é conexo e não é um ciclo ímpar nem um grafo "
      "completo, então χ(G) ≤ Δ(G). Por outro lado, todo clique de tamanho ω "
      "estabelece o limite inferior χ(G) ≥ ω(G), e vale ainda χ(G) ≥ n / α(G), em "
      "que α(G) é o tamanho do maior conjunto independente.")
    P("2.3 Complexidade computacional", H2)
    P("Determinar o número cromático é NP-difícil (GAREY; JOHNSON, 1979). Métodos "
      "exatos — como a força bruta, com custo O(kⁿ), a programação dinâmica, com "
      "O(2,445ⁿ), e técnicas de programação inteira (branch and bound) — possuem "
      "complexidade exponencial e tornam-se inviáveis para instâncias grandes. Os "
      "métodos heurísticos, em contrapartida, têm complexidade polinomial e "
      "constituem o objeto de estudo deste trabalho.")

    # ===================== 3 HEURÍSTICAS =====================
    P("3 HEURÍSTICAS DE COLORAÇÃO", H1)
    P("3.1 Heurística Sequencial (Gulosa)", H2)
    P("A heurística Sequencial percorre os vértices em uma ordem fixa e atribui a "
      "cada vértice a menor cor (inteiro) ainda não utilizada por seus vizinhos já "
      "coloridos. Trata-se de um método guloso, de custo O(V + E), que garante o "
      "limitante χ(G) ≤ Δ + 1. Seu resultado depende da ordem de varredura dos "
      "vértices: ordens diferentes podem produzir colorações com quantidades "
      "diferentes de cores.")
    P("3.2 Algoritmo de Welsh-Powell", H2)
    P("O algoritmo de Welsh-Powell prioriza os vértices de maior grau, que tendem a "
      "ser mais difíceis de colorir. Seus passos são: (1) calcular o grau de cada "
      "vértice; (2) ordenar os vértices em ordem decrescente de grau; (3) atribuir a "
      "cor corrente, percorrendo a lista, a todo vértice ainda sem cor que não seja "
      "adjacente a nenhum vértice já pintado com essa cor; e (4) repetir com a "
      "próxima cor até que todos os vértices estejam coloridos. Costuma utilizar "
      "menos cores do que a Sequencial pura.")
    P("3.3 Algoritmo DSATUR (Brélaz)", H2)
    P("Proposto por Brélaz (1979), o DSATUR constrói a ordem de coloração de forma "
      "dinâmica, com base no grau de saturação de cada vértice — definido como o "
      "número de cores distintas presentes em sua vizinhança. A cada iteração, o "
      "algoritmo seleciona o vértice não colorido de maior grau de saturação "
      "(desempatando pelo maior grau no subgrafo ainda não colorido) e lhe atribui a "
      "menor cor disponível. Com custo O(V²) na implementação direta, costuma "
      "produzir as melhores soluções entre as três heurísticas e é ótimo para grafos "
      "bipartidos e ciclos.")

    # ===================== 4 APLICAÇÕES =====================
    P("4 APLICAÇÕES PRÁTICAS", H1)
    P("Em todas as aplicações a seguir, o número cromático representa a quantidade "
      "mínima de recursos necessários (galpões, frequências, horários, registradores, "
      "fases ou aquários).", BODY_NOINDENT)
    P("4.1 Separação de produtos explosivos", H2)
    P("Uma indústria química precisa armazenar reagentes que, por segurança, não "
      "podem coabitar o mesmo galpão. Cada reagente é um vértice; reagentes "
      "incompatíveis definem as arestas; e cada galpão é uma cor. O número cromático "
      "indica o número mínimo de galpões necessários para o armazenamento seguro.")
    P("4.2 Atribuição de frequências de rádio", H2)
    P("Os vértices representam os transmissores das estações de rádio. Duas estações "
      "são adjacentes quando suas áreas de transmissão se sobrepõem, o que causaria "
      "interferência caso usassem a mesma frequência. Cada cor agrupa estações que "
      "podem receber a mesma frequência, e χ(G) é o número mínimo de frequências.")
    P("4.3 Agendamento de provas", H2)
    P("Deseja-se agendar exames de modo que duas disciplinas com estudantes em comum "
      "não tenham provas no mesmo horário. As disciplinas são vértices; há aresta "
      "entre disciplinas que compartilham estudantes; e cada horário é uma cor. O "
      "número cromático fornece o número mínimo de horários necessários.")
    P("4.4 Alocação de registradores", H2)
    P("Em um laço de programa, várias variáveis permanecem ativas simultaneamente. O "
      "compilador constrói um grafo de interferência cujos vértices são os "
      "registradores simbólicos, com aresta entre dois vértices usados ao mesmo "
      "tempo. Cada registrador físico é uma cor, e χ(G) é o número mínimo de "
      "registradores para evitar o problema de overswapping.")
    P("4.5 Sudoku", H2)
    P("O Sudoku é uma variação da coloração de vértices. Cada uma das 81 células é "
      "um vértice; há aresta entre células na mesma linha, coluna ou bloco 3×3; e os "
      "números de 1 a 9 são as nove cores. Como o grafo contém cliques de nove "
      "vértices, toda coloração válida exige pelo menos nove cores.")
    P("4.6 Semáforos", H2)
    P("Em um cruzamento de duas ruas há oito pistas de tráfego. Em cada fase do "
      "semáforo, apenas os carros das pistas com luz verde prosseguem com segurança. "
      "Cada pista é um vértice; há aresta entre pistas cujos fluxos conflitam; e cada "
      "fase é uma cor. O número cromático é o número mínimo de fases necessárias.")
    P("4.7 Designação de peixes em aquários", H2)
    P("O dono de uma loja comprou peixes de diversas espécies que, em alguns casos, "
      "não podem coabitar o mesmo aquário. Cada espécie é um vértice; há aresta entre "
      "espécies incompatíveis; e cada aquário é uma cor. O número cromático determina "
      "o número mínimo de aquários necessários.")

    # ===================== 5 METODOLOGIA =====================
    P("5 METODOLOGIA", H1)
    P("5.1 Geração dos grafos", H2)
    P("Os grafos são não-orientados, conexos e não-completos, representados por "
      "listas de adjacências. A conexidade é assegurada pela construção inicial de "
      "uma árvore geradora aleatória (n − 1 arestas que ligam todos os vértices); as "
      "demais arestas são inseridas segundo o modelo Erdős–Rényi G(n, p), em que "
      "cada par de vértices ainda não ligado recebe uma aresta com probabilidade "
      "p = 0,5. Como p &lt; 1, o grafo permanece não-completo. A reprodutibilidade é "
      "garantida pelo gerador Mersenne Twister, com semente derivada por instância.")
    P("5.2 Ambiente e medição", H2)
    P("As heurísticas foram implementadas em C++17 e compiladas com otimização "
      "(-O2) via CMake. O tempo de execução foi medido com a biblioteca std::chrono, "
      "cronometrando exclusivamente o algoritmo de coloração — excluídas a geração do "
      "grafo e a validação. Toda coloração foi validada (verificando-se a ausência de "
      "vértices adjacentes com a mesma cor) antes de ser contabilizada. Os "
      "experimentos foram executados em um computador Apple M4 (10 núcleos), 16 GB de "
      "memória, sistema macOS 26.5.1, compilador Apple Clang 21.")
    P("5.3 Parâmetros do experimento", H2)
    P("Foram avaliadas cinco dimensões (V = 50, 100, 200, 500 e 1000), com dez "
      "instâncias independentes por dimensão e densidade fixa p = 0,5. Para cada "
      "configuração, registraram-se a média e o desvio-padrão do tempo de execução e "
      "do número de cores das dez instâncias.")

    # ===================== 6 RESULTADOS =====================
    P("6 RESULTADOS E DISCUSSÃO", H1)
    t1, t2 = build_tables()

    P("6.1 Esforço computacional", H2)
    P("A Tabela 1 e a Figura 1 apresentam o tempo médio de execução. A heurística "
      "Sequencial é a mais rápida em todas as dimensões, como esperado de seu custo "
      "O(V + E): em V = 1000, executa em 0,25 ms, contra 2,53 ms do DSATUR e 3,33 ms "
      "do Welsh-Powell — respectivamente cerca de 10 e 13 vezes mais lentos. "
      "Observa-se ainda uma inversão entre Welsh-Powell e DSATUR: aquele é mais "
      "rápido até V = 500, mas torna-se o mais lento em V = 1000, pois suas "
      "sucessivas varreduras completas por cor crescem rapidamente em grafos densos.")
    story.append(KeepTogether([
        Paragraph("Tabela 1 — Tempo médio de execução, em milissegundos "
                  "(10 instâncias por dimensão)", CAP),
        t1,
        Paragraph("Fonte: Elaborada pelos autores (2026).", FONTE),
    ]))
    story.append(KeepTogether([
        Paragraph("Figura 1 — Esforço computacional das heurísticas "
                  "(escala logarítmica)", CAP),
        Image(FIG_TEMPO, width=14 * cm, height=8.4 * cm, hAlign="CENTER"),
        Paragraph("Fonte: Elaborada pelos autores (2026).", FONTE),
    ]))

    P("6.2 Qualidade da solução", H2)
    P("A Tabela 2 e a Figura 2 mostram o número médio de cores. O DSATUR produz "
      "consistentemente as melhores colorações (menos cores) em todas as dimensões, "
      "seguido pelo Welsh-Powell e, por último, pela Sequencial. Em V = 1000, o "
      "DSATUR utiliza em média 115,6 cores, contra 123,4 do Welsh-Powell e 126,3 da "
      "Sequencial — uma economia de aproximadamente 8,5% (cerca de 10,7 cores) em "
      "relação à Sequencial. A ordenação DSATUR ≤ Welsh-Powell ≤ Sequencial mantém-se "
      "em todas as instâncias avaliadas.")
    story.append(KeepTogether([
        Paragraph("Tabela 2 — Número médio de cores utilizadas "
                  "(10 instâncias por dimensão)", CAP),
        t2,
        Paragraph("Fonte: Elaborada pelos autores (2026).", FONTE),
    ]))
    story.append(KeepTogether([
        Paragraph("Figura 2 — Qualidade da solução: cores utilizadas por heurística", CAP),
        Image(FIG_CORES, width=14 * cm, height=8.4 * cm, hAlign="CENTER"),
        Paragraph("Fonte: Elaborada pelos autores (2026).", FONTE),
    ]))

    P("6.3 Discussão", H2)
    P("Os resultados evidenciam um claro compromisso entre custo computacional e "
      "qualidade da solução. A Sequencial é imbatível em velocidade, mas entrega as "
      "piores colorações; o DSATUR oferece a melhor qualidade ao preço de maior "
      "tempo; e o Welsh-Powell situa-se em posição intermediária, perdendo "
      "competitividade de tempo nas maiores instâncias. A escolha adequada depende da "
      "aplicação: em cenários como a atribuição de frequências de rádio ou a "
      "separação de produtos explosivos, em que cada cor a menos representa economia "
      "real de recursos, o custo adicional do DSATUR é justificável; já em contextos "
      "que exigem resposta quase imediata sobre grafos muito grandes, a Sequencial "
      "pode ser preferível.")

    # ===================== 7 CONCLUSÃO =====================
    P("7 CONCLUSÃO", H1)
    P("Este trabalho implementou e comparou três heurísticas de coloração de "
      "vértices sobre grafos aleatórios de dimensões crescentes. Confirmou-se "
      "empiricamente o compromisso entre esforço e qualidade: a heurística Sequencial "
      "é a mais rápida, o DSATUR é o mais econômico em cores e o Welsh-Powell ocupa "
      "posição intermediária. O DSATUR mostrou-se a opção mais equilibrada quando a "
      "qualidade da solução é prioritária, reduzindo o número de cores sem custo "
      "proibitivo. Como limitações, destacam-se o uso de densidade fixa e de "
      "instâncias puramente aleatórias. Trabalhos futuros podem variar a densidade, "
      "avaliar grafos provenientes de aplicações reais e incorporar metaheurísticas, "
      "como Busca Tabu e Algoritmos Genéticos, em busca de soluções ainda mais "
      "próximas do número cromático.")

    # ===================== REFERÊNCIAS =====================
    P("REFERÊNCIAS", H1C)
    refs = [
        "APPEL, K.; HAKEN, W. Every planar map is four colorable. <b>Bulletin of the "
        "American Mathematical Society</b>, v. 82, n. 5, p. 711-712, 1976.",
        "BRÉLAZ, D. New methods to color the vertices of a graph. <b>Communications "
        "of the ACM</b>, v. 22, n. 4, p. 251-256, 1979.",
        "BROOKS, R. L. On colouring the nodes of a network. <b>Mathematical "
        "Proceedings of the Cambridge Philosophical Society</b>, v. 37, n. 2, "
        "p. 194-197, 1941.",
        "GAREY, M. R.; JOHNSON, D. S. <b>Computers and Intractability</b>: a guide "
        "to the theory of NP-completeness. New York: W. H. Freeman, 1979.",
        "WELSH, D. J. A.; POWELL, M. B. An upper bound for the chromatic number of a "
        "graph and its application to timetabling problems. <b>The Computer "
        "Journal</b>, v. 10, n. 1, p. 85-86, 1967.",
        "WEST, D. B. <b>Introduction to Graph Theory</b>. 2. ed. Upper Saddle River: "
        "Prentice Hall, 2001.",
    ]
    for r in refs:
        P(r, REF)

    # ===================== BUILD =====================
    frame = Frame(3 * cm, 2 * cm, A4[0] - 5 * cm, A4[1] - 5 * cm, id="normal")
    doc = ReportDoc(SAIDA, pagesize=A4, title="Relatório — Coloração de Grafos",
                    author="Grupo de Teoria e Aplicação de Grafos — UFPI")
    doc.addPageTemplates([
        PageTemplate(id="front", frames=[frame]),
        PageTemplate(id="body", frames=[frame], onPage=draw_page_number),
    ])
    doc.multiBuild(story)
    print(f"PDF gerado: {SAIDA}")


if __name__ == "__main__":
    main()
