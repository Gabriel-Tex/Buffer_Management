#include <iostream>
#include <string>
#include <limits>
#include "../include/BufferManagement.h"

void CLS()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
    std::cout << "\nPressione Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// seleção de política
Policy selectPolicy()
{
    int option{};
    std::cout << " ====== GERENCIADOR DE BUFFER ====== \n";
    std::cout << "Selecione a política de substituição:\n\n";
    std::cout << " [1] LRU - Least Recently Used\n";
    std::cout << " [2] FIFO - First In First Out\n";
    std::cout << " [3] CLOCK - Clock\n";
    std::cout << " [4] MRU - Most Recently Used\n\n";
    std::cout << "Opção: ";

    std::cin >> option;

    switch (option)
    {
    case 1:
        return Policy::LRU;
    case 2:
        return Policy::FIFO;
    case 3:
        return Policy::CLOCK;
    case 4:
        return Policy::MRU;
    default:
        return Policy::LRU;
    }
}

std::string policyToString(Policy p)
{
    switch (p)
    {
    case Policy::LRU:
        return "LRU";
    case Policy::FIFO:
        return "FIFO";
    case Policy::CLOCK:
        return "CLOCK";
    case Policy::MRU:
        return "MRU";
    default:
        return "?";
    }
}

int main()
{
    // seleção de política
    CLS();
    Policy policy = selectPolicy();
    std::string policyName = policyToString(policy);
    BufferManagement buffer(policy);

    int option{-1};
    while (option != 0)
    {
        CLS();
        std::cout << " ====== GERENCIADOR DE BUFFER ====== \n";
        std::cout << "Política: " << policyName << "\n\n";

        std::cout << " [1] Fetch - Requisitar página\n";
        std::cout << " [2] Cache - Exibir estado do buffer\n";
        std::cout << " [3] Stats - Exibir cache hit/miss\n";
        std::cout << " [0] Sair\n\n";
        std::cout << "Opção: ";

        std::cin >> option;

        CLS();
        std::cout << " ====== GERENCIADOR DE BUFFER ====== \n";
        std::cout << "Política: " << policyName << "\n\n";

        switch (option)
        {
        case 1:
        {
            int key{};
            std::cout << "Digite a chave: ";
            std::cin >> key;

            std::string result = buffer.Fetch(key);
            std::cout << "Resultado: " << result << "\n";
            pause();
            break;
        }

        case 2:
        {
            buffer.DisplayCache();
            pause();
            break;
        }

        case 3:
        {
            buffer.DisplayStats();
            pause();
            break;
        }

        case 0:
        {
            CLS();
            std::cout << "Proframa finalizado.\n\n";
            break;
        }

        default:
        {
            std::cout << "Opção inválida.\n";
            pause();
            break;
        }
        }
    }

    return 0;
}