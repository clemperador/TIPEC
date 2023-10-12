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
    if(i >= m){return false;}
    if(mat[i][j] != 0 || mat[i+1][j] != 0){return false;}
    return true;
}

bool legalhori(int m, int n, int mat[][n], int i, int j){
    if(j >= n){return false;}
    if(mat[i][j] != 0 || mat[i][j+1] != 0){return false;}
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
        for(int j = 0; j<n ; j++){
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
        for(int i = 0; i<m ; i++){
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
    int i = rand() % m;
    int j = rand() % n;
    while(!legalvert(m,n,mat,i,j)){
        i = rand() % m; j = rand() % n;}

    mat[i][j] = 1;
    mat[i+1][j] = 1;
    p.pla = 1; p.pos_x = i; p.pos_y = j;
    return p;
}

play placerandhori(int m, int n, int mat[][n]){ // Places horizontal domino at (i)(j) and (i)(j+1)
    play p;
    int i = rand() % m; int j = rand() % n;
    while(!legalhori(m,n,mat,i,j)){
        i = rand() % m;
        j = rand() % n;}

    mat[i][j] = 2;
    mat[i][j+1] = 2;
    p.pla = 2; p.pos_x = i; p.pos_y = j;
    return p;
}

play* playrandom(int m, int n, int mat[][n], int s){ 
    init_matrix(m,n,mat);
    int c = s;
    play* game = malloc(m*n*sizeof(play));
    int i = 0;
    if(c == 1){
        while(!(nomorevert(m,n,mat) || nomorehori(m,n,mat))){
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
        while(!(nomorehori(m,n,mat) || nomorevert(m,n,mat))){
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
    
    if (nomorevert(m,n,mat)){
        game[i].pla = 1; game[i].pos_x = -1;
    }
    else if (nomorehori(m,n,mat)){
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
        printgame(game);
        printmat(m,n,mat);
        if(andthewinneris(game) == 2)
        {printf("GAME %d: H\n", i);h++;} else {printf("GAME %d: V\n\n", i);v++;}
        }
    
    printf("Horizontal won %d/%d games. Vertical won %d/%d.\n",h,c,v,c);
    return h;
}

int main(int argc, char *argv[]) {

    int m = 40;
    int n = 40;
    int ng = 10000;

    int mat[m][n];
    init_matrix(m,n,mat);
    int cpt = 0;
    
    double per1;
    double per2;

    // First cycle. Vertical Starts.
    system("clear");
    printf("Loading...\n");
    for(int j = 0;j<ng;j++){
        init_matrix(m,n,mat);
        play* game;
        game = playrandom(m,n,mat,1);
        
        if(andthewinneris(game) == 1){
            cpt++;}
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
        
    }

    per2 = (double)cpt2/(double)ng;


    system("clear");
    printf("== %dx%d BOARD RESULTS: ==\n\n",m,n);

    printf("When (V) starts, they win %d out of %d games. (%f\%)\n", cpt, ng, per1*100);

    printf("When (H) starts, they win %d out of %d games. (%f\%)\n\n", cpt2, ng, per2*100);

    double totalperh = 
    (double)((ng-cpt)+(cpt2))/(2*(double)ng);

    printf("Total H wins: %d/%d (%f\%).\n",(ng-cpt)+(cpt2),2*ng, totalperh*100);
    printf("Total V wins: %d/%d (%f\%).\n",cpt+(ng-cpt2),2*ng,(1-totalperh)*100);

    printf("\n");

    totalperh = 
    (double)(cpt+cpt2)/(2*(double)ng);

    printf("Total 1P wins: %d/%d (%f\%).\n",
    cpt+cpt2, 2*ng, totalperh*100);
    printf("Total 2P wins: %d/%d (%f\%).\n",(ng-cpt)+(ng-cpt2),2*ng, (1-totalperh)*100);

    printf("\n");

    printf("=========================\n");
    printf("\n\n\n");

    return 0;

}   
