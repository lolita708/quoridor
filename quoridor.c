#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
struct position {
  int l;
  int c;
};

struct player {
  char name;
  struct position *p;
  int nbWall ;
};


struct plateau {
  size_t lignes;
  size_t colonnes;
  struct casee *casee;
};

struct casee {
  char mur; //h ou v 
  char joueur;
};

struct plateauDeJeu {
  char joueur;
  char adversaire;
  struct player *p1;
  struct player *p2;
  struct plateau *p;
  //char board[9][9];
};

struct casee* accesPlateau (struct plateau *self, size_t l, size_t c){
	assert (l<self->lignes);
	assert (c< self->colonnes);
	return (&self->casee[l*self->colonnes +c]);
}

//struct position casePlateau (

void init_player (struct player *p, char nom){
  p->p = malloc (sizeof(struct position));
  if(nom == 'A')
  {
    p->name = nom;
    p->p->l = 0;
    p->p->c = 4;
    p->nbWall = 10;
  }
  else 
  {
    p->name = nom;
    p->p->l = 8;
    p->p->c = 4;
    p->nbWall = 10;
  }
}

void init_plateau (struct plateau *p){
  p->lignes = 9;
  p->colonnes = 9;
  struct casee *c = calloc (9*9,sizeof(struct casee));
  p->casee = c;
  
}

void init_plateauDeJeu (struct plateauDeJeu *plateau,char joueur) {
  struct player *p1 = malloc (sizeof(struct player));
  init_player(p1, 'A');
  struct player *p2 = malloc (sizeof(struct player));
  init_player(p2, 'B');
  plateau->p1	= p1;
  plateau->p2 = p2;
  plateau->p = malloc(sizeof(struct plateau));
  init_plateau (plateau->p);
  accesPlateau(plateau->p,0,4)->joueur = 'A';
  accesPlateau(plateau->p,8,4)->joueur = 'B';
  plateau->joueur = joueur;
  plateau->adversaire = (joueur == 'A') ? 'B' :'A';  
}

void movePlayer (struct player *self,int l, int c){
  self->p->l = l;
  self->p->c = c;
}



void send_move(int l, int c){
 
  puts("MOVE"); // or "WALL H" or "WALL V"
  printf("%d\n%d\n", l, c);
}

void send_wall(char orientation, int c, int l){
  if(orientation=='H'){
    puts("WALL H"); // or "WALL H" or "WALL V"
    printf("%d\n%d\n", l, c);
  }else {
    puts("WALL V"); // or "WALL H" or "WALL V"
    printf("%d\n%d\n", l, c);
  }
  
}

//-1 non autorisé, 0 = autorisé , 1 que mouvement spécial
int moveLeft(struct plateauDeJeu *p){
    struct player *player = (p->joueur == 'A') ? p->p1 : p->p1;
  int l = player->p->l;
  int c = player->p->c;
  
  if(c-1<0 || accesPlateau(p->p,l-1,c-1)->mur == 'V' || accesPlateau(p->p,l,c-1)->mur == 'V'){
    return -1;
  }
  if (accesPlateau(p->p,l,c-1)->joueur == p->adversaire ){
    return 1;
  }
  return 0;
}

int moveUp(struct plateauDeJeu *p){
    struct player *player = (p->joueur == 'A') ? p->p1 : p->p1;
  int l = player->p->l;
  int c = player->p->c;
  
  if(l-1<0 || accesPlateau(p->p,l-1,c-1)->mur == 'H' || accesPlateau(p->p,l-1,c)->mur == 'H'){
    return -1;
  }
  if (accesPlateau(p->p,l-1,c)->joueur == p->adversaire ){
    return 1;
  }
  return 0;
}

int moveRight (struct plateauDeJeu *p){
  struct player *player = (p->joueur == 'A') ? p->p1 : p->p1;
  int l = player->p->l;
  int c = player->p->c;
  
  if(c+1>8 || accesPlateau(p->p,l-1,c)->mur == 'V' || accesPlateau(p->p,l,c)->mur == 'V'){
    return -1;
  }
  if (accesPlateau(p->p,l,c+1)->joueur == p->adversaire ){
    return 1;
  }
  return 0;
}

int moveDown(struct plateauDeJeu *p){
    struct player *player = (p->joueur == 'A') ? p->p1 : p->p1;
  int l = player->p->l;
  int c = player->p->c;
  
  if(l+1>8 || accesPlateau(p->p,l,c-1)->mur == 'H' || accesPlateau(p->p,l,c)->mur == 'H'){
    return -1;
  }
  if (accesPlateau(p->p,l+1,c)->joueur == p->adversaire ){
    return 1;
  }
  return 0;
}

int moveSaute(struct plateauDeJeu *p){
    struct player *player = (p->joueur == 'A') ? p->p1 : p->p1;
  int l = player->p->l;
  int c = player->p->c;
  
  if(l+2>8 || accesPlateau(p->p,l,c-1)->mur == 'H' || accesPlateau(p->p,l,c)->mur == 'H'||accesPlateau(p->p,l+1,c-1)->mur == 'H'||accesPlateau(p->p,l+1,c)->mur == 'H'){
    return -1;
  }
  return 0;
}


void move(struct plateauDeJeu *plateau,char player, int l , int c){
  if(player =='A'){
    accesPlateau(plateau->p,l,c)->joueur='A';
    accesPlateau(plateau->p,plateau->p1->p->l,plateau->p1->p->c)->joueur=' ';
    movePlayer(plateau->p1,l,c);
    
  }else{
    accesPlateau(plateau->p,l,c)->joueur='B';
    accesPlateau(plateau->p,plateau->p2->p->l,plateau->p2->p->c)->joueur=' ';
    movePlayer(plateau->p2,l,c);
  }
  if(player == plateau->joueur){
    send_move (l,c);
  }
  
}

void doWall(struct plateauDeJeu *plateau,char player, int l , int c, char orientation){
  if(player =='A' && plateau->p1->nbWall > 0){    
    accesPlateau(plateau->p,l,c)->mur = orientation;
    plateau->p1->nbWall = plateau->p1->nbWall - 1;
    
  }else{
    accesPlateau(plateau->p,l,c)->mur = orientation;
    plateau->p2->nbWall = plateau->p2->nbWall - 1;
  }
   if(player == plateau->joueur){
    send_wall (orientation,c,l);
  }
  
}

//0  move right, 1 move left, 2 move down, 3 move up, 4 saute mouton, 5 diag gauche, 6 diag droite
void do_move(struct plateauDeJeu *plateau, int init_move){
  struct player *player = (plateau->joueur == 'A') ? plateau->p1 : plateau->p2;
  int l = player->p->l;
  int c = player->p->c;
  
  switch(init_move){
    case 0:
      if(moveRight(plateau)==0){
        move(plateau, plateau->joueur, l,c +1);
      }   
    break;
    case 1 :
      if(moveLeft(plateau)==0){
        move(plateau,  plateau->joueur, l,c -1);
      } 
    break;
    case 2 : 
      if(moveDown(plateau)==0){
        move(plateau,  plateau->joueur, l + 1,c);
      }
    break;
    case 3 : 
      if(moveUp(plateau)==0){
        move(plateau,  plateau->joueur,l-1,c);
      }

    break;
    case 4 :
      if(moveSaute(plateau) ==0){
        move(plateau, plateau->joueur,plateau->p1->p->l+2,plateau->p1->p->c);
      }
    break;
    case 5 : 
    break;
    case 6 : 
    break;
    default:
    break ;
  }
}

void calculMoveA (struct plateauDeJeu * plateau, int tour){
  if(tour<3){
    do_move(plateau,2);
  }
  if(tour == 3 ){
    doWall(plateau,'A', plateau->p1->p->l-1,plateau->p1->p->c,'H');
  }
  if (tour == 4){
    doWall(plateau,'A', plateau->p1->p->l-1,plateau->p1->p->c-2,'H');
  }
  if(tour == 5){
    doWall(plateau,'A', plateau->p1->p->l,plateau->p1->p->c+1,'V');
    
  }
  if (tour == 6){
    doWall(plateau,'A', plateau->p1->p->l,plateau->p1->p->c-3,'V');
  }
  if (tour >6 ){
    if(moveDown(plateau)==0){
      move(plateau, 'A', plateau->p1->p->l + 1,plateau->p1->p->c);
    }
    if(moveLeft(plateau)==0){
      move(plateau, 'A', plateau->p1->p->l,plateau->p1->p->c -1);
    }
    if(moveRight(plateau)==0){
        move(plateau, 'A', plateau->p1->p->l,plateau->p1->p->c +1);
    }  
    if(moveUp(plateau)==0){
      move(plateau, 'A', plateau->p1->p->l-1,plateau->p1->p->c);
    }
  }
  
}

void calculMoveB (struct plateauDeJeu * plateau, int tour){
  if(tour<3){
    do_move(plateau,3);
  }
  if(tour == 3 ){
    doWall(plateau,'B', plateau->p2->p->l,plateau->p2->p->c-1,'H');
  }
  if (tour == 4){
    doWall(plateau,'B', plateau->p2->p->l,plateau->p2->p->c+1,'H');
  }
  if(tour == 5){
    
    doWall(plateau,'B', plateau->p2->p->l-1,plateau->p2->p->c+2,'V');
  }
  if (tour == 6){
    doWall(plateau,'B', plateau->p2->p->l-1,plateau->p2->p->c-2,'V');
  }
  if (tour >6 ){
    if(moveUp(plateau)==0){
      do_move(plateau,3);
    }
    // if(moveLeft(plateau)==0){
    //   do_move(plateau,1);
    // }
    // if(moveRight(plateau)==0){
    //   do_move(plateau,0);
    // }  
    // if(moveDown(plateau)==0){
    //   do_move(plateau,2);
    // }
  }
  
}


#define BUFSIZE 1024
int main() {
  struct plateauDeJeu *plateau = malloc (sizeof (struct plateauDeJeu));
  
  setbuf(stdout, NULL);
  char buf[BUFSIZE];
  // get the letter of the player
  fgets(buf, BUFSIZE, stdin);
  int turn = (buf[0] == 'A') ? 0 : 1;
  char joueur = buf[0];
  init_plateauDeJeu(plateau,joueur);
  // initialize the state of the game
  int i = 0;
  for (;;) {
    
    if (turn == 0) { // my turn
      
      // int l,c;
      // l=8;
      // c=5;
      // puts("MOVE"); // or "WALL H" or "WALL V"
      // printf("%d\n%d\n", l, c);
      if(plateau->joueur=='A'){
        calculMoveA(plateau,i);
      }else {
        calculMoveB(plateau,i);
      }

      fgets(buf, BUFSIZE, stdin);
      if (strcmp(buf, "WIN\n") == 0
      || strcmp(buf, "LOSE\n") == 0) {
        break;
      }
      ++i;
    } else { 
      // other player’s turn
      fgets(buf, BUFSIZE, stdin);
      if (strcmp(buf, "WIN\n") == 0 || strcmp(buf, "LOSE\n") == 0) {
        break;
      }
      if (strcmp(buf, "MOVE\n")== 0){
        fgets(buf, BUFSIZE, stdin);
        int l = atoi(buf);
        fgets(buf, BUFSIZE, stdin);
        int c = atoi(buf);
        move(plateau,plateau->adversaire,l,c);
      }else if(strcmp(buf, "WALL H\n") == 0){
        fgets(buf, BUFSIZE, stdin);
        int l = atoi(buf);
        fgets(buf, BUFSIZE, stdin);
        int c = atoi(buf);
        doWall(plateau, plateau->adversaire, l , c, 'H');
      }else{
        fgets(buf, BUFSIZE, stdin);
        int l = atoi(buf);
        fgets(buf, BUFSIZE, stdin);
        int c = atoi(buf);
        doWall(plateau, plateau->adversaire, l , c, 'V');
      }
      
      // update the state of the game
    }
    turn = 1 - turn;
  }
  return 0;
}
// ./quoridor-viewer &
// ./quoridor-server ./a.out ./quoridor-07
//chmod +x ./quoridor-viewer
// sudo apt install libsndio7.0 