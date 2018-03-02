#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#define NB_SEG 40
#define PI 3.14159

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/* Gère le redimensionnement de la fenêtre */
void resize(){
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-4., 4., -3., 3.);
}

/* dessine un carré rempli ou non */
void drawSquare(int full){
	/* remplissage */
	if(full==1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	

	glBegin(GL_QUADS);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0.5, -0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(-0.5, 0.5);
	glEnd();
}

/* dessine un cercle rempli ou non */
void drawCircle(int full){
	int i;
	/* couleur et remplissage */
	if(full==1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	

	/* dessine un polygone en fonction du nb de côtés voulus */
	/* plus le nb de côtés est grand, plus on se rapproche d'un cercle */
	glBegin(GL_POLYGON);
	for(i=0;i<=NB_SEG;i++){
		float angle=(2*PI/NB_SEG)*(float)i;
		glVertex2f(0.5*cos(angle),0.5*sin(angle));
	}
	glEnd();
}
 
/* dessine un repère */ 
void drawLandmark(){
	/* quadrillage blanc */
	glLineWidth(0.75);
	glColor3ub(255, 255, 255);
	int i;
	for(i=-4;i<4;i++){
		glBegin(GL_LINES);
			glVertex2f(i, -3);
			glVertex2f(i, 3);
		glEnd();	
	}
	for(i=-3;i<3;i++){
		glBegin(GL_LINES);
			glVertex2f(-4, i);
			glVertex2f(4, i);
		glEnd();	
	}

	/*repère*/
	glLineWidth(5);
	/* rouge pour abscisse */
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(-4, 0);
		glVertex2f(4, 0);
	glEnd();
	/* vert pour ordonnée */
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
		glVertex2f(0, -3);
		glVertex2f(0, 3);
	glEnd();
}

int randInterv(int a, int b){
	return rand()%(b-a) +a;
}

int main(int argc, char** argv) {
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
  
	/* Titre de la fenêtre */
	SDL_WM_SetCaption("Benzona", NULL);
	resize();

	/* coordonnées pour la translation */
	float coordTransX = 0, coordTransY = 0;
	/* coordonnées pour la rotation */
	float coordRotX=0, coordRotY=0, angle = 0;

	/* booléens */
	int rotate = 0;

  /* Boucle d'affichage */
  int loop = 1;
  while(loop){

    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	/* carré rouge */
  	glLoadIdentity();
    gluOrtho2D(-4., 4., -3., 3.);
    glRotatef(45, 0.0,0.0,1.0);
    glTranslatef(2,0,0);
    glColor3ub(255, 0,0);
    drawSquare(1);

    /* carré violet */
  	glLoadIdentity();
    gluOrtho2D(-4., 4., -3., 3.);
    glTranslatef(2,0,0);
    glRotatef(45, 0.0,0.0,1.0);
    glColor3ub(255, 0, 255);
    drawSquare(1);
	
	/* repère */
	glLoadIdentity();
    gluOrtho2D(-4., 4., -3., 3.);
    drawLandmark();

    /* rond bleu */
    glLoadIdentity();
    gluOrtho2D(-4., 4., -3., 3.);
    glTranslatef(randInterv(-4, 4), randInterv(-3,3), 0);
    glColor3ub(0,0,255);
    drawCircle(1);

    /* si rotation activé par clic droit, change l'angle */
    if(rotate == 1)
    	angle = atan(coordRotY/coordRotX)*180/2*PI;

    /* carré jaune */
  	glLoadIdentity();
    gluOrtho2D(-4., 4., -3., 3.);
    glTranslatef(coordTransX, coordTransY, 0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3ub(255, 244, 140);
    drawSquare(1);

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
      switch(e.type){
      	/* clic de souris relevé */
      	case SDL_MOUSEBUTTONUP:
      		/* clic gauche : on déplace le carré jaune aux coordonnées correspondantes */
      		if(e.button.button==SDL_BUTTON_LEFT){
      			coordTransX = -4 + 8. * e.button.x / WINDOW_WIDTH;
      			coordTransY = -(-4 + 8. * e.button.y / WINDOW_HEIGHT);
     		}
     		/* relèvement du clic droit : rotation off (est on que quand appuyé) */
     		else if(e.button.button==SDL_BUTTON_RIGHT){
     			rotate = 0;
     		}
	    	break;	

	    /* clic de souris appuyé */
	    case SDL_MOUSEBUTTONDOWN:
	    	/* activation de la rotation */
	    	if(e.button.button==SDL_BUTTON_RIGHT)
		    	rotate = 1;	 	   		
     		break;

     	/* mouvement de la souris */
     	case SDL_MOUSEMOTION:
     		/* si rotation on change l'angle */
     		if(rotate==1){
	     		coordRotX = -4 + 8. * e.button.x / WINDOW_WIDTH;
	    	 	coordRotY = -(-4 + 8. * e.button.y / WINDOW_HEIGHT);
     		}
     		break;
 
 		/* redimensionnement de la fenêtre */
        case SDL_VIDEORESIZE:
        	/* on actualise les dimensions */
         	WINDOW_WIDTH=e.resize.w;
         	WINDOW_HEIGHT=e.resize.h;
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
  }
  SDL_GL_SwapBuffers();

  /* Liberation des ressources associées à la SDL */ 
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
