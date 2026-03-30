# BIBLIOGRAFIA: https://www.w3schools.com/cpp/cpp_deque.asp

## O que é uma Deque?

Uma **deque** (*double-ended queue* — fila de duas pontas) é uma estrutura de dados sequencial que permite inserir e remover elementos **tanto pela frente quanto pelo final**.

Isso a diferencia da fila simples (`queue`), que só aceita inserção no final e remoção pela frente.

```
fila (queue):        só entra pelo back, só sai pelo front
                     [ ] [ ] [ ] [ ]
                      ↑             ↑
                    front          back

deque:               entra e sai pelos dois lados
                     [ ] [ ] [ ] [ ]
                      ↑             ↑
                 push/pop_front   push/pop_back
```

Além disso, a deque permite **acesso direto por índice**, como um vetor — coisa que filas e pilhas não oferecem.

---

## Inclusão e Declaração

```cpp
#include <deque>

// Declara uma deque vazia de strings
deque<string> cars;

// Declara e inicializa ao mesmo tempo
deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};
```

> O tipo declarado (`string`, `int`, etc.) **não pode ser alterado** após a declaração.

---

## Acesso a Elementos

### Por índice — `[]` e `.at()`

```cpp
deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

cout << cars[0];      // Volvo (índice 0 = primeiro)
cout << cars[1];      // BMW

cout << cars.at(2);   // Ford
cout << cars.at(3);   // Mazda
```

**Diferença importante entre `[]` e `.at()`:**

| Operação | Índice fora do range | Uso recomendado |
|---|---|---|
| `cars[6]` | Comportamento indefinido (UB) | Quando tem certeza do índice |
| `cars.at(6)` | Lança exceção `std::out_of_range` | Quando quer segurança |

```cpp
// Exemplo de segurança com .at()
try {
    cout << cars.at(99); // lança exceção
} catch (const out_of_range& e) {
    cout << "Índice inválido!\n";
}
```

### Primeiro e último elemento — `.front()` e `.back()`

```cpp
deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

cout << cars.front(); // Volvo (primeiro)
cout << cars.back();  // Mazda (último)
```

---

## Modificação de Elementos

```cpp
deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

cars[0] = "Opel";       // via índice (sem verificação)
cars.at(0) = "Opel";    // via .at() (com verificação — preferível)

cout << cars[0];  // Opel
```

---

## Inserção de Elementos

```cpp
deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

cars.push_front("Tesla"); // insere no início
cars.push_back("VW");     // insere no final

// Estado atual:
// ["Tesla", "Volvo", "BMW", "Ford", "Mazda", "VW"]
```

---

## Remoção de Elementos

```cpp
deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

cars.pop_front(); // remove o primeiro → ["BMW", "Ford", "Mazda"]
cars.pop_back();  // remove o último  → ["BMW", "Ford"]
```

> Chamar `pop_front()` ou `pop_back()` em uma deque **vazia** causa comportamento indefinido. Sempre verifique com `.empty()` antes.

```cpp
if (!cars.empty()) cars.pop_front(); // seguro
```

---

## Tamanho e Estado

```cpp
deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

cout << cars.size();   // 4

// Verificar se está vazia
deque<string> empty;
cout << empty.empty(); // 1 (true)
cout << cars.empty();  // 0 (false)
```

---

## Iteração

### Com índice (estilo `for` clássico)

```cpp
deque<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

for (int i = 0; i < cars.size(); i++) {
    cout << cars[i] << "\n";
}
```

### Com range-based `for` (C++11 — mais limpo)

```cpp
for (string car : cars) {
    cout << car << "\n";
}
```

---

## Resumo de Operações

| Operação | Método | Complexidade |
|---|---|---|
| Acesso por índice | `[i]` / `.at(i)` | O(1) |
| Primeiro elemento | `.front()` | O(1) |
| Último elemento | `.back()` | O(1) |
| Inserir no início | `.push_front(val)` | O(1) |
| Inserir no final | `.push_back(val)` | O(1) |
| Remover do início | `.pop_front()` | O(1) |
| Remover do final | `.pop_back()` | O(1) |
| Tamanho | `.size()` | O(1) |
| Está vazia? | `.empty()` | O(1) |

---

## Quando usar `deque` vs outras estruturas?

| Estrutura | Use quando... |
|---|---|
| `vector` | Acesso por índice frequente, inserções só no final |
| `deque` | Inserções e remoções nos **dois extremos** com acesso por índice |
| `queue` | Só precisa de fila simples (FIFO), sem acesso por índice |
| `stack` | Só precisa de pilha (LIFO), sem acesso por índice |
| `list` | Inserções e remoções no **meio** com frequência |

---

