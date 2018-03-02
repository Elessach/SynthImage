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
#define TAILLE 15

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
	for(i=-TAILLE;i<TAILLE;i++){
		glBegin(GL_LINES);
			glVertex2f(i, -TAILLE);
			glVertex2f(i, TAILLE);
		glEnd();	
	}
	for(i=-TAILLE;i<TAILLE;i++){
		glBegin(GL_LINES);
			glVertex2f(-TAILLE, i);
			glVertex2f(TAILLE, i);
		glEnd();	
	}

	/*repère*/
	glLineWidth(5);
	/* rouge pour abscisse */
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(-TAILLE, 0);
		glVertex2f(TAILLE, 0);
	glEnd();
	/* vert pour ordonnée */
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
		glVertex2f(0, -TAILLE);
		glVertex2f(0, TAILLE);
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

void varyAngle(int* angle, int a, int b, int* plus){
	if(*plus == 1){
		if(*angle<b)
			*angle+=1;
		else if(*angle==b)
			*plus=0;
	} 
	else{
		if(*angle>a)
			*angle-=1;
		else if(*angle==a){
			*plus=1;
		}
	}
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

	/* angles et booléens indiquant si on est arrivés au max ou min de l'intervalle */
	int alpha=0, beta=0, gamma=0, delta=0; 
	int plusA = 1, plusB =1, plusG=1, plusD =1;

  /* Boucle d'affichage */
  int loop = 1;
  while(loop){

    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    /* varyAngle fait varier l'angle dans un intervale donné */
    varyAngle(&alpha, 0, 135, &plusA);
    varyAngle(&beta, -10, 0, &plusB);
    varyAngle(&gamma, 0, 45, &plusG);
    varyAngle(&delta, -90, 90, &plusD);

    glLoadIdentity();
    gluOrtho2D(-TAILLE, TAILLE, -TAILLE, TAILLE);
    drawLandmark();

    glColor3ub(255, 0, 0);
    glPushMatrix();
	    glRotatef(alpha,0.0,0.0,1.0);
	    drawFirstArm();
	    glPushMatrix();
		    glColor3ub(0, 0, 255);
		    // on garde la rotation puisque on rotate par rapport au precedent angle
	    	glTranslatef(0, -6, 0);
	    	glRotatef(beta, 0.0, 0.0, 1.0);
	    	drawSecondArm();
	    	glPushMatrix();
	    		    glColor3ub(0, 255, 0);
	    		    glTranslatef(0, -5, 0);
	    		    glPushMatrix();
	    		   		glRotatef(gamma, 0.0, 0.0, 1.0);
	    		   		drawThirdArm();
	    		   	glPopMatrix();
	    		   	glPushMatrix();
	    		   		glColor3ub(255, 255, 0);
	    		   		glRotatef(delta, 0.0, 0.0, 1.0);
	    		   		drawThirdArm();
	    		   	glPopMatrix();
	    	glPopMatrix();
	  	glPopMatrix();
    glPopMatrix();
 


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
