package br.com.criandoapi.projeto.Excecoes;

public class VeiculoNaoCadastrado extends Exception {
    public VeiculoNaoCadastrado(String placa) {
        super("Veículo não cadastrado. Placa: " + placa);
    }
}