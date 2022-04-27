
int ehSimetrico(char* tab1, char* tab2);
void copiaTabuileiro(char* original, char* copia);
std::vector <struct jogada> jogadasPossiveisGanso(char* tab);
void insereSaltos(char *tab, int x_ori, int y_ori, int i, int j, std::vector <struct jogada>* jogadas, std::vector <std::pair <int, int>> caminho);
std::vector <struct jogada> jogadasPossiveisRaposa(char *tab);
float estimativa_raposa(char* tab);
float estimativa_ganso(char* tab);
char* updated_board(char *tab, jogada curr_j);
float calcula_proxima_jogada(char *board, char turn, int depth);
char* formata_proxima_jogada(char *tab, char lado);
