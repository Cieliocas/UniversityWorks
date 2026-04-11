package br.com.criandoapi.projeto.DAO;
import br.com.criandoapi.projeto.model.Cliente;
import org.springframework.data.repository.CrudRepository;

// Interface para acesso aos dados da entidade Cliente, com operações CRUD básicas
public interface ICliente extends CrudRepository<Cliente, Integer> {

    // busca um cliente pelo CPF (chave única de cliente)
    Cliente findByCpf(int cpf);
}
