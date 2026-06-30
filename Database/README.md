<div align="center">
  <h1>🗄️ Banco de Dados — Plataforma Pedagógica "BioAtiva"</h1>
  <p><b>Trabalho prático final (3ª nota) da disciplina de Banco de Dados — UFPI.</b></p>

  [![SGBD](https://img.shields.io/badge/SGBD-MySQL_8.0+-4479A1?style=for-the-badge&logo=mysql&logoColor=white)]()
  [![Ferramenta](https://img.shields.io/badge/Modelagem-MySQL_Workbench-00618A?style=for-the-badge)]()
</div>

<br>

Projeto de banco de dados para a **BioAtiva**, uma plataforma pedagógica idealizada no contexto de uma defesa de mestrado, na qual professores da educação infantil publicam, consultam e interagem sobre **Sequências Didáticas Investigativas (SDIs)** voltadas às áreas de ciências da natureza.

> 🏛️ Disciplina de **Banco de Dados** — Prof. Flávio Ferry de Oliveira Moreira — Bacharelado em Ciência da Computação, **Universidade Federal do Piauí (UFPI)**.

---

## 📦 Conteúdo da pasta

| Arquivo | Descrição |
| :--- | :--- |
| 📄 `Estudo de Caso - BioAtiva - Francielio Castro.docx` | Estudo de caso com as regras de negócio e os relacionamentos do domínio. |
| 🧩 `bioativa_francielio_castro.mwb` | Modelo Entidade-Relacionamento (Diagrama E-R) do MySQL Workbench. |
| 🛢️ `bioativa_francielio_castro.sql` | Script completo: criação do banco (DDL), dados de teste (DML) e consultas (DQL). |

---

## 🧱 Modelo de Dados

Cinco tabelas, normalizadas, com o relacionamento muitos-para-muitos resolvido por uma entidade associativa:

- **Professor** `1:N` **SequenciaDidatica** — um professor publica várias SDIs.
- **Professor** `1:N` **Interacao** — um professor faz vários comentários/avaliações.
- **SequenciaDidatica** `1:N` **Interacao** — uma SDI recebe várias interações.
- **SequenciaDidatica** `N:M` **Categoria** — resolvido pela tabela associativa **SdiCategoria**.

---

## ▶️ Como executar

```bash
# 1. Conecte-se ao MySQL
mysql -u root -p

# 2. Rode o script (cria o banco, popula e deixa as consultas prontas)
mysql> SOURCE bioativa_francielio_castro.sql;
mysql> USE bioativa;
```

O script inclui **9 consultas de teste** demonstrando `JOIN`, funções de agregação (`COUNT`, `AVG`, `GROUP_CONCAT`), `GROUP BY`, `HAVING` e `WHERE`.

---
<div align="center">
  <small>⌨️ Franciélio Evangelista dos Santos Castro — Matrícula 20249050551 — 2026</small>
</div>
