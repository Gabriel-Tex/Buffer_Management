# BIBLIOGRAFIA: https://www.learncpp.com/cpp-tutorial/input-and-output-io-streams/

## Input and output (I/O) streams

Uma coisa importante de entender logo no começo é que entrada e saída não fazem parte do núcleo da linguagem C++. Diferente de variáveis, ponteiros ou classes, I/O é fornecido pela biblioteca padrão, dentro do namespace std. É por isso que sempre usamos coisas como #include <iostream> e std::cout.

Toda a infraestrutura de entrada e saída em C++ é baseada em streams.

### Classes principais

A biblioteca define algumas classes fundamentais:
* std::istream → usada para entrada
* std::ostream → usada para saída
* std::iostream → suporta entrada e saída

### Standard streams (streams padrão)

* std::cin → entrada padrão (normalmente teclado)
* std::cout → saída padrão (normalmente tela)
* std::cerr → saída de erro (não bufferizada)
* std::clog → saída de log (bufferizada)

### Buffered vs unbuffered

* std::cerr é unbuffered → imprime imediatamente
* std::clog é buffered → acumula e imprime depois

Na prática:

* use cerr para erros críticos (quer ver na hora)
* use clog para logs (pode esperar acumular)

## Input com istream

### String input com std::cin (problema clássico)

#### Problema

std::cin >> name; // this won't work as expected since std::cin breaks on whitespace

Exemplo:

Enter your full name: John Doe

Resultado:

name = "John"
" Doe" fica no buffer

Isso acontece porque >> para em whitespace.

#### std::getline() para ler linhas completas

getline() lê toda a string até `\n`

```
std::getline(std::cin >> std::ws, name); 
// read a full line of text into name
```

#### O que é std::ws?

std::ws é um manipulador de entrada do C++ que serve para ignorar (descartar) espaços em branco do stream, limpando whitespace antes da leitura.

Ele remove caracteres como:

* ' ' (espaço)
* '\n' (quebra de linha)
* '\t' (tab)

até encontrar o primeiro caractere não branco.

Ex.: 

```
std::getline(std::cin >> std::ws, name);
```

### Quantidade de caracteres lidos

```
int main()
{
    char strBuf[100]{};
    std::cin.getline(strBuf, 100);

    std::cout << strBuf << '\n';
    std::cout << std::cin.gcount() << " characters were read" << '\n';

    return 0;
}
```

## Saída com ostream e ios

### Formatação

Há duas formas de alterar as opções de formatação: flags e manipuladores. 
* Você pode pensar nas flags como variáveis booleanas que podem ser ligadas e desligadas. 
* Manipuladores são objetos inseridos em um stream que afetam a forma como as coisas são lidas ou escritas.

#### Flags

Para ativar uma flag, use a função setf(), passando a flag adequada como parâmetro.

```
std::cout.setf(std::ios::showpos); // ativa a flag std::ios::showpos
std::cout << 27 << '\n';
```

Isso resulta na seguinte saída:

```
+27
```

flag showpos -> imprime números positivos com + na frente

É possível ativar múltiplas flags ao mesmo tempo usando o operador OR bit a bit (|)

```
std::cout.setf(std::ios::showpos | std::ios::uppercase); // ativa as flags showpos e uppercase
std::cout << 1234567.89f << '\n';
```

Saída:

```
+1.23457E+06
```

Para desativar uma flag, use a função unsetf():

```
std::cout.setf(std::ios::showpos); // ativa a flag showpos
std::cout << 27 << '\n';
std::cout.unsetf(std::ios::showpos); // desativa a flag showpos
std::cout << 28 << '\n';
```

Saída:
```
+27
28
```

Muitas flags pertencem a grupos, chamados de format groups (grupos de formatação). Um grupo de formatação é um conjunto de flags que realizam opções de formatação similares — e às vezes mutuamente exclusivas. Por exemplo, o grupo chamado "basefield" contém as flags "oct", "dec" e "hex", que controlam a base numérica usada para valores inteiros. Por padrão, a flag "dec" está ativada.

```
std::cout.setf(std::ios::hex); // tenta ativar a saída em hexadecimal
std::cout << 27 << '\n';
```

Obtemos a seguinte saída:

```
27
```

Não funcionou! O motivo é que setf() apenas ativa flags — ela não é inteligente o suficiente para desativar flags mutuamente exclusivas. Consequentemente, quando ativamos std::hex, std::ios::dec ainda estava ativo, e aparentemente std::ios::dec tem precedência. Há duas formas de contornar esse problema.

Solução: usar uma versão diferente de setf() que aceita dois parâmetros — o primeiro é a flag a ser ativada, e o segundo é o grupo de formatação ao qual ela pertence. Ao usar essa forma, todas as flags do grupo são desativadas, e apenas a flag passada é ativada:

```
// Ativa std::ios::hex como a única flag do grupo std::ios::basefield
std::cout.setf(std::ios::hex, std::ios::basefield);
std::cout << 27 << '\n';
```

Isso também produz a saída esperada:
```
1b
```

#### Manipuladores

São inteligentes o suficiente para ativar e desativar as flags apropriadas automaticamente.

```
std::cout << std::hex << 27 << '\n'; // imprime 27 em hexadecimal
std::cout << 28 << '\n';             // ainda estamos em hexadecimal
std::cout << std::dec << 29 << '\n'; // volta para decimal
```

Esse programa produz a saída:
```
1b
1c
29
```

##  I/O básico de arquivos

Existem 3 classes básicas de I/O de arquivos em C++: 

* ifstream (derivada de istream)
* ofstream (derivada de ostream)
* fstream (derivada de iostream). 

Essas classes realizam entrada, saída e entrada/saída de arquivos, respectivamente. Para usar as classes de I/O de arquivos, você precisará incluir o header fstream.

Diferentemente dos streams cout, cin, cerr e clog, que já estão prontos para uso, os streams de arquivo precisam ser configurados explicitamente pelo programador.

Para tal, basta instanciar um objeto da classe de I/O de arquivo apropriada, passando o nome do arquivo como parâmetro. 
Então use o operador de inserção (<<) ou extração (>>) para escrever ou ler dados do arquivo. 
Quando terminar, há várias formas de fechar um arquivo: 
* chamar explicitamente a função close()
* simplesmente deixar a variável de I/O de arquivo sair de escopo (o destrutor da classe de I/O de arquivo fechará o arquivo para você)

### Saída para arquivo

```
#include <fstream>
#include <iostream>

int main()
{
    // ofstream é usado para escrever arquivos
    // Vamos criar um arquivo chamado Sample.txt
    std::ofstream outf{ "Sample.txt" };

    // Se não conseguimos abrir o stream de saída para escrita
    if (!outf)
    {
        // Imprime um erro e sai
        std::cerr << "Uh oh, Sample.txt não pôde ser aberto para escrita!\n";
        return 1;
    }

    // Escreveremos duas linhas neste arquivo
    outf << "This is line 1\n";
    outf << "This is line 2\n";

    return 0;

    // Quando outf sai de escopo, o destrutor de ofstream
    // fechará o arquivo
}
```

### Leitura de arquivo

```
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    // ifstream é usado para ler arquivos
    // Vamos ler de um arquivo chamado Sample.txt
    std::ifstream inf{ "Sample.txt" };

    // Se não conseguimos abrir o stream de entrada para leitura
    if (!inf)
    {
        // Imprime um erro e sai
        std::cerr << "Uh oh, Sample.txt não pôde ser aberto para leitura!\n";
        return 1;
    }

    // Enquanto ainda houver conteúdo para ler
    std::string strInput{};
    while (inf >> strInput)
        std::cout << strInput << '\n';

    return 0;

    // Quando inf sai de escopo, o destrutor de ifstream
    // fechará o arquivo
}
```

Isso produz o resultado:
```
This
is
line
1
This
is
line
2
```

Lembre-se que o operador de extração quebra a leitura em espaços em branco. Para ler linhas inteiras, precisaremos usar a função getline().

```
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    // ifstream é usado para ler arquivos
    // Vamos ler de um arquivo chamado Sample.txt
    std::ifstream inf{ "Sample.txt" };

    // Se não conseguimos abrir o stream de entrada para leitura
    if (!inf)
    {
        // Imprime um erro e sai
        std::cerr << "Uh oh, Sample.txt não pôde ser aberto para leitura!\n";
        return 1;
    }

    // Enquanto ainda houver conteúdo para ler
    std::string strInput{};
    while (std::getline(inf, strInput))
        std::cout << strInput << '\n';

    return 0;

    // Quando inf sai de escopo, o destrutor de ifstream
    // fechará o arquivo
}
```

Isso produz o resultado:
```
This is line 1
This is line 2
```

### Modos de arquivo

O que acontece se tentarmos escrever em um arquivo que já existe? Executar o exemplo de saída novamente mostra que o arquivo original é completamente sobrescrito toda vez que o programa é executado.

E se, em vez disso, quiséssemos acrescentar mais dados ao final do arquivo? 
Os construtores dos streams de arquivo aceitam um segundo parâmetro opcional que permite especificar como o arquivo deve ser aberto. Esse parâmetro é chamado de **modo**, e as flags válidas que ele aceita vivem na classe `ios`.

| Modo de arquivo ios | Significado |
|---|---|
| `app` | Abre o arquivo em modo de acréscimo (append) |
| `ate` | Posiciona-se no final do arquivo antes de ler/escrever |
| `binary` | Abre o arquivo em modo binário (em vez de modo texto) |
| `in` | Abre o arquivo em modo de leitura (padrão para `ifstream`) |
| `out` | Abre o arquivo em modo de escrita (padrão para `ofstream`) |
| `trunc` | Apaga o arquivo se ele já existir |

É possível especificar múltiplas flags usando OR bit a bit (`|`). `ifstream` usa por padrão o modo `std::ios::in`. `ofstream` usa por padrão o modo `std::ios::out`. E `fstream` usa por padrão o modo `std::ios::in | std::ios::out`, o que significa que você pode tanto ler quanto escrever por padrão.

Vamos escrever um programa que acrescenta mais duas linhas ao arquivo `Sample.txt` que criamos anteriormente:

```cpp
#include <iostream>
#include <fstream>

int main()
{
    // Passamos a flag ios::app para dizer ao ofstream para acrescentar
    // em vez de reescrever o arquivo. Não precisamos passar std::ios::out
    // porque ofstream já usa std::ios::out por padrão
    std::ofstream outf{ "Sample.txt", std::ios::app };

    // Se não conseguimos abrir o stream de saída para escrita
    if (!outf)
    {
        // Imprime um erro e sai
        std::cerr << "Uh oh, Sample.txt não pôde ser aberto para escrita!\n";
        return 1;
    }

    outf << "This is line 3\n";
    outf << "This is line 4\n";

    return 0;

    // Quando outf sai de escopo, o destrutor de ofstream
    // fechará o arquivo
}
```

Agora se olharmos para `Sample.txt`, veremos o seguinte:

```
This is line 1
This is line 2
This is line 3
This is line 4
```

---

### Abrindo arquivos explicitamente com `open()`

Assim como é possível fechar explicitamente um stream de arquivo usando `close()`, também é possível abrir explicitamente um stream de arquivo usando `open()`. `open()` funciona assim como os construtores dos streams de arquivo — recebe um nome de arquivo e um modo de arquivo opcional.

Por exemplo:

```cpp
std::ofstream outf{ "Sample.txt" };
outf << "This is line 1\n";
outf << "This is line 2\n";
outf.close(); // fecha o arquivo explicitamente

// Ops, esquecemos algo
outf.open("Sample.txt", std::ios::app);
outf << "This is line 3\n";
outf.close();
```

Esse padrão é útil quando você precisa fechar e reabrir o mesmo arquivo durante a execução do programa — por exemplo, para mudar o modo de abertura — sem precisar criar um novo objeto de stream. 

## I/O aleatório de arquivos

### O ponteiro de arquivo

Cada classe de stream de arquivo contém um ponteiro de arquivo que é usado para rastrear a posição atual de leitura/escrita dentro do arquivo. 

Quando algo é lido ou escrito em um arquivo, a leitura/escrita acontece na localização atual do ponteiro de arquivo. Por padrão, ao abrir um arquivo para leitura ou escrita, o ponteiro de arquivo é posicionado no início do arquivo. 

Porém, se um arquivo é aberto em modo de acréscimo (append), o ponteiro de arquivo é movido para o final do arquivo, de forma que a escrita não sobrescreva nenhum conteúdo existente.

### Acesso aleatório a arquivos com seekg() e seekp()

Até agora, todo o acesso a arquivos que fizemos foi sequencial — ou seja, lemos ou escrevemos o conteúdo do arquivo em ordem. Porém, também é possível fazer acesso aleatório a arquivos — isto é, pular para vários pontos do arquivo para ler seu conteúdo. Isso pode ser útil quando seu arquivo está cheio de registros e você deseja recuperar um registro específico. Em vez de ler todos os registros até chegar ao que deseja, você pode pular diretamente para o registro que quer recuperar.

O acesso aleatório a arquivos é feito manipulando o ponteiro de arquivo usando as funções seekg() (para entrada) e seekp() (para saída). Caso você esteja se perguntando, o g vem de "get" e o p de "put". 

Para alguns tipos de streams, seekg() (alterar a posição de leitura) e seekp() (alterar a posição de escrita) operam de forma independente — porém, com streams de arquivo, a posição de leitura e escrita são sempre idênticas, então seekg e seekp podem ser usados de forma intercambiável.

As funções seekg() e seekp() recebem dois parâmetros. O primeiro é um offset que determina quantos bytes mover o ponteiro de arquivo. O segundo é uma flag ios que especifica a partir de onde o offset deve ser calculado.

Flag de seek ios
* beg -> O offset é relativo ao início do arquivo (padrão)
* cur -> O offset é relativo à posição atual do ponteiro de arquivo
* end -> O offset é relativo ao final do arquivo

Um offset positivo move o ponteiro de arquivo em direção ao final do arquivo, enquanto um offset negativo move o ponteiro em direção ao início do arquivo.

Ex.:

```
inf.seekg(14, std::ios::cur);  // avança 14 bytes
inf.seekg(-18, std::ios::cur); // recua 18 bytes
inf.seekg(22, std::ios::beg);  // vai para o 22º byte do arquivo
inf.seekg(24);                 // vai para o 24º byte do arquivo
inf.seekg(-28, std::ios::end); // vai para 28 bytes antes do fim do arquivo
```

Mover para início ou final do arquivo:

```
inf.seekg(0, std::ios::beg); // vai para o início do arquivo
inf.seekg(0, std::ios::end); // vai para o final do arquivo
```

> **Aviso:** Em um arquivo de texto, fazer seek para uma posição diferente do início do arquivo pode resultar em comportamento inesperado.
>
> Em programação, uma nova linha (`'\n'`) é na verdade uma abstração:
> - No **Windows**, uma nova linha é representada como os caracteres CR (carriage return) e LF (line feed) em sequência (ocupando 2 bytes).
> - No **Unix**, uma nova linha é representada apenas como LF (line feed) (ocupando 1 byte).
>
> Fazer seek além de uma nova linha em qualquer direção exige um número variável de bytes dependendo de como o arquivo foi codificado, o que significa que os resultados variarão dependendo da codificação usada.
>
> Além disso, em alguns sistemas operacionais, arquivos podem ser preenchidos com bytes zero no final. Fazer seek para o final do arquivo (ou a partir do final) produzirá resultados diferentes nesses arquivos.

Para ilustrar como funcionam, vamos fazer um exemplo usando `seekg()` e o arquivo de entrada que criamos na lição anterior. Esse arquivo tem a seguinte aparência:
```
This is line 1
This is line 2
This is line 3
This is line 4
```

Você pode obter um resultado diferente para a terceira linha dependendo de como seu arquivo está codificado.

