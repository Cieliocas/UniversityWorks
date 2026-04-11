package br.com.criandoapi.projeto.model;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;


@Entity
@Table(name = "veiculo")
public class Veiculo {

    @Id
    @Column(name = "placa")
    private String placa;

    @Column(name = "marca", length = 100, nullable = true)
    private String marca;

    @Column(name = "modelo", length = 100, nullable = true)
    private String modelo;

    @Column(name = "ano_fabricacao")
    private Integer ano_fabricacao;

    @Column(name = "val_bem")
    private float val_bem;

    @Column(name = "val_diaria")
    private float val_diaria;

    @Column(name = "tipo", length = 50, nullable = true)
    private String tipo;

    @Column(name = "cilindrada")
    private int cilindrada;

    @Column(name = "autonomia")
    private int autonomia;

    @Column(name = "capac_passageiros")
    private int capacPassageiros;

    @Column(name = "capac_carga")
    private float capacCarga;

    // Getters e Setters

    public String getPlaca() {
        return placa;
    }

    public void setPlaca(String placa) {
        this.placa = placa;
    }
    public String getMarca() {
        return marca;
    }

    public void setMarca(String marca) {
        this.marca = marca;
    }

    public String getModelo() {
        return modelo;
    }

    public void setModelo(String modelo) {
        this.modelo = modelo;
    }

    public int getAno_fabricacao() {
        return ano_fabricacao;
    }

    public void setAno_fabricacao(int anoFabricacao) {
        this.ano_fabricacao = anoFabricacao;
    }

    public float getVal_bem() {
        return val_bem;
    }

    public void setVal_bem(float valBem) {
        this.val_bem = valBem;
    }

    public float getVal_diaria() {
        return val_diaria;
    }

    public void setVal_diaria(float valDiaria) {
        this.val_diaria = valDiaria;
    }

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public int getCilindrada() {
        return cilindrada;
    }

    public void setCilindrada(int cilindrada) {
        this.cilindrada = cilindrada;
    }

    public int getAutonomia() {
        return autonomia;
    }

    public void setAutonomia(int autonomia) {
        this.autonomia = autonomia;
    }

    public int getCapacPassageiros() {
        return capacPassageiros;
    }
    public void setCapacPassageiros(int capacPassageiros) {
        this.capacPassageiros = capacPassageiros;
    }

    public float getCapacCarga() {
        return capacCarga;
    }
    public void setCapacCarga(float capacCarga) {
        this.capacCarga = capacCarga;
    }

    public float calcularSeguro() {
        // Define taxa de seguro conforme o tipo do veículo
        float taxaSeguro = switch (this.tipo) {
            case "Moto" -> 0.11f;     // 11%
            case "Carro" -> 0.03f;    // 3%
            case "Caminhao" -> 0.08f; // 8%
            case "Onibus" -> 0.20f;   // 20%
            default -> 0f;            // Nenhuma taxa para outros tipos
        };

        return (val_bem * taxaSeguro) / 365;
    }

    // Calcula o valor total do aluguel para 'x' dias
    public float calcularAluguel(int dias) {
        float seguroDiario = calcularSeguro(); // valor do seguro diário
        return (val_diaria + seguroDiario) * dias; // soma diária + seguro multiplicado pelos dias
    }

}



