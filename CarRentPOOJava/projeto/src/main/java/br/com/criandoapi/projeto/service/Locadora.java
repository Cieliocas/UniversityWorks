package br.com.criandoapi.projeto.service;

import br.com.criandoapi.projeto.Excecoes.*;
import br.com.criandoapi.projeto.model.Cliente;
import br.com.criandoapi.projeto.model.Veiculo;

import java.util.ArrayList;
import java.util.Date;

public interface Locadora {

    public void inserir(Veiculo v) throws VeiculoJaCadastrado;
    public void inserir(Cliente c) throws ClienteJaCadastrado;
    public Veiculo pesquisar(String placa) throws VeiculoNaoCadastrado;

    public ArrayList<Veiculo> pesquisarMoto(int cilindrada);
    public ArrayList<Veiculo> pesquisarCarro(int autonomia);
    public ArrayList<Veiculo> pesquisarCaminhao(int carga);
    public ArrayList<Veiculo> pesquisarOnibus(int passageiros);

    //Seguro Moto = (valor do bem * 11%)/365
    //Seguro Carro = (valor do bem * 3%)/365
    //Seguro Caminhão = (valor do bem * 8%)/365
    //Seguro Ônibus = (valor do bem * 20%)/365
    //Aluguel = (valor da diária + seguro) * quantidade de dias
    public double calcularAluguel(String placa, int dias) throws VeiculoNaoCadastrado;

    public void registrarAluguel(String placa, Date data, int dias, int cpf) throws VeiculoNaoCadastrado, VeiculoAlugado, ClienteNaoCadastrado;

    public String registrarDevolucao(String placa, Date data, int cpf) throws VeiculoNaoCadastrado, VeiculoNaoAlugado, ClienteNaoCadastrado;

    //tipo de veiculo
    // 0 (todos), 1 (moto), 2 (carro), 3 (caminhão), 4 (ônibus)
    public String depreciarVeiculos(int tipo, float taxaDepreciacao);
    public String aumentarDiaria(int tipo, float taxaAumento);
    public String faturamentoTotal(String stipo, Date inicio, Date fim);
    public String quantidadeTotalDeDiarias(String stipo, Date inicio, Date fim);
    public void removerTudo();
}
