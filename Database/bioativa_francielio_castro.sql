-- ============================================================================
--  TRABALHO PRÁTICO DE BANCO DE DADOS (3ª NOTA) - UFPI / CCN
--  Disciplina: Banco de Dados - Prof. Flávio Ferry de Oliveira Moreira
--  Aluno: Franciélio Evangelista dos Santos Castro  -  Matrícula: 20249050551
--  Estudo de Caso: Plataforma Pedagógica "BioAtiva"
--  SGBD alvo: MySQL 8.0+  (utf8mb4)
-- ============================================================================
--
--  Como usar no terminal (simulação de uso):
--    $ mysql -u root -p
--    mysql> SOURCE /caminho/para/bioativa_francielio_castro.sql;
--    mysql> USE bioativa;
--    -- (em seguida, executar os comandos das seções DML e DQL abaixo)
--
-- ============================================================================


-- ############################################################################
-- # PARTE 1 - DDL: CRIAÇÃO DO BANCO E DAS TABELAS                             #
-- ############################################################################

DROP DATABASE IF EXISTS bioativa;
CREATE DATABASE bioativa
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;
USE bioativa;

-- ----------------------------------------------------------------------------
-- Tabela PROFESSOR (entidade independente)
-- Um professor publica VÁRIAS SDIs e realiza VÁRIAS interações (1:N).
-- ----------------------------------------------------------------------------
CREATE TABLE Professor (
    idProfessor  INT          NOT NULL AUTO_INCREMENT,
    nome         VARCHAR(120) NOT NULL,
    email        VARCHAR(120) NOT NULL,
    titulacao    ENUM('Graduação','Especialização','Mestrado','Doutorado')
                              NOT NULL DEFAULT 'Graduação',
    instituicao  VARCHAR(120) NOT NULL,
    PRIMARY KEY (idProfessor),
    CONSTRAINT uq_professor_email UNIQUE (email)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------------------------------------------------------
-- Tabela CATEGORIA (tags temáticas; entidade independente)
-- Uma categoria classifica VÁRIAS SDIs e uma SDI tem VÁRIAS categorias (N:M).
-- ----------------------------------------------------------------------------
CREATE TABLE Categoria (
    idCategoria  INT          NOT NULL AUTO_INCREMENT,
    nome         VARCHAR(60)  NOT NULL,
    descricao    VARCHAR(255) NULL,
    PRIMARY KEY (idCategoria),
    CONSTRAINT uq_categoria_nome UNIQUE (nome)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------------------------------------------------------
-- Tabela SEQUENCIADIDATICA (SDI) - depende de PROFESSOR (autor)
-- Cada SDI pertence a UM professor; um professor possui VÁRIAS SDIs (1:N).
-- ----------------------------------------------------------------------------
CREATE TABLE SequenciaDidatica (
    idSdi           INT          NOT NULL AUTO_INCREMENT,
    titulo          VARCHAR(150) NOT NULL,
    areaCiencia     VARCHAR(60)  NOT NULL COMMENT 'Ex.: Seres Vivos, Astronomia, Meio Ambiente',
    metodologia     VARCHAR(80)  NOT NULL COMMENT 'Ex.: Experimentação, Investigação de Campo',
    faixaEtaria     VARCHAR(40)  NOT NULL COMMENT 'Ex.: Maternal (2-3 anos), Pré-escola (4-5 anos)',
    dataPublicacao  DATE         NOT NULL,
    idProfessor     INT          NOT NULL,
    PRIMARY KEY (idSdi),
    CONSTRAINT fk_sdi_professor
        FOREIGN KEY (idProfessor) REFERENCES Professor (idProfessor)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------------------------------------------------------
-- Tabela INTERACAO (comentários/avaliações) - depende de PROFESSOR e de SDI
-- Cada interação é feita por UM professor sobre UMA SDI.
-- Um professor faz VÁRIAS interações e uma SDI recebe VÁRIAS interações (1:N).
-- ----------------------------------------------------------------------------
CREATE TABLE Interacao (
    idInteracao    INT       NOT NULL AUTO_INCREMENT,
    tipo           ENUM('Comentário','Dúvida','Sugestão','Elogio','Avaliação')
                             NOT NULL DEFAULT 'Comentário',
    conteudo       TEXT      NOT NULL,
    nota           TINYINT   NULL COMMENT 'Avaliação de 1 a 5 (opcional)',
    dataInteracao  DATETIME  NOT NULL,
    idProfessor    INT       NOT NULL,
    idSdi          INT       NOT NULL,
    PRIMARY KEY (idInteracao),
    CONSTRAINT fk_interacao_professor
        FOREIGN KEY (idProfessor) REFERENCES Professor (idProfessor),
    CONSTRAINT fk_interacao_sdi
        FOREIGN KEY (idSdi) REFERENCES SequenciaDidatica (idSdi),
    CONSTRAINT chk_interacao_nota CHECK (nota IS NULL OR nota BETWEEN 1 AND 5)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------------------------------------------------------
-- Tabela ASSOCIATIVA SDICATEGORIA - resolve o relacionamento N:M
-- entre SEQUENCIADIDATICA e CATEGORIA. Chave primária composta.
-- ----------------------------------------------------------------------------
CREATE TABLE SdiCategoria (
    idSdi        INT NOT NULL,
    idCategoria  INT NOT NULL,
    PRIMARY KEY (idSdi, idCategoria),
    CONSTRAINT fk_sdicat_sdi
        FOREIGN KEY (idSdi) REFERENCES SequenciaDidatica (idSdi),
    CONSTRAINT fk_sdicat_categoria
        FOREIGN KEY (idCategoria) REFERENCES Categoria (idCategoria)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- ############################################################################
-- # PARTE 2 - DML: COMANDOS SIMULADOS EM TERMINAL (INSERÇÃO DE DADOS)        #
-- #            -> Estes são os comandos digitados para popular o banco.      #
-- ############################################################################

-- Professores (4 registros) -------------------------------------------------
INSERT INTO Professor (nome, email, titulacao, instituicao) VALUES
('Mariana Albuquerque Lima',      'mariana.lima@ufpi.edu.br',     'Mestrado',       'Universidade Federal do Piauí'),
('Roberto Carvalho Nunes',        'roberto.nunes@semec.pi.gov.br','Especialização', 'SEMEC - Teresina/PI'),
('Patrícia Gomes Vasconcelos',    'patricia.gomes@ifpi.edu.br',   'Doutorado',      'Instituto Federal do Piauí'),
('Carlos Eduardo Tavares Sousa',  'carlos.tavares@uespi.br',      'Graduação',      'Universidade Estadual do Piauí');

-- Categorias / Tags (7 registros) -------------------------------------------
INSERT INTO Categoria (nome, descricao) VALUES
('Experimentação',  'Atividades baseadas em experimentos práticos'),
('Meio Ambiente',   'Temas ligados à natureza e preservação ambiental'),
('Seres Vivos',     'Estudo de plantas, animais e organismos'),
('Corpo Humano',    'Sentidos, partes do corpo e saúde'),
('Astronomia',      'Céu, dia e noite, Sol, Lua e estrelas'),
('Sustentabilidade','Consumo consciente e reaproveitamento'),
('Água',            'Ciclo da água, estados físicos e usos');

-- Sequências Didáticas Investigativas (5 registros) -------------------------
INSERT INTO SequenciaDidatica (titulo, areaCiencia, metodologia, faixaEtaria, dataPublicacao, idProfessor) VALUES
('Descobrindo o Ciclo da Água com Experimentos Simples', 'Meio Ambiente', 'Experimentação',                 'Pré-escola (4-5 anos)', '2026-03-10', 1),
('A Horta na Escola: Observando o Crescimento das Plantas','Seres Vivos',  'Observação e Registro',          'Maternal (2-3 anos)',   '2026-03-22', 2),
('Pequenos Astrônomos: Por que Existe Dia e Noite?',      'Astronomia',    'Roda de Conversa Investigativa', 'Pré-escola (4-5 anos)', '2026-04-05', 3),
('Os Cinco Sentidos: Explorando o Corpo Humano',          'Corpo Humano',  'Aprendizagem Baseada em Problemas','Pré-escola (4-5 anos)','2026-04-18', 1),
('Bichos do Jardim: Investigando os Insetos',             'Seres Vivos',   'Investigação de Campo',          'Maternal (2-3 anos)',   '2026-05-02', 3);

-- Interações / Comentários (12 registros) -----------------------------------
INSERT INTO Interacao (tipo, conteudo, nota, dataInteracao, idProfessor, idSdi) VALUES
('Avaliação','Apliquei na minha turma e as crianças adoraram observar a evaporação. Sequência muito bem amarrada.', 5, '2026-03-12 09:30:00', 2, 1),
('Dúvida',   'Qual o tempo médio que você sugere para a etapa de observação da condensação?',                       NULL,'2026-03-15 14:05:00', 3, 1),
('Elogio',   'Material muito bem estruturado e com linguagem acessível. Parabéns!',                                  4, '2026-03-18 20:12:00', 4, 1),
('Avaliação','A ideia da horta é excelente para trabalhar paciência e cuidado com o tempo.',                         4, '2026-03-25 08:45:00', 1, 2),
('Sugestão', 'Sugiro incluir o registro em desenho do crescimento semanal das mudas.',                               NULL,'2026-03-28 11:20:00', 3, 2),
('Avaliação','A roda de conversa funcionou muito bem para levantar as hipóteses das crianças.',                      5, '2026-04-08 16:40:00', 1, 3),
('Comentário','Vou adaptar para o maternal usando lanterna e globo terrestre. Obrigada por compartilhar!',           NULL,'2026-04-10 10:15:00', 2, 3),
('Avaliação','Atividade sensorial riquíssima, explora todos os sentidos com materiais simples.',                     4, '2026-04-20 19:00:00', 2, 4),
('Elogio',   'Excelente conexão com o cotidiano das crianças.',                                                      5, '2026-04-22 07:50:00', 3, 4),
('Dúvida',   'Você utilizou materiais de baixo custo para montar a caixa dos sentidos?',                             NULL,'2026-04-25 13:30:00', 4, 4),
('Avaliação','Investigação de campo é sempre envolvente. O roteiro de observação ficou ótimo.',                      5, '2026-05-05 15:10:00', 1, 5),
('Comentário','As crianças amam procurar insetos no jardim, vou aplicar na próxima semana.',                         NULL,'2026-05-07 09:05:00', 2, 5);

-- Vínculos SDI <-> Categoria (relacionamento N:M) ---------------------------
INSERT INTO SdiCategoria (idSdi, idCategoria) VALUES
(1, 1), (1, 2), (1, 7),   -- Ciclo da Água: Experimentação, Meio Ambiente, Água
(2, 2), (2, 3), (2, 6),   -- Horta na Escola: Meio Ambiente, Seres Vivos, Sustentabilidade
(3, 5),                   -- Pequenos Astrônomos: Astronomia
(4, 1), (4, 4),           -- Cinco Sentidos: Experimentação, Corpo Humano
(5, 2), (5, 3);           -- Bichos do Jardim: Meio Ambiente, Seres Vivos


-- ############################################################################
-- # PARTE 3 - DQL: COMANDOS SIMULADOS EM TERMINAL (CONSULTAS DE TESTE)       #
-- #            -> Estes são os SELECTs digitados para comprovar o uso.       #
-- ############################################################################

-- 1) Catálogo de SDIs com o professor autor e sua titulação (INNER JOIN).
SELECT s.idSdi, s.titulo, s.areaCiencia, s.faixaEtaria,
       p.nome AS autor, p.titulacao
FROM   SequenciaDidatica s
JOIN   Professor p ON p.idProfessor = s.idProfessor
ORDER  BY s.dataPublicacao;

-- 2) Quantas SDIs cada professor publicou (LEFT JOIN + COUNT + GROUP BY).
--    O LEFT JOIN garante que professores sem SDI também apareçam (com 0).
SELECT p.nome AS professor, COUNT(s.idSdi) AS total_sdis
FROM   Professor p
LEFT JOIN SequenciaDidatica s ON s.idProfessor = p.idProfessor
GROUP  BY p.idProfessor, p.nome
ORDER  BY total_sdis DESC;

-- 3) Média de avaliação por SDI, considerando só interações com nota
--    (JOIN + AVG + GROUP BY + HAVING). Mostra apenas as bem avaliadas (>= 4).
SELECT s.titulo,
       ROUND(AVG(i.nota), 2) AS media_avaliacao,
       COUNT(i.nota)         AS qtd_avaliacoes
FROM   SequenciaDidatica s
JOIN   Interacao i ON i.idSdi = s.idSdi
WHERE  i.nota IS NOT NULL
GROUP  BY s.idSdi, s.titulo
HAVING AVG(i.nota) >= 4
ORDER  BY media_avaliacao DESC;

-- 4) Todas as interações de uma SDI específica, com o autor (JOIN + WHERE).
SELECT i.dataInteracao, p.nome AS autor, i.tipo, i.nota, i.conteudo
FROM   Interacao i
JOIN   Professor p ON p.idProfessor = i.idProfessor
WHERE  i.idSdi = 1
ORDER  BY i.dataInteracao;

-- 5) Distribuição das SDIs por área da ciência (COUNT + GROUP BY).
SELECT s.areaCiencia, COUNT(*) AS total_sequencias
FROM   SequenciaDidatica s
GROUP  BY s.areaCiencia
ORDER  BY total_sequencias DESC;

-- 6) Categorias (tags) de cada SDI - relacionamento N:M
--    (duplo JOIN pela tabela associativa + GROUP_CONCAT).
SELECT s.titulo,
       GROUP_CONCAT(c.nome ORDER BY c.nome SEPARATOR ', ') AS categorias
FROM   SequenciaDidatica s
JOIN   SdiCategoria sc ON sc.idSdi = s.idSdi
JOIN   Categoria    c  ON c.idCategoria = sc.idCategoria
GROUP  BY s.idSdi, s.titulo;

-- 7) Professores mais ativos na interação com a comunidade
--    (JOIN + COUNT + GROUP BY + ORDER BY).
SELECT p.nome, COUNT(i.idInteracao) AS total_interacoes
FROM   Professor p
JOIN   Interacao i ON i.idProfessor = p.idProfessor
GROUP  BY p.idProfessor, p.nome
ORDER  BY total_interacoes DESC;

-- 8) Categorias mais utilizadas no acervo (N:M + COUNT + GROUP BY).
SELECT c.nome AS categoria, COUNT(sc.idSdi) AS qtd_sdis
FROM   Categoria c
LEFT JOIN SdiCategoria sc ON sc.idCategoria = c.idCategoria
GROUP  BY c.idCategoria, c.nome
ORDER  BY qtd_sdis DESC, categoria;

-- 9) SDIs voltadas à pré-escola publicadas a partir de abril/2026
--    (WHERE composto com filtro de data e de faixa etária).
SELECT s.titulo, s.faixaEtaria, s.dataPublicacao, p.nome AS autor
FROM   SequenciaDidatica s
JOIN   Professor p ON p.idProfessor = s.idProfessor
WHERE  s.dataPublicacao >= '2026-04-01'
  AND  s.faixaEtaria = 'Pré-escola (4-5 anos)'
ORDER  BY s.dataPublicacao;

-- ============================================================================
--  FIM DO SCRIPT - BioAtiva / Franciélio Evangelista dos Santos Castro
-- ============================================================================
