#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

typedef struct Point{
  float x, y;       // coordonnées
  unsigned char r, g, b;  // couleur
  struct Point* next;
} Point, *PointList;

typedef struct Primitive{
  GLenum primitiveType; // type de forme
  PointList points;   // points formant la forme
  struct Primitive* next;
} Primitive, *PrimitiveList;

/* alloue et initialise un point */
Point* allocPoint(float x, float y, unsigned int r, unsigned int g, unsigned int b){
  Point* tmp;
  tmp=malloc(sizeof(Point));
  if(!tmp){
    return NULL;
  }
  tmp->x = x;
  tmp->y = y;
  tmp->r = r;
  tmp->g = g;
  tmp->b = b;
  tmp->next=NULL;
  return tmp;
}

/* alloue et initialise une primitive */
Primitive* allocPrimitive(GLenum primitiveType){
  Primitive* tmp;
  tmp=malloc(sizeof(Primitive));
  if(tmp!=NULL){
    tmp->primitiveType=primitiveType;
    tmp->points=NULL;
    tmp->next=NULL;
    return tmp;
  }
  else{return NULL;}
}

/* ajoute un point à une liste de point */
/* permettra d'ajouter un point à la liste de point membre d'une prim */
void addPointToList(Point* point, PointList* list){
  if(*list==NULL){
    *list=point; }
  else{
    addPointToList(point, &(*list)->next);
  }
}

/* ajoute une primitive à la liste de primitives */
void addPrimitive(Primitive* primitive, PrimitiveList* list){
  if(*list==NULL){
    *list=primitive;
  }
  else{ 
    addPrimitive(primitive, &(*list)->next);  
  }
}

/* dessine les points correspondant à une primitive */
void drawPoints(PointList list){ 
  while(list){
    glColor3ub(list->r, list->g, list->b );
    glVertex2f(list->x, list->y);
    list=list->next;
  }
}

/* dessine une primitive en fonction de sa liste de points */
void drawPrimitive(PrimitiveList list){
  while(list!=NULL){
    glBegin(list->primitiveType);
    drawPoints(list->points);
    glEnd();
    list=list->next;
  }
}

/* supprime les points quand on ferme le programme */
void emptyPoints(PointList* list){
  while(*list){
    *list=NULL;
    *list=(*list)->next;
  }
}

/* supprime les points quand on ferme le programme */
void deletePoints(PointList* list){
  while(*list){
    Point* next = (*list)->next;
    free(*list);
    *list=next;
  }
}
/* supprime les primitives en fin de programme */
void deletePrimitive(PrimitiveList* list){
  while(*list){
    Primitive* next = (*list)->next;
    deletePoints(&(*list)->points);
    free(*list);
    *list=next;
  }
}

int main(int argc, char** argv) {
  /* Initialisation de la SDL */
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  
  /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
  if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  
  /* Titre de la fenêtre */
  SDL_WM_SetCaption("Oignon", NULL);
  
  glClearColor(0.1, 0.1, 0.1, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);


  //PrimitiveList list=NULL;
 // PointList current=NULL;

  //int nb_pts=0;

  /* Boucle d'affichage */
  int loop = 1;
  while(loop) {
    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();
    
    /* Placer ici le code de dessin */
   //s glClear(GL_COLOR_BUFFER_BIT);

      glBegin(GL_LINES);
      glColor3ub(255, 255, 255);
      glVertex2f(-1 + 2. * 300 / WINDOW_WIDTH, -1 + 2. * 500 / WINDOW_WIDTH);
      glVertex2f(-1 + 2. * 400 / WINDOW_WIDTH, -1 + 2. * 600 / WINDOW_WIDTH); 
      glEnd();
      //nb_pts=0;
    

    /* Boucle traitant les evenements */
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      /* L'utilisateur ferme la fenêtre : */
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }
      
      /* Quelques exemples de traitement d'evenements : */
      switch(e.type) {
        /* Clic souris */
        /*case SDL_MOUSEBUTTONUP:;
          float x = -1 + 2. * e.button.x / WINDOW_WIDTH;
          float y = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
          addPointToList(allocPoint(x, y, 255, 255, 255), &current);
          nb_pts+=1;
          break;*/

        default:
          break;
      }
    }
    /* Calcul du temps écoulé */
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    /* Si trop peu de temps s'est écoulé, on met en pause le programme */
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }
  
  SDL_GL_SwapBuffers();

  //deletePrimitive(&list);
  printf("delete prim ok\n");
  /* Liberation des ressources associées à la SDL */ 
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
