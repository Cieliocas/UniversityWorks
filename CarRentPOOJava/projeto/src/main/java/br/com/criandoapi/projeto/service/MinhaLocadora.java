package br.com.criandoapi.projeto.service;

import br.com.criandoapi.projeto.DAO.ICliente;
import br.com.criandoapi.projeto.DAO.IRegistroAluguel;
import br.com.criandoapi.projeto.DAO.IVeiculo;
import br.com.criandoapi.projeto.Excecoes.*;
import br.com.criandoapi.projeto.model.Cliente;
import br.com.criandoapi.projeto.model.RegistroAluguel;
import br.com.criandoapi.projeto.model.Veiculo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.Duration;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Optional;
@Service
public class MinhaLocadora implements Locadora {
    @Autowired
    private IVeiculo dao; // Repositório de veículos
    @Autowired
    private ICliente clienteDao;// Repositório de clientes
    @Autowired
    private IRegistroAluguel registroDao; // Repositório de registros de aluguel

    @Override
    public void inserir(Veiculo v) throws VeiculoJaCadastrado {
        // Se veículo já existe, lança exceção
        if (dao.existsById(v.getPlaca())) {
            throw new VeiculoJaCadastrado(v.getPlaca());
        }
        dao.save(v); // Salva veículo no banco
    }

    @Override
    public void inserir(Cliente c) throws ClienteJaCadastrado {
        // Se cliente já existe, lança exceção
        if (clienteDao.existsById(c.getCpf())) {
            throw new ClienteJaCadastrado();
        }
        clienteDao.save(c); // Salva cliente no banco
    }

    @Override
    public Veiculo pesquisar(String placa) throws VeiculoNaoCadastrado {
        Optional<Veiculo> veiculoOpt = dao.findById(placa);
        // Se veículo não encontrado, lança exceção
        if (veiculoOpt.isEmpty()) {
            throw new VeiculoNaoCadastrado(placa);
        }
        return veiculoOpt.get(); // Retorna veículo encontrado
    }

    public Cliente pesquisarCliente(int cpf) throws ClienteNaoCadastrado {
        // Busca cliente pelo CPF, lança exceção se não encontrar
        return clienteDao.findById(cpf)
                .orElseThrow(() -> new ClienteNaoCadastrado(cpf));
    }

    @Override
    public ArrayList<Veiculo> pesquisarMoto(int cilindrada) {
        try {
            // Busca motos com cilindrada igual ou maior que o parâmetro
            return (ArrayList<Veiculo>) dao.findByTipoAndCilindradaGreaterThanEqual("Moto", cilindrada);
        } catch (Exception e) {
            // Lança erro genérico se falhar
            throw new RuntimeException("Erro ao pesquisar motos", e);
        }
    } //o mesmo nas outras funções de pesquisar

    @Override
    public ArrayList<Veiculo> pesquisarCarro(int autonomia) {
        try {
            return (ArrayList<Veiculo>) dao.findByTipoAndAutonomiaGreaterThanEqual("Carro", autonomia);
        } catch (Exception e) {
            throw new RuntimeException("Erro ao pesquisar motos", e);
        }
    }

    @Override
    public ArrayList<Veiculo> pesquisarCaminhao(int capacCarga) {
        try {
            return (ArrayList<Veiculo>) dao.findByTipoAndCapacCargaGreaterThanEqual("Caminhao", capacCarga);
        } catch (Exception e) {
            throw new RuntimeException("Erro ao pesquisar caminhões", e);
        }
    }

    @Override
    public ArrayList<Veiculo> pesquisarOnibus(int capacPassageiros) {
        try {
            return (ArrayList<Veiculo>) dao.findByTipoAndCapacPassageirosGreaterThanEqual("Onibus", capacPassageiros);
        } catch (Exception e) {
            throw new RuntimeException("Erro ao pesquisar ônibus", e);
        }
    }

    @Override
    public double calcularAluguel(String placa, int dias) throws VeiculoNaoCadastrado {
        // Busca veículo pela placa ou lança exceção
        Veiculo veiculo = dao.findById(placa).orElseThrow(() -> new VeiculoNaoCadastrado(placa));
        // Calcula e retorna valor do aluguel para os dias informados
        return veiculo.calcularAluguel(dias);
    }

    @Override
    public void registrarAluguel(String placa, Date data, int dias, int cpf) throws VeiculoNaoCadastrado, VeiculoAlugado, ClienteNaoCadastrado {
        // Busca o veículo pela placa
        Veiculo v = dao.findByPlaca(placa);
        // Busca o cliente pelo CPF
        Cliente c = clienteDao.findByCpf(cpf);

        // Se veículo não existe, lança exceção
        if (v == null) throw new VeiculoNaoCadastrado(placa);
        // Se cliente não existe, lança exceção
        if (c == null) throw new ClienteNaoCadastrado(cpf);

        // Verifica se já existe registro de aluguel com data de devolução nula (veículo alugado)
        RegistroAluguel ra = registroDao.findByPlaca(placa);
        if (ra != null && ra.getData_devolucao() == null) throw new VeiculoAlugado();

        // Cria novo registro de aluguel com dados informados
        RegistroAluguel a = new RegistroAluguel();
        a.setPlaca(placa);
        a.setCpf(cpf);
        a.setDias(dias);
        a.setData(data);
        a.setData_devolucao(null);

        // Salva o registro no banco de dados
        registroDao.save(a);
    }

    public String registrarDevolucao(String placa, Date data, int cpf) throws VeiculoNaoCadastrado, VeiculoNaoAlugado, ClienteNaoCadastrado {
        // Busca veículo e cliente pelo identificador
        Veiculo v = dao.findByPlaca(placa);
        Cliente c = clienteDao.findByCpf(cpf);

        if (v == null) throw new VeiculoNaoCadastrado(placa);
        if (c == null) throw new ClienteNaoCadastrado(cpf);

        // Busca todos os registros de aluguel do veículo
        List<RegistroAluguel> registros = registroDao.findAllByPlaca(placa);
        if (registros.isEmpty()) throw new VeiculoNaoAlugado();

        // Verifica se há registro sem devolução
        for (RegistroAluguel ra : registros) {
            if (ra.getData_devolucao() == null) {
                // Marca a devolução
                ra.setData_devolucao(data);

                // Calcula dias alugados, mínimo 1
                long diasAlugados = Duration.between(ra.getData().toInstant(), data.toInstant()).toDays();
                diasAlugados = Math.max(diasAlugados, 1);

                // Atualiza dias caso tenha mudado
                if (ra.getDias() != diasAlugados) ra.setDias((int) diasAlugados);

                // Calcula faturamento e salva no registro
                double faturamento = diasAlugados * v.getVal_diaria();
                ra.setFaturamento(faturamento);
                registroDao.save(ra);

                return "Veículo devolvido com sucesso";
            }
        }
        // Caso todos já tenham sido devolvidos
        return "Veículo já foi devolvido";
    }

    @Override
    public String depreciarVeiculos(int tipo, float taxaDepreciacao) {
        List<Veiculo> veiculos;
        String stipo = "";

        // Converte o tipo numérico em string correspondente
        if (tipo == 1) stipo = "Moto";
        else if (tipo == 2) stipo = "Carro";
        else if (tipo == 3) stipo = "Caminhao";
        else if (tipo == 4) stipo = "Onibus";
        else stipo = null;

        // Busca veículos filtrando por tipo, ou todos se tipo nulo
        if (stipo != null) veiculos = dao.findByTipo(stipo);
        else veiculos = (List<Veiculo>) dao.findAll();

        // Aplica a depreciação em cada veículo e salva
        for (Veiculo v : veiculos) {
            float novoValor = v.getVal_bem() * (1 - taxaDepreciacao);
            v.setVal_bem(novoValor);
            dao.save(v);
        }

        // Retorna mensagem de sucesso
        if(stipo != null) {
            return "Veículos do tipo " + stipo + " depreciados com sucesso.";
        } else {
            return "Todos os veiculos foram depreciados.";
        }
    }

    @Override
    public String aumentarDiaria(int tipo, float taxaAumento) {
        List<Veiculo> veiculos;
        String stipo = "";

        // Converte o tipo numérico em string correspondente
        if (tipo == 1) stipo = "Moto";
        else if (tipo == 2) stipo = "Carro";
        else if (tipo == 3) stipo = "Caminhao";
        else if (tipo == 4) stipo = "Onibus";
        else stipo = null;

        // Busca veículos filtrando por tipo, ou todos se tipo nulo
        if (stipo != null) veiculos = dao.findByTipo(stipo);
        else veiculos = (List<Veiculo>) dao.findAll();

        // Aplica o aumento na diária e salva cada veículo
        for (Veiculo v : veiculos) {
            float novoValor = v.getVal_diaria() * (1 + taxaAumento);
            v.setVal_diaria(novoValor);
            dao.save(v);
        }

        // Retorna mensagem de sucesso
        if (stipo != null) {
            return "Diária dos veículos do tipo " + stipo + " aumentada com sucesso.";
        } else {
            return "Diária de todos os veículos aumentada com sucesso.";
        }
    }
    @Override
    public String faturamentoTotal(String stipo, Date inicio, Date fim) {
        double valor = registroDao.faturamentoTotal(stipo, inicio, fim); // calcula faturamento
        if (stipo != null) {
            return "Faturamento total dos veículos do tipo " + stipo + " é igual a " + valor + ".";
        } else {
            return "Faturamento total de todos os veículos é igual a " + valor + ".";
        }
    }

    @Override
    public String quantidadeTotalDeDiarias(String stipo, Date inicio, Date fim) {
        int dias = registroDao.quantidadeTotalDeDiarias(stipo, inicio, fim); // soma dias alugados
        if (stipo != null) {
            return "Quantidade total de diárias dos veículos do tipo " + stipo + " é igual a " + dias + ".";
        } else {
            return "Quantidade total de diárias de todos os veículos é igual a " + dias + ".";
        }
    }

    @Override
    public void removerTudo() {
        registroDao.deleteAll(); // apaga todos os registros de aluguel
        dao.deleteAll();         // apaga todos os veículos
        clienteDao.deleteAll();  // apaga todos os clientes
    }


}
