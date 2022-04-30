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
#define MAX_DEPTH 4

struct jogada{
    char tipo;
    int x_ori;
    int y_ori;
    vector <pair<int, int>> des; // quando o tipo eh salto, pode haver mais de um destino (varios saltos)
};

float max_dist = 1.0;
float max_jogadas_raposa = 1.0;
float max_dist_intra = 1.0;


int ehSimetrico(char* tab1, char* tab2){
    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab1[i*COL + j] != tab2[i*COL + (COL - 2 - j)])
                return 0;
        }
    }
    return 1;
}

// faz uma cópia do tabuleiro original
void copiaTabuileiro(char* original, char* copia){
    for(int i = 0; i < LIN * COL; i++){
        copia[i] = original[i];
    }
}

// Funcao que atribui todas as próximos jogadas possiveis
// dos gansos dado um estado do tabuleiro.
vector <struct jogada> jogadasPossiveisGanso(char* tab){
    vector <struct jogada> jogadas;
    struct jogada jogada;

    for(int i = 0; i < LIN - 1; i++){
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

// Funcao para atribuir todos os possiveis saltos da raposa
// A cada chamada eh criado um novo tabuleiro movendo a raposa e eliminando o ganso
void insereSaltos(char *tab, int x_ori, int y_ori, int i, int j, vector <struct jogada>* jogadas, vector <pair <int, int>> caminho){
    struct jogada jogada;
    jogada.x_ori = x_ori;
    jogada.y_ori = y_ori;
    jogada.tipo = 's';


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

// retorna as possiveis jogadas da raposa
vector<struct jogada> jogadasPossiveisRaposa(char *tab){
    vector<struct jogada> jogadas;
    int x_ori, y_ori;
    struct jogada jogada;

    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'r'){
                x_ori = i;
                y_ori = j;

            }
        }
    }

    jogada.x_ori = x_ori;
    jogada.y_ori = y_ori;
    jogada.tipo = 'm';

    // verifica todos os lados
    // diferentemente dos gansos, devemos verificar saltos

    // cima
    if(tab[(x_ori-1)*COL + y_ori] == '-'){
        jogada.des.push_back(make_pair(x_ori - 1, y_ori));
        jogadas.push_back(jogada);
        jogada.des.clear();
    }

    // baixo
    if(tab[(x_ori+1)*COL + y_ori] == '-'){
        jogada.des.push_back(make_pair(x_ori + 1, y_ori));
        jogadas.push_back(jogada);
        jogada.des.clear();
    }

    // esquerda
    if(tab[(x_ori*COL) + (y_ori - 1)] == '-'){
        jogada.des.push_back(make_pair(x_ori, y_ori - 1));
        jogadas.push_back(jogada);
        jogada.des.clear();
    }

    // direita
    if(tab[(x_ori*COL) + y_ori + 1] == '-'){
        jogada.des.push_back(make_pair(x_ori, y_ori + 1));
        jogadas.push_back(jogada);
        jogada.des.clear();
    }

    vector <pair <int, int>> caminho;
    vector<struct jogada> saltos;
    insereSaltos(tab, x_ori, y_ori, x_ori, y_ori,&saltos, caminho);

    for(auto j : saltos){
        jogadas.push_back(j);
    }

    return jogadas;
}

// retorna a media da distancia da raposa para todos os gansos
float distancia_media(char *tab){
    int x_raposa, y_raposa;
    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'r'){
                x_raposa = i;
                y_raposa = j;
            }
        }
    }
    float soma = 0;
    float qnt_ganso = 0;
    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'g'){
                soma += abs(x_raposa - i) + abs(y_raposa - j);
                qnt_ganso += 1;
            }
        }
    }

    return soma/qnt_ganso;
}


// retorna a media das distancias de todos os gansos
// para todos os outros gansos
float distancia_intra_gansos(char *tab){
    vector<pair<int,int>> gansos_pos;
    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'g'){
                gansos_pos.push_back(make_pair(i,j));
            }
        }
    }
    float soma = 0;
    for(auto g : gansos_pos){
        for(int i = 0; i < LIN; i++){
            for(int j = 0; j < COL - 2; j++){
                if(tab[i*COL + j] == 'g'){
                    soma += abs(g.first - i) + abs(g.second - j);

                }
            }
        }
    }
    float denominador = 1/(gansos_pos.size()*(gansos_pos.size() - 1));

    return soma * denominador;
}


// estimativa do estado do tabuleiro para a raposa
float estimativa_raposa(char* tab){
    float qtde_gansos = 0;
    for(int i = 0; i < LIN; i++){
        for(int j = 0; j < COL - 2; j++){
            if(tab[i*COL + j] == 'g')
                qtde_gansos++;
        }
    }


    float a1 = 0.5;
    float a2 = 0.5;
    float a3 = 0.25;

    vector<struct jogada> jogadas_raposa;
    jogadas_raposa = jogadasPossiveisRaposa(tab);
    float qnt_jogadas_raposa = (float)jogadas_raposa.size();
    float pulos = 0;

    for(auto jog : jogadas_raposa){
        if(jog.tipo == 's') pulos += 1.0;
    }

    float distancia = distancia_media(tab);

    if(distancia > max_dist) max_dist = distancia;

    return (((pulos > 0) * a1) - ((distancia/max_dist) * a2));
}

// estimativa do estado do tabuleiro para os gansos
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


    float a1 = 0.25;
    float a2 = 0.25;
    float a3 = 0.5;
    vector<struct jogada> jogadas_raposa;
    jogadas_raposa = jogadasPossiveisRaposa(tab);
    float qnt_jogadas_raposa = (float)jogadas_raposa.size();

    float pulos = 0;
    for(auto jog : jogadas_raposa){
        if(jog.tipo == 's') pulos += 1.0;
    }

    if(pulos > 0) return 1;

    if(qnt_jogadas_raposa > max_jogadas_raposa) max_jogadas_raposa = qnt_jogadas_raposa;

    float distancia_intra = distancia_intra_gansos(tab);
    if(distancia_intra > max_dist_intra) max_dist_intra = distancia_intra;

    return  (((qnt_jogadas_raposa/max_jogadas_raposa)*a1) -
            ((distancia_intra/max_dist_intra) * a2) +
            ((distancia_media(tab)/max_dist) * a3)
        );




}

// tabuleiro atualizado com uma jogada 'curr_j'
char* updated_board(char *tab, jogada curr_j, char *tab_copia){

    strcpy( tab_copia, tab);

    if(curr_j.tipo == 's'){
        tab_copia[(curr_j.x_ori) * COL + curr_j.y_ori] = '-';
        for(auto i : curr_j.des){
            int x_des = i.first;
            int y_des = i.second;
            int gx, gy;
            gx = (curr_j.x_ori - x_des)/2;
            gy = (curr_j.y_ori - y_des)/2;


            tab_copia[(curr_j.x_ori - gx)*COL + (curr_j.y_ori - gy)] = '-';
            curr_j.x_ori = x_des;
            curr_j.y_ori = y_des;
        }

        tab_copia[curr_j.des.back().first * COL + curr_j.des.back().second] = 'r';

    }else{
        int x_des = curr_j.des[0].first;
        int y_des = curr_j.des[0].second;
        char animal = tab_copia[curr_j.x_ori * COL + curr_j.y_ori];
        tab_copia[curr_j.x_ori * COL + curr_j.y_ori] = '-';
        tab_copia[x_des * COL + y_des] = animal;
    }

}

// calcula o valor da arvore de jogadas, implementa o MIN-MAX
float calcula_proxima_jogada(char *board, char turn, int depth, char lado_original){

    if(depth == 0){
        return(estimativa_raposa(board) + estimativa_ganso(board));
    }
    else{
        vector<struct jogada> possiveis_jogadas;
        bool raposa = (turn == 'r');
        char next_turn;


        if(raposa){
            possiveis_jogadas = jogadasPossiveisRaposa(board);
            next_turn = 'g';
        }
        else{
            possiveis_jogadas = jogadasPossiveisGanso(board);
            next_turn = 'r';
        }
        float *estimativas = new float[possiveis_jogadas.size()];
        jogada curr_j;
        char new_board[MAXSTR];
        for(int i = 0; i < possiveis_jogadas.size(); i++){
            curr_j = possiveis_jogadas[i];
            updated_board(board, curr_j, new_board);
            estimativas[i] = calcula_proxima_jogada(board, next_turn, depth - 1, lado_original);
        }
        if(raposa) return *max_element(estimativas, estimativas + possiveis_jogadas.size());
        else return *min_element(estimativas, estimativas + possiveis_jogadas.size());
    }

}

// calcula a melhor jogada de acordo com o algoritmo MIN-MAX e a heuristica
// e formata no padrão de jogadas especificados retornando uma string ex: 'r m 4 4 4 3'
char* formata_proxima_jogada(char *tab, char *prox_jogada, int max_depth, char lado){

    int ind = 0;
    while(tab[ind] != '#'){
        ind++;
    }
    for(int i = 0; i < LIN * COL; i++){
        tab[i] = tab[i + ind];
    }

    vector<struct jogada> possiveis_jogadas;
    char next_turn;
    if(lado == 'r'){
        possiveis_jogadas = jogadasPossiveisRaposa(tab);
        next_turn = 'g';
    }else{
        possiveis_jogadas = jogadasPossiveisGanso(tab);
        next_turn = 'r';
    }

    float *estimativas = new float[possiveis_jogadas.size()];
    char new_board[MAXSTR];

    for(int i = 0; i < possiveis_jogadas.size(); i++){
        updated_board(tab, possiveis_jogadas[i], new_board);

        estimativas[i] = calcula_proxima_jogada(new_board, next_turn, max_depth, lado);
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
        if(estimativas[i] == highest){
            index = rand() % possiveis_jogadas.size();
        }

    }

    prox_jogada[0] = '\0';
    char *tipo_jogada;
    char x_inicial, y_inicial, lado_meu;
    char b1, b2;
    x_inicial = possiveis_jogadas[index].x_ori + '0';
    y_inicial = possiveis_jogadas[index].y_ori + '0';
    lado_meu = lado;



    sprintf(prox_jogada, "%s%c", prox_jogada, lado);
    sprintf(prox_jogada, "%s %c", prox_jogada, possiveis_jogadas[index].tipo);
    if(possiveis_jogadas[index].tipo == 's'){
        char n_tuplas = (possiveis_jogadas[index].des.size() + 1) + '0';
        sprintf(prox_jogada,"%s %c", prox_jogada, n_tuplas);
    }
    sprintf(prox_jogada, "%s %c", prox_jogada, x_inicial);
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
      formata_proxima_jogada(buf, prox_jogada, MAX_DEPTH, *argv[1]);
      tabuleiro_envia(prox_jogada);
    }

}
