package br.com.criandoapi.projeto.controller;

import br.com.criandoapi.projeto.DAO.IVeiculo;
import br.com.criandoapi.projeto.Excecoes.VeiculoJaCadastrado;
import br.com.criandoapi.projeto.Excecoes.VeiculoNaoCadastrado;
import br.com.criandoapi.projeto.model.Veiculo;
import br.com.criandoapi.projeto.service.MinhaLocadora;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/veiculos")
public class VeiculoController {
    private final MinhaLocadora locadora;
    private final IVeiculo dao;

    @Autowired
    public VeiculoController(MinhaLocadora locadora, IVeiculo dao) {
        this.locadora = locadora;
        this.dao = dao;
    }

    // Retorna a lista de todos os veículos cadastrados
    @GetMapping
    public List<Veiculo> listaVeiculos() {
        return (List<Veiculo>) dao.findAll();
    }

    // Insere um novo veículo no sistema
    @PostMapping
    public String inserirVeiculo(@RequestBody Veiculo veiculo) {
        try {
            locadora.inserir(veiculo); // chama a lógica de inserção
            return "Veículo inserido com sucesso";
        } catch (VeiculoJaCadastrado e) {
            return "Erro: " + e.getMessage(); // se já existe, retorna erro
        }
    }

    // Pesquisa um veículo pela placa
    @GetMapping("/{placa}")
    public ResponseEntity<?> pesquisarVeiculo(@PathVariable String placa) {
        try {
            Veiculo veiculo = locadora.pesquisar(placa); // busca veículo
            return ResponseEntity.ok(veiculo); // retorna veículo encontrado
        } catch (VeiculoNaoCadastrado e) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND).body(e.getMessage()); // 404 se não achar
        }
    }

    // retorna Motos com cilindrada >= ao que é pedido
    @GetMapping("/motos")
    public List<Veiculo> pesquisarMotos(@RequestParam int cilindrada) {
        return locadora.pesquisarMoto(cilindrada);
    }

    // Carros por autonomia >= ao que é pedido
    @GetMapping("/carros")
    public List<Veiculo> pesquisarCarros(@RequestParam int autonomia) {
        return locadora.pesquisarCarro(autonomia);
    }

    // Caminhões por carga >= ao que é pedido
    @GetMapping("/caminhoes")
    public List<Veiculo> pesquisarCaminhoes(@RequestParam int cargaMinima) {
        return locadora.pesquisarCaminhao(cargaMinima);
    }

    // Ônibus por passageiros >= ao que é pedido
    @GetMapping("/onibus")
    public List<Veiculo> pesquisarOnibus(@RequestParam int passageirosMinimos) {
        return locadora.pesquisarOnibus(passageirosMinimos);
    }

    @GetMapping("/calculoaluguel")
    public ResponseEntity<String> calcularAluguel(@RequestParam String placa, @RequestParam int dias) {
        try {
            double valor = locadora.calcularAluguel(placa, dias); // calcula o valor do aluguel
            return ResponseEntity.ok("Valor do aluguel para o veículo " + placa + ": R$ " + valor);
        } catch (VeiculoNaoCadastrado e) {
            // retorna 404 se o veículo não for encontrado
            return ResponseEntity.status(HttpStatus.NOT_FOUND)
                    .body("Veículo não cadastrado. Placa: " + placa);
        } catch (Exception e) {
            // retorna erro 500 para outros problemas inesperados
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body("Erro ao calcular aluguel: " + e.getMessage());
        }
    }

    @PutMapping("/depreciarveiculos")
    public String depreciarVeiculos(@RequestParam int tipo, @RequestParam float taxa) {
        return locadora.depreciarVeiculos(tipo, taxa);
    }
    @PutMapping("/aumentardiaria")
    public String aumentarDiaria(@RequestParam int tipo, @RequestParam float taxa) {
        return locadora.aumentarDiaria(tipo, taxa);
    }
    @DeleteMapping("/removertudo")
    public ResponseEntity<String> apagarTudo() {
        locadora.removerTudo();
        return ResponseEntity.ok("Todos os dados apagados.");
    }
    @DeleteMapping("/{placa}")
    public Optional<Veiculo> excluirVeiculo (@PathVariable String placa){
        Optional<Veiculo> veiculo = dao.findById(placa);
        dao.deleteById(placa);
        return veiculo;
    }

}
