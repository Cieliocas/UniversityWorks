package br.com.criandoapi.projeto.model;

import jakarta.persistence.*;

import java.util.Date;


@Entity
@Table(name = "registroaluguel")
public class RegistroAluguel {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Integer id;

    @Column(name = "placa")
    private String placa;

    @Column(name = "cpf")
    private int cpf;

    @Column(name = "data")
    private Date data;

    @Column(name = "dias")
    private Integer dias;

    @Column(name = "data_devolucao")
    private Date data_devolucao;

    @Column(name = "faturamento")
    private Double faturamento;

    public Double getFaturamento() {
        return faturamento;
    }

    public void setFaturamento(Double faturamento) {
        this.faturamento = faturamento;
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getPlaca() {
        return placa;
    }

    public void setPlaca(String placa) {
        this.placa = placa;
    }

    public int getCpf() {
        return cpf;
    }

    public void setCpf(int cpf) {
        this.cpf = cpf;
    }

    public Date getData() {
        return data;
    }

    public void setData(Date data) {
        this.data = data;
    }

    public int getDias() {
        return dias;
    }

    public void setDias(int dias) {
        this.dias = dias;
    }

    public Date getData_devolucao() {
        return data_devolucao;
    }

    public void setData_devolucao(Date data_devolucao) {
        this.data_devolucao = data_devolucao;
    }

}
