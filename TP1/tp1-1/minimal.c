#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de couleurs dans la palette */
#define NB_COLORS 8
static int currentColor[3]={1, 1, 1};

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

Primitive* allocPrimitive(int primType) {
	GLenum primitiveType = GL_POINTS;
	switch(primType){
		case 1:
			primitiveType=GL_POINTS;
			break;
		case 2:
			primitiveType=GL_LINES;
			break;
		case 3:
			primitiveType=GL_TRIANGLES;
			break;
		default:
			break;
	}

    /*
    On alloue un espace mémoire suffisant pour pouvoir stocker une primitive
    Attention : la fonction malloc() renvoie un void* qu'il faut impérativement caster en Primitive*.
    */
    Primitive* primitive = (Primitive*) malloc(sizeof(Primitive));
    if (!primitive) {
        return NULL;
    }
    primitive->primitiveType = primitiveType;
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

static int colors[NB_COLORS][3] = {
	{255, 0, 65},
	{255, 135, 0},
	{255, 255, 0},
	{0, 255, 175},
	{0, 225, 255},
	{65, 0, 255},
	{125, 0, 255},
	{255, 0, 230}
};

void showColors(){
	int i;
	for(i=0;i<8;i++){
		glBegin(GL_QUADS);
		glColor3ub(colors[i][0], colors[i][1],colors[i][2]);
		glVertex2f(-1 + 2. * (i*100)/WINDOW_WIDTH, -(-1 + 2. * 0));
		glVertex2f(-1 + 2. * ((i+1)*100)/WINDOW_WIDTH, -(-1 + 2. * 0));
		glVertex2f(-1 + 2. * ((i+1)*100)/WINDOW_WIDTH, -(-1 + 2. * 1));
		glVertex2f(-1 + 2. * (i*100)/WINDOW_WIDTH, -(-1 + 2. * 1));
		glEnd();		
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

  int currentType = 1;
  int nb_points=0;
  int mode = 0;

  PrimitiveList list=NULL;
  Primitive* prim = allocPrimitive(currentType);
 
  /* Boucle d'affichage */
  int loop = 1;
  while(loop) {
    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);

	glClear(GL_COLOR_BUFFER_BIT);
    if(mode==1){
    	showColors();
    }
    else{
	    drawPrimitives(list);
    }
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
            printf("%f, %f\n", x, y);
        	if(mode==1){
        		x+=1;
        		int x2=8*x/2;
        		printf("quota : %d\n", x2);
        		int i;
        		for(i=0;i<3;i++){
        			currentColor[i]=colors[x2][i];
        		}
        	}
        	else{
	        	addPointToList(allocPoint(x, y, currentColor[0], currentColor[1], currentColor[2]), &prim->points);
	        	nb_points+=1;
	        	printf("nb pts : %d\n", nb_points);
	        	printf("currentType : %d\n", currentType);

	        	if(nb_points==currentType){
	        		printf("draw OK\n");
	 	    	   	addPrimitive(prim, &list);
		       		prim = allocPrimitive(currentType);
	    	    	nb_points=0;
	        	}
        	}
          break;
          
        /* Touche clavier */
        case SDL_KEYDOWN:
        	switch(e.key.keysym.sym){
        		case SDLK_q:
        			loop=0;
        			break;
        		case SDLK_l:
        			currentType=2;
		       		prim = allocPrimitive(currentType);
        			printf("type l\n");
        			break;
        		case SDLK_t:
        			currentType=3;			
		       		prim = allocPrimitive(currentType);
        			printf("type t\n");
        			break;
        		case SDLK_p:
        			currentType=1;
		       		prim = allocPrimitive(currentType);
        			printf("type p\n");
        			break;
        		case SDLK_SPACE:
        			mode=1;
        			break;
        		default:
        			break;
        	}
          break;

         case SDL_KEYUP:
         	if(e.key.keysym.sym==SDLK_SPACE)
	         	mode=0;
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
