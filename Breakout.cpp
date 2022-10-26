#include "raylib.h"
#include <string>

const int x_tela = 780; // Constante global com o valor da largura da tela.
const int y_tela = 600; // Constante global com o valor da altura da tela.

class raquete{
    public:
        float alt = 10;
        float larg = 100;
        float x_pos = x_tela/2 - larg/2; 
        float y_pos = 595 - alt;
        
        void mover_esqueda(bool m){
            if(m){
                if(x_pos != 0) x_pos -= 10;
            }
        }
        void mover_direita(bool m){
            if(m){
                if(x_pos != x_tela - larg) x_pos += 10;
            }
        }
        
};

class bola{
    public:
        float x_pos=x_tela/2; // Inicialmente a posição x será igual á 300.00 pixeis
        float y_pos=y_tela/2; // Inicialmente a posição y será igual á 300.00 pixeis
        float vel_x = 4.0; // Inicialmente a bola se movimentará 4.0 pixeis horizontalmete por frame
        float vel_y = -6.0; // Inicialmente a bola se movimentará -6.0 pixeis verticalmete por frame
        int raio = 10; // O raio da bola será de 10 pixeis
        int vida = 3; // Inicialmente a vida da bola sera inicialmente 3
        
        void reset(){ // Metodo para resetar a bola
            int direcao= GetRandomValue(1, 2);
            x_pos = x_tela/2;
            y_pos = y_tela/2;
            vel_x = (direcao == 1)? -4.0: 4.0;
            vel_y = -6.0;
            
        }
        bool colisao(float rx_pos, float ry_pos, float r_larg, float r_alt){ //Verifica se colidiu na raquete
            //Se colidiu na raquete e a velocidade for maior que 0
            if(CheckCollisionCircleRec((Vector2){x_pos, y_pos}, raio, (Rectangle){rx_pos, ry_pos, r_larg,r_alt})){
                if(vel_y > 0){
                    float pos_hit = x_pos - (rx_pos + r_larg/2);
                    vel_y *= -1;   
                    vel_x = pos_hit * 0.28;
                    return true;
                }
            }
            if((x_pos >= (x_tela - raio)) || (x_pos <= raio)){ //Colisão dos lados da esquerda e direita
                vel_x *= -1.0;
                vel_y *= 0.75;
                return true;
            }
            if((y_pos >= (y_tela - raio))){ // Colisão do lado de baixo
                --vida;
                reset();
                return true;
            }
            if(y_pos <= raio + 35){ // Colisão do lado de cima
                vel_y = 6.0;
                vel_x = (vel_x < 0)? -5.0 : 5.0;
                return true;
            }
            return false;
        }
        int colisao(float rx_pos, float ry_pos, float r_larg, float r_alt, int v){ //Verifica se colidiu no tijolo 

            if(CheckCollisionCircleRec((Vector2){x_pos, y_pos}, raio, (Rectangle){rx_pos, ry_pos, r_larg,r_alt})){
                if(v > 0){
                    vel_y = 6.0;
                    vel_x = (vel_x < 0)? -5.0 : 5.0;
                    return -1;
                }
            
            }
            return 0;
        }
        
        
};
class tijolo{
    public:
        float x_pos;
        float y_pos;
        float larg = 100;
        float alt = 25;
        int vida;
        Color cor;
        void desenhar(){
            if(vida == 3)cor=BLUE; // Se a vida for igual a 3 a cor sera azul
            if(vida == 2)cor=GREEN; // Se a vida for igual 2 a cor sera verde
            if(vida == 1)cor=RED; // Se a vida for igual a 1 a cor sera vermelho
            if(vida == 0)cor=RAYWHITE; // Se a vida for igual a 0 a cor sera branco
            
            DrawRectangle(x_pos,y_pos,larg,alt,cor);
        }
        void inic(int i){
            if(i < 14){
                if(i < 7){
                    x_pos = 23 + 5*i + i*larg;
                    y_pos = 40;
                    vida = 3;
                    
                }else{
                    x_pos = 23 + 5*(i-7) + (i-7)*larg;
                    y_pos = 50 + alt;
                    vida = 2;
                    
                } 
            }else{
                x_pos = (23 + 5 + larg) + 5*(i-14) + (i-14)*larg;
                y_pos = 60 + alt*2;
                vida = 1;
                
            }
        }
        bool vivo(){
            if(vida == 0){
                return false;
            }else {
                return true;
            }
        }
};
int main(){
    
    bola b1; // Objeto b1 da classe bola.
    raquete r1; // Objeto r1 da classe raquete.
    tijolo t[19]; // Vetor t com 19 objetos da classe tijolo.
    int frame = 0; // Variavel responsavel por contar a quantidade de frames.
    int segundo = 0; // Variavel responsavel por contar a quantidade de segundos.
    int minuto = 0; //Variavel responsavel por contar a quantidade de minutos.
    int pontos = 0; //Variavel responsavel por contar a quantidade de pontos.
    int qts_tij = 19; //Variavel responsavel por contar a quantidade de tijolos restantes.
    bool morreu = false; //Variavel responsavel por dizer se o player morreu ou não.
    
    // Loop que defini a posição de todos os tijolos.
    for(int i = 0; i < qts_tij; ++i) t[i].inic(i);
    
    InitWindow(x_tela,y_tela, "Breakout - Informática S5"); // Inicializando a janela
    InitAudioDevice();
    Sound wallHitSound = LoadSound("WallHit.wav");
    Sound paddleHitSound = LoadSound("PaddleHit.wav");
    SetTargetFPS(60); // O jogo rodará 60 quadros por segundo
    while(!WindowShouldClose()){
        if(b1.vida == 0) morreu = true;
        if(!morreu){ // Se o player não morreu
            //Movimenta a bola e a raquete
            b1.x_pos += b1.vel_x;
            b1.y_pos += b1.vel_y;
            r1.mover_esqueda(IsKeyDown(KEY_LEFT));
            r1.mover_direita(IsKeyDown(KEY_RIGHT));
            //Faz a contagem do tempo
            frame++; 
            segundo = (frame/60); 
            if(segundo/60 == 1){
                minuto++;
                segundo = 0;
                frame = 0;
            }
        }
        //Se o jogador estiver morto e tiver apertado enter o jogo será resetado
        if(IsKeyPressed(KEY_ENTER) && b1.vida == 0){
            morreu = false;
            qts_tij=19;
            b1.reset();
            for(int i = 0; i < qts_tij; ++i) t[i].inic(i);

            frame = 0;
            segundo = 0;
            minuto = 0;
            pontos = 0;
            b1.vida = 3;
            r1.x_pos = x_tela/2 - r1.larg/2; 
        }
        //Verifica se a bola colidiu nas paredes e na raquete
        if(b1.colisao(r1.x_pos, r1.y_pos, r1.larg, r1.alt)){
            PlaySound(wallHitSound);
        }
        //Conta a quantidade de tijolo restande
        qts_tij = 0;
        for(int i = 0; i < 19; ++i){
            if(t[i].vivo()) ++qts_tij;
        }
        //Se não houver mais tijolos, irá resetar a posição da bola e criará mais tijolos
        if(qts_tij == 0){
            qts_tij=19;
            b1.reset();
            for(int i = 0; i < qts_tij; ++i){
                t[i].inic(i);
            }
        } 
        //Se a bola colidiu em um tijolo, a vida do mesmo irá diminuir e o jogador ganhará 100 pontos
        for(int i=0; i < 19; ++i){
            int colidiu = b1.colisao(t[i].x_pos,t[i].y_pos,t[i].larg,t[i].alt, t[i].vida);
            t[i].vida+= colidiu;
            pontos -= colidiu * 100;
            if(colidiu == -1)PlaySound(paddleHitSound);
        }
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(r1.x_pos, r1.y_pos, r1.larg, r1.alt, BLACK);
            
            for(int i = 0; i <19; ++i) t[i].desenhar();
            if(!morreu) DrawCircle(b1.x_pos,b1.y_pos,b1.raio,BLACK);
            DrawRectangle(0,0,x_tela,35,BLACK);
            DrawText(TextFormat("%d:%d",minuto,segundo),10, 0, 30 , LIGHTGRAY);
            DrawText(TextFormat("Pontos: %d",pontos),250, 0, 30 , LIGHTGRAY);
            DrawText(TextFormat("Vida: %d",b1.vida),650, 0, 30 , LIGHTGRAY);
            
            if(morreu){
                DrawRectangle(0,35,x_tela,y_tela,(Color){0,0,0,100 });
                DrawText("Você morreu!", 50 ,233, 100, RED);
                DrawText("Clique Enter para recomeçar.", 50,350, 30, RED);
                DrawText("Clique Esc para sair.", 50,400, 30, RED);
            }
        EndDrawing();
    }
    UnloadSound(wallHitSound);
    UnloadSound(paddleHitSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}