#include "../include/BufferManagement.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

BufferManagement::BufferManagement(Policy p)
    : pool(MAX_SIZE), cacheHit{0}, cacheMiss{0},
      policy{p}, circularPtr{0}
{
    std::srand(std::time(nullptr));
}

bool BufferManagement::isFull() const
{
    for (int i = 0; i < MAX_SIZE; ++i)
        if (pool[i].isEmpty())
            return false;
    return true;
}

bool BufferManagement::randomDirty()
{
    return std::rand() % 2 == 0;
}

std::string BufferManagement::Read(int key)
{
    std::ifstream inf{"../database/bancodedados.csv"};
    if (!inf)
    {
        std::cerr << "Erro: arquivo não pôde ser aberto.\n";
        return "";
    }

    std::string line;
    int count = 0;
    while (std::getline(inf, line))
    {
        if (count == key)
            return line;
        ++count;
    }
    return "";
}

static void removeFromDeque(std::deque<int> &queue, int index)
{
    queue.erase(std::remove(queue.begin(), queue.end(), index), queue.end());
}

std::string BufferManagement::Fetch(int key)
{
    for (int i = 0; i < MAX_SIZE; ++i)
    {

        // a página está na buffer pool
        if (!pool[i].isEmpty() && pool[i].getKey() == key)
        {
            attCacheHit();

            // atualizando dados para cada política após requisição de página
            if (policy == Policy::LRU)
            {
                // remove índice recém requisitado e coloca no final
                removeFromDeque(lruQueue, i);
                lruQueue.push_back(i);
            }

            else if (policy == Policy::MRU)
            {
                // remove índice recém requisitado e coloca no final
                removeFromDeque(mruStack, i);
                mruStack.push_back(i);
            }

            else if (policy == Policy::CLOCK)
            {
                // segunda chance
                pool[i].referenceBit = true;
            }

            return pool[i].getContent();
        }
    }

    // página não está na buffer pool
    attCacheMiss();

    // busca a página solicitada no banco de dados
    std::string content = Read(key);
    if (content == "")
        return "Página não encontrada";

    // caso o buffer esteja cheio, remover uma página segundo as políticas
    if (isFull())
        Evict();

    // inserindo página no primeiro quadro vazio
    for (int i = 0; i < MAX_SIZE; ++i)
    {
        if (pool[i].isEmpty())
        {
            pool[i].setKey(key);
            pool[i].setContent(content);
            pool[i].setDirty(randomDirty());
            pool[i].referenceBit = true;

            if (policy == Policy::FIFO)
                fifoQueue.push_back(i);

            else if (policy == Policy::LRU)
                lruQueue.push_back(i);

            else if (policy == Policy::MRU)
                mruStack.push_back(i);
            break;
        }
    }

    return content;
}

void BufferManagement::Evict()
{

    // indíce de página que será removida da buffer pool
    int index = -1;

    // selecionando índice com base na política
    if (policy == Policy::FIFO)
    {
        // pega o índice mais antigo (da frente) e remove da fila
        index = fifoQueue.front();
        fifoQueue.pop_front();
    }

    else if (policy == Policy::LRU)
    {
        // pega o índice que foi requisitado a mais tempo (da frente) e remove da fila
        index = lruQueue.front();
        lruQueue.pop_front();
    }

    else if (policy == Policy::MRU)
    {
        // pega o índice que foi recém requisitado (último) e remove da fila
        index = mruStack.back();
        mruStack.pop_back();
    }

    else if (policy == Policy::CLOCK)
    {
        while (true)
        {
            // se o bit de referência for 0, a página será removida
            if (!pool[circularPtr].referenceBit)
            {
                index = circularPtr;
                circularPtr = (circularPtr + 1) % MAX_SIZE;
                break;
            }

            // segunda chance da página
            pool[circularPtr].referenceBit = false;
            circularPtr = (circularPtr + 1) % MAX_SIZE;
        }
    }

    std::cout << "Removido: [" << pool[index].getKey() << "] "
              << pool[index].getContent()
              << (pool[index].isDirty() ? " [W] " : "") << "\n";

    // removendo página do frame correspondente ao index
    pool[index].clearFrame();
}

void BufferManagement::DisplayCache() const
{
    std::cout << "=== Cache ===\n";

    for (int i = 0; i < MAX_SIZE; ++i)
    {
        if (!pool[i].isEmpty())
        {
            std::cout << "  Chave: " << pool[i].getKey()
                      << " | Dirty: " << (pool[i].isDirty() ? "true" : "false")
                      << " | Conteúdo: " << pool[i].getContent() << "\n";
        }
    }
}

void BufferManagement::DisplayStats() const
{
    std::cout << "=== Stats ===\n";
    std::cout << " Cache Hit: " << cacheHit << "\n";
    std::cout << " Cache Miss: " << cacheMiss << "\n";
}