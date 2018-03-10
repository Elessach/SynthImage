#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

#define SIZE 5
#define NB_SEG 100
#define PI 3.14159

void resizeViewport(){
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-SIZE, SIZE, -SIZE, SIZE);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

/* dessine un repère */ 
void drawLandmark(){
	/* quadrillage blanc */
	glLineWidth(0.75);
	glColor3ub(255, 255, 255);
	int i;
	for(i=-SIZE;i<SIZE;i++){
		glBegin(GL_LINES);
			glVertex2f(i, -SIZE);
			glVertex2f(i, SIZE);
		glEnd();	
	}
	for(i=-SIZE;i<SIZE;i++){
		glBegin(GL_LINES);
			glVertex2f(-SIZE, i);
			glVertex2f(SIZE, i);
		glEnd();	
	}

	/*repère*/
	glLineWidth(5);
	/* rouge pour abscisse */
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(-SIZE, 0);
		glVertex2f(SIZE, 0);
	glEnd();
	/* vert pour ordonnée */
	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
		glVertex2f(0, -SIZE);
		glVertex2f(0, SIZE);
	glEnd();
}

/* dessine un carré rempli ou non */
void drawMark(){
	glBegin(GL_QUADS);
		glVertex2f(-0.025, 0);
		glVertex2f(0.025, 0);
		glVertex2f(0.025, 0.75);
		glVertex2f(-0.025, 0.75);
	glEnd();
}

void drawCircle(int full, int borderWidth){
	/* détermination de la largeur de la bordure */
	glLineWidth(borderWidth);

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

/* récupère l'heure actuelle */
void getTime(int (*currentTime)[3]){
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	/* case 0 : heure ; case 1 : minutes ; case 2 : secondes */
	(*currentTime)[0]=(timeinfo->tm_hour);
	(*currentTime)[1]=(timeinfo->tm_min);
	(*currentTime)[2]=(timeinfo->tm_sec);
}

void drawDial(){
	/* fond cadran */
	glPushMatrix();
		glScalef(10,10,0);
		glColor3ub(200, 200, 200);
		drawCircle(1, 1);
	glPopMatrix();

	int i;
	for(i=0;i<60;i++){
		glPushMatrix();
			glTranslatef(4*cos(PI/30*i), 4*sin(2*PI/60*i), 0);
			glRotatef(-90+6*i,0.0,0.0,1.0);
			if(i%5==0)
				glScalef(2, 1, 1);
			glColor3ub(0,0,0);
			drawMark();
		glPopMatrix();	
	}
}

void drawNeedles(int time[3]){
	/* heure */
	int heure;
	if(time[0]>=12)
		heure=time[0]-12;
	else
		heure=time[0];
	glPushMatrix();
		glRotatef(-(360/12*(heure +1)), 0.0, 0.0, 1.0);
		glScalef(2,4,1);
		glColor3ub(0,0,0);
		drawMark();
	glPopMatrix();

	/* minutes */
	glPushMatrix();
		glRotatef(-(360/60*time[1]), 0.0, 0.0, 1.0);
		glScalef(2,5,1);
		glColor3ub(0,0,0);
		drawMark();
	glPopMatrix();

	/* secondes */
	glPushMatrix();
		glRotatef(-(360/60*time[2]), 0.0, 0.0, 1.0);
		glScalef(1,5.5,1);
		glColor3ub(100,100,100);
		drawMark();
	glPopMatrix();

}

int main(int argc, char** argv) {

    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Ouverture d'une fenêtre et création d'un contexte OpenGL
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("TIC FUCKING TAC", NULL);
    resizeViewport();

    /* tableau qui va contenir l'heure */
	int time[3];

	int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        /* on récupère l'heure */
		getTime(&time);
		
		drawDial();
		drawNeedles(time);

		//drawLandmark();

        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            switch(e.type) {

                case SDL_QUIT:
                    loop = 0;
                    break;

                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    resizeViewport();

                default:
                    break;
            }
        }

        SDL_GL_SwapBuffers();
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
