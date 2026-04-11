import java.util.ArrayList;
import java.util.List;

public class Conta {

    private int numero;
    private double saldo;
    private Pessoa dono;
    private List<String> operacoes;

    public Conta(int n, Pessoa p) {
        if (n > 0) {
            numero = n;
            dono = p;
            operacoes = new ArrayList<>();
        }
    }

    public Pessoa getDono() {
        return dono;
    }

    public int getNumero() {
        return numero;
    }

    public double getSaldo() {
        return saldo;
    }

    public void credito(double valor) {
        if (valor > 0) {
            saldo = saldo + valor;
            operacoes.add("Credito: " + String.format("%.2f", valor) + ". Saldo: " + String.format("%.2f", saldo));
        }
    }

    public void debito(double valor) {
        if (valor > 0 && valor <= saldo) {
            saldo = saldo - valor;
            operacoes.add("Debito: " + String.format("%.2f", valor) + ". Saldo: " + String.format("%.2f", saldo));
        } else {
            System.out.println("Valor debitado invalido.");
        }
    }

    public void exibirExtrato() {
        for (String operacao : operacoes) {
            System.out.println("Conta: " + numero + ". " + operacao);
        }
    }
}
