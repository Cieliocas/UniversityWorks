package br.com.criandoapi.projeto.Excecoes;

public class ClienteJaCadastrado extends Exception {
    public ClienteJaCadastrado(){
        super("Cliente ja cadastrado.");
    }
}
