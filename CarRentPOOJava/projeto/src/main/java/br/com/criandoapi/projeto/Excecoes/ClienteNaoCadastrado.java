package br.com.criandoapi.projeto.Excecoes;

public class ClienteNaoCadastrado extends Exception {
    public ClienteNaoCadastrado(int cpf){
        super("Cliente nao encontrado.");
    }
}
