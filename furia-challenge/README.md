# Furia Fan App

Bem-vindo ao **Furia Fan App**, uma aplicação desenvolvida para conectar fãs e entusiastas do time FURIA Esports. Este projeto foi criado para oferecer uma experiência interativa e intuitiva, permitindo que os usuários explorem informações, interajam com a comunidade e acompanhem as novidades do time.

---

## 🌐 Rotas da Aplicação

### 1. **Home (`/`)**
   - A página inicial da aplicação.
   - Apresenta uma visão geral do aplicativo, com informações básicas sobre o time FURIA e links para acessar outras seções.

### 2. **Dashboard (`/dashboard`)**
   - Área principal para usuários autenticados.
   - Exibe informações personalizadas, como notícias, estatísticas e interações da comunidade.

### 3. **Login (`/login`)**
   - Página de autenticação.
   - Permite que os usuários façam login com suas credenciais para acessar funcionalidades exclusivas.

### 4. **Register (`/register`)**
   - Página de registro.
   - Permite que novos usuários criem uma conta para acessar a aplicação.

---

## 📝 O que o app faz?

O **Furia Fan App** é uma plataforma dedicada aos fãs do time FURIA Esports. Ele oferece:
- **Informações e notícias**: Atualizações sobre o time, partidas e eventos.
- **Interação com a comunidade**: Espaço para fãs se conectarem e compartilharem experiências.
- **Dashboard personalizado**: Conteúdo exclusivo para usuários autenticados.
- **Autenticação segura**: Login e registro para proteger os dados dos usuários.

---

## 📂 Estrutura do Projeto

A estrutura do projeto segue uma organização modular para facilitar a manutenção e escalabilidade:

```
furia-fan-app/
├── components/       # Componentes reutilizáveis da interface
├── pages/            # Páginas principais da aplicação (rotas)
│   ├── index.tsx     # Página inicial
│   ├── dashboard.tsx # Página do dashboard
│   ├── login.tsx     # Página de login
│   ├── register.tsx  # Página de registro
├── styles/           # Estilos globais e específicos
├── utils/            # Funções utilitárias e helpers
├── public/           # Arquivos estáticos (imagens, ícones, etc.)
├── tsconfig.json     # Configuração do TypeScript
├── next.config.js    # Configuração do Next.js
└── package.json      # Dependências e scripts do projeto
```

---

## 🛠️ Tecnologias Utilizadas

- **Next.js**: Framework React para renderização do lado do servidor (SSR) e geração de sites estáticos.
- **TypeScript**: Superset do JavaScript para tipagem estática e maior segurança no desenvolvimento.
- **Tailwind CSS**: Framework de CSS utilitário para estilização rápida e responsiva.
- **Axios**: Biblioteca para requisições HTTP.
- **React Hook Form**: Gerenciamento de formulários de forma eficiente.
- **Yup**: Validação de esquemas para formulários.
- **JWT (JSON Web Token)**: Autenticação segura para usuários.

---

## 🚀 Etapas de Desenvolvimento

1. **Planejamento**:
   - Definição das funcionalidades principais e estrutura do projeto.
   - Criação de wireframes para as páginas.

2. **Configuração Inicial**:
   - Configuração do ambiente de desenvolvimento com Next.js e TypeScript.
   - Configuração de rotas e estrutura inicial.

3. **Desenvolvimento das Funcionalidades**:
   - Implementação das páginas principais (`/`, `/dashboard`, `/login`, `/register`).
   - Criação de componentes reutilizáveis.

4. **Estilização**:
   - Aplicação de estilos com Tailwind CSS para garantir uma interface moderna e responsiva.

5. **Autenticação**:
   - Implementação de login e registro com validação de formulários e autenticação JWT.

6. **Testes e Refinamento**:
   - Testes de usabilidade e correção de bugs.
   - Otimização para SEO e performance.

7. **Deploy**:
   - Publicação da aplicação em um serviço de hospedagem (ex.: Vercel).

---

## 📌 Próximos Passos

- Adicionar suporte a notificações em tempo real.
- Implementar um sistema de chat para interação entre fãs.
- Melhorar a acessibilidade da aplicação.
- Expandir as funcionalidades do dashboard com estatísticas detalhadas.

---

## Learn More

To learn more about Next.js, take a look at the following resources:

- [Next.js Documentation](https://nextjs.org/docs) - learn about Next.js features and API.
- [Learn Next.js](https://nextjs.org/learn) - an interactive Next.js tutorial.

You can check out [the Next.js GitHub repository](https://github.com/vercel/next.js/) - your feedback and contributions are welcome!

## Deploy on Vercel

The easiest way to deploy your Next.js app is to use the [Vercel Platform](https://vercel.com/new?utm_medium=default-template&filter=next.js&utm_source=create-next-app&utm_campaign=create-next-app-readme) from the creators of Next.js.

Check out our [Next.js deployment documentation](https://nextjs.org/docs/deployment) for more details.
