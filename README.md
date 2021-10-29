# Projeto 3 - Implementação do vector
Neste projeto foi implementado um container vetor semelhante ao do std::vector, que possue as principais operações que devem ser suportadas pelo vector do std.

## Categorias do Projeto
O projeto está organizado da seguinte forma:
```
trabalho-01-neylanepl      ---> Nome da pasta do projeto.
├── build                  ---> Pasta onde os executáveis serão gerados. [será criada]
├── docs                   ---> Pasta que contém o documento com as instruções para a implementação deste trabalho.
├── source                 ---> Pasta que contém o código fonte do projeto
│   ├── include            ---> Pasta que contém os arquivos headers ".h".
│   ├── tests              ---> Pasta que contém os arquivos para os testes do programa.
│   ├── CMakeLists.txt     ---> Script de configuração do cmake.
├── README.md              ---> Instruções para compilar e executar o programa.
```

# Compilando e Executando
A compilação e execução do projeto pode ser feita usando *cmake*. 

## Como compilar o projeto

1. Entre na pasta principal do projeto.

2. Execute o comando abaixo para criar o diretório de construção e gerar o Makefile baseado no script encontrado em `source/CMakeLists.txt`.
```console
cmake -S source -B build
```


3. Execute o comando abaixo para disparar o processo de compilação dentro da pasta `build`.
Assim, todos os arquivos gerados serão armazenados na pasta `build`.
```console
cmake --build build
```


## Como executar o projeto

1. Para executar os testes, utilize o comando:
```console
./build/tests/all_tests
```

# Limitações ou funcionalidades não implementadas no programa

Analisando as implementações realizadas, acredita-se que todas as funcionalidades esperadas foram desenvolvidas. Além disso, limitações não foram encontradas no projeto.

--------
&copy; Jonas&Neylane/UFRN 2021.