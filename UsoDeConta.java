public class UsoDeConta {

    public static void main(String[] args) {
        Pessoa p1 = new Pessoa(1, "Pedro");
        Pessoa p2 = new Pessoa(2, "Raimundo");

        Conta c1 = new Conta(1, p1);
        Conta c2 = new Conta(2, p2);

        c1.credito(100);
        c1.debito(20);
        c1.credito(8);
        c1.debito(77.50);

        c2.credito(100);
        c2.credito(22);
        c2.debito(21);
        c2.debito(150);

        System.out.println("Conta 1: " + c1.getSaldo());
        System.out.println("Conta 2: " + c2.getSaldo());
        
        c1.exibirExtrato();
        c2.exibirExtrato();
    }
}
