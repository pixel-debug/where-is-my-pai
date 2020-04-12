/*---------------------------------------------------
gcc galaxian.c  -lGL -lglut -lGLEW -lGLU -lSOIL -lm
./a.out
----------------------------------------------------*/



#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>



                                /*---------------------------------------------------
                                  -> texturas que serão usadas
                                  -> eixos de rotação
                                  -> variavéis globais
                                ----------------------------------------------------*/
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

// foi para corrigir um bug, não entendemos o por quê que funcionou assim, mas tem toda uma logica por trás
const int alt_personagens = 24;
const int larg_personagens = 20;
const float alt_larg_sheet = 0.25;

const int larg_onibus = 80;
const int alt_onibus = 100;
// quando acionada a tecla r este seão os valores default
const int xvilaodefault[16] = {80, 72, 57, 47, 27, 37, 16, 7,
                                -80, -72, -47, -57, -27, -37, -7, -16};
const int yvilaodefault[16] = {30, 60, 30, 60, 60, 30, 30, 60,
                               60, 30, 30, 60, 30, 60, 30, 60};

int controle_velocidade_vilao;
int controle_velocidade_prota;
int controle_velocidade_onibus = 0;

int contador_de_viloes = 16;
int viloes_aleatorios[16];

int tiro = 0; // usamos como boolean
int xtiroProta; //coordenada x do tiro do protagonista
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

// vai brotar no meio da tela o maluquinho
coordenada protago = {0, -76};

//posição dos viloes na tela
int xvilao[16] = {80, 72, 57, 47, 27, 37, 16, 7,
                -78, -72, -47, -57, -27, -37, -7, -16};
int yvilao[16] = {30, 60, 30, 60, 60, 30, 30, 60,
                 60, 30, 30, 60, 30, 60, 30, 60};

int xbusao = 95;
int ybusao;
                                  /*---------------------------------------------------
                                                    carregando texturas
                                  ----------------------------------------------------*/
GLuint idProta[4];
GLuint idVilao[6];
GLuint idFundo1;
GLuint idtiro1[3];
GLuint idmenuzin;
GLuint idPensao;
GLuint idLose;
GLuint idcaixa[3];
GLuint idBusao;
GLuint idboleto;


GLuint carregaTextura(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}


void inicializa() {
    glClearColor(1, 0.5, 1, 1);
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idFundo1 = carregaTextura("primeiraFase.png");

    idboleto = carregaTextura("boleto.png");

    idtiro1[0] = carregaTextura("singaro1.png");
    idtiro1[1] = carregaTextura("singaro2.png");
    idtiro1[2] = carregaTextura("singaro3.png");
    
    idcaixa[0] = carregaTextura("singar1.png");
    idcaixa[1] = carregaTextura("singar2.png");
    idcaixa[2] = carregaTextura("singar3.png");
    
    idProta[3] = carregaTextura("sheet_protagonista.png");
    idProta[2] = carregaTextura("sheet_protagonista1.png");
    idProta[1] = carregaTextura("sheet_protagonista2.png");
    idProta[0] = carregaTextura("sheet_protagonista3.png");
    
    idVilao[0] = carregaTextura("sheet_vilao_1.png");
    idVilao[1] = carregaTextura("sheet_vilao_2.png");
    idVilao[2] = carregaTextura("sheet_vilao_3.png");
    idVilao[3] = carregaTextura("sheet_vilao_4.png");
    idVilao[4] = carregaTextura("sheet_vilao_5.png");
    idVilao[5] = carregaTextura("sheet_vilao_6.png");
    
    idmenuzin = carregaTextura("dados_de_jogo.png");
     
    idPensao = carregaTextura("pensao.png");
    
    idLose = carregaTextura("lose.png");
    

    
    idBusao = carregaTextura("carro.png");
}

coordenada itemglobal;
int modoitem;
int modotiro = 0;
int quantidadeDeTiro = 0;


void Background(){

    if(vidas >= 0){        
        glBindTexture(GL_TEXTURE_2D, idFundo1);
                                                 /*---------------------------
                                                     COLOCA O BACKGROUND 
                                                 ----------------------------*/
            glBegin( GL_QUADS );

            glTexCoord2f(0, 1);
            glVertex3f(-100.0, 100.0, 0.0);

            glTexCoord2f(0, 0);
            glVertex3f(-100.0, -100.0, 0.0);

            glTexCoord2f(1, 0);
            glVertex3f(100.0, -100.0, 0.0);

            glTexCoord2f(1, 1);
            glVertex3f(100.0, 100.0, 0.0);


        glEnd();
      
            glBindTexture(GL_TEXTURE_2D, idmenuzin);
                                                 /*-----------------------------------
                                                     COLOCA O BACKGROUNDIZINHO MENU
                                                 -----------------------------------*/
            glBegin( GL_QUADS );

            glTexCoord2f(0, 1);
            glVertex3f(-100, -100, 0.0);

            glTexCoord2f(0, 0);
            glVertex3f(-100, -140, 0.0);

            glTexCoord2f(1, 0);
            glVertex3f(100, -140, 0.0);

            glTexCoord2f(1, 1);
            glVertex3f(100, -100, 0.0);


        glEnd();

        glBindTexture(GL_TEXTURE_2D, idcaixa[modotiro]);
                                                 /*-----------------------------------
                                                     COLOCA O BACKGROUNDIZINHO MENU
                                                 -----------------------------------*/
            glBegin( GL_QUADS );

            glTexCoord2f(0, 1);
            glVertex3f(65, -110, 0.0);

            glTexCoord2f(0, 0);
            glVertex3f(65, -140, 0.0);

            glTexCoord2f(1, 0);
            glVertex3f(100, -140, 0.0);

            glTexCoord2f(1, 1);
            glVertex3f(100, -110, 0.0);


        glEnd();
    }

}

void lose(){

    if(vidas <= -1){
        glBindTexture(GL_TEXTURE_2D, idLose);
                                                 /*------------------------------------
                                                     COLOCA O BACKGROUND DO GAME 0VER
                                                 --------------------------------------*/
            glBegin( GL_QUADS );

            glTexCoord2f(0, 1);
            glVertex3f(-100.0, 100.0, 0.0);

            glTexCoord2f(0, 0);
            glVertex3f(-100.0, -140.0, 0.0);

            glTexCoord2f(1, 0);
            glVertex3f(100.0, -140.0, 0.0);

            glTexCoord2f(1, 1);
            glVertex3f(100.0, 100.0, 0.0);


        glEnd();
    }
}




void Onibus(int xbus, int ybus){
    glBindTexture(GL_TEXTURE_2D, idBusao);
                                             /*----------------------------------
                                                  LÊ O SPRITE DO BUSAO E O
                                                   MOVIMENTA
                                              -----------------------------------*/
    
    glBegin( GL_POLYGON );
        
        glTexCoord2f(0, 1);
        glVertex3f(xbus , ybus, 0.0); //coordenadas iniciais na tela

        glTexCoord2f(0, 0);
        glVertex3f(xbus, ybus - alt_onibus , 0.0);

        glTexCoord2f(1, 0);
        glVertex3f(xbus + larg_onibus, ybus - alt_onibus, 0.0);

        glTexCoord2f(1 , 1);
        glVertex3f(xbus + larg_onibus, ybus, 0.0);

    glEnd();
}


void Protagonista(float passo, float sentido){
    glBindTexture(GL_TEXTURE_2D, idProta[vidas]);
                                             /*----------------------------------
                                                  LÊ O SPRITE SHEET DO PROTA 
                                                  E DETERMINA QUAL SPRITE DEVE
                                                  SER ACIONADO QUANDO ELE SE 
                                                   MOVIMENTA
                                              -----------------------------------*/
    
    glBegin( GL_POLYGON );
        
        glTexCoord2f(passo, sentido); //coordenadas de corte do sheet
        glVertex3f(protago.x, protago.y, 0.0); //coordenadas iniciais na tela

        glTexCoord2f(passo, sentido - alt_larg_sheet);
        glVertex3f(protago.x ,protago.y - alt_personagens , 0.0);

        glTexCoord2f(passo + alt_larg_sheet, sentido - alt_larg_sheet);
        glVertex3f(protago.x + larg_personagens, protago.y - alt_personagens, 0.0);

        glTexCoord2f(passo + alt_larg_sheet , sentido);
        glVertex3f(protago.x + larg_personagens, protago.y, 0.0);

    glEnd();
}





void geraItem(coordenada item,int modo){
      if(modo == 1)
        glBindTexture(GL_TEXTURE_2D, idcaixa[1]);   //0 para o tiro do inimigo
      if(modo == 2)
        glBindTexture(GL_TEXTURE_2D, idcaixa[2]);  //1 para o tiro do prota
      if(modo == 3)
        glBindTexture(GL_TEXTURE_2D, idboleto);  //1 para o tiro do prota

        glBegin( GL_POLYGON );
            
            glTexCoord2f(0, 1);
            glVertex3f(item.x, item.y, 0.0);

            glTexCoord2f(0, 0);
            glVertex3f(item.x, item.y - 15, 0.0);

            glTexCoord2f(1, 0);
            glVertex3f(item.x + 10, item.y -15, 0.0);

            glTexCoord2f(1, 1);
            glVertex3f(item.x + 10, item.y, 0.0);

            glEnd();
    

}

void itemPego(coordenada itemn, int item){
                                        /*-------------------------------------------------
                                                    QUANDO ACERTA O HEROI 
                                         -------------------------------------------------*/
    if((itemn.x + 7) >= protago.x && (itemn.x + 7) <= (protago.x + larg_personagens) ){ //hitbox em x e em y, melhores valores
        if((itemn.y + 10) <= protago.y && (itemn.y + 10) >= (protago.y - alt_personagens) ){
          itemglobal.x = -159;
          itemglobal.y = -159;
          if(item == 1){
            modotiro = 1;
            quantidadeDeTiro = 3;
          }
          if(item == 2){
            modotiro = 2;
            quantidadeDeTiro =3;
          }
          if(item == 3){
            if(vidas < 3)
              vidas++;
          }
          
        }
    }
}

int escolheModo (){
  int item = rand()%100;
  if(item <= 9)
    return 1;
  else if(item < 19)
    return 2;
  else if(item < 29)
    return 3;
  else 
    return 0;
}

void item(coordenada vilua, int i){
  

  itemglobal.y -= 0.06;

  itemglobal.x += (sin(itemglobal.y/6)/10);

  if(sai[i] == 1 ){
    if(vilua.x == xvilaodefault[i] && vilua.y == yvilaodefault[i]){
        modoitem = escolheModo();
        itemglobal.x = vilua.x;
        itemglobal.y = vilua.y;
      }
    
    if(modoitem && itemglobal.x >= -95){
      geraItem(itemglobal, modoitem);
      itemPego(itemglobal,modoitem);
    }
  }
}







void viloesAleatorios(){
                                    /*--------------------------------
                                         ALEATORIZAÇÃO DOS VILOES
                                     ----------------------------------*/
    for(int i = 0; i < 16; i++){
        viloes_aleatorios[i] = rand()%6;
    }
    
}
void vilao(coordenada posicao, int abssissa, int ordenada, int i){
                                     /*--------------------------------
                                          LÊ O SPRITE SHEET DOS VILÕES
                                          E DETERMINA QUAL SPRITE DEVE
                                          SER ACIONADO QUANDO ELE SE 
                                          MOVIMENTA 
                                     ----------------------------------*/
    

    if(atingiu[i]){ // parametro de atingiu na posição x, 1 desenha e 0 some
         
         glBindTexture(GL_TEXTURE_2D, idVilao[viloes_aleatorios[i]]);
        
            glBegin( GL_POLYGON );
                
                glTexCoord2f(xpasso_vilao[abssissa] ,ysentido_vilao[ordenada] ); 
                glVertex3f( posicao.x, posicao.y, 0.0);

                glTexCoord2f(xpasso_vilao[abssissa],ysentido_vilao[ordenada] - alt_larg_sheet);
                glVertex3f(posicao.x, posicao.y - alt_personagens , 0.0);

                glTexCoord2f(xpasso_vilao[abssissa] + alt_larg_sheet, ysentido_vilao[ordenada] - alt_larg_sheet);
                glVertex3f(posicao.x + larg_personagens, posicao.y - alt_personagens, 0.0);

                glTexCoord2f(xpasso_vilao[abssissa] + alt_larg_sheet, ysentido_vilao[ordenada]);
                glVertex3f(posicao.x + larg_personagens, posicao.y, 0.0);

                glEnd();
            
              
            }

}


void andaHorizontal(int key, int direcao){
                                 /*----------------------------------------
                                     FAZ O PROTAGONISTA ANDAR PELO EIXO X
                                     QUANDO A TECLA É ACIONADA, POSSIBILITANDO
                                     A ANIMAÇÃO
                                  -----------------------------------------*/
        sentido = direcao;

        if(key == 1 && protago.x + larg_personagens < 100)
            protago.x+=2;   //velocidade do protagonista a direita, limitando até a posição 100
        else if (protago.x > -100 && key == 0)
            protago.x-=2; //velocidade do protagonista a esquerda, limitando até a posição -100

        if(controle_velocidade_prota % 5 == 0){
             if (passo >= 3)  //limitar
                    passo = 0;

            else
                passo++;
        }
        controle_velocidade_prota++;
}



void andaVilao(coordenada posicao, int i, int lado){

    
   if(controle_velocidade_vilao % 5 == 0){
        if(passo_vilao[i] == 3)
             passo_vilao[i] = 0;
        else
            passo_vilao[i] ++;
    }

   if (lado == 0){
     sentido_vilao[i] = 0;
     posicao.x += 2 ;
   }

   else if(lado == 1){
        sentido_vilao[i] = 1;
       posicao.x-= 2 ;
    }
    else if(lado == 2){
        sentido_vilao[i] = 2;
      posicao.x -= 2 ;
    }
    if(lado == 3){
       sentido_vilao[i] = 3;
     posicao.x += 2 ;
    }
     controle_velocidade_vilao++;
}




void atira(int xtiro, int ytiro, int shape){
                             /*-------------------------------------------------
                                LÊ O SPRITE DO TIRO E DETERMINA SUA TRAGETÓRIA
                              -------------------------------------------------*/
    if(shape)
        glBindTexture(GL_TEXTURE_2D, idtiro1[modotiro]);  //1 para o tiro do prota

    else
    glBindTexture(GL_TEXTURE_2D, idPensao);   //0 para o tiro do inimigo

        glBegin( GL_POLYGON );
            
            glTexCoord2f(0, 1);
            glVertex3f(xtiro + 10, ytiro, 0.0);

            glTexCoord2f(0, 0);
            glVertex3f(xtiro + 10, ytiro - 15, 0.0);

            glTexCoord2f(1, 0);
            glVertex3f(xtiro, ytiro -15, 0.0);

            glTexCoord2f(1, 1);
            glVertex3f(xtiro, ytiro, 0.0);

            glEnd();
}


void singaro(int key){        
                                         /*-------------------------------------------------
                                            QUANDO ACIONADA A BARRA DE ESPAÇO O TIRO VAI
                                            SAIR DE ONDE O PROTAGONISTA ESTÁ
                                         -------------------------------------------------*/

    if(key && ytiroProta<=100){ //key funciona como booleano
        ytiroProta+= 4;         //velocidade
       atira(xtiroProta ,ytiroProta,1); //coordenas do tiro do prota, shape
    }

}

void pensaoPaga(int xpensao, float ypensao, int i){
                                        /*-------------------------------------------------
                                                    QUANDO ACERTA O HEROI 
                                         -------------------------------------------------*/
    if((xpensao + 7) >= protago.x && (xpensao + 7) <= (protago.x + larg_personagens) ){ //hitbox em x e em y, melhores valores
        if((ypensao + 10) <= protago.y && (ypensao + 10) >= (protago.y - alt_personagens) ){
            vidas--;
            xtiroVilao[i] = 140; //faz o tiro sumir quando acerta
            ytiroVilao[i] = -150;
        }
    }
}

void pensao (coordenada vilua, int i){
                                        /*-------------------------------------------------
                                            AQUI DETERMINA A TRAJETORIA DO TIRO DOS VILÕES 
                                         -------------------------------------------------*/
    ytiroVilao[0]-= 0.15; //velocidade de queda
    ytiroVilao[1]-= 0.15;
    ytiroVilao[2]-= 0.15;
    int segundotiro = rand()%16;
    int terceirotiro = rand()%16;
    

    if(sai[i] == 0){ //não atirar depois de morto
        if(protago.x >= vilua.x && protago.x <= (vilua.x + larg_personagens) && ytiroVilao[0] <= -95 ){ //protagonista em baixo do vilao
                                                                                                        // quando atingir o chão, reinicia o tiro
            xtiroVilao[0] = vilua.x;       //posição do tiro inicial do tiro que segue                 // que segue
            ytiroVilao[0] = vilua.y;
        }

        if(xvilao[segundotiro] != xtiroVilao[0] && ytiroVilao[1] <= -95 && sai[segundotiro] == 0){  //não deixa atirar duas vezes ao mesmo tempo
            xtiroVilao[1] = xvilao[segundotiro];                    // são tiros aleatorios
            ytiroVilao[1] = yvilao[segundotiro];
        }
        if(xvilao[terceirotiro] != xtiroVilao[0] && xvilao[terceirotiro] != xtiroVilao[1] && ytiroVilao[2] <= -95 && sai[terceirotiro] == 0){
            xtiroVilao[2] = xvilao[terceirotiro];                   // são tiros aleatorios
            ytiroVilao[2] = yvilao[terceirotiro];
        }
    }
    atira(xtiroVilao[0], ytiroVilao[0], 0);
    atira(xtiroVilao[1], ytiroVilao[1], 0);
    atira(xtiroVilao[2], ytiroVilao[2], 0);
    pensaoPaga(xtiroVilao[0], ytiroVilao[0], 0);
    pensaoPaga(xtiroVilao[1], ytiroVilao[1], 1);
    pensaoPaga(xtiroVilao[2], ytiroVilao[2], 2);
}



void SaiDaTela(int i,coordenada vilua){
                                        /*-------------------------------------------------
                                            QUANDO ATINGIDO, O VILÃO IRÁ RETORNAR PARA O 
                                            CANTO SUPERIOR ESQUERDO DA TELA
                                         -------------------------------------------------*/
    if(sai[i]){ //assume 1 quando o vilão é atingido
        if(vilua.x >= 0){        //anda até o meio 
            xvilao[i]--;
            andaVilao(vilua, i, 1);
        }
        else if(vilua.y <= 80){
            yvilao[i]++;            //vai até a calçada
            andaVilao(vilua, i, 2);
        }
        else if(vilua.x >= -120){
            xvilao[i]--;             // sai da tela
            andaVilao(vilua, i, 1);
        }
        else{
            atingiu[i] = 0;         //some
            andaVilao(vilua, i, 4);
        }
    }

}

void impacto(int i, coordenada posicao){
                                        /*----------------------------------------------------
                                            VÊ SE O TIRO ACERTOU O TAL INIMIGO OU NÃO
                                         ----------------------------------------------------*/
    if(atingiu[i]){ // impede que o tiro trave
        if(xtiroProta >= posicao.x && xtiroProta <= (posicao.x + larg_personagens)){ //intervalo de x
            if(ytiroProta >= posicao.y && ytiroProta <= (posicao.y + alt_personagens) && ytiroProta != 104){ // intervalo de y
                sai[i] = 1;
                ytiroProta = 110; // valor pra y fora da margem de erro
            }
        }
    }

    SaiDaTela(i, posicao);
}

void rage(int i, coordenada vilua){
                                        /*---------------------------------------------------------
                                            FAZ O BICHINHO INIMIGO FICAR DOIDÃO. IMAGINA QUE 
                                            VOCÊ É O BATMAN, BATEU EM 29 DOS 30 INIMGOS, O ÚLTIMO
                                            QUE SOBRA ACHA QUE PODE CONTIGO E FICA LOUCAO, RUSHANDO
                                            E TE PROCURANDO. FICANDO FORA DE POSIÇÃO
                                         ---------------------------------------------------------*/
    if(contador_de_viloes == 1 && sai[i] == 0){ //se sobrar um vilão e ele não foi atingido
        if(modo == 1){                          // qual direção e sentido ele vai
            if(vilua.y >= 80)                   // limita atè onde vai
                modo = (rand()%3)+2;            // sorteia próxima direção impedindo que seja 1 novamente
            yvilao[i] += 2;                     // ele vai pra cima, o 2 é velocidade
            andaVilao(vilua, i, 2);
        }
        else if(modo == 2){
            if(vilua.x <= -90 && vilua.y <80)   //liimita até onde vai
                modo = (rand()%4)+1;
            
            else if(vilua.x <= -90)              //impede que vá mais pra direita, em cima dos prédios
                modo = (rand()%2)+3;
            xvilao[i] -= 2;                     //vai para a esquerda
            
            andaVilao(vilua, i, 1);
        }
        else if(modo == 3){
            if(vilua.y <= 30 && vilua.y < 80)
                modo = (rand()%4)+1;
            
            else if (vilua.y <= 30)                 //impede que vá pra cima dos prédios
                modo = (rand()%3)+2;
            yvilao[i] -= 2;                         //vai pra baixo
            
            andaVilao(vilua, i, 3);
        }
        else if(modo == 4){
            if(vilua.x >= 70 && vilua.y <80)
                modo = (rand()%3)+1;
            
            else if(vilua.x >= 70)                 // não vá para os prédios
                modo = (rand()%3)+2;
            xvilao[i] += 2;                         // vai ṕara direita
            
            andaVilao(vilua, i, 0);
        }
    }
}
void vilao_impacto(){
                                        /*-------------------------------------------------
                                            AQUI, FAZE-SE A ATRIBUIÇÃO DAS COORDENADAS,
                                            CONFIRMA SE TEVE IMPACTO AO CHAMAR A FUNÇÃO 
                                            IMPACTO E, SE TIVER, CHAMA A VILÃO COM O VALOR
                                            DO IMPACTO PARA DESENHA-LO OU PEDIR GENTILMENTE
                                            QUE SE RETIRE PELO CANTO SUPERIOR ESQUERDO
                                         -------------------------------------------------*/
     int num_de_viloes = 0;
    coordenada vilua[16];
    for(int i = 0; i < 16; i++){  // posição dos vilões
        vilua[i].x = xvilao[i];
        vilua[i].y = yvilao[i];
    }
    for(int i = 0; i<16; i++){ //quantos vilões vivos ainda temos
        if(sai[i] == 0){  // não levou tiro ainda
            num_de_viloes++;
        }
    }
    contador_de_viloes = num_de_viloes; //quanto tiver apenas 1 = range

    for (int i = 0; i < 16; ++i){   //faz a chamada dessas funções para cada personagem
        rage(i, vilua[i]);
        impacto(i, vilua[i]);
        vilao(vilua[i], passo_vilao[i] , sentido_vilao[i], i );
        pensao(vilua[i] , i);
        item(vilua[i], i);
    }
      

}
void andaBusao(){
    
        coordenada vilua[16];          
        if(xbusao <= 95 && xbusao != 0){
            xbusao -= 1;
        }

        if(xbusao == 10 ){
           
            while(controle_velocidade_onibus >= 1 && controle_velocidade_onibus <= 7){
                xbusao -= 0.15;
                   /*  for (int i = 0; i < 16; ++i){   
                        rage(i, vilua[i]);
                        impacto(i, vilua[i]);
                        vilao(vilua[i], passo_vilao[i] , sentido_vilao[i], i );
                        pensao(vilua[i] , i);
                     }*/
            }
            
            while(controle_velocidade_onibus > 7 ){
                
                    xbusao -=1;
                
            }controle_velocidade_onibus++;
            
        }
        
        
        Onibus(xbusao, ybusao);
        
    }
    
void reinicia(){
                                        /*-------------------------------------------------
                                            REINICIA O JOGO QUANDO ACIONADA A TELA 'R'
                                            COM OS VALORES DEFINIDOS LÁ EM RIBA COMO DEFAULT
                                         -------------------------------------------------*/             
    for (int i = 0; i < 16; ++i){
        xvilao[i] = xvilaodefault[i];
        yvilao[i] = yvilaodefault[i];

        sai[i] = 0;

        atingiu[i] = 1;

         passo_vilao[i] = 0;
         sentido_vilao[i] = 3;
    }

    xtiroVilao[0] = 140;
    ytiroVilao[0] = -100;
    xtiroVilao[1] = 140;
    ytiroVilao[1] = -100;
    xtiroVilao[2] = 140;
    ytiroVilao[2] = -100;
    vidas = 3;

    tiro= 0;
    xtiroProta;
    ytiroProta = -80;


    passo = 1;
    sentido = 0; 
 
    protago.x = 0;
    protago.y = -76;
}


void desenha() {
   
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    Background();
  //  andaBusao();
   
    vilao_impacto();


    singaro(tiro);
    Protagonista(xpasso[passo], ysentido[sentido]);

    lose();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}




void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -140, 100, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
}

void teclasLenvatadas(unsigned char key, int x, int y){
    switch (key){
        case 32:
            quantidadeDeTiro--;
            if(quantidadeDeTiro == 0)
              modotiro = 0;
            break;
    }
}


void teclado(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'r':
        reinicia();
        break;
    case 'a':
        andaHorizontal(0, 1);
        break;
    case 'd':
        andaHorizontal(1, 2);
        break;
    case 'w':    // vira para cima
        passo = 1;
        sentido = 0;
        break;
    case 's':   //vira de costas
        passo = 1;
        sentido = 3;
        break; 
    case 32:   //espaço
    passo = 2;        //coordenadas do sheet
    sentido = 0; 
    if(ytiroProta >= 100 || ytiroProta == -80){    // primeiro e segundo tiro iniciais
                xtiroProta = protago.x;
                ytiroProta= -80;
                tiro = 0;
            }
        tiro = 1;
        modo = (rand()%4)+1;
        break;
    }
}
void atualiza() {
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 700);
    glutInitWindowPosition(1000, 1000);

    srand(time(0));

    glutCreateWindow("Where's my Pai");

    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutDisplayFunc(desenha);
    glutIdleFunc(atualiza);

  
    glutKeyboardUpFunc(teclasLenvatadas);

    viloesAleatorios();

    inicializa();

    glutMainLoop();

    return 0;
}