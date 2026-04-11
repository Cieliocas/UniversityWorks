# Questão 2) Crie um programa em Python que manipule uma lista de tuplas de pontos
# GPS que represente a origem (x0, y0), os pontos intermediários (xk, yk) e destino final (xn,
# yn) de uma rota de forma que exercite as operações de inserção, remoção de pontos
# GPS dessa lista de tuplas.

# Francielio Evangelista dos Santos Castro
# UFPI - CCN - Ciência da Computação
# Matrícula: 20249050551

rota = []

while True:
    print("\n--- Gerenciador de Rota GPS ---")
    print("1. Adicionar novo ponto GPS")
    print("2. Remover um ponto GPS")
    print("3. Exibir rota atual")
    print("4. Sair do programa")

    opcao = input("Digite a opção desejada: ")

    if opcao == '1':
        try:
            x = float(input("Digite a coordenada x: "))
            y = float(input("Digite a coordenada y: "))
            novo_ponto = (x, y)
            rota.append(novo_ponto)
            print(f"\nPonto {novo_ponto} adicionado com sucesso")
            print("Rota atual: ", rota)
        except ValueError:
            print("\nEntrada inválida. Por favor, digite numeros p/ as coordenadas")

    elif opcao == '2':
        try: 
            if not rota:
                print("\nA rota está vazia. Não há pontos para remover.")
                continue
            print("\nRota atual com índices:")
            for i, ponto in enumerate(rota):
                print(f"Indice {i}: {ponto}")
            
            indice = int(input("\nDigite o índice do ponto a ser removido: "))

            if 0 <= indice < len(rota):
                ponto_removido = rota.pop(indice)
                print(f"\nPonto {ponto_removido} removido.")
                print("Rota atual:", rota)
            else:
                print("\nÍndice inválido. Por favor, digite um número dentro do intervalo da rota.")
        except ValueError:
            print("\nEntrada inválida. Por favor, digite um número inteiro para o índice.")
    elif opcao == '3':
        if rota:
            print("\n----- Rota atual -----")
            for i, ponto in enumerate(rota):
                print(f"Ponto {i}: {ponto}")
        else:
            print("\nA rota está vazia")
    
    elif opcao == '4':
        print("\nSaindo do programa!")
        break
    else:
        print("\nOpção inválida, digite um número válido!")
