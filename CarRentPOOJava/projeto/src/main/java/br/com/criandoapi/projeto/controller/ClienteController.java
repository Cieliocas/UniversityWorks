package br.com.criandoapi.projeto.controller;
import br.com.criandoapi.projeto.DAO.ICliente;
import br.com.criandoapi.projeto.Excecoes.ClienteJaCadastrado;
import br.com.criandoapi.projeto.model.Cliente;
import br.com.criandoapi.projeto.service.MinhaLocadora;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/clientes")
public class ClienteController  {
    private final MinhaLocadora locadora;
    private final ICliente dao;

    @Autowired
    public ClienteController(MinhaLocadora locadora, ICliente dao) {
        this.locadora = locadora;
        this.dao = dao;
    }

    @GetMapping //mostra clientes no banco de dados
    public List<Cliente> listaClientes(){
        return (List<Cliente>) dao.findAll();
    }
    @PostMapping // insere clientes ao banco de dados
    public String inserirCliente(@RequestBody Cliente cliente) {
        try {
            locadora.inserir(cliente);
            return "Cliente inserido com sucesso";
        } catch (ClienteJaCadastrado e) {
            return "Erro: " + e.getMessage();
        }
    }
    @PutMapping // mudar informações de cliente
    public Cliente editarCliente(@RequestBody Cliente cliente){
        Cliente clienteNovo = dao.save(cliente);
        return clienteNovo;
    }
    @DeleteMapping("/{cpf}") // exclui cliente com cpf e retorna o cliente ou retorna null
    public Optional<Cliente> excluirCliente(@PathVariable int cpf){
        Optional<Cliente> cliente = dao.findById(cpf);
        dao.deleteById(cpf);
        return cliente;
    }
    @GetMapping("/{cpf}")//procura cliente e retorna null ou cliente com cpf
    public Optional<Cliente> pesquisarCliente(@PathVariable int cpf) {
        return dao.findById(cpf);
    }

}
