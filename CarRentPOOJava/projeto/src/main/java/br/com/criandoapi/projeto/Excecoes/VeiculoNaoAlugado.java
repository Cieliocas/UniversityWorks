package br.com.criandoapi.projeto.Excecoes;

public class VeiculoNaoAlugado extends Exception {
    public VeiculoNaoAlugado(){
        super("Veiculo nao alugado.");
    }
}
