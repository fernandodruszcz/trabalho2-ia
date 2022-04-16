#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <chrono>
using namespace std;

#define MAXSTR 512
#define LIN 9
#define COL 10 // decima coluna sempre tem um '\n'

struct jogada{
    char tipo;
    int x_ori;
    int y_ori;
    vector <pair<int, int>> des; // quando o tipo eh salto, pode haver mais de um destino (varios saltos)
};

char tabuleiro[MAXSTR] =
    "#########\n"
    "#  ggg  #\n"
    "#  ggg  #\n"
    "#ggggggg#\n"
    "#-------#\n"
    "#---r---#\n"
    "#  ---  #\n"
    "#  ---  #\n"
    "#########\n";


int ehSimetrico(char* tab1, char* tab2){
    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab1[i*COL + j] != tab2[i*COL + (COL - 2 - j)])
                return 0;
        }
    }
    return 1;
}

void copiaTabuileiro(char* original, char* copia){
    for(int i = 0; i < LIN * COL; i++){
        copia[i] = original[i];
    }
}

vector <struct jogada> jogadasPossiveisGanso(char* tab){
    vector <struct jogada> jogadas;
    struct jogada jogada;

    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'g'){
                jogada.x_ori = i;
                jogada.y_ori = j;
                jogada.tipo = 'm';

                // verifica todos os lados

                // cima
                if(tab[(i-1)*COL + j] == '-'){
                    jogada.des.push_back(make_pair(i - 1, j));
                    jogadas.push_back(jogada);
                    jogada.des.clear(); // limpa o vetor de destino para as proximas atribuicoes
                }

                // baixo
                if(tab[(i+1)*COL + j] == '-'){
                    jogada.des.push_back(make_pair(i + 1, j));
                    jogadas.push_back(jogada);
                    jogada.des.clear();
                }

                // esquerda
                if(tab[i*COL + j - 1] == '-'){
                    jogada.des.push_back(make_pair(i, j - 1));
                    jogadas.push_back(jogada);
                    jogada.des.clear();
                }

                // direita
                if(tab[i*COL + j + 1] == '-'){
                    jogada.des.push_back(make_pair(i, j + 1));
                    jogadas.push_back(jogada);
                    jogada.des.clear();
                }

            } // if == 'g'
        } // for j
    } // for i

    return jogadas;
}

//
// Funcao para atribuir todos os possiveis saltos da raposa
//
// A cada chamada eh criado um novo tabuleiro movendo a raposa e eliminando o ganso
// 
void insereSaltos(char *tab, int x_ori, int y_ori, int i, int j, vector <struct jogada>* jogadas, vector <pair <int, int>> caminho){
    struct jogada jogada;
    jogada.x_ori = x_ori;
    jogada.y_ori = y_ori;
    jogada.tipo = 's';

    // for(int k = 0; k < caminho.size(); k++)
    //     cout << caminho[k].first << "," << caminho[k].second << " ";
    // cout << endl;
    // cout << tab;

    // salta para cima
    if( (tab[(i-1)*COL + j] == 'g') && (tab[(i-2)*COL + j] == '-') ){
        // cria uma copia do tabuleiro recebido para operar em cima
        char tab_copia[LIN*COL];
        copiaTabuileiro(tab, tab_copia);

        // adiciona movimento ao caminho
        caminho.push_back(make_pair(i - 2, j));
        jogada.des = caminho;
        (*jogadas).push_back(jogada);

        // atualiza copia para proximas atribuicoes
        tab_copia[(i-1)*COL + j] = '-';
        tab_copia[(i-2)*COL + j] = 'r';
        tab_copia[i*COL + j]     = '-';

        // recursao
        insereSaltos(tab_copia, x_ori, y_ori, i - 2, j, jogadas, caminho);
        caminho.pop_back();
        
    }

    // salta para baixo
    if( (tab[(i+1)*COL + j] == 'g') && (tab[(i+2)*COL + j] == '-') ){
        char tab_copia[LIN*COL];
        copiaTabuileiro(tab, tab_copia);

        caminho.push_back(make_pair(i + 2, j));
        jogada.des = caminho;
        (*jogadas).push_back(jogada);

        tab_copia[(i+1)*COL + j] = '-';
        tab_copia[(i+2)*COL + j] = 'r';
        tab_copia[i*COL + j]     = '-';

        insereSaltos(tab_copia, x_ori, y_ori, i + 2, j, jogadas, caminho);
        caminho.pop_back();
    }

    // salta para a esquerda
    if( (tab[i*COL + j - 1] == 'g') && (tab[i*COL + j - 2] == '-') ){
        char tab_copia[LIN*COL];
        copiaTabuileiro(tab, tab_copia);

        caminho.push_back(make_pair(i, j - 2));
        jogada.des = caminho;
        (*jogadas).push_back(jogada);

        tab_copia[i*COL + j - 1] = '-';
        tab_copia[i*COL + j - 2] = 'r';
        tab_copia[i*COL + j]     = '-';

        insereSaltos(tab_copia, x_ori, y_ori, i, j - 2, jogadas, caminho);
        caminho.pop_back();
    }

    // salta para a direita
    if( (tab[i*COL + j + 1] == 'g') && (tab[i*COL + j + 2] == '-') ){
        char tab_copia[LIN*COL];
        copiaTabuileiro(tab, tab_copia);

        caminho.push_back(make_pair(i, j + 2));
        jogada.des = caminho;
        (*jogadas).push_back(jogada);

        tab_copia[i*COL + j + 1] = '-';
        tab_copia[i*COL + j + 2] = 'r';
        tab_copia[i*COL + j]     = '-';

        insereSaltos(tab_copia, x_ori, y_ori, i, j + 2, jogadas, caminho);
        caminho.pop_back();
    }

}

vector <struct jogada> jogadasPossiveisRaposa(char *tab){
    vector <struct jogada> jogadas;
    struct jogada jogada;

    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'r'){
                jogada.x_ori = i;
                jogada.y_ori = j;
                jogada.tipo = 'm';

                // verifica todos os lados
                // diferentemente dos gansos, devemos verificar saltos

                // cima
                if(tab[(i-1)*COL + j] == '-'){
                    jogada.des.push_back(make_pair(i - 1, j));
                    jogadas.push_back(jogada);
                    jogada.des.clear();
                } 

                // baixo
                if(tab[(i+1)*COL + j] == '-'){
                    jogada.des.push_back(make_pair(i + 1, j));
                    jogadas.push_back(jogada);
                    jogada.des.clear();
                } 

                // esquerda
                if(tab[i*COL + j - 1] == '-'){
                    jogada.des.push_back(make_pair(i, j - 1));
                    jogadas.push_back(jogada);
                    jogada.des.clear();
                } 

                // direita
                if(tab[i*COL + j + 1] == '-'){
                    jogada.des.push_back(make_pair(i, j + 1));
                    jogadas.push_back(jogada);
                    jogada.des.clear();
                } 

                vector <pair <int, int>> caminho;
                insereSaltos(tab, i, j, i, j, &jogadas, caminho);

            } // if == 'r'
        } // for j
    } // for i

    return jogadas;
}


//
// Le dois tabuleiros, verifica se sao simetricos horizontalmente e calcula os movimentos possiveis no primeiro tabuleiro
//
int main(){
    char tab1[MAXSTR];
    int desloc = 0;
    char linha[COL];
    for(int i = 0; i < LIN; i++){
        cin.getline (linha, COL);
        linha[COL - 1] = '\n';
        for(int j = 0; j < COL; j++){
            tab1[i*COL + j] = linha[j];
        }
    }

    tab1[90] = '\0';
    cout << tab1;

    cin.getline (linha, COL);

    char tab2[MAXSTR];
    desloc = 0;
    for(int i = 0; i < LIN; i++){
        cin.getline (linha, COL);
        linha[COL - 1] = '\n';
        for(int j = 0; j < COL; j++){
            tab2[i*COL + j] = linha[j];
        }
    }

    tab2[90] = '\0';
    cout << tab2;

    if(ehSimetrico(tab1, tab2))
        cout << "É simétrico\n";
    else
        cout << "Não é simétrico\n";


    // Jogadas possiveis

    vector <struct jogada> jogadas = jogadasPossiveisGanso(tab1);
    for(int i = 0; i < jogadas.size(); i++){
        cout << "g " << jogadas[i].tipo          << " "
                     << jogadas[i].x_ori         << " " << jogadas[i].y_ori          << " "
                     << jogadas[i].des[0].first << " " << jogadas[i].des[0].second << endl;
    }
    cout << endl;
    jogadas = jogadasPossiveisRaposa(tab1);
    for(int i = 0; i < jogadas.size(); i++){
        if(jogadas[i].tipo == 'm'){
            cout << "r " << jogadas[i].tipo         << " "
                         << jogadas[i].x_ori        << " " << jogadas[i].y_ori          << " "
                         << jogadas[i].des[0].first << " " << jogadas[i].des[0].second << endl;
        } else {
            cout << "r " << jogadas[i].tipo  << " " << jogadas[i].des.size() + 1 << " "
                         << jogadas[i].x_ori << " " << jogadas[i].y_ori << " ";
            for(int j = 0; j < jogadas[i].des.size(); j++)
                cout << jogadas[i].des[j].first << " " << jogadas[i].des[j].second << " ";
            cout << endl;
        }
        
    }

    return 0;
}