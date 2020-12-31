#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
struct position {
  int l;
  int c;
};

/*struct wall{
  int l1; //line1(y1) position
  int c1; //colone1(x1) position
  int l2; //line2(y2) position
  int c2; //colone2(x2) position

  /*struct position p;
  char orientaion c
};*/

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
  struct player *p1;
  struct player *p2;
  struct plateau *p;
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

void init_plateauDeJeu (struct plateauDeJeu *plateau) {
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
  
}

void movePlayer (struct player *self,int l, int c){
  self->p->l = l;
  self->p->c = c;
}


//appliquer le mouvement au plateau de jeu
// void move(struct player *p, int c, int l){

//   p->p->c = c;
//   p->p->l = l;
// }

// applique le mur créé au plateau de jeu
// void do_wall(struct plateau *plateau, char player, char orientation, int c, int l){
//   int i =0;
//   while((plateau->bufferWall[i].c1 != -1) && (i<20)){
//     ++i;
//   }
//   plateau->bufferWall[i].c1 = c;
//   plateau->bufferWall[i].l1 = l;
//   if(orientation == 'H'){
//     plateau->bufferWall[i].c2 = c+1;
//     plateau->bufferWall[i].l2 = l;
//   }
//   else{
//     plateau->bufferWall[i].c2 = c;
//     plateau->bufferWall[i].l2 = l-1;
//   }
//   if(player == 'A')--plateau->p1->nbWall;
//   else --plateau->p1->nbWall;
// }

void send_move(int l, int c){
 
  puts("MOVE"); // or "WALL H" or "WALL V"
  printf("%d\n%d\n", l, c);
}

// void send_wall(char orientation, int c, int l){
//   printf("WALL %C\n", orientation);
//   printf("%d\n", l);
//   printf("%d\n", c);
// }

//-1 non autorisé, 0 = autorisé , 1 que mouvement spécial
int moveLeft(struct plateauDeJeu *p){
  int l = p->p1->p->l;
  int c = p->p1->p->c;
  
  if(c-1<0 || accesPlateau(p->p,l-1,c-1)->mur == 'V' || accesPlateau(p->p,l,c-1)->mur == 'V'){
    return -1;
  }
  if (accesPlateau(p->p,l,c-1)->joueur == 'B' ){
    return 1;
  }
  return 0;
}

int moveUp(struct plateauDeJeu *p){
  int l = p->p1->p->l;
  int c = p->p1->p->c;
  
  if(l-1<0 || accesPlateau(p->p,l-1,c-1)->mur == 'H' || accesPlateau(p->p,l-1,c)->mur == 'H'){
    return -1;
  }
  if (accesPlateau(p->p,l-1,c)->joueur == 'B' ){
    return 1;
  }
  return 0;
}

int moveRight (struct plateauDeJeu *p){
  int l = p->p1->p->l;
  int c = p->p1->p->c;
  
  if(c+1>8 || accesPlateau(p->p,l-1,c)->mur == 'V' || accesPlateau(p->p,l,c)->mur == 'V'){
    return -1;
  }
  if (accesPlateau(p->p,l,c+1)->joueur == 'B' ){
    return 1;
  }
  return 0;
}

int moveDown(struct plateauDeJeu *p){
  int l = p->p1->p->l;
  int c = p->p1->p->c;
  
  if(l+1>8 || accesPlateau(p->p,l,c-1)->mur == 'H' || accesPlateau(p->p,l,c)->mur == 'H'){
    return -1;
  }
  if (accesPlateau(p->p,l+1,c)->joueur == 'B' ){
    return 1;
  }
  return 0;
}

void move (struct plateauDeJeu *plateau,char player, int l , int c){
  if(player =='A'){
    accesPlateau(plateau->p,l,c)->joueur='A';
    accesPlateau(plateau->p,plateau->p1->p->l,plateau->p1->p->c)->joueur=' ';
    movePlayer(plateau->p1,l,c);
    send_move (l,c);
  }
  
}


//0  move right, 1 move left, 2 move down, 3 move up
void do_move(struct plateauDeJeu *plateau, int init_move){
  switch(init_move){
    case 0:
      if(moveRight(plateau)==0){
        move(plateau, 'A', plateau->p1->p->l,plateau->p1->p->c +1);
      }   
    break;
    case 1 :
      if(moveLeft(plateau)==0){
        move(plateau, 'A', plateau->p1->p->l,plateau->p1->p->c -1);
      } 
    break;
    case 2 : 
      if(moveDown(plateau)==0){
        move(plateau, 'A', plateau->p1->p->l + 1,plateau->p1->p->c);
      }
    break;
    case 3 : 
      if(moveUp(plateau)==0){
        move(plateau, 'A', plateau->p1->p->l-1,plateau->p1->p->c);
      } 

    break;
    default:
    break ;
  }
}


#define BUFSIZE 1024
int main() {
  struct plateauDeJeu *plateau = malloc (sizeof (struct plateauDeJeu));
  init_plateauDeJeu(plateau);
  setbuf(stdout, NULL);
  char buf[BUFSIZE];
  // get the letter of the player
  fgets(buf, BUFSIZE, stdin);
  int turn = (buf[0] == 'A') ? 0 : 1;
  // initialize the state of the game
  int i = 0;
  for (;;) {
    
    if (turn == 0) { // my turn
      // int l, c;
      // l= 1;
      // c = 4;
      // // compute the action and the coordinates
      // puts("MOVE"); // or "WALL H" or "WALL V"
      // printf("%d\n%d\n", l, c);
      switch (i)
      {
      case 0:
        do_move(plateau,2);
        break;
      case 1 :
        do_move(plateau,0);
      break;
      case 2 : 
        do_move(plateau,1);
      break ; 
      case 3 : 
        do_move(plateau,3);
      break;
      default:
        break;
      }
      

      fgets(buf, BUFSIZE, stdin);
      if (strcmp(buf, "WIN\n") == 0
      || strcmp(buf, "LOSE\n") == 0) {
        break;
      }
      ++i;
    } else { // other player’s turn
      fgets(buf, BUFSIZE, stdin);
      if (strcmp(buf, "WIN\n") == 0
      || strcmp(buf, "LOSE\n") == 0) {
        break;
      }
      fgets(buf, BUFSIZE, stdin);
      int l = atoi(buf);
      fgets(buf, BUFSIZE, stdin);
      int c = atoi(buf);
      // update the state of the game
    }
    turn = 1 - turn;
  }
  return 0;
}
// ./quoridor-viewer &
// ./quoridor-server ./a.out ./quoridor-07