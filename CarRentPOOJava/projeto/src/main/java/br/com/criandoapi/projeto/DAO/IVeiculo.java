package br.com.criandoapi.projeto.DAO;
import br.com.criandoapi.projeto.model.Veiculo;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

public interface IVeiculo  extends CrudRepository<Veiculo,String>{
    // Busca veículos por tipo e cilindrada mínima
    List<Veiculo> findByTipoAndCilindradaGreaterThanEqual(String tipo, int cilindrada);

    // Busca veículos por tipo e autonomia mínima
    List<Veiculo> findByTipoAndAutonomiaGreaterThanEqual(String tipo, int autonomia);

    // Busca veículos por tipo e capacidade de carga mínima
    List<Veiculo> findByTipoAndCapacCargaGreaterThanEqual(String tipo, float capacCarga);

    // Busca veículos por tipo e capacidade de passageiros mínima
    List<Veiculo> findByTipoAndCapacPassageirosGreaterThanEqual(String tipo, int capacPassageiros);

    // Busca veículo por placa exata
    Veiculo findByPlaca(String placa);

    // Busca lista de veículos pelo tipo
    List<Veiculo> findByTipo(String tipo);

}


