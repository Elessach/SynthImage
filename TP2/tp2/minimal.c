#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

typedef struct Point {
    float x, y;
    unsigned char r, g, b;
    struct Point* next;
} Point, *PointList;

Point* allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) {
    /*
    On alloue un espace mémoire suffisant pour pouvoir stocker un point
    Attention : la fonction malloc() renvoie un void* qu'il faut impérativement caster en Point*.
    */
    Point* point = (Point*) malloc(sizeof(Point));
    if (!point) {
        return NULL;
    }
    point->x = x;
    point->y = y;
    point->r = r;
    point->g = g;
    point->b = b;
    point->next = NULL;
    return point;
}

void addPointToList(Point* point, PointList* list) {
    assert(point);
    assert(list);
    if (*list == NULL) {
        /* Si le pointeur sur la liste est vide, on le modifie pour le faire pointer sur le point donné en paramètre */
        *list = point;
    } else {
        /* Sinon, on réapplique la même fonction mais sur le point suivant */
        addPointToList(point, &(*list)->next);
    }
}

void drawPoints(PointList list) {
    while(list) {
        glColor3ub(list->r, list->g, list->b);
        glVertex2f(list->x, list->y);
        list = list->next;
    }
}

void deletePoints(PointList* list) {
    assert(list);
    while (*list) {
        Point* next = (*list)->next;
        free(*list);
        *list = next;
    }
}

typedef struct Primitive{
    GLenum primitiveType;
    PointList points;
    struct Primitive* next;
} Primitive, *PrimitiveList;

Primitive* allocPrimitive(GLenum primType) {
    /*
    On alloue un espace mémoire suffisant pour pouvoir stocker une primitive
    Attention : la fonction malloc() renvoie un void* qu'il faut impérativement caster en Primitive*.
    */
    Primitive* primitive = (Primitive*) malloc(sizeof(Primitive));
    if (!primitive) {
        return NULL;
    }
    primitive->primitiveType = primType;
    primitive->points = NULL;
    primitive->next = NULL;
    return primitive;
}

void addPrimitive(Primitive* primitive, PrimitiveList* list) {
    assert(primitive);
    assert(list);
    if (*list == NULL) {
        // Si le pointeur sur la liste est vide, on le modifie pour le faire pointer sur la primitive donnée en paramètre
        *list = primitive;
    } else {
        // Sinon, on réapplique la même fonction mais sur le point suivant
        addPrimitive(primitive, &(*list)->next);
    }
}

void drawPrimitives(PrimitiveList list) {
    while(list) {
        glBegin(list->primitiveType);
        drawPoints(list->points);
        glEnd();
        list = list->next;
    }
}

void deletePrimitive(PrimitiveList* list) {
    assert(list);
    while(*list) {
        Primitive* next = (*list)->next;
        deletePoints(&(*list)->points);
        free(*list);
        *list = next;
    }
}

void resize(){
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
}

int main(int argc, char** argv) {
  /* Initialisation de la SDL */
  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  
  /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
  if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    return EXIT_FAILURE;
  }
  
  /* Titre de la fenêtre */
  SDL_WM_SetCaption("Benzona", NULL);
  glClearColor(0,0,0,0);

  int nb_points=0, first=1;
  float firstCoord[2]={0,0};

  PrimitiveList list=NULL;
  Primitive* prim = allocPrimitive(GL_LINES);
 
  /* Boucle d'affichage */
  int loop = 1;
  while(loop){
    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);

	glClear(GL_COLOR_BUFFER_BIT);
    drawPrimitives(list);
    
    /* Echange du front et du back buffer : mise à jour de la fenêtre */
	SDL_GL_SwapBuffers();

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
        case SDL_MOUSEBUTTONUP:;
        	/* transformation des coordonnées */
          	float x = -1 + 2. * e.button.x / WINDOW_WIDTH;
            float y = -(-1 + 2. * e.button.y / WINDOW_HEIGHT);
            if(first==1){
            	firstCoord[0]=x;
            	firstCoord[1]=y;
            	first=0;
            }
        	addPointToList(allocPoint(x, y, 255, 255, 255), &prim->points);
        	nb_points+=1;
        	if(nb_points==2){
 	    	   	addPrimitive(prim, &list);
	       		prim = allocPrimitive(GL_LINES);
	        	addPointToList(allocPoint(x, y, 255, 255, 255), &prim->points);
    	    	nb_points=1;
        	}
        	
        	if(e.button.button==SDL_BUTTON_RIGHT){
	        	addPointToList(allocPoint(x, y, 255, 255, 255), &prim->points);
	        	nb_points+=1;
	        	if(nb_points==2){
	 	    	   	addPrimitive(prim, &list);
		       		prim = allocPrimitive(GL_LINES);
		        	addPointToList(allocPoint(x, y, 255, 255, 255), &prim->points);
	    	    	nb_points=1;
	        	}
        		prim = allocPrimitive(GL_LINES);
	        	addPointToList(allocPoint(x, y, 255, 255, 255), &prim->points);
        		addPointToList(allocPoint(firstCoord[0], firstCoord[1], 255, 255, 255), &prim->points);
        		addPrimitive(prim, &list);
        	}
	        break;
          
        /* Touche clavier */
        case SDL_KEYDOWN:
        	switch(e.key.keysym.sym){
        		case SDLK_q:
        			loop=0;
        			break;
        		default:
        			break;
        	}
          break;
         
         case SDL_VIDEORESIZE:
         	WINDOW_WIDTH=e.resize.w;
         	WINDOW_HEIGHT=e.resize.h;
         	resize();

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

  /* Liberation des ressources associées à la SDL */ 
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
