# Buffer management
Gerenciador de buffer em C++.

## Introdução

### Contexto

Este é o trabalho 01 da disciplina de Sistemas de Gerenciamento de Bancos de Dados do curso de Ciência da Computação da UFC.

### Descrição 

#### Organização

O projeto busca implementar uma versão simples de um gerenciador de buffer que lê linhas - as quais representam páginas de dados - do arquivo [database/bancodedados.csv], qeu por sua vez representa um disco. 
Essa leitura se dá por meio de I/O streams, implementada pela função `std::string BufferManagement::Read(int key)` em [src/BufferManagement.cpp].

O projeto está organizado estruturalmente da seguinte maneira:

* Header files no diretório [include], contendo os headers `BufferManagement.h`
e `Page.h`.
* No diretório [src], está a `main.cpp`, que contém uma interface simples no terminal para testar as funcionalidades do projeto, e a implementação de `BufferManagement.h`.
* Na pasta [database], está o arquivo csv
* Por fim, a pasta [notes] contém algumas anotações referentes à linguagem C++ estudados para auxiliar no desenvolvimento do projeto

#### Gerenciador de buffer

A lógica do **gerenciador de buffer** está presente na classe BufferManagement.

Atributos de BufferManagement:

* `std::vector<Page> pool {};` -> vetor que representa a buffer pool
* `int cacheHit {};` -> variável de controle de cache hit, que é incrementada para cada página que for solicitada e estiver salva no buffer
* `int cacheMiss {};` -> variável de controle cache miss, que é incrementada para cada página que for solicitada e **não** estiver salva no buffer
* `Policy policy {};` -> variável do tipo Policy que irá definir a política selecionada pelo usuário utilizando enum class
* `int circularPtr {};` -> ponteiro circular para a política de substituição CLOCK

Filas de BufferManagement:

* `std::deque<int> fifoQueue;` -> representa a fila de índices da política FIFO
* `std::deque<int> lruQueue;` -> representa a fila de índices da política LRU
* `std::deque<int> mruStack;` -> representa a fila de índices da política MRU

Funções de BufferManagement: estão devidamente comentadas em [include/BufferManagement.h]!!

#### Page

Agora falemos sobre a classe **Page**, que representa um quadro na buffer pool.

Atributos de Page:

* `std::string content {};` -> conteúdo da página, isto é, a linha de texto 
* `bool dirty {};` -> variável de atualização: true significa que a página foi modificada, enquanto false o contrário.
* `bool referenceBit {false};` -> bit de referência para a política de substituição CLOCK.

#### Políticas de substituição

Cada política usa uma `deque<int>` própria que guarda **índices do `pool`** de 0 a MAX_SIZE - 1.

---

**FIFO:**

Fetch:
```cpp
fifoQueue.push_back(i);
```
O índice do quadro recém-ocupado vai para o final da fila. Hits não alteram a fila, de forma que o primeiro da fila seja de fato o primeiro que entrou.

Evict:
```cpp
index = fifoQueue.front();
fifoQueue.pop_front();
```
O índice que entrou primeiro (front) será o índice da página removida. A ordem de chegada é o único critério.

---

**LRU**

Fetch (inserção):
```cpp
lruQueue.push_back(i);
```

Fetch (hit):
```cpp
removeFromDeque(lruQueue, i);
lruQueue.push_back(i);
```
Após um hit, o índice é retirado de onde está e reinserido no `back`. Com isso, o `front` sempre aponta para o índice **menos recentemente usado**.

Evict:
```cpp
index = lruQueue.front();
lruQueue.pop_front();
```
O índice escolhido será o `front`, o que está há mais tempo sem ser requisitado.

---

#### MRU

Fetch (inserção):
```cpp
mruStack.push_back(i);
```

Fetch (hit):
```cpp
removeFromDeque(mruStack, i);
mruStack.push_back(i);
```
Igual ao LRU: o índice vai para o `back`. Mas aqui o `back` é o escolhido, não o `front`.

Evict:
```cpp
index = mruStack.back();
mruStack.pop_back();
```
Escolhe o `back`, isto é, o índice usado mais recentemente.

---

## CLOCK

O ponteiro `circularPtr` percorre os quadros de forma circular.

Fetch (Inserção e hit):
```cpp
pool[i].referenceBit = true;
```
Toda página que entra ou é acessada recebe `referenceBit = 1`.

Evict:
```cpp
while (true) {
    if (!pool[circularPtr].referenceBit) {
        index = circularPtr;
        circularPtr = (circularPtr + 1) % MAX_SIZE;
        break;
    }
    pool[circularPtr].referenceBit = false;
    circularPtr = (circularPtr + 1) % MAX_SIZE;
}
```
O loop avança quadro a quadro. Se `referenceBit = 1`, zera e continua (segunda chance). Se `refBit = 0`, o quadro é o escolhido e o ponteiro avança mais um antes de parar.

---

### Especificação

Implementar um gerenciador de buffer simples, com as seguintes políticas de substituição de página: LRU, FIFO, CLOCK, MRU. Será aceita uma versão do código para cada política ou uma versão única com política de substituição selecionável. A linha de texto simula uma página e deve ter conteúdo, além de identificador de página (page#) e uma variável booleana de atualização. O gerenciador de buffer deve manter duas variáveis de controle: cache hit, incrementada todas as vezes que encontrar no buffer uma página solicitada, e cache miss, caso contrário.

As páginas do buffer serão linhas de texto lidas sob demanda a partir de um arquivo de texto anexo. O número máximo de entradas de buffer (linhas de texto) mantidas em memória deverá ser 5.

A interface do gerenciador de buffer deverá contar com as funções descritas a seguir:

- Fetch(int key): busca a informação de uma determinada entrada, ou seja, a página de texto. A “key” se refere a um page#. Esta função consulta primeiro o buffer. Caso encontre a entrada, retorna o conteúdo (linha de texto) nela contido. Caso contrário, busca a página no arquivo e transfere seu conteúdo para o buffer; neste momento, use uma função de randomização para escolher o valor a ser atribuído à variável de atualização (TRUE ou FALSE). Observe que a função Fetch(int key) deve invocar Evict() caso o buffer esteja cheio.

- Evict(): seleciona uma entrada do buffer e a remove. A escolha de qual entrada remover deve ser feita conforme a política de substituição. Durante a execução da função, deve ser exibida a página removida do buffer e, caso a variável de atualização seja TRUE, deve ser exibido o texto “W” após o conteúdo da página.

- DisplayCache(): exibe todas as entradas presentes no buffer (Chave -> Número da linha, Valor -> Texto da linha, Atualização -> valor booleano)

- DisplayStats(): exibe informações sobre cache hit e cache miss.

## Como funciona

### Como rodar

#### Utilizando cmake:

* É a princípio preciso instalar o cmake na máquina
* Criar um diretório chamado `build` na raiz do projeto
* Entrar na pasta com `cd build`
* Digitar no terminal respectivamente:
* `cmake ..`
* `make` 
* `./buffer`

#### Compilação manual:

Compilar
* `g++ -std=c++17 -Iinclude src/main.cpp src/BufferManagement.cpp -o executable`

Executar
* `./executable` (linux)
* `.\executable.exe` (windows) -> é preciso compilar para windows primeiramente

#### OBS.: o executável do projeto foi compilado em linux

## Notas de aprendizado
Notas referentes aos conceitos e features da linguagem C++ nos quais me aprofundei durante o desenvolvimento do projeto.

* [Input and Output](notes/IO.md)
* [deque](notes/deque.md)

OBS.: nem todos os assuntos e recursos das notas foram/serão utilizados no desenvolvimento do projeto. As anotações são baseadas num estudo geral de cada tópico realizado antes do desenvolvimento, que geralmente são capítulos compreendidos em https://www.learncpp.com.# Buffer_Management

## Bibliografias
* https://www.learncpp.com/
* https://www.w3schools.com/cpp/
* Sistemas de Gerenciamento de Bancos de Dados - Ramakrishnan • Gehrke
* Database System Concepts - Abraham Silberschatz
