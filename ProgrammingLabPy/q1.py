# Francielio Evangelista dos Santos Castro
# UFPI - CCN - Ciência da Computação
# Matrícula: 20249050551

# Q1) Crie um programa em Python que manipule uma lista de nomes de alunos de
# forma que exercite as operações de inserção, busca, ordenação e remoção de alunos
# dessa lista.

print("Lista de alunos")
alunos = []

while True:
    print("----- Menu -----")
    print("1. Inserir aluno")
    print("2. Buscar aluno na lista")
    print("3. Exibir lista de alunos em ordem alfabética")
    print("4. Remover aluno da lista")
    print("5. Sair do programa")

    num = int(input("Selecione a opção desejada: "))

    if num == 1:
        novoAluno = (input("Digite o nome do aluno a ser adicionado: "))
        alunos.append(novoAluno)
        print(f"Você adicionou o aluno: {novoAluno}!")
        print(f"A lista atual de alunos: {alunos}")
    if num == 2:
        pesquisa = input("Digite o nome do aluno: ")
        if pesquisa in alunos:
            print(f"O aluno {pesquisa} está na lista")
        else:
            print("O aluno não está na lista")
    if num == 3:
        ordenados = sorted(alunos)
        print(f"Aqui está a lista organizada em ordem alfabética:\n {ordenados}")
    if num == 4:
        alunoRemovido = (input("Digite o nome do aluno a ser excluido da lista: "))
        if alunoRemovido in alunos:
            alunos.remove(alunoRemovido)
            print(f"O aluno {alunoRemovido} foi removido da lista com sucesso!")
        else:
            print("O aluno não está na lista")
    if num == 5:
        break;
    else:
        print("Selecione uma opção válida!")