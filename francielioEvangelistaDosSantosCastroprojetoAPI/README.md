---

# 🚗 Projeto API - Locadora de Veículos

Este projeto é uma API REST desenvolvida em **Spring Boot** que simula o funcionamento de uma locadora de veículos. Ele permite o gerenciamento completo de clientes, veículos e registros de aluguel, com funcionalidades como aluguel, devolução, cálculo de faturamento e operações específicas por tipo de veículo.

## 🧰 Tecnologias Utilizadas

- Java 17+
- Spring Boot
- Spring Data JPA
- Hibernate
- Banco de Dados H2 (ou substituível por MySQL/PostgreSQL)
- RESTful APIs
- Maven

## 📦 Estrutura do Projeto

- `controller`: Define os endpoints REST.
- `model`: Contém as entidades Cliente, Veiculo e RegistroAluguel.
- `DAO`: Interfaces JPA para acesso ao banco.
- `service`: Contém a lógica de negócios da locadora.
- `excecoes`: Classes de exceções personalizadas.

## 🚀 Funcionalidades

### Clientes
- Cadastro, edição, exclusão e consulta por CPF.
- Validação de cliente já existente.

### Veículos
- Cadastro, pesquisa por placa ou atributos como autonomia, carga, passageiros e cilindrada.
- Cálculo de aluguel com seguro proporcional ao tipo de veículo.
- Depreciação de valor do bem.
- Aumento do valor da diária.
- Exclusão e remoção em massa.

### Registros de Aluguel
- Criar registro de aluguel.
- Devolver veículo e registrar faturamento.
- Cálculo de faturamento total por tipo e período.
- Consulta de quantidade total de diárias.

## 📈 Regras de Negócio

- 🚫 Veículo não pode ser alugado se já estiver alugado.
- ⛔ Cliente precisa estar cadastrado para alugar.
- ✅ Aluguel = (valor_diária + seguro_diário) × dias
- 💰 Seguro é calculado com base no tipo de veículo:
  - Moto: 11%
  - Carro: 3%
  - Caminhão: 8%
  - Ônibus: 20%

## 🛠️ Executando o Projeto

```bash
# Clonar o repositório
git clone https://github.com/seuusuario/projeto-locadora.git

# Entrar na pasta
cd projeto-locadora

# Rodar com Maven
mvn spring-boot:run
```

A API será acessível em: `http://localhost:8080`

## 📮 Endpoints Principais

| Método | URL                         | Descrição                           |
|--------|-----------------------------|-------------------------------------|
| GET    | `/clientes`                 | Lista todos os clientes             |
| POST   | `/veiculos`                 | Cadastra novo veículo               |
| PUT    | `/veiculos/depreciarveiculos` | Deprecia veículos por tipo          |
| GET    | `/registrosalugueis`       | Lista registros de aluguel          |
| POST   | `/registrosalugueis`       | Cria registro de aluguel            |
| PUT    | `/registrosalugueis/devolucao` | Registra devolução de veículo    |

## 📌 Observações

- O projeto utiliza **exceções customizadas** para validações mais claras.
- Implementado com práticas de **Programação Orientada a Objetos**, como abstrações, encapsulamento e modularidade.
- Fácil integração com banco relacional.

---
