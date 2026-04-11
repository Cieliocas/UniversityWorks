package br.com.criandoapi.projeto.Excecoes;

public class VeiculoJaCadastrado extends Exception {
    public VeiculoJaCadastrado(String p){
        super("Veiculo ja cadastrado. Placa: " + p);
    }
}
