#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static unsigned int WINDOW_WIDTH = 1000;
static unsigned int WINDOW_HEIGHT = 1000;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

#define SIZE 26
#define NB_SEG 100
#define PI 3.14159

#define SUN_DIAM 14.0

/* on crée une structure planète */
typedef struct planet{
	float diam, dist, speed;
	int r, g, b;
} Planet;

void initColor(Planet (*system)[8]){
	/* Mercure est gris */ 
	(*system)[0].r=150;
	(*system)[0].g=150;
	(*system)[0].b=150;
	/* Vénus est orange */ 
	(*system)[1].r=250;
	(*system)[1].g=150;
	(*system)[1].b=65;
	/* Terre est verte */ 
	(*system)[2].r=50;
	(*system)[2].g=90;
	(*system)[2].b=60;
	/* Mars est rouge */ 
	(*system)[3].r=220;
	(*system)[3].g=45;
	(*system)[3].b=45;
	/* Jupiter est marron */ 
	(*system)[4].r=115;
	(*system)[4].g=75;
	(*system)[4].b=44;
	/* Saturne est jaune */ 
	(*system)[5].r=255;
	(*system)[5].g=255;
	(*system)[5].b=120;
	/* Uranus est turquoise */ 
	(*system)[6].r=0;
	(*system)[6].g=160;
	(*system)[6].b=160;
	/* Neptune est bleue */ 
	(*system)[7].r=70;
	(*system)[7].g=55;
	(*system)[7].b=150;
}
/* initialise les diametres des planètes */
void initDiamPlanets(Planet (*system)[8]){
	(*system)[0].diam=0.25;
	(*system)[1].diam=0.6;
	(*system)[2].diam=0.65;
	(*system)[3].diam=0.35;
	(*system)[4].diam=7.25;
	(*system)[5].diam=6;
	(*system)[6].diam=2.5;
	(*system)[7].diam=2.5;
}
/* initialise les vitesses des planètes */
void initSpeedPlanets(Planet (*system)[8]){
	(*system)[0].speed=1.7;
	(*system)[1].speed=1.3;
	(*system)[2].speed=1;
	(*system)[3].speed=0.9;
	(*system)[4].speed=0.5;
	(*system)[5].speed=0.4;
	(*system)[6].speed=0.3;
	(*system)[7].speed=0.2;
}
/* initialise les distances */
void initDistPlanets(Planet (*system)[8]){
	/* pour l'instant on met une distance minimale */
	(*system)[0].dist= SUN_DIAM/2+(*system)[0].diam+0.5;
	int i;
	for(i=1;i<8;i++){
		(*system)[i].dist=((*system)[i-1].dist)+(*system)[i].diam+(*system)[i-1].diam+0.5;
	}
}

/* on initialise le tableaux des vitesses à 0 */
void initTime(float (*time)[8]){
	int i;
	for(i=0;i<8;i++){
		(*time)[i]=0;
	}
}
/* on ajoute la "vitesse" */
void changeTime(float (*time)[8], Planet system[8]){
	int i;
	for(i=0;i<8;i++){
		(*time)[i]+= system[i].speed;
	}
}

/* gère le redimensionnement */
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
	glLineWidth(0.01);
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

/* dessine un cercle */
void drawCircle(int full){
	/* couleur et remplissage */
	if(full==1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int i;
	/* dessine un polygone en fonction du nb de côtés voulus */
	/* plus le nb de côtés est grand, plus on se rapproche d'un cercle */
	glBegin(GL_POLYGON);
	for(i=0;i<=NB_SEG;i++){
		float angle=(2*PI/NB_SEG)*(float)i;
		glVertex2f(0.5*cos(angle),0.5*sin(angle));
	}
	glEnd();
}

void drawPlanets(Planet system[8], float time[8]){
	int i;
	for(i=0;i<8;i++){
		/* on dessine les orBITEs */
		glPushMatrix();
			glScalef(system[i].dist, system[i].dist, 1);
			glColor3ub(255,255,255);
			glLineWidth(1);
			drawCircle(0);
		glPopMatrix();

		glPushMatrix();
			glTranslatef((system[i].dist/2)*cos(2*time[i]*PI/180), (system[i].dist/2)*sin(2*time[i]*PI/180), 0);
			glScalef(system[i].diam, system[i].diam, 1);
			glColor3ub(system[i].r, system[i].g, system[i].b);
			drawCircle(1);
		glPopMatrix();
	}
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
    SDL_WM_SetCaption("SOLAR SYSTEM MUTHAFUCKA", NULL);
    resizeViewport();

    /* on crée le système solaire */
    Planet solarSystem[8];
    initDiamPlanets(&solarSystem);
    initSpeedPlanets(&solarSystem);
    initDistPlanets(&solarSystem);
    initColor(&solarSystem);

    /* on crée le tableau de temps pour les vitesses */
    float newTime[8];
    initTime(&newTime);

	int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        changeTime(&newTime, solarSystem);

        /* sun */
        glPushMatrix();
        	glScalef(7,7,1);
       		glColor3ub(255,255,0);
	        drawCircle(1);
		glPopMatrix();
		
		drawPlanets(solarSystem, newTime);

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
