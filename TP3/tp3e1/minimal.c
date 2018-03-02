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
static unsigned int WINDOW_WIDTH = 600;
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
	for(i=-10;i<10;i++){
		glBegin(GL_LINES);
			glVertex2f(i, -10);
			glVertex2f(i, 10);
		glEnd();	
	}
	for(i=-10;i<10;i++){
		glBegin(GL_LINES);
			glVertex2f(-10, i);
			glVertex2f(10, i);
		glEnd();	
	}

	/*repère*/
	glLineWidth(5);
	/* rouge pour abscisse */
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(-10, 0);
		glVertex2f(10, 0);
	glEnd();
	/* vert pour ordonnée */
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
		glVertex2f(0, -10);
		glVertex2f(0, 10);
	glEnd();
}

void drawRoundedSquare(){	
	/* on dessine une croix (2 rect) qu'on va combler avec des ronds */
	glPushMatrix();
		glScalef(0.5, 1, 1);
		drawSquare(1);
	glPopMatrix();
	glPushMatrix();
		glScalef(1, 0.5, 1);
		drawSquare(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.25, 0.25, 0);
		glScalef(0.5, 0.5, 1);
		drawCircle(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.25, 0.25, 0);
		glScalef(0.5, 0.5, 1);
		drawCircle(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-0.25, -0.25, 0);
		glScalef(0.5, 0.5, 1);
		drawCircle(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.25, -0.25, 0);
		glScalef(0.5, 0.5, 1);
		drawCircle(1);
	glPopMatrix();
}

void drawFirstArm(){
	glPushMatrix();
		glScalef(4,4,1);
		drawCircle(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,-6.,0);
		glScalef(2, 2, 1);
		drawCircle(1);
	glPopMatrix();

	glBegin(GL_QUADS);
		glVertex2f(-2., 0);
		glVertex2f(2., 0);
		glVertex2f(1, -6);
		glVertex2f(-1, -6);
	glEnd();
}

void drawSecondArm(){
	drawRoundedSquare();

	glPushMatrix();
		glTranslatef(0,-5.,0);
		drawRoundedSquare();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,-2.5, 0);
		glScalef(0.6, 4.6, 1);
		drawSquare(1);
	glPopMatrix();
}

void drawThirdArm(){
	glPushMatrix();
		glScalef(0.6, 0.6, 1);
		drawRoundedSquare();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, -3.7, 0);
		glScalef(0.8,0.8, 1);
		drawCircle(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, -1.85, 0);
		glScalef(0.4, 3.7, 1);
		drawSquare(1);
	glPopMatrix();
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

  /* Boucle d'affichage */
  int loop = 1;
  while(loop){

    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    gluOrtho2D(-10., 10., -10., 10);
    drawLandmark();
    glColor3ub(255, 255, 255);
    //drawRoundedSquare();
    //drawFirstArm();
    //drawSecondArm();
    drawThirdArm();

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
