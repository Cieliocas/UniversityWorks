package br.com.criandoapi.projeto.controller;

import br.com.criandoapi.projeto.DAO.ICliente;
import br.com.criandoapi.projeto.DAO.IRegistroAluguel;
import br.com.criandoapi.projeto.DAO.IVeiculo;
import br.com.criandoapi.projeto.Excecoes.*;
import br.com.criandoapi.projeto.model.RegistroAluguel;
import br.com.criandoapi.projeto.service.MinhaLocadora;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.web.bind.annotation.*;

import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Optional;


@RestController
@RequestMapping("/registrosalugueis")
public class RegistroAluguelController {
    private final MinhaLocadora locadora;
    private final ICliente daoCliente;
    private final IVeiculo daoVeiculo;

    @Autowired
    private IRegistroAluguel dao;

    public RegistroAluguelController(MinhaLocadora locadora, ICliente daoCliente, IVeiculo daoVeiculo) {
        this.locadora = locadora;
        this.daoCliente = daoCliente;
        this.daoVeiculo = daoVeiculo;
    }

    @GetMapping//lista registros de aluguel
    public List<RegistroAluguel> listaRegistroAluguel(){
        return (List<RegistroAluguel>) dao.findAll();
    }

    @PostMapping
    public String criarRegistroAluguel(@RequestBody RegistroAluguel registro) throws VeiculoAlugado, VeiculoNaoCadastrado, ClienteNaoCadastrado {
        try {
            // Registra um novo aluguel
            locadora.registrarAluguel(registro.getPlaca(), registro.getData(), registro.getDias(), registro.getCpf());
            return "Veiculo alugado com sucesso";
        } catch (Exception e) {
            // Retorna erro se algo falhar
            return "Erro: " + e.getMessage();
        }
    }

    @PutMapping("/devolucao")
    public String registrarDevolução(@RequestParam String placa,@RequestParam @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) Date dataDevolucao, @RequestParam int cpf) {
        try {
            // Registra devolução de veículo
            return locadora.registrarDevolucao(placa, dataDevolucao, cpf);
        } catch (Exception e) {
            return "Erro: " + e.getMessage();
        }
    }

    @GetMapping("/faturamentototal")
    public String faturamentoTotal(@RequestParam int tipo, @RequestParam @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) Date inicio, @RequestParam(required = false) @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) Date fim) {
        // Mapeia o número para o tipo de veículo correspondente
        String tipoVeiculo = Map.of(
                1, "Moto",
                2, "Carro",
                3, "Caminhao",
                4, "Onibus"
        ).get(tipo); // Se não existir, retorna null

        return locadora.faturamentoTotal(tipoVeiculo, inicio, fim);
    }

    @GetMapping("/diariatotal")
    public String diariaTotal(@RequestParam int tipo, @RequestParam @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) Date inicio, @RequestParam(required = false) @DateTimeFormat(iso = DateTimeFormat.ISO.DATE) Date fim) {
        // Mesmo mapeamento usado para o tipo do veículo
        String tipoVeiculo = Map.of(
                1, "Moto",
                2, "Carro",
                3, "Caminhao",
                4, "Onibus"
        ).get(tipo);

        return locadora.quantidadeTotalDeDiarias(tipoVeiculo, inicio, fim);
    }

    @DeleteMapping("/{id}")
    public Optional<RegistroAluguel> excluirRegistroAluguel (@PathVariable Integer id){
        Optional<RegistroAluguel> registroAluguel = dao.findById(id);
        dao.deleteById(id);
        return registroAluguel;
    }

}

