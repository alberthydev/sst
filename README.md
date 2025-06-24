# SST - Sistema de Suporte Técnico

Repositório do sistema de suporte técnico desenvolvido em C++ com Qt utilizando padrões de projetos vistos em aula.

---

## Descrição

O **SST (Sistema de Suporte Técnico)** é uma aplicação desktop desenvolvida utilizando o framework Qt, que tem como objetivo facilitar o gerenciamento e controle de chamados técnicos, usuários e técnicos responsáveis. A aplicação oferece interfaces gráficas para login, gerenciamento de usuários e técnicos, além do controle e abertura de chamados.

---

## Funcionalidades

- Tela de login com autenticação.
- Cadastro e gerenciamento de usuários.
- Abertura e acompanhamento de chamados técnicos.
- Chat entre o técnico e o solicitante.
- Interface intuitiva.

---
## Padrões Utilizados

- Singleton
- Facade
- State

---

## Tecnologias Utilizadas

- Linguagem: C++
- Framework: Qt 6 (Widgets)
- Banco de Dados: SQLite (arquivo `sst_db.db`)

---

## Requisitos para Build

- Qt 6.x (Qt Widgets)
- Compilador C++ compatível com C++17 ou superior
- qmake (para gerar os arquivos de build)
- SQLite (embutido no projeto, não é necessário instalar separadamente)

---

## Como Executar 

### Windows
1. Baixe a release mais recente do repositório
2. Extraia a pasta
3. Execute o SST.exe

### Linux/MAC
#### Usando Qt Creator (recomendado) (CERTIFIQUE-SE DE TER TODOS OS PACOTES CORRETOS COM O QT MAINTENCE TOOL)

1. Clone o repositório:

```bash
git clone https://github.com/alberthydev/sst.git
```

2. Abra o arquivo SST.pro no Qt Creator.

3. Configure o kit (selecionando Qt 6 e compilador compatível).

4. Compile e execute o projeto.


#### Usando linha de comando com CMake (CERTIFIQUE-SE DE TER TODOS OS PACOTES CORRETOS COM O QT MAINTENCE TOOL)

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

