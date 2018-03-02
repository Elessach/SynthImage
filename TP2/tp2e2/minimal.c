#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define NB_SEG 40
#define PI 3.14159

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resize(){
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
}

void drawSquare(int full){
	/* couleur et remplissage */
	glColor3ub(255, 255, 255);
	if(full==1){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	}

	glBegin(GL_QUADS);
	glColor3ub(255, 255, 255);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0.5, -0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(-0.5, 0.5);
	glEnd();
}

void drawCircle(int full){
	int i; // index

	/* couleur et remplissage */
	glColor3ub(0,0,255);
	if(full==1){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	}

	glBegin(GL_POLYGON);
	for(i=0;i<=NB_SEG;i++){
		float angle=(2*PI/NB_SEG)*(float)i;
		glVertex2f(0.5*cos(angle),0.5*sin(angle));
	}
	glEnd();
}
 
void drawLandmark(){
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(-0.5, 0);
		glVertex2f(0.5, 0);
	glEnd();
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
		glVertex2f(0, -0.5);
		glVertex2f(0, 0.5);
	glEnd();
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

  /* Boucle d'affichage */
  int loop = 1;
  while(loop){
    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    drawSquare(0);	
    drawCircle(1);
	drawLandmark();
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
