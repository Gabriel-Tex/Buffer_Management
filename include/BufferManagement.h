#ifndef BUFFERMANAGEMENT_H
#define BUFFERMANAGEMENT_H
#include <vector>
#include <string>
#include <deque>
#include "Page.h"
#define MAX_SIZE 5

enum class Policy { LRU, FIFO, CLOCK, MRU };

class BufferManagement{
    private:
        std::vector<Page> pool {};
        int cacheHit {};
        int cacheMiss {};
        Policy policy {};       
        int circularPtr {};      

        std::deque<int> fifoQueue; 
        std::deque<int> lruQueue; 
        std::deque<int> mruStack;  

        bool isFull() const;
        bool randomDirty();
        std::string Read(int key);  

    public:
        
        BufferManagement(Policy p);

/*
        ============ FUNÇÃO FETCH: ============
        * Busca a informação de uma determinada entrada, ou seja, a 
        página de texto. 
        * key -> page#. 
        * Esta função consulta primeiro o buffer.
            * Caso encontre a entrada, retorna o conteúdo 
            (linha de texto) nela contido.
            * Caso contrário, busca a página no arquivo e transfere 
            seu conteúdo para o buffer; 
*/
        std::string Fetch(int key);

/*
        ============ FUNÇÃO EVICT: ============
        * Seleciona uma entrada do buffer e a remove. 
        * A escolha de qual entrada remover deve ser feita conforme 
        a política de substituição. 
        * Durante a execução da função, deve ser exibida a 
        página removida do buffer e, caso a variável de atualização
        seja TRUE, deve ser exibido o texto “W” após o conteúdo da 
        página.
*/
        void Evict();

        /*
        ============ FUNÇÃO DISPLAYCACHE: ============
        * Exibe todas as entradas presentes no buffer 
        (Chave − > Número da linha, Valor − > Texto da linha, 
        Atualização − > valor booleano)
*/
        void DisplayCache() const;

/*
        ============ FUNÇÃO DISPLAYSTATUS: ============
        * Exibe informações sobre cache hit e cache miss
*/
        void DisplayStats() const;
        
        // ============ GETTERS E ATT's ============

        int getCacheHit() const{
                return cacheHit;
        }

        void attCacheHit() {
                ++cacheHit;
        }

        int getCacheMiss() const{
                return cacheMiss;
        }

        void attCacheMiss() {
                ++cacheMiss;
        }

};

#endif