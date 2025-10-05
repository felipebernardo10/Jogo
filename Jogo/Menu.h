#pragma once

#include <iostream>
#include <cstdlib> // para system("clear") e system("aplay")

using namespace std;

class Menu {
public:
    int exibir() {
        int opcao = 0;

        while (true) {
            system("clear");
            cout << "==============================\n"<< endl;
            cout << "        FLAPPY FACE 🐤        \n";
            cout << "==============================\n";
            cout << "1 - Iniciar jogo\n";
            cout << "2 - Ver recorde\n";
            cout << "3 - Créditos\n";
            cout << "==============================\n";
            cout << "Escolha uma opcao: ";
            cin >> opcao;

            if (opcao >= 1 && opcao <= 3) break;

            cout << "\nOpcao invalida! Tente novamente.\n";
            system("sleep 1");
        }

        return opcao;
    }

    void mostrarRecorde() {
        system("clear");
        cout << "===== RECORDE =====\n";
        // Aqui você pode ler o recorde do arquivo se quiser
        system("cat record.txt");
        cout << "\n=====================\n";
        cout << "\nPressione Enter para voltar...";
        cin.ignore();
        cin.get();
    }

    void mostrarCreditos() {
        system("clear");
        cout << "===== CRÉDITOS =====\n";
        cout << "Desenvolvido por: Felipe Bernardo, Habner Gabriel, Arthur Bruno\n";
        cout << "Versão: 1.0\n";
        cout << "=====================\n";
        cout << "\nPressione Enter para voltar...";
        cin.ignore();
        cin.get();
    }
};

