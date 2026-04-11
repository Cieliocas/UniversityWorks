package br.com.criandoapi.projeto.Excecoes;

public class VeiculoAlugado extends Exception {
    public VeiculoAlugado(){
        super("O veiculo ja foi alugado.");
    }
}
