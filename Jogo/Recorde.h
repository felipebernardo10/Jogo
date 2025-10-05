#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Recorde {
private:
    string nome;
    string arquivo;
    int pontos;

public:
    Recorde(string nomeArquivo = "record.txt") {
        arquivo = nomeArquivo;
        nome = "Ninguém";
        carregar();
    }

    void carregar() {
        ifstream file(arquivo);
        if (file.is_open()) {
            file >> pontos;
            file.ignore();
            getline(file, nome);
            file.close();
        }
    }

    void salvar() {
        ofstream file(arquivo);
        if (file.is_open()) {
            file << pontos << endl;
            file << nome << endl;
            file.close();
        }
    }

    void mostrar() const {
        cout << "Recorde atual: " << pontos << " (" << nome << ")" << endl;
    }

    void verificarNovoRecorde(int score) {
        cout << "\nSua pontuação: " << score << endl;
        if (score > pontos) {
            cout << "🎉 Parabéns! Você bateu o recorde! 🎉" << endl;
            cout << "Digite seu nome: ";
            string novoNome;
            getline(cin, novoNome);

            pontos = score;
            nome = novoNome;
            salvar();
            cout << "Novo recorde salvo com sucesso!" << endl;
        } else {
            cout << "Você não bateu o recorde. Tente novamente!" << endl;
        }
    }

    
    int getPontos() const {
        return pontos;
    }
};
