#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

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

int printgame (play game[]){ //prints game and returns game length in moves
    int i = 0;

    while(game[i].pos_x != -1){
    printplay(game[i]);
    i++;}
    
    printplay(game[i]);

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
    if(i+1 >= m){return false;}
    if(mat[i][j] != 0 || mat[i+1][j] != 0){return false;}
    return true;
}
bool legalhori(int m, int n, int mat[][n], int i, int j){
    if(j+1 >= n){return false;}
    if(mat[i][j] != 0 || mat[i][j+1] != 0){return false;}
    return true;
}

// Places a V/H domino in (i,j), if legal.
play placevert(int m, int n, int mat[][n], int i, int j){ // Places vertical domino at (i)(j) and (i+1)(j)
    play p;
    if(!legalvert(m,n,mat,i,j)){printf("Can't place at (%d,%d).\n", i, j);}
    else{
        mat[i][j] = 1;
        mat[i+1][j] = 1;
        p.pla = 1; p.pos_x = i; p.pos_y = j;
    }
    return p;
}
play placehori(int m, int n, int mat[][n], int i, int j){ // Places vertical domino at (i)(j) and (i+1)(j)
    play p;
    if(!legalhori(m,n,mat,i,j)){printf("Can't place at (%d,%d).\n", i, j);}
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
            if(mat[i][j] == 0 && mat[i][j+1] == 0){
                return false;
            }
        }
    }
    return true;
}
bool nomorevert(int m, int n, int mat[][n]){ // returns true if no more pieces can be placed
    for(int j = 0 ; j<n ; j++ ){
        for(int i = 0; i<m-1 ; i++){
            if(mat[i][j] == 0 && mat[i+1][j] == 0){
                return false;
            }
        }
    }
    return true;
}

// Places a V/H on the first place it finds. Returns play 
play placefirstvert(int m, int n, int mat[][n]){ 
    play p;
    for(int j = 0 ; j<n ; j++ ){
        for(int i = 0; i<m-1 ; i++){
            if(mat[i][j] == 0 && mat[i+1][j] == 0){
                mat[i][j] = 1; mat[i+1][j] = 1;
                p.pla = 1; p.pos_x = i; p.pos_y = j;
                return p;
            }
        }
    }
    printf("No more plays can be made for V\n"); p.pla = 1; p.pos_x = -1; p.pos_y = -1; return p;
}
play placefirsthori(int m, int n, int mat[][n]){ 
    play p;
    for(int i = 0 ; i<m ; i++ ){
        for(int j = 0; j<n-1 ; j++){
            if(mat[i][j] == 0 && mat[i][j+1] == 0){
                mat[i][j] = 2; mat[i][j+1] = 2;
                p.pla = 2; p.pos_x = i; p.pos_y = j;
                return p;
            }
        }
    }
    printf("No more plays can be made by H\n"); p.pla = 2; p.pos_x = -1; p.pos_y = -1; return p;
}

play* playslow(int m, int n, int mat[][n], int s, double t){ // t = temps entre chaque tour.
    int c = s;
    play* game = malloc(m*n*sizeof(play));
    int i = 0;

    system("clear"); printmat(m,n,mat);

    if(s == 1){
        while(!(nomorevert(m,n,mat) || nomorehori(m,n,mat))){
            if(nomorevert(m,n,mat)){
                game[i].pla = 1; game[i].pos_x = -1;
            }
            else{
                sleep(t);
                game[i] = placefirstvert(m,n,mat);
                system("clear"); printmat(m,n,mat); printplay(game[i]); if(i>0){printplay(game[i-1]);}
                c = 1;
            }
            i++;
            sleep(t);

            if(nomorehori(m,n,mat)){
                game[i].pla=2; game[i].pos_x = -1;
            }
            else{
                game[i] = placefirsthori(m,n,mat);
                system("clear"); printmat(m,n,mat); printplay(game[i]); if(i>0){printplay(game[i-1]);}
                c = 0;
                i++;
            }
        }

    }
    else{
        while(!(nomorehori(m,n,mat) || nomorevert(m,n,mat))){
            if(nomorehori(m,n,mat))
            {
                game[i].pla = 2; game[i].pos_x = -1;
            }
            else{
            game[i] = placefirsthori(m,n,mat);
            sleep(t);
            system("clear"); printmat(m,n,mat); printplay(game[i]); if(i>0){printplay(game[i-1]);}
            }
            i++;
            c = 0;
            if(nomorevert(m,n,mat)){
                game[i].pla = 1; game[i].pos_x = -1;
            }
            else{
                game[i] = placefirstvert(m,n,mat);
                sleep(t);
                system("clear"); printmat(m,n,mat); printplay(game[i]); if(i>0){printplay(game[i-1]);}
            }
            i++;
            c = 1;
        }
    }
    
    if (c==2){
        game[i].pla = 1; game[i].pos_x = -1;
    }
    else{
        game[i].pla = 2; game[i].pos_x = -1;
    }
    
    printf("\n");
    return game;
}


int andthewinneris(play game[]){ 
    int i = 0;
    while(game[i].pos_x != -1){
    i++;}
    if(game[i].pos_x == -1){
            printf("Winner:");
            if(game[i].pla == 1){
                printf("H.\n");
                return 2;
            }
            else{
                printf("V.\n");
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

int main(int argc, char *argv[]) {

    int m = 50, n = 50, s = 0; // (m , n) dimensions du tableau. s = 1 , 2 si V ou H commence resp.

    int matrix[m][n];

    play* game = malloc(m*n*sizeof(play));

    init_matrix(m,n,matrix);

    game = playslow(m,n,matrix,1,1);

    printgame(game);

    andthestarterwas(game);
    andthewinneris(game);

    return 0;
}   