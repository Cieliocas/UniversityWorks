// ===========================================================================
// main.cpp
// ---------------------------------------------------------------------------
// Programa de BENCHMARK das heuristicas de coloracao de grafos.
//
// O que faz, em alto nivel:
//   Para cada dimensao V (por padrao 50, 100, 200, 500 e 1000):
//     - gera 10 instancias de grafos aleatorios conexos e nao-completos;
//     - roda as 3 heuristicas (Sequencial, Welsh-Powell, DSATUR) em cada uma;
//     - mede o tempo de execucao e conta o numero de cores;
//     - valida cada coloracao (seguranca);
//     - calcula media e desvio-padrao das 10 instancias;
//   e exporta tudo para um arquivo CSV.
//
// Uso:
//   ./coloracao [--sizes 50,100,200,500,1000] [--instances 10]
//               [--density 0.5] [--seed 42] [--output resultados.csv]
// ===========================================================================
#include <algorithm>
#include <chrono>     // medicao de tempo (high_resolution_clock)
#include <cmath>      // std::sqrt
#include <cstdlib>    // std::exit
#include <fstream>    // gravacao do CSV
#include <iomanip>    // formatacao (setprecision, setw)
#include <iostream>
#include <random>     // std::mt19937 (gerador reprodutivel)
#include <sstream>    // parsing dos tamanhos separados por virgula
#include <string>
#include <vector>

#include "Coloring.h"
#include "Graph.h"

namespace {

// ---------------------------------------------------------------------------
// Config: parametros do experimento (valores padrao + sobrescritos por CLI).
// ---------------------------------------------------------------------------
struct Config {
    std::vector<int> sizes{50, 100, 200, 500, 1000};  // dimensoes V a testar
    int instances = 10;         // instancias por dimensao
    double density = 0.5;       // probabilidade de aresta (0 < p < 1)
    unsigned seed = 42;         // semente base (reprodutibilidade)
    std::string output = "resultados.csv";
};

// ---------------------------------------------------------------------------
// Accumulator: soma corrente das metricas de UMA heuristica em UMA dimensao.
// Guarda a soma e a soma dos quadrados de cada metrica para permitir calcular
// media e desvio-padrao ao final (sem armazenar todas as amostras).
// ---------------------------------------------------------------------------
struct Accumulator {
    double sumTime = 0.0, sumTimeSq = 0.0;      // tempo (ms)
    double sumColors = 0.0, sumColorsSq = 0.0;  // numero de cores
    double sumEdges = 0.0, sumDensity = 0.0;    // arestas e densidade (medias)

    void add(double timeMs, int colors, long long edges, double density) {
        sumTime += timeMs;
        sumTimeSq += timeMs * timeMs;
        sumColors += colors;
        sumColorsSq += static_cast<double>(colors) * colors;
        sumEdges += static_cast<double>(edges);
        sumDensity += density;
    }
};

// Media aritmetica de uma soma sobre k amostras.
double mean(double sum, int k) { return sum / k; }

// Desvio-padrao AMOSTRAL (denominador k-1), calculado a partir da soma e da
// soma dos quadrados: dp = sqrt((somaQ - k*media^2) / (k-1)). Retorna 0 se
// houver menos de 2 amostras. O max(0, .) evita raiz de negativo por erro
// numerico quando a variancia e ~0.
double stddev(double sum, double sumSq, int k) {
    if (k < 2) return 0.0;
    const double m = sum / k;
    const double var = (sumSq - k * m * m) / (k - 1);
    return var > 0.0 ? std::sqrt(var) : 0.0;
}

// Converte uma string "50,100,200" na lista de inteiros {50, 100, 200}.
std::vector<int> parseSizes(const std::string& csv) {
    std::vector<int> result;
    std::stringstream ss(csv);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (!token.empty()) result.push_back(std::stoi(token));
    }
    return result;
}

// Imprime a ajuda das opcoes de linha de comando.
void printUsage(const char* prog) {
    std::cout
        << "Uso: " << prog << " [opcoes]\n"
        << "  --sizes 50,100,200,500,1000   dimensoes V a testar\n"
        << "  --instances 10                instancias por dimensao\n"
        << "  --density 0.5                 probabilidade de aresta (0<p<1)\n"
        << "  --seed 42                     semente base (reprodutibilidade)\n"
        << "  --output resultados.csv       arquivo CSV de saida\n"
        << "  --help                        mostra esta ajuda\n";
}

// Interpreta os argumentos de linha de comando e devolve a Config resultante.
// Aborta o programa (com mensagem) se um argumento for invalido ou faltar valor.
Config parseArgs(int argc, char** argv) {
    Config cfg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        // Le o proximo token como o valor da opcao atual (ou aborta).
        auto next = [&]() -> std::string {
            if (i + 1 >= argc) {
                std::cerr << "Faltou valor para " << arg << "\n";
                std::exit(1);
            }
            return argv[++i];
        };
        if (arg == "--sizes") cfg.sizes = parseSizes(next());
        else if (arg == "--instances") cfg.instances = std::stoi(next());
        else if (arg == "--density") cfg.density = std::stod(next());
        else if (arg == "--seed") cfg.seed = static_cast<unsigned>(std::stoul(next()));
        else if (arg == "--output") cfg.output = next();
        else if (arg == "--help") { printUsage(argv[0]); std::exit(0); }
        else {
            std::cerr << "Argumento desconhecido: " << arg << "\n";
            printUsage(argv[0]);
            std::exit(1);
        }
    }
    return cfg;
}

// ---------------------------------------------------------------------------
// Tabela das heuristicas: associa o nome de exibicao a um ponteiro para a
// respectiva funcao. Isso permite iterar sobre as 3 heuristicas em um laco,
// sem repetir codigo.
// ---------------------------------------------------------------------------
struct Heuristic {
    const char* name;
    ColoringResult (*run)(const Graph&);
};

const std::vector<Heuristic> kHeuristics = {
    {"Sequencial", &Coloring::greedySequential},
    {"Welsh-Powell", &Coloring::welshPowell},
    {"DSATUR", &Coloring::dsatur},
};

}  // namespace

// ---------------------------------------------------------------------------
// main: orquestra todo o benchmark e a exportacao dos resultados.
// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    const Config cfg = parseArgs(argc, argv);

    // Abre o CSV e escreve o cabecalho (formato "tidy": uma linha por
    // combinacao de dimensao x heuristica, ideal para o seaborn).
    std::ofstream csv(cfg.output);
    if (!csv) {
        std::cerr << "Erro: nao foi possivel abrir '" << cfg.output << "'\n";
        return 1;
    }
    csv << "V,heuristica,tempo_medio_ms,tempo_dp_ms,cores_media,cores_dp,"
        << "arestas_media,densidade_media,instancias\n";
    csv << std::fixed;

    // Cabecalho da tabela impressa no terminal (acompanhamento em tempo real).
    std::cout << "=== Benchmark de Coloracao de Grafos ===\n";
    std::cout << "Densidade-alvo p = " << cfg.density
              << " | instancias por dimensao = " << cfg.instances
              << " | semente base = " << cfg.seed << "\n\n";
    std::cout << std::left << std::setw(7) << "V"
              << std::setw(16) << "Heuristica"
              << std::right << std::setw(16) << "Tempo medio(ms)"
              << std::setw(14) << "Cores media"
              << std::setw(14) << "Arestas med." << "\n";
    std::cout << std::string(67, '-') << "\n";

    // Laco externo: uma dimensao V por vez.
    for (int V : cfg.sizes) {
        // Um acumulador por heuristica (zerado a cada nova dimensao).
        std::vector<Accumulator> acc(kHeuristics.size());

        // Laco medio: as 'instances' instancias desta dimensao.
        for (int inst = 0; inst < cfg.instances; ++inst) {
            // Semente derivada -> cada instancia e diferente, porem
            // totalmente reprodutivel (mesma semente => mesmo grafo).
            unsigned seed = cfg.seed + static_cast<unsigned>(V) * 100u
                            + static_cast<unsigned>(inst);
            std::mt19937 rng(seed);

            Graph g = GraphGenerator::generate(V, cfg.density, rng);

            // Laco interno: as 3 heuristicas sobre a MESMA instancia.
            for (size_t h = 0; h < kHeuristics.size(); ++h) {
                // Cronometra SOMENTE o algoritmo de coloracao (a geracao do
                // grafo e a validacao ficam de fora da medicao).
                auto t0 = std::chrono::high_resolution_clock::now();
                ColoringResult res = kHeuristics[h].run(g);
                auto t1 = std::chrono::high_resolution_clock::now();
                double ms =
                    std::chrono::duration<double, std::milli>(t1 - t0).count();

                // Rede de seguranca: aborta se alguma coloracao for invalida,
                // garantindo que os numeros coletados sejam confiaveis.
                if (!Coloring::isValid(g, res.colors)) {
                    std::cerr << "ERRO: coloracao invalida em V=" << V
                              << " inst=" << inst << " heuristica="
                              << kHeuristics[h].name << "\n";
                    return 1;
                }

                acc[h].add(ms, res.numColors, g.numEdges(), g.density());
            }
        }

        // Consolida as estatisticas desta dimensao e grava/exibe.
        const int k = cfg.instances;
        for (size_t h = 0; h < kHeuristics.size(); ++h) {
            const Accumulator& a = acc[h];
            double tMean = mean(a.sumTime, k);
            double tSd = stddev(a.sumTime, a.sumTimeSq, k);
            double cMean = mean(a.sumColors, k);
            double cSd = stddev(a.sumColors, a.sumColorsSq, k);
            double eMean = mean(a.sumEdges, k);
            double dMean = mean(a.sumDensity, k);

            // Linha do CSV (precisao ajustada por coluna).
            csv << V << ',' << kHeuristics[h].name << ','
                << std::setprecision(6) << tMean << ',' << tSd << ','
                << std::setprecision(4) << cMean << ',' << cSd << ','
                << std::setprecision(1) << eMean << ','
                << std::setprecision(4) << dMean << ',' << k << '\n';

            // Linha correspondente no terminal.
            std::cout << std::left << std::setw(7) << V
                      << std::setw(16) << kHeuristics[h].name
                      << std::right << std::fixed << std::setprecision(4)
                      << std::setw(16) << tMean
                      << std::setprecision(2) << std::setw(14) << cMean
                      << std::setprecision(0) << std::setw(14) << eMean << "\n";
        }
        std::cout << std::string(67, '-') << "\n";
    }

    csv.close();
    std::cout << "\nResultados exportados para: " << cfg.output << "\n";
    std::cout << "Proximo passo: python3 scripts/plot_results.py\n";
    return 0;
}
