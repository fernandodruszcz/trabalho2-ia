#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <chrono>

extern "C" {
#include "tabuleiro.h"
}
using namespace std;

#define MAXSTR 512
#define LIN 9
#define COL 10 // decima coluna sempre tem um '\n'

#define QTDE_MAX_GANSOS 13
#define QTDE_MAX_BLOCOS 6
#define QTDE_MAX_NA_PAREDE 13

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

//char prox_jogada[MAXSTR] = {'\0'};

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

void jogadasPossiveisGanso(char* tab, vector <struct jogada> jogadas){

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

    //return jogadas;
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

void jogadasPossiveisRaposa(char *tab, vector <struct jogada> jogadas){

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

    //return jogadas;
}

//
//
//
float estimativa_raposa(char* tab){
    int qtde_gansos = 0;
    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'g')
                qtde_gansos++;
        }
    }
    //cout << qtde_gansos << endl;
    //cout << QTDE_MAX_GANSOS << endl;
    return (float)(qtde_gansos/QTDE_MAX_GANSOS);
}

//
//
//
float estimativa_ganso(char* tab){
    float na_parede = 0;
    float qtde_gansos = 0;
    float qtde_blocos = 0;
    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'g'){
                qtde_gansos++;
                if((tab[i*COL + j + 1] == 'g') &&
                   (tab[(i+1)*COL + j] == 'g') &&
                   (tab[(i+1)*COL + j + 1] == 'g'))
                   qtde_blocos += 1;
                if((tab[i*COL + j + 1] == '#') || (tab[i*COL + j + 1] == ' ') ||
                   (tab[i*COL + j - 1] == '#') || (tab[i*COL + j - 1] == ' ') ||
                   (tab[(i+1)*COL + j] == '#') || (tab[(i+1)*COL + j] == ' ') ||
                   (tab[(i-1)*COL + j] == '#') || (tab[(i-1)*COL + j] == ' '))
                   na_parede += 1;
            }

            if( (tab[i*COL + j] == 'r') && // situacao em que os gansos ganham
                ((tab[i*COL + j + 1] == '#') || (tab[i*COL + j + 1] == ' ') || (tab[i*COL + j + 1] == 'g') ) &&
                ((tab[i*COL + j - 1] == '#') || (tab[i*COL + j - 1] == ' ') || (tab[i*COL + j - 1] == 'g') ) &&
                ((tab[(i+1)*COL + j] == '#') || (tab[(i+1)*COL + j] == ' ') || (tab[(i+1)*COL + j] == 'g') ) &&
                ((tab[(i-1)*COL + j] == '#') || (tab[(i-1)*COL + j] == ' ') || (tab[(i-1)*COL + j] == 'g') ) )
                return 1;
        } // for j
    } // for i

    if(qtde_gansos <= 4) // situacao em que os gansos perdem
        return 0;

    float a1 = 0.33333;
    float a2 = 0.33333;
    float a3 = 0.33333;
    //cout << "Qtde ganso: " << qtde_gansos << " qtde blocos: " << qtde_blocos << " qtde parede: " << na_parede << endl;
    return (qtde_gansos/QTDE_MAX_GANSOS)*a1 + (qtde_blocos/QTDE_MAX_BLOCOS)*a2 + (na_parede/QTDE_MAX_NA_PAREDE)*a3;

}



char* updated_board(char *tab, jogada curr_j){
    char *tab_copia = new char[LIN*COL];
    copiaTabuileiro(tab, tab_copia);

    if(curr_j.tipo == 's'){
        for(auto i : curr_j.des){
            int x_des = i.first;
            int y_des = i.second;
            int gx, gy;
            gx = (curr_j.x_ori - x_des)/2;
            gy = (curr_j.y_ori - y_des)/2;
            tab_copia[(curr_j.x_ori + gx)*COL + (curr_j.y_ori + gy)] = '-';
        }

        tab_copia[(curr_j.x_ori) * COL + curr_j.y_ori] = '-';
        tab_copia[curr_j.des.back().first * COL + curr_j.des.back().second] = 'r';

    }else{
        int x_des = curr_j.des[0].first;
        int y_des = curr_j.des[0].second;
        char animal = tab_copia[curr_j.x_ori * COL + curr_j.y_ori];
        tab_copia[curr_j.x_ori * COL + curr_j.y_ori] = '-';
        tab_copia[x_des * COL + y_des] = animal;
    }

    return tab_copia;
}





float calcula_proxima_jogada(char *board, char turn, int depth){

    if(depth == 0){
        return estimativa_ganso(board);
    }
    else{
        vector<struct jogada> possiveis_jogadas;
        bool raposa = (turn == 'r');
        char next_turn;


        if(raposa){
            jogadasPossiveisRaposa(board, possiveis_jogadas);
            next_turn = 'g';
        }
        else{
            jogadasPossiveisGanso(board, possiveis_jogadas);
            next_turn = 'r';
        }

        float *estimativas = new float[possiveis_jogadas.size()];
        //float estimativas[13];
        for(int i = 0; i < possiveis_jogadas.size(); i++){
            estimativas[i] = calcula_proxima_jogada(updated_board(board, possiveis_jogadas[i]), next_turn, depth - 1);
        }
        if(raposa) return *max_element(estimativas, estimativas + possiveis_jogadas.size());
        else return *min_element(estimativas, estimativas + possiveis_jogadas.size());
    }

}

char* formata_proxima_jogada(char *tab, char *prox_jogada,char lado){
    vector<struct jogada> possiveis_jogadas;
    char next_turn;
    if(lado == 'r'){
        jogadasPossiveisRaposa(tab, possiveis_jogadas);
        next_turn = 'g';
    }else{
        jogadasPossiveisGanso(tab, possiveis_jogadas);
        next_turn = 'r';
    }

    float *estimativas = new float[possiveis_jogadas.size()];
    //float estimativas[13] =

    cout << possiveis_jogadas.size() << endl;
    for(int i = 0; i < possiveis_jogadas.size(); i++){
        cout << possiveis_jogadas[i].tipo;
        //estimativas[i] = calcula_proxima_jogada(updated_board(tab, possiveis_jogadas[i]), next_turn, 0);
        //cout << estimativas[i] << endl;

    }

    float highest = estimativas[0];
    int index = 0;
    for(int i = 1; i < possiveis_jogadas.size(); i++){
        if(lado == 'r'){
            if(estimativas[i] > highest){
                highest = estimativas[i];
                index = i;
            }
        }else{
            if(estimativas[i] < highest){
                highest = estimativas[i];
                index = i;
            }
        }

    }

    prox_jogada[0] = '\0';
    char *tipo_jogada;
    char x_inicial, y_inicial, lado_meu;
    char b1, b2;
    x_inicial = possiveis_jogadas[index].x_ori + '0';
    y_inicial = possiveis_jogadas[index].y_ori + '0';
    lado_meu = lado;


    //strcat(prox_jogada, &possiveis_jogadas[index].tipo);

    sprintf(prox_jogada, "%s %c", prox_jogada, lado);
    sprintf(prox_jogada, "%s %c", prox_jogada, possiveis_jogadas[index].tipo);
    sprintf(prox_jogada, "%s %c", prox_jogada, x_inicial);
    //cout << y_inicial << endl;
    sprintf(prox_jogada, "%s %c", prox_jogada, y_inicial);



    for(auto i : possiveis_jogadas[index].des){

        b1 = i.first + '0';
        b2 = i.second + '0';
        sprintf(prox_jogada, "%s %c", prox_jogada, b1);
        sprintf(prox_jogada, "%s %c", prox_jogada, b2);

    }
    return prox_jogada;
}


int main(int argc, char **argv){
    char buf[MAXSTR];
    char prox_jogada[MAXSTR];

    tabuleiro_conecta(argc, argv);
    while(1) {
      tabuleiro_recebe(buf);
      formata_proxima_jogada(buf, prox_jogada, *argv[1]);
      cout << prox_jogada << endl;
      tabuleiro_envia(prox_jogada);
    }

}

/*
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


    cout << formata_proxima_jogada(tab1, 'r') << endl;
    //vector<jogada> possiveis_jogadas = jogadasPossiveisRaposa(tab1);

    //for(int i = 0; i < possiveis_jogadas.size(); i++){
        //estimativas[i] = calcula_proxima_jogada(updated_board(board, possiveis_jogadas[i]), next_turn, depth - 1);
    //    cout << updated_board(tab1, possiveis_jogadas[i]) << endl;
    //}
    // cin.getline (linha, COL);

    // char tab2[MAXSTR];
    // desloc = 0;
    // for(int i = 0; i < LIN; i++){
    //     cin.getline (linha, COL);
    //     linha[COL - 1] = '\n';
    //     for(int j = 0; j < COL; j++){
    //         tab2[i*COL + j] = linha[j];
    //     }
    // }

    // tab2[90] = '\0';
    // cout << tab2;

    // if(ehSimetrico(tab1, tab2))
    //     cout << "É simétrico\n";
    // else
    //     cout << "Não é simétrico\n";


    // Jogadas possiveis

    // vector <struct jogada> jogadas = jogadasPossiveisGanso(tab1);
    // for(int i = 0; i < jogadas.size(); i++){
    //     cout << "g " << jogadas[i].tipo          << " "
    //                  << jogadas[i].x_ori         << " " << jogadas[i].y_ori          << " "
    //                  << jogadas[i].des[0].first << " " << jogadas[i].des[0].second << endl;
    // }
    // cout << endl;
    // jogadas = jogadasPossiveisRaposa(tab1);
    // for(int i = 0; i < jogadas.size(); i++){
    //     if(jogadas[i].tipo == 'm'){
    //         cout << "r " << jogadas[i].tipo         << " "
    //                      << jogadas[i].x_ori        << " " << jogadas[i].y_ori          << " "
    //                      << jogadas[i].des[0].first << " " << jogadas[i].des[0].second << endl;
    //     } else {
    //         cout << "r " << jogadas[i].tipo  << " " << jogadas[i].des.size() + 1 << " "
    //                      << jogadas[i].x_ori << " " << jogadas[i].y_ori << " ";
    //         for(int j = 0; j < jogadas[i].des.size(); j++)
    //             cout << jogadas[i].des[j].first << " " << jogadas[i].des[j].second << " ";
    //         cout << endl;
    //     }

    // }

    return 0;
}
*/
