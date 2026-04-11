package br.com.criandoapi.projeto.DAO;

import br.com.criandoapi.projeto.model.RegistroAluguel;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

public interface IRegistroAluguel extends CrudRepository<RegistroAluguel,Integer> {

    // Retorna soma do faturamento (ou 0) filtrando por tipo e intervalo de devolução
    @Query("""
    SELECT COALESCE(SUM(faturamento), 0.0)
    FROM RegistroAluguel ra JOIN Veiculo v ON ra.placa = v.placa
    WHERE (:tipo IS NULL OR v.tipo = :tipo)
      AND ra.data_devolucao >= :inicio
      AND (:fim IS NULL OR ra.data_devolucao <= :fim)
    """)
    Double faturamentoTotal(@Param("tipo") String tipo, @Param("inicio") Date inicio, @Param("fim") Date fim);

    // Retorna soma total de dias alugados (ou 0) com mesmo filtro
    @Query("""
    SELECT COALESCE(SUM(ra.dias), 0)
    FROM RegistroAluguel ra JOIN Veiculo v ON ra.placa = v.placa
    WHERE (:tipo IS NULL OR v.tipo = :tipo)
      AND ra.data_devolucao >= :inicio
      AND (:fim IS NULL OR ra.data_devolucao <= :fim)
    """)
    Integer quantidadeTotalDeDiarias(@Param("tipo") String tipo, @Param("inicio") Date inicio, @Param("fim") Date fim);

    RegistroAluguel findByPlaca(String placa);

    List<RegistroAluguel> findAllByPlaca(String placa);
}

