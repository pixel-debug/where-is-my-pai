

int IDtexturas[3]; //serão usadas 10 texturas(9 sprites personargens e 1 fundo)
char *enderecotexturas[] = {
    "fundo.png"
    "sheet_protagonista.png"
    "sheet_vilao_1.png"
    "explosões variadas.png"
    "dados_de_jogo.png"
    "singaro1.png"
};
typedef struct{
    float x;
    float y;
}coordenada;
// estrutura e variáveis para El Menu
struct ponto {
    float x, y;
};

int start = 0;
int creditos = 0;
int howtopray = 0;
int volta = 1;
int chave = 0;

struct ponto posicaoMouse;
// foi para corrigir um bug, não entendemos o por quê que funcionou assim, mas tem toda uma logica por trás
const int alt_personagens = 24;
const int larg_personagens = 20;
const float alt_cabecc = 0.11;
const float alt_larg_sheet = 0.25;

const int larg_carro = 30;
const int alt_carro = 50;

const int larg_corvo = 25;
const int alt_corvo = 25;
const float alt_larg_corvo = 0.25;
// quando acionada a tecla r este seão os valores default
const int xvilaodefault[16] = {-78, 72, -57, 47, 27, -37, -16, 7,
                80, -72, -47, 57, -27, 37, -7, 16};
const int yvilaodefault[16] = {60, 60, 60, 60, 60, 60, 60, 60,
                 30, 30, 30, 30, 30, 30, 30, 30};

int vidaBoto = 3;


int controle_velocidade_vilao;
int controle_velocidade_prota;
int controle_velocidade_onibus = 0;
int controle_velocidade_corvo = 0;

int contador_de_viloes = 16;
int viloes_aleatorios[16];

int tiro = 0; // usamos como boolean
int xtiroProta = 150; //coordenada x do tiro do protagonista
int ytiroProta= -80; // coordenada y do tiro do protagonista
int atingiu[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; 
int sai[16] = {0};

//corte do sheet para o prota andar com base nas coordenadas do sheet
int passo = 1; 
int sentido = 0; 
float xpasso[4] = {0, 0.245, 0.5, 0.75};
float ysentido[4] = {0.245, 0.495 , 0.745, 1};

int modo = 1;

int passo_vilao[16] = {0};
int sentido_vilao[16] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
float xpasso_vilao[4] = {0, 0.245, 0.5, 0.75};
float ysentido_vilao[4] = {0.245, 0.495 , 0.745, 1};
int xtiroVilao[3] = {140, 140, 140};
float ytiroVilao[3] = {-100, -100, -100};
int vidas = 3;
int pontos = 0;
// vai brotar no meio da tela o maluquinho
coordenada protago = {0, -76};

//posição dos viloes na tela
float xvilao[16] = {120, 123, 126, 129, 132, 135, 138, 141,
                80, -72, -47, 57, -27, 37, -7, 16};
float yvilao[16] = {80, 80, 80, 80, 80, 80, 80, 80,
                 30, 30, 30, 30, 30, 30, 30, 30};

int inicializacao = 16;//16
int EmPosicao[16] = {0};
int carroinicial = 0;

int corvox = 95;
int corvoy = 80;
int xbate = 0;
int ybate = 1;
float xasa[4] = {0, 0.245, 0.5, 0.75};
float yasa[4] = {0.245, 0.495 , 0.745, 1};

coordenada itemglobal = {-159, -159};
int modoitem;
int modotiro = 0; // 0
int quantidadeDeTiro = 0;

int ladotiro = 0;

int xcarro = -250;
int ycarro;

coordenada sheet = {0,1};
const coordenada dimensoesBoto = {20,40};
coordenada posicaoBoto = {0,40};
int passoBoto = 0;
int controle = 0;

int pausar = 1;

int clique_sim = 0;
int clique_nao = 0;
int saida = 1;
int refresh = 1;

Mix_Chunk* somTiro;
Mix_Music* musicMenu;
Mix_Music* musicGameOver;
Mix_Music* musicWin;
                                  /*---------------------------------------------------
                                                    carregando texturas
                                  ----------------------------------------------------*/

GLuint idBoto;                                  
GLuint idProta[4];
GLuint idVilao[6];
GLuint idFundo1;
GLuint idtiro1[3];
GLuint idmenuzin;
GLuint idPensao;
GLuint idLose;
GLuint idcaixa[3];
GLuint idCarro;
GLuint idboleto;
GLuint idCorvo;
GLuint idConfirmacao;
GLuint idMenu;
GLuint idCreditos;
GLuint idHowtopray;
GLuint idLoseInvertido;

void iniciarJogador();
void inicializafundos();
void inicializatiro();
void inicializaVilao();
void iniciarJogador();
