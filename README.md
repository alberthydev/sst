# SST - Sistema de Suporte Técnico

Repositório do sistema de suporte técnico desenvolvido em C++ com Qt.

---

## Descrição

O **SST (Sistema de Suporte Técnico)** é uma aplicação desktop desenvolvida utilizando o framework Qt, que tem como objetivo facilitar o gerenciamento e controle de chamados técnicos, usuários e técnicos responsáveis. A aplicação oferece interfaces gráficas para login, gerenciamento de usuários e técnicos, além do controle e abertura de chamados.

---

## Funcionalidades

- Tela de login com autenticação.
- Cadastro e gerenciamento de usuários.
- Cadastro e gerenciamento de técnicos.
- Abertura e acompanhamento de chamados técnicos.
- Interface intuitiva baseada em Qt Designer.

---

## Tecnologias Utilizadas

- Linguagem: C++
- Framework: Qt 6 (Widgets)
- Banco de Dados: SQLite (arquivo `sst_db.db`)
- Ferramentas: Qt Creator, CMake para build

---

## Requisitos para Build

- Qt 6.x (Qt Widgets)
- Compilador C++ compatível com C++17 ou superior
- CMake (para gerar os arquivos de build)
- SQLite (embutido no projeto, não é necessário instalar separadamente)

---

## Como Compilar e Executar

### Usando Qt Creator (recomendado)

1. Clone o repositório:

```bash
git clone https://github.com/alberthydev/sst.git
```

2. Abra o arquivo SST.pro no Qt Creator.

3. Configure o kit (selecionando Qt 6 e compilador compatível).

4. Compile e execute o projeto.


## Usando linha de comando com CMake

1. Clone o repositório:
   
```bash
git clone https://github.com/alberthydev/sst.git
cd sst
```

2. Crie um diretório para o build:

```bash
mkdir build && cd build
```

3. Gere os arquivos do projeto com CMake:

```bash
cmake ..
```

4. Compile o projeto:

```bash
cmake --build .
```

5. Execute o binário gerado (Linux/macOS):

```bash
./sst
```

## Banco de Dados

O projeto utiliza um banco SQLite chamado sst_db.db, que armazena informações de usuários, técnicos e chamados.
Certifique-se que o arquivo sst_db.db esteja no diretório correto para que o sistema funcione corretamente.

