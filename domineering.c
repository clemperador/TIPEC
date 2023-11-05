#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include "rlgl.h"
#include "raymath.h"


// 0 = EMPTY
// 1 = V
// 2 = H

struct plays{
    int pla;
    int pos_x;
    int pos_y;

};

typedef struct plays play;

void printplay (play p){
    if (p.pla == 1){ 
    printf("(V: %d,%d)\n", p.pos_x,p.pos_y);}
    else{printf("(H: %d,%d)\n", p.pos_x,p.pos_y);}
}

int printgame (play game[], int p){ //prints game and returns game length in moves
    int i = 0;

    while(game[i].pos_x != -1){
    if(p){printplay(game[i]);}
    i++;}
    
    if(p){printplay(game[i]);}

    return i;
}


void printbool(bool b){ // :3
    if(b){printf("True\n");}
    else printf("False\n");
}

void init_matrix(int m, int n, int mat[][n]){ // initializes nxm integer matrix
    for(int i = 0; i<m; i++){
        for(int j=0; j<n; j++){
            mat[i][j] = 0;
        }
    } 
}

void printmat(int m, int n, int mat[][n]){

    for(int i = 0;i<m;i++){
        for(int j = 0; j<n; j++){
            printf("%d, ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Can you place a V/H domino in (i,j) ?
bool legalvert(int m, int n, int mat[][n], int i, int j){
    if(i >= m - 1 || mat[i][j] != 0 || mat[i+1][j] != 0){return false;}
    return true;
}

bool legalhori(int m, int n, int mat[][n], int i, int j){
    if(j >= n - 1 || mat[i][j] != 0 || mat[i][j+1] != 0){return false;}
    return true;
}

// Places a V/H domino in (i,j), if legal.
play placevert(int m, int n, int mat[][n], int i, int j){ // Places vertical domino at (i)(j) and (i+1)(j)
    play p;
    if(!legalvert(m,n,mat,i,j)){/*printf("Can't place at (%d,%d).\n", i, j);*/}
    else{
        mat[i][j] = 1;
        mat[i+1][j] = 1;
        p.pla = 1; p.pos_x = i; p.pos_y = j;
    }
    return p;
}
play placehori(int m, int n, int mat[][n], int i, int j){ // Places vertical domino at (i)(j) and (i+1)(j)
    play p;
    if(!legalhori(m,n,mat,i,j)){/*printf("Can't place at (%d,%d).\n", i, j);*/}
    else{
        mat[i][j] = 2;
        mat[i][j+1] = 2;
        p.pla = 2; p.pos_x = i; p.pos_y = j;
    }
    return p;
}

// Checks if V/H can still do more moves.
bool nomorehori(int m, int n, int mat[][n]){ // returns true if no more pieces can be placed
    for(int i = 0 ; i<m ; i++ ){
        for(int j = 0; j<n-1 ; j++){
            if(mat[i][j] == 0 &&
             mat[i][j+1] == 0){
                return false;
            }
        }
    }
    return true;
}
bool nomorevert(int m, int n, int mat[][n]){ // returns true if no more pieces can be placed
    for(int j = 0 ; j<n ; j++ ){
        for(int i = 0; i<m-1 ; i++){
            if(mat[i][j] == 0 
            && mat[i+1][j] == 0){
                return false;
            }
        }
    }
    return true;
}

play placerandvert(int m, int n, int mat[][n]){ // Places vertical domino at (i)(j) and (i+1)(j)
    play p;
    int i = rand() % (m - 1);
    int j = rand() % n;
    while(!legalvert(m,n,mat,i,j)){
        i = rand() % (m - 1);
        j = rand() % n;}
    mat[i][j] = 1;
    mat[i+1][j] = 1;
    p.pla = 1; p.pos_x = i; p.pos_y = j;
    return p;
}

play placerandhori(int m, int n, int mat[][n]){ // Places horizontal domino at (i)(j) and (i)(j+1)
    play p;
    int i = rand() % m; 
    int j = rand() % (n - 1);
    while(!legalhori(m,n,mat,i,j)){
        i = rand() % m;
        j = rand() % (n -1);}

    mat[i][j] = 2;
    mat[i][j+1] = 2;
    p.pla = 2; p.pos_x = i; p.pos_y = j;
    return p;
}

play placerandwho(int m, int n, int mat[][n], int w){
    if (w== 1){
        return placerandvert(m,n,mat);
    }
    else{return placerandhori(m,n,mat);
    }
}

play* playrandom(int m, int n, int mat[][n], int s){ 
    init_matrix(m,n,mat);
    int c = s;
    play* game = malloc(m*n*sizeof(play));
    int i = 0;
    if(c == 1){
        while(! ( ((c == 1) && (nomorevert(m,n,mat))) || ((c == 2) && (nomorehori(m,n,mat))) )){
            if(nomorevert(m,n,mat)){
                game[i].pla = 1; game[i].pos_x = -1;
            }
            else{
                game[i] = placerandvert(m,n,mat);
                c = 2;
                i++;
            }


            if(nomorehori(m,n,mat)){
                game[i].pla=2; game[i].pos_x = -1;
            }
            else{
                game[i] = placerandhori(m,n,mat);
                c = 1;
                i++;
            }
        }
    }
    else{
        while(!(((c == 2) && (nomorehori(m,n,mat))) || ((c == 1) && (nomorevert(m,n,mat)))  )){
            if(nomorehori(m,n,mat))
            {
                game[i].pla = 2; game[i].pos_x = -1;
            }
            else{
            game[i] = placerandhori(m,n,mat);
                i++;
                c = 1;
            }

            if(nomorevert(m,n,mat)){
                game[i].pla = 1; game[i].pos_x = -1;
            }
            else{
                game[i] = placerandvert(m,n,mat);
                i++;
                c = 2;
            }
            
        }
    }
    
    if (c == 1 && nomorevert(m,n,mat)){
        game[i].pla = 1; game[i].pos_x = -1;
    }
    else if (c == 2 && nomorehori(m,n,mat)){
        game[i].pla = 2; game[i].pos_x = -1;
    }
    else {assert(false);}

    return game;
}


int andthewinneris(play game[]){ // Returns 1 if vertical wins, 2 if horizontal wins.
    int i = 0;
    while(game[i].pos_x != -1){
    i++;}
    if(game[i].pos_x == -1){
            if(game[i].pla == 1){
                return 2;
            }
            else{
                return 1;
            }}
}

int andthestarterwas(play game[]){
    if(game[0].pla == 1){
        printf("First player: V.\n");
        return 1;
    }
    else{
        printf("First player: H.\n");
        return 2;
    }
}

int horwincount (int m, int n, int s, int c){ // Plays c random games. s starts. Returns horizontal wincount.
    
    int mat[m][n];
    init_matrix(m,n,mat);

    int h = 0; int v = 0;
    for (int i = 0; i < c; i++){
        init_matrix(m,n,mat);
        play* game;
        game = playrandom(m,n,mat,s);
        printgame(game,1);
        printmat(m,n,mat);
        if(andthewinneris(game) == 2)
        {printf("GAME %d: H\n", i);h++;} else {printf("GAME %d: V\n\n", i);v++;}
        }
    
    printf("Horizontal won %d/%d games. Vertical won %d/%d.\n",h,c,v,c);
    return h;
}

void experimental(int mi, int mf, int ni, int nf, int ng, int s) {

    for(int i = mi; i< mf+1 ; i++){
        for(int j = ni; j < nf+1; j++){

            int m = i;
            int n = j;

            int mat[m][n];
            init_matrix(m,n,mat);
            int cpt = 0;
            
            double per1;
            double per2;

            // First cycle. Vertical Starts.

            for(int j = 0;j<ng;j++){
                init_matrix(m,n,mat);
                play* game;
                game = playrandom(m,n,mat,1);
                
                if(andthewinneris(game) == 1){
                    cpt++;}
                free(game);
            }

            per1 = (double)cpt/(double)ng;

            int cpt2 = 0;

            // Second cycle. Horizontal starts.
            for(int j = 0;j<ng;j++){
                init_matrix(m,n,mat);
                play* game;
                game = playrandom(m,n,mat,2);

                if(andthewinneris(game) == 2){
                    cpt2++;}
                free(game);
                
            }

            per2 = (double)cpt2/(double)ng;

            printf("== %dx%d BOARD RESULTS: ==\n\n",m,n);

            printf("When (V) starts, they win %d out of %d games. (%f\%)\n", cpt, ng, per1*100);

            printf("When (H) starts, they win %d out of %d games. (%f\%)\n\n", cpt2, ng, per2*100);

            double totalperh = 
            (double)((ng-cpt)+(cpt2))/(2*(double)ng);

            printf("Total H wins: %d/%d (%f\%).\n",(ng-cpt)+(cpt2),2*ng, totalperh*100);
            printf("Total V wins: %d/%d (%f\%).\n",cpt+(ng-cpt2),2*ng,(1-totalperh)*100);

            printf("\n");

            double totalperh2 = 
            (double)(cpt+cpt2)/(2*(double)ng);

            printf("Total 1P wins: %d/%d (%f\%).\n",
            cpt+cpt2, 2*ng, totalperh2*100);
            printf("Total 2P wins: %d/%d (%f\%).\n",(ng-cpt)+(ng-cpt2),2*ng, (1-totalperh2)*100);

            printf("\n");

            // RÉVISION D'AVANTAGES. (( To be reworked! ))

            char a = 'n'; // p: 1P, d: 2P, v: V, h: H , n: none (yet)

            if(m == n){ // If the matrix is nxn, only 1 or 2 can have an advantage.
                if(totalperh2 > 0.5){
                    printf("C'est une matrice carrée. 1 a un avantage.\n");
                    a = 'p';
                }
                else{
                    printf("C'est une matrice carrée. 2 a un avantage.\n");
                    a = 'd';
                }
            }
            else{
            if(totalperh > 0.55){
                printf("H semble avoir un avantage...\n");
                
                if((1-per1) < 0.55){
                    printf("Mais on peut pas être sur...\n");
                }
                else{printf("Oui. H a l'avantage dans un tableau %dx%d. \n",m,n); a = 'h';}
            }
            if(totalperh < 0.45){
                printf("V semble avoir un avantage...\n");

                if((1-per2) < 0.55){
                    printf("Mais on peut pas être sur...\n");
                }
                else{printf("Oui. V a l'avantage dans un tableau %dx%d. \n",m,n); a = 'v';}
            }
            if (a == 'n'){
                if(totalperh2 > 0.50){
                    printf("1P a un avantage dans un tableau %dx%d.\n",m,n);
                    a = 'p';
                }
                else{
                    printf("2P a un avantage dans un tableau %dx%d.\n",m,n);
                    a = 'd';
                }
            }

            if(a == 'n'){
                printf("Pas d'avantage trouvé.\n");
            }}

            printf("=========================\n");
            printf("\n\n\n");

            char *append = malloc(1000  * sizeof(char));
            
            char *todo   = malloc(10000 * sizeof(char));
            char *string = malloc(10000 * sizeof(char));

            sprintf(string, "%d , %d , %f , %f, %f , %f , %c , %d",
            m , n , totalperh*100, (1-totalperh)*100, totalperh2*100 , (1-totalperh2)*100 , a, ng);

            
            if (s == 1){
                strcpy(todo, "echo \"");
                strcat(todo, string); 
                strcat(todo,"\"");
                strcat(todo,">> stats.csv");
                system(todo);
            }

            free(todo);
            free(string);
            free(append);
        }
    }
}   

// Execution:

// .dom/ im m in n k s
// Plays k games alternating the starting player, from dimensions im*in to m*n. s determines if the data is saved or just printed.


// =============== GAME GRAPHICS ==============

const int screenWidth = 600;
const int screenHeight = 600;

const char* Vwins = "Vertical wins!!";
const char* Hwins = "Horizontal wins!!";
const char* pressRToRestart = "Press 'r' to play again!";


void GridNOW(int m,int n){
    int cellWidth = screenWidth/m;
    int cellHeight = screenHeight/n;
    for(int i = 0; i < m ; i++){
        for(int j = 0; j < n ; j++){
        DrawRectangleLines(i*cellWidth, j*cellHeight , cellWidth, cellHeight, WHITE);
    }
}
}

void colorplace(int i, int j, Color c, int m, int n, int x){
    char text[10];
    snprintf(text, sizeof(text), "%d", x);
    int cellWidth = screenWidth/m; int cellHeight = screenHeight/n;
    DrawRectangle(cellHeight*j,cellWidth * i, cellWidth, cellHeight, c);
    DrawText(text,cellHeight*(j+1)- (MeasureText(text, 10)*1.9), cellWidth*(i+1) - 15, 15, ColorBrightness(c,-0.3f));
    
}

void DominoPlace(play p, int m, int n, int x){
    if(p.pla == 1){
        colorplace(p.pos_x,p.pos_y,BLUE,m,n, x);
        colorplace(p.pos_x + 1,p.pos_y,BLUE,m,n, x);
    }
    else if (p.pla == 2){
        colorplace(p.pos_x,p.pos_y,RED,m,n, x);
        colorplace(p.pos_x,p.pos_y +1,RED,m,n, x);
    }
}

void DominoRemove(play p, int m, int n){
    if(p.pla == 1){
        colorplace(p.pos_x,p.pos_y,BLACK,m,n, 0);
        colorplace(p.pos_x + 1,p.pos_y,BLACK,m,n, 0);
    }
    else if (p.pla == 2){
        colorplace(p.pos_x,p.pos_y,BLACK,m,n, 0);
        colorplace(p.pos_x,p.pos_y +1,BLACK,m,n, 0);
    }
}

void FinalScreen(int w){
    if (w == 1){
        DrawRectangle(0, 0, screenWidth,screenHeight, Fade(WHITE, 0.8f));
        DrawText(Vwins, screenWidth / 2 - MeasureText(Vwins, 40) / 2, screenHeight / 2 - 10, 40, BLUE);
        DrawText(pressRToRestart, screenWidth / 2 - MeasureText(pressRToRestart, 20) / 2, screenHeight * 0.75f - 10, 20, DARKGRAY);
    }
    else{
        DrawRectangle(0, 0, screenWidth,screenHeight, Fade(WHITE, 0.8f));
        DrawText(Hwins, screenWidth / 2 - MeasureText(Hwins, 40) / 2, screenHeight / 2 - 10, 40, RED);
        DrawText(pressRToRestart, screenWidth / 2 - MeasureText(pressRToRestart, 20) / 2, screenHeight * 0.75f - 10, 20, DARKGRAY);
    }
}

void AnyScreen(char* text){
    DrawRectangle(0, 0, screenWidth,screenHeight, Fade(WHITE, 0.8f));
    DrawText(text, screenWidth / 2 - MeasureText(text, 40) / 2, screenHeight / 2 - 10, 40, DARKGRAY);
}


void randomgaming (int m, int n){

    InitWindow(screenWidth, screenHeight, "Domineering");
    int mat[m][n];
    init_matrix(m,n,mat);
    play* game = malloc(m*n*sizeof(play));
    int x = 0;
	while(!WindowShouldClose())
	{
        int mat[m][n];
        init_matrix(m,n,mat);
        play* game = malloc(m*n*sizeof(play));
        int x = 0;
        int c;
        bool r = false;

        
		BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, screenWidth,screenHeight, Fade(WHITE, 0.8f));
        DrawText("Who starts? (H/V)", screenWidth / 2 - MeasureText("Who starts? (H/V)", 40) / 2, screenHeight / 2 - 10, 40, DARKGRAY);

        while(!IsKeyPressed(KEY_H) && !IsKeyPressed(KEY_V)){
        EndDrawing();
        if(IsKeyPressed(KEY_H)){ c = 2;}
        if(IsKeyPressed(KEY_V)){ c = 1;}
        }

        BeginDrawing();
        ClearBackground(BLACK);
        GridNOW(m,n);

        game = playrandom(m,n,mat,c);
        printmat(m,n,mat);

        
        while(!r){
            while(x <= printgame(game,0)){
                char title[60];
                char c; if(game[x].pla == 1){c = 'V';}else{c='H';}
                snprintf(title, sizeof(title), "Move nº %d, Next move: %c",x,c);
                SetWindowTitle(title);
                EndDrawing();
                if(IsKeyPressed(KEY_RIGHT)){
                    BeginDrawing();
                    DominoPlace(game[x],m,n, x);
                    x++;GridNOW(m,n);
                }
                else if(IsKeyPressed(KEY_LEFT) && x > 0){
                    BeginDrawing();
                    DominoRemove(game[x-1],m,n);
                    x--;GridNOW(m,n);
                }
            }
            
            DominoPlace(game[x],m,n, x);GridNOW(m,n);

            FinalScreen(andthewinneris(game)); 
            EndDrawing();
            if(IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ESCAPE)){
                r = true;
            }
            if(IsKeyPressed(KEY_LEFT)){
                x--;
                BeginDrawing();
                ClearBackground(BLACK);
                GridNOW(m,n);
                for(int i = 0; i < x; i++){
                    DominoPlace(game[i],m,n, i);GridNOW(m,n);
                }
            }
        }
        ClearBackground(BLACK);
        free(game);
    }
	CloseWindow();
}


int main(int argc, char* argv[]){
    int h = atoi(argv[1]);
    if (h == 1) {SetTargetFPS(30); int m = atoi(argv[2]); int n = atoi(argv[3]); randomgaming(m,n);}
    else{
        system("clear");
        int mi = atoi(argv[2]); int mf = atoi(argv[3]); 
        int ni = atoi(argv[4]); int nf = atoi(argv[5]);
        int k = atoi(argv[6]); int s = atoi(argv[7]);
        experimental(mi,mf,ni,nf,k,s);
    }
    return 0;
}

// Por hacer: Estudio masivo de estadisticas: Forma de estudiar tabla de dos dimensiones comparable a aquella del Obsequi. Comparar teoría con stats y arreglar criterios de ventaja.
