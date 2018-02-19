#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 1000;
static unsigned int WINDOW_HEIGHT = 500;
/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;
/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/* entiers définissant la couleur utilisée. Défaut : blanc */
static unsigned int RED = 255;
static unsigned int GREEN = 255;
static unsigned int BLUE = 255;

/* char définissant le mode (dessin ou choix couleur)*/
/* default : dessin */
static unsigned char MODE = 'd';
/* int associant le nb de points nécesaire pour dessiner la primitive en cours */
/* default : point */
static unsigned char PRIM = 1;

/* affiche la palette de couleurs */
void showColors(){
	/* blanc */
    glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
	glVertex2f(-1 + 2. * 0, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 200 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 200 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glVertex2f(-1 + 2. * 0, -(-1 + 2. * 1));
	glEnd(); 
	/* bleu */
    glColor3ub(0, 205, 255);
	glBegin(GL_QUADS);
	glVertex2f(-1 + 2. * 200 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 400 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 400 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glVertex2f(-1 + 2. * 200 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glEnd();

	/* vert */
    glColor3ub(85, 255, 0);
	glBegin(GL_QUADS);
	glVertex2f(-1 + 2. * 400 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 600 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 600 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glVertex2f(-1 + 2. * 400 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glEnd();  

	/* orange */
	glColor3ub(255, 155, 55);
	glBegin(GL_QUADS);
	glVertex2f(-1 + 2. * 600 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 800 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 800 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glVertex2f(-1 + 2. * 600 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glEnd();

	/* rose */
	glColor3ub(255, 55, 145);
	glBegin(GL_QUADS);
	glVertex2f(-1 + 2. * 800 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 1, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 1, -(-1 + 2. * 1));
	glVertex2f(-1 + 2. * 800 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glEnd();
}

typedef struct Point{
	float x, y;				// coordonnées
	unsigned char r, g, b;	// couleur
	struct Point* next;
} Point, *PointList;

typedef struct Primitive{
	GLenum primitiveType;	// type de forme
	PointList points;		// points formant la forme
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
		deletePoints(&(*list)->next);
		free(*list);
		*list=next;
	}
}

/* gère le redimensionnement de la fenêtre */
void resize(){
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
}

/* en mode dessin, permet de changer la couleur des formes */
void changeColors(int x){
	if(x<200){	RED = 255; GREEN = 255; BLUE = 255; }
	else if(x<400){ RED = 0; GREEN = 205; BLUE = 255; }
	else if(x<600){ RED = 85; GREEN = 255; BLUE = 0; }
	else if(x<800){ RED = 255; GREEN = 155; BLUE = 55; }
	else if(x<WINDOW_WIDTH){ RED = 255; GREEN = 55; BLUE = 145; }
}

/*void showList(PrimitiveList list){
	while(list!=NULL){
		printf("IN LOOP1 \n");
		if(list->points==NULL){
			printf("LISTE POINTS NULLE\n");
		}
		while(list->points!=NULL){
			printf("%f, %f\n", list->points->x, list->points->y),
			list->points=list->points->next;
		}
		list=list->next;
	}
}*/

int main(int arg, char** argv) {

	/* Initialisation de la SDL */
	if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    	fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    	return EXIT_FAILURE;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

	/* Ouverture d'une fenêtre et création d'un contexte OpenGL */
  	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
    	fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    	return EXIT_FAILURE;
  	}

	/* On crée la liste des primitives avec une primitive point par défaut */
	PrimitiveList list = NULL;
	Primitive* prim = allocPrimitive(GL_POINTS);
	PrimitiveList lastPrim = prim;
	addPrimitive(prim, &primitives);

	PrimitiveList currentPrim;
	/* nb de points dessinés pour la primitive en cours */
	int NB_POINTS = 0;

  	/* Titre de la fenêtre */
 	SDL_WM_SetCaption("Anticonstitutionnellement", NULL);
  	
  	glClear(GL_COLOR_BUFFER_BIT);
  	int loop = 1;
  	while(loop){
  		/* Récupération du temps au début de la boucle */
   		Uint32 startTime = SDL_GetTicks();

   		glClear(GL_COLOR_BUFFER_BIT);

	    /* Echange du front et du back buffer : mise à jour de la fenêtre */
	    SDL_GL_SwapBuffers();

	    /* Boucle traitant les évènements */
	    SDL_Event e;

	    while(SDL_PollEvent(&e)){
	    	/* l'utilisateur ferme la fenêtre : */
	    	if(e.type == SDL_QUIT){
	    		loop=0;
	    		break;
	    	}

		    /* Les évènements */
		    switch(e.type){
		    	/* Touche clavier pressée : pour le mode et la prim */
		    	case SDL_KEYDOWN:
		    		switch(e.key.keysym.sym){
		    			case 32: // touche espace révélant la palette
		    				showColors();
		    				/* mode couleur : le clic changera la couleur au lieu de dessiner */
		    				MODE='c';
		    				break;
		    			case 108: // touche l pour dessiner des lignes
		    				printf("PRIM LINES\n");
		    				PRIM = 2;
		    				currentPrim=*allocPrimitive(GL_LINES);
		    				break;
		    			case 112: // touche p pour dessiner des points
		    				printf("PRIM POINTS\n");
		    				PRIM = 2;
		    				currentPrim=*allocPrimitive(GL_POINTS);
		    				break;
		    			case 113: // touche q pour quitter 
		    				loop =0;
		    				break;
		    			case 116: // touche t pour dessiner des triangles
		    				printf("PRIM TRIANGLES\n");
		    				PRIM = 3;
		    				currentPrim=*allocPrimitive(GL_TRIANGLES);
		    				break;
		    			default:
		    				break;
		    		}
		    		break;
		    	/* Touche clavier relachée */
		    	case SDL_KEYUP:
		    		switch(e.key.keysym.sym){
		    			case 32: // on relache la touches espace
		    				MODE='d';
		    				glColor3ub(0, 0, 0);
							glBegin(GL_QUADS);
							glVertex2f(-1 + 2. * 0, -(-1 + 2. * 0));
							glVertex2f(-1 + 2. * 1, -(-1 + 2. * 0));
							glVertex2f(-1 + 2. * 1, -(-1 + 2. * 1));
							glVertex2f(-1 + 2. * 0, -(-1 + 2. * 1));
							glEnd();
		    				break;
		    			default:
		    				break;
		    		}
		    		break;
		    	/* Souris clique */
		    	case SDL_MOUSEBUTTONUP:
		    		switch(MODE){
		    			case 'd':
		    				printf("%d\n", NB_POINTS);
		    				addPointToList(allocPoint(e.button.x, e.button.y, RED, GREEN, BLUE), &currentPrim.points);
		    				printf("AJOUT POINT OK\n");
		    				NB_POINTS+=1;
		    				printf("%d\n", NB_POINTS);
		    				/*si on a le nb de points suffisants pour dessiner la prim */
		    				if(NB_POINTS == PRIM){
		    					/* on ajoute la primitive enregistrée à la liste des formes */
		    					addPrimitive(&currentPrim, &list);
		    					printf("AJOUT PRIM OK\n");
		    					/* réinitialisation nb points pour prochaine forme */
		    					NB_POINTS=0;
		    				}
		    				//showList(list);
		    				break;
		    			case 'c':
		    				changeColors(e.button.x);
		    				break;
		    			default:
		    				break;
		    		}
		    		break;

		    	/* Redimensionnement de la fenêtre */
		    	case SDL_VIDEORESIZE:
		        	WINDOW_WIDTH = e.resize.w;
		        	WINDOW_HEIGHT = e.resize.h;
		        	resize();
		        	break;

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

		/* on dessine les formes */
		//drawPrimitive(list);	
	}
    SDL_GL_SwapBuffers();

    /*Libération des ressources associées à la SDL */
    deletePrimitive(&list);
    SDL_Quit();

    return EXIT_SUCCESS;
}