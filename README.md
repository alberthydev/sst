SST - Sistema de Suporte Técnico

Repositório do sistema de suporte técnico desenvolvido em C++ com Qt.
Descrição

O SST (Sistema de Suporte Técnico) é uma aplicação desktop desenvolvida utilizando o framework Qt, que tem como objetivo facilitar o gerenciamento e controle de chamados técnicos, usuários e técnicos responsáveis. A aplicação oferece interfaces gráficas para login, gerenciamento de usuários e técnicos, além do controle e abertura de chamados.
Funcionalidades

    Tela de login com autenticação.

    Cadastro e gerenciamento de usuários.

    Cadastro e gerenciamento de técnicos.

    Abertura e acompanhamento de chamados técnicos.

    Interface intuitiva baseada em Qt Designer.

Tecnologias Utilizadas

    Linguagem: C++

    Framework: Qt 6 (Widgets)

    Banco de Dados: SQLite (arquivo sst_db.db)

    Ferramentas: Qt Creator, CMake para build

Estrutura do Projeto

/
├── build/                   # Arquivos gerados da compilação
├── login/                   # Módulo da tela de login
│   ├── loginwindow.cpp
│   ├── loginwindow.h
│   └── loginwindow.ui
├── principal.cpp            # Janela principal do sistema
├── principal.ui             # Interface gráfica da janela principal
├── technician.cpp           # Código para gerenciamento de técnicos
├── technician.ui            # Interface de técnicos
├── userwindow.cpp           # Tela de usuários
├── userwindow.ui
├── sst_db.db                # Banco de dados SQLite com dados do sistema
├── main.cpp                 # Ponto de entrada da aplicação
├── CMakeLists.txt           # Arquivo de configuração do build com CMake
├── SST.pro                  # Arquivo de projeto Qt (caso utilize qmake)
└── README.md                # Este arquivo

Requisitos para Build

    Qt 6.x (Qt Widgets)

    Compilador C++ compatível com C++17 ou superior

    CMake (para gerar os arquivos de build)

    SQLite (embutido no projeto, não é necessário instalar separadamente)

Como Compilar e Executar
Usando Qt Creator (recomendado)

    Clone o repositório:

    git clone https://github.com/alberthydev/sst.git

    Abra o arquivo SST.pro no Qt Creator.

    Configure o kit (selecionando Qt 6 e compilador compatível).

    Compile e execute o projeto.

Usando linha de comando com CMake

    Clone o repositório:

git clone https://github.com/alberthydev/sst.git
cd sst

Crie um diretório para o build:

mkdir build && cd build

Gere os arquivos do projeto com CMake:

cmake ..

Compile o projeto:

    cmake --build .

    Execute o binário gerado (ex: ./sst ou sst.exe).

Banco de Dados

O projeto utiliza um banco SQLite sst_db.db, que armazena informações de usuários, técnicos e chamados. Certifique-se que o arquivo sst_db.db esteja no diretório correto para que o sistema funcione corretamente.
