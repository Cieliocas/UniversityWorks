#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cctype>
#include <algorithm> // Necessário para a função std::swap

using namespace std;

// Função para limpar nome (remove números e caracteres estranhos)
string limparNome(const string &nome) {
    string limpo;
    for (char c : nome) {
        if (isalpha(static_cast<unsigned char>(c)) || isspace(static_cast<unsigned char>(c))) {
            limpo += static_cast<char>(tolower(static_cast<unsigned char>(c)));
        }
    }
    while (!limpo.empty() && isspace(limpo.front())) limpo.erase(limpo.begin());
    while (!limpo.empty() && isspace(limpo.back())) limpo.pop_back();
    return limpo;
}

// ---------------------- Algoritmos de Ordenação O(N*log(N)) ----------------------

// Função para unir duas sub-listas ordenadas (usada no Mergesort)
void merge(vector<string>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<string> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Mergesort
void mergeSort(vector<string>& lista, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(lista, left, mid);
    mergeSort(lista, mid + 1, right);
    merge(lista, left, mid, right);
}

// Função de wrapper para o Mergesort para encaixar no modelo medirTempo
void mergeSortWrapper(vector<string>& lista) {
    mergeSort(lista, 0, lista.size() - 1);
}

// Função para particionar a lista (usada no Quicksort)
int partition(vector<string>& arr, int low, int high) {
    string pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; ++j) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Quicksort
void quickSort(vector<string>& lista, int low, int high) {
    if (low < high) {
        int pi = partition(lista, low, high);
        quickSort(lista, low, pi - 1);
        quickSort(lista, pi + 1, high);
    }
}

// Função de wrapper para o Quicksort para encaixar no modelo medirTempo
void quickSortWrapper(vector<string>& lista) {
    quickSort(lista, 0, lista.size() - 1);
}

// ---------------------- Funções de Suporte (Já no seu modelo) ----------------------

// Função para medir o tempo de ordenação
void medirTempo(void (*ordenar)(vector<string>&), vector<string> &nomes, const string &nomeAlgoritmo) {
    auto inicio = chrono::high_resolution_clock::now();
    ordenar(nomes);
    auto fim = chrono::high_resolution_clock::now();

    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    long long total_ms = duracao.count();

    long long horas = total_ms / (1000 * 60 * 60);
    total_ms %= (1000 * 60 * 60);
    long long minutos = total_ms / (1000 * 60);
    total_ms %= (1000 * 60);
    long long segundos = total_ms / 1000;
    long long milissegundos = total_ms % 1000;

    cout << "Tempo gasto pelo " << nomeAlgoritmo << ": "
        << horas << "h "
        << minutos << "m "
        << segundos << "s "
        << milissegundos << "ms" << endl;
}

int main() {
    cout << "Escolha o algoritmo de ordenacao:\n";
    cout << "1 - Bubble Sort\n2 - Insertion Sort\n3 - Selection Sort\n4 - Mergesort\n5 - Quicksort\n";
    int escolhaAlg;
    cin >> escolhaAlg;

    string arquivo;
    cout << "Digite o nome do arquivo a ser ordenado (ex: nomes100k.txt): ";
    cin >> arquivo;

    ifstream entrada(arquivo);
    if (!entrada.is_open()) {
        cerr << "Erro ao abrir o arquivo " << arquivo << endl;
        return 1;
    }

    vector<string> nomes;
    string linha;
    while (getline(entrada, linha)) {
        string nome = limparNome(linha);
        if (!nome.empty()) nomes.push_back(nome);
    }
    entrada.close();

    cout << "Quantidade de nomes lidos: " << nomes.size() << endl;

    // Executar o algoritmo escolhido
    switch (escolhaAlg) {
        // Casos 1 a 3 (já existentes no seu código)
        // ...

        case 4:
            medirTempo(mergeSortWrapper, nomes, "Mergesort");
            break;
        case 5:
            medirTempo(quickSortWrapper, nomes, "Quicksort");
            break;
        default:
            cout << "Escolha inválida!" << endl;
            return 1;
    }

    // Salvar arquivo ordenado
    ofstream saida("nomes_ordenados.txt");
    for (const auto &nome : nomes) {
        saida << nome << "\n";
    }
    saida.close();
    cout << "Ordenacao concluida! Resultado salvo em nomes_ordenados.txt" << endl;

    return 0;
}