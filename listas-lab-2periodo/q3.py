# Crie um programa em Python que manipule três conjuntos de cores de forma
# que exercite as operações de união, intersecção, diferença entre dois conjuntos
# selecionados dentre os três conjuntos de cores.

# Francielio Evangelista dos Santos Castro
# UFPI - CCN - Ciência da Computação
# Matrícula: 20249050551

conjunto1 = set()
conjunto2 = set()
conjunto3 = set()

conjunto1.add("azul")
conjunto1.add("vermelho")
conjunto1.add("verde")
conjunto1.add("amarelo")
conjunto1.add("preto")

conjunto2.add("vermelho")
conjunto2.add("amarelo")
conjunto2.add("roxo")
conjunto2.add("branco")
conjunto2.add("cinza")

conjunto3.add("preto")
conjunto3.add("verde")
conjunto3.add("azul")
conjunto3.add("laranja")
conjunto3.add("amarelo")


conjuntos_disponiveis = {
    '1': conjunto1,
    '2': conjunto2,
    '3': conjunto3
}

def selecionar_conjuntos():
    while True:
        try:
            print("\nConjuntos disponíveis:")
            print("1 - Conjunto 1")
            print("2 - Conjunto 2")
            print("3 - Conjunto 3")

            escolha1 = input("Selecione o primeiro conjunto (1, 2 ou 3): ")
            escolha2 = input("Selecione o segundo conjunto (1, 2 ou 3): ")

            if escolha1 in conjuntos_disponiveis and escolha2 in conjuntos_disponiveis:
                conj1 = conjuntos_disponiveis[escolha1]
                conj2 = conjuntos_disponiveis[escolha2]
                return conj1, conj2, escolha1, escolha2
            else:
                print("Escolha inválida. Por favor, selecione 1, 2 ou 3.")
        except KeyboardInterrupt:
            print("\nOperação cancelada.")
            return None, None, None, None

while True:
    print("\n--- Gerenciador de Conjuntos de Cores ---")
    print("1 - Realizar União")
    print("2 - Realizar Interseção")
    print("3 - Realizar Diferença")
    print("4 - Sair do programa")

    opcao = input("Digite a sua opção: ")

    if opcao == '1':
        conj1, conj2, nome1, nome2 = selecionar_conjuntos()
        if conj1 and conj2:
            resultado = conj1.union(conj2)
            print(f"\nUnião de Conjunto {nome1} e Conjunto {nome2}:")
            print(resultado)
    
    elif opcao == '2':
        conj1, conj2, nome1, nome2 = selecionar_conjuntos()
        if conj1 and conj2:
            resultado = conj1.intersection(conj2)
            print(f"\nInterseção de Conjunto {nome1} e Conjunto {nome2}:")
            print(resultado)
            
    elif opcao == '3':
        conj1, conj2, nome1, nome2 = selecionar_conjuntos()
        if conj1 and conj2:
            resultado = conj1.difference(conj2)
            print(f"\nDiferença (Conjunto {nome1} - Conjunto {nome2}):")
            print(resultado)

    elif opcao == '4':
        print("\nPrograma encerrado")
        break
        
    else:
        print("\nOpção inválida. Por favor, digite 1, 2, 3 ou 4.")