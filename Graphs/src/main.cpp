/**
 * @file main.cpp
 * @brief Rotina de benchmarking das heurísticas de coloração de grafos.
 *
 * Para cada dimensão V em {50, 100, 200, 500, 1000} (configurável), gera
 * 10 instâncias de grafos aleatórios conexos e não-completos, executa as três
 * heurísticas (Sequencial, Welsh-Powell, DSATUR) em cada instância, mede o
 * tempo de execução e o número de cores, e exporta médias e desvios-padrão
 * para 'resultados.csv'.
 *
 * Uso:
 *   ./coloracao [--sizes 50,100,200,500,1000] [--instances 10]
 *               [--density 0.5] [--seed 42] [--output resultados.csv]
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "Coloring.h"
#include "Graph.h"

namespace {

struct Config {
    std::vector<int> sizes{50, 100, 200, 500, 1000};
    int instances = 10;
    double density = 0.5;
    unsigned seed = 42;
    std::string output = "resultados.csv";
};

// Acumuladores estatísticos de uma heurística para uma dimensão V.
struct Accumulator {
    double sumTime = 0.0, sumTimeSq = 0.0;   // tempo (ms)
    double sumColors = 0.0, sumColorsSq = 0.0;
    double sumEdges = 0.0, sumDensity = 0.0;

    void add(double timeMs, int colors, long long edges, double density) {
        sumTime += timeMs;
        sumTimeSq += timeMs * timeMs;
        sumColors += colors;
        sumColorsSq += static_cast<double>(colors) * colors;
        sumEdges += static_cast<double>(edges);
        sumDensity += density;
    }
};

double mean(double sum, int k) { return sum / k; }

// Desvio-padrão amostral (k-1 no denominador).
double stddev(double sum, double sumSq, int k) {
    if (k < 2) return 0.0;
    const double m = sum / k;
    const double var = (sumSq - k * m * m) / (k - 1);
    return var > 0.0 ? std::sqrt(var) : 0.0;
}

std::vector<int> parseSizes(const std::string& csv) {
    std::vector<int> result;
    std::stringstream ss(csv);
    std::string token;
    while (std::getline(ss, token, ',')) {
        if (!token.empty()) result.push_back(std::stoi(token));
    }
    return result;
}

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

Config parseArgs(int argc, char** argv) {
    Config cfg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
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

// Tabela das heurísticas: nome de exibição + ponteiro para a função.
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

int main(int argc, char** argv) {
    const Config cfg = parseArgs(argc, argv);

    std::ofstream csv(cfg.output);
    if (!csv) {
        std::cerr << "Erro: nao foi possivel abrir '" << cfg.output << "'\n";
        return 1;
    }
    csv << "V,heuristica,tempo_medio_ms,tempo_dp_ms,cores_media,cores_dp,"
        << "arestas_media,densidade_media,instancias\n";
    csv << std::fixed;

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

    for (int V : cfg.sizes) {
        std::vector<Accumulator> acc(kHeuristics.size());

        for (int inst = 0; inst < cfg.instances; ++inst) {
            // Semente derivada -> cada instancia e diferente e reprodutivel.
            unsigned seed = cfg.seed + static_cast<unsigned>(V) * 100u
                            + static_cast<unsigned>(inst);
            std::mt19937 rng(seed);

            Graph g = GraphGenerator::generate(V, cfg.density, rng);

            for (size_t h = 0; h < kHeuristics.size(); ++h) {
                // Cronometra SOMENTE o algoritmo de coloracao.
                auto t0 = std::chrono::high_resolution_clock::now();
                ColoringResult res = kHeuristics[h].run(g);
                auto t1 = std::chrono::high_resolution_clock::now();
                double ms =
                    std::chrono::duration<double, std::milli>(t1 - t0).count();

                // Validacao de corretude (fora da cronometragem).
                if (!Coloring::isValid(g, res.colors)) {
                    std::cerr << "ERRO: coloracao invalida em V=" << V
                              << " inst=" << inst << " heuristica="
                              << kHeuristics[h].name << "\n";
                    return 1;
                }

                acc[h].add(ms, res.numColors, g.numEdges(), g.density());
            }
        }

        // Consolida e grava as estatisticas desta dimensao.
        const int k = cfg.instances;
        for (size_t h = 0; h < kHeuristics.size(); ++h) {
            const Accumulator& a = acc[h];
            double tMean = mean(a.sumTime, k);
            double tSd = stddev(a.sumTime, a.sumTimeSq, k);
            double cMean = mean(a.sumColors, k);
            double cSd = stddev(a.sumColors, a.sumColorsSq, k);
            double eMean = mean(a.sumEdges, k);
            double dMean = mean(a.sumDensity, k);

            csv << V << ',' << kHeuristics[h].name << ','
                << std::setprecision(6) << tMean << ',' << tSd << ','
                << std::setprecision(4) << cMean << ',' << cSd << ','
                << std::setprecision(1) << eMean << ','
                << std::setprecision(4) << dMean << ',' << k << '\n';

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
