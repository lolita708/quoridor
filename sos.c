#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

struct position {
  int l;
  int c;
};

struct parcours{
  int actuel;
  int distance;
  int suivant;
};

struct resultat {
  int size;
  int capacity;
  struct parcours * res;
};


struct graphe {
  int *data;
  int size;
  int capacity;
  struct parcours * liaison;

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

// void graphe_destroy(struct graphe *self) {
//   free(self->graphe);
//   self=NULL;
// }
struct casee* accesPlateau (struct plateau *self, size_t l, size_t c){
  // if(l>self->lignes|| c> self->colonnes || c<0 || l <0){
  //   struct casee cas;
  //   cas.mur = ' ';
  //   cas.joueur = ' ';
  //   return &cas;
  // }
	assert (l<self->lignes);
	assert (c< self->colonnes);
	return (&self->casee[l*self->colonnes +c]);
}

int posToIndex (int l, int c){
  return (l + 9*c);
}

void indexToPos (int index ,int *l, int *c){
  *l = index%9;
  *c = index /9;
    
}
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

void grow ( struct graphe *self){
  size_t capacity = self->capacity *2;
  struct parcours *data = calloc(sizeof(struct parcours), capacity);
  memcpy(data,self->liaison,self->size*sizeof(struct parcours));
  free(self->liaison);
  self->liaison = data;
  self->capacity= capacity;
}
void graphe_push_back(struct graphe *self, struct parcours value) {
  if(self->size +1 >=self->capacity ){
    grow (self);
  }
  self->liaison[self->size]= value;
  self->size =self->size +1;
  
}

bool moveLeftPossible(struct plateauDeJeu *p, int index){
  int l,c;
  indexToPos(index,&l,&c);
 
  if(c-1<0 ){
    return false;
  }

  if (accesPlateau(p->p,l,c-1)->mur == 'V'){
    return false;
  }
  if (l-1 >=0){
    if( accesPlateau(p->p,l-1,c-1)->mur == 'V'){
    return false;
    }
  }  
  
  return true;
}

bool moveUpPossible(struct plateauDeJeu *p,int index ){
  int l,c;
  indexToPos(index,&l,&c);
  printf ("%d, %d \n", l, c);
  
  if(l-1 < 0 ){
    return false;
  }
  if(accesPlateau(p->p,l-1,c)->mur == 'H'){
    return false;
  }

  if(c-1 >= 0){
    if( accesPlateau(p->p,l-1,c-1)->mur == 'H' ){
    return false;
    }
  }
  
  
  return true;
}

bool moveRightPossible(struct plateauDeJeu *p , int index){
  int l,c;
  indexToPos(index,&l,&c);
  
  if(c+1>8 ){
    return false;
  }
  if(accesPlateau(p->p,l,c)->mur == 'V'){
    return false;
  }
  if(l-1>=0){ 
    if(accesPlateau(p->p,l-1,c)->mur == 'V' || accesPlateau(p->p,l,c)->mur == 'V'){
      return false;
    }
  }
  return true;
}

bool moveDownPossible(struct plateauDeJeu *p, int index ){
  int l,c;
  indexToPos(index,&l,&c);
  
  if (l+1>8 ){
    return false;
  }
  if(accesPlateau(p->p,l,c)->mur == 'H'){
    return false;
  }
  if (c-1 >=0){
    if (accesPlateau(p->p,l,c-1)->mur == 'H'){
      return false;
    }
  }
  return true;
  
}
void grow1 ( struct resultat *self){
  size_t capacity = self->capacity *2;
  struct parcours *data = calloc(sizeof(struct parcours), capacity);
  memcpy(data,self->res,self->size*sizeof(struct parcours));
  free(self->res);
  self->res = data;
  self->capacity= capacity;
}
void decal_droite(struct resultat *self,size_t index){
  
  size_t taille = self->size ++;
  if(self->size  >=self->capacity ){
    grow1 (self);
  }
  for(size_t i =taille ;i>index; i--){
    self->res[i]= self->res[i-1];

  }
  
  
}
void res_insert(struct resultat *self, int dist, int ind) {
  int index = 0;
  while (dist>= self->res[index].distance && index < self->size){
    ++index;
  }
  decal_droite(self,index);
  struct parcours par;
  par.distance = dist;
  par.actuel = ind;
  self->res[index]= par;
  
}

// bool indexInGraphe (struct graphe *g, int index){
//     for (size_t i = 0 ; i< g->size; ++i){
//         if (g->graphe[i].actuel == index){
//             return true;
//         }
//     }
//     return false;
// }

int indexDown (int index ){
  int l,c;
  indexToPos(index,&l,&c);
  return posToIndex(l+1,c);
}

int indexUp(int index){
  int l,c;
  indexToPos(index,&l,&c);
  return posToIndex(l-1,c);
}

int indexLeft (int index){
  int l,c;
  indexToPos(index,&l,&c);
  return posToIndex(l,c-1);
}
int indexRight (int index){
  int l,c;
  indexToPos(index,&l,&c);
  return posToIndex(l,c+1);
}
void initRes(struct resultat *g){  
  g->res = calloc(sizeof(struct parcours),5);
  g->capacity = 5;
  g->size =0;
}
void initGraphe (struct graphe *g){
  g->data = calloc(sizeof(int),81);
  g->liaison = calloc(sizeof(struct parcours),5);
  g->capacity = 5;
  g->size =0;
}
void remplirGraphe (struct plateauDeJeu *p, struct graphe *g){
  for (size_t i = 0; i<81; ++i){
    if (moveDownPossible(p,i)){
      struct parcours liaison ;
      liaison.actuel =i ;
      liaison.distance = 1;
      liaison.suivant =indexDown(i);
      graphe_push_back(g,liaison);
    }
    if (moveUpPossible(p,i)){
      struct parcours liaison ;
      liaison.actuel = i;
      liaison.distance = 1;
      liaison.suivant =indexUp(i);
      graphe_push_back(g,liaison);
    }
    if (moveLeftPossible(p,i)){
      struct parcours liaison ;
      liaison.actuel =i ;
      liaison.distance = 1;
      liaison.suivant  = indexLeft(i);
      graphe_push_back(g,liaison);
    }
     if (moveRightPossible(p,i)){
      struct parcours liaison ;
      liaison.actuel = i;
      liaison.distance = 1;
      liaison.suivant  = indexRight(i);
      graphe_push_back(g,liaison);
    }
  }
    


}
int resTopSecond (struct resultat *res){
  return res->res[0].actuel;

}


bool isResEmpty (struct resultat *res){
  return res->size == 0;
}

void popFront(struct resultat *res){ 
  
 
  for(size_t i =0 ; i< res->size-1; i++){
    res->res[i]= res->res[i+1];

  }  
  size_t taille = res->size --;
}

int adj (struct graphe *graphe, int index, int *index2){
  int i =0;
  int compte = 0;
  while (i<graphe->size && graphe->liaison[i].actuel<=index)
  {
    if(graphe->liaison[i].actuel== index){
      *index2 = graphe->liaison[i].suivant;
      index2 ++;
      compte ++;
    }
    i++;
  }
  return compte;
}

void Dijkstra (struct graphe *graphe, int index, int *dist, int *pi){
  // int dist[80];
  // int INF = 9999;
  // for (size_t i = 0; i< 80; ++i ){
  //   dist [i] = INF;
  // }
  // dist[index] = 0;
  // int pi [80];
  // for (size_t i = 0; i< 80; ++i ){
  //   pi [i] = -1;
  // }
  bool mark [81];
  for (size_t i = 0; i< 81; ++i ){
    mark [i] = false;
  }
  struct resultat *res = malloc (sizeof(struct resultat));
  initRes(res);
  res_insert(res,0,index);
  int dest[4];
  while (!isResEmpty(res)){
    int u = resTopSecond(res);
    popFront(res);
    if (mark [u]) continue;
    mark [u] = true;
    int i = adj(graphe,u, dest);
    for(i; i>0; i--){
      if (dist[ dest[i-1]]>dist[u]+1){
        dist[dest[i-1]] = dist[u]+1;
        pi[dest[i-1]]=u;
        res_insert(res,dist[dest[i-1]],dest[i-1]);
      }
    }

  }  

}

void calculMeilleur (struct plateauDeJeu *p, int *dist, int *pi){
  int distance;
  int index ;
  if (p->joueur=='A'){
    index = 8;
    distance = dist[index];
    printf("noeud : %d, dist- %d \n",index, distance);
    for(size_t c = 0; c<9; c++){
      if(dist[posToIndex(8,c)]<distance){
        distance = dist[posToIndex(8,c)];
        index = posToIndex(8,c);
        printf("noeud : %d, dist- %d \n",index, distance);
      }
    }
  }
  printf("noeud : %d, dist- %d , prec %d,\n",index, distance, pi[index]);

}

void calculDistance (struct plateauDeJeu *p, struct graphe *g){
  struct player *player = (p->joueur == 'A') ? p->p1 : p->p2;
  int l = player->p->l;
  int c = player->p->c;

  int index = posToIndex(l,c);
  int dist[81];
  int INF = 9999;
  for (size_t i = 0; i< 81; ++i ){
    dist [i] = INF;
  }
  dist[index] = 0;
  int pi [81];
  for (size_t i = 0; i< 81; ++i ){
    pi [i] = -1;
  }
  Dijkstra(g,index,dist,pi);

  for(int i = 0; i < 81 ; ++i ){
    printf("noeud : %d , distance- %d  : precédent- %d \n",i, dist[i], pi[i]);
  }
  calculMeilleur(p,dist,pi);

}




int main() {
  struct plateauDeJeu *plateau = malloc (sizeof (struct plateauDeJeu));
  init_plateauDeJeu(plateau,'A');
  accesPlateau(plateau->p,0,3)->mur='V';
  accesPlateau(plateau->p,7,4)->mur='H';
  struct graphe *graphe = malloc(sizeof(struct graphe));
  initGraphe(graphe);
  remplirGraphe(plateau,graphe);
  for(int i = 0 ; i < 10 ; ++ i){
    printf("[%d , %d , %d ] \n", graphe->liaison[i].actuel,graphe->liaison[i].distance,  graphe->liaison[i].suivant);
  }
  //printf("%d ", posToIndex(8,8));

  calculDistance(plateau,graphe);

}