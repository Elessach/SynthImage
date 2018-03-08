#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

#define SIZE 4

void resizeViewport(){
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-SIZE, SIZE, -SIZE, SIZE);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

/* récupère l'heure actuelle */
void getTime(int (*currentTime)[6]){
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	/* case 0 : heure ; case 1 : minutes ; case 2 : secondes */
	(*currentTime)[0]=(timeinfo->tm_hour)/10;
	(*currentTime)[1]=(timeinfo->tm_hour)%10;
	(*currentTime)[2]=(timeinfo->tm_min)/10;
	(*currentTime)[3]=(timeinfo->tm_min)%10;
	(*currentTime)[4]=(timeinfo->tm_sec)/10;
	(*currentTime)[5]=(timeinfo->tm_sec)%10;
}

/* affiche un chiffre */
void drawNumber(GLuint textureID){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
        glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex2f(-0.5, 0.5);
            glTexCoord2f(1,0);
            glVertex2f(0.5, 0.5);
            glTexCoord2f(1,1);
            glVertex2f(0.5, -0.5);
            glTexCoord2f(0,1);
            glVertex2f(-0.5, -0.5);
        glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);   
}

/* affiche la cadran entier */
void drawScreen(GLuint textureID[11], int time[6]){
	glPushMatrix();
		glTranslatef(-3.5,0,0);
		drawNumber(textureID[time[0]]);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-2.5,0,0);
		drawNumber(textureID[time[1]]);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.5,0,0);
		drawNumber(textureID[10]);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.5,0,0);
		drawNumber(textureID[time[2]]);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.5,0,0);
		drawNumber(textureID[time[3]]);
	glPopMatrix();


	glPushMatrix();
		glTranslatef(1.5,0,0);
		drawNumber(textureID[10]);
	glPopMatrix();


	glPushMatrix();
		glTranslatef(2.5,0,0);
		drawNumber(textureID[time[4]]);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(3.5,0,0);
		drawNumber(textureID[time[5]]);
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

    /* chargement des images des chiffres */
	SDL_Surface* img[11];
	img[0] = IMG_Load("numbers/0.png");
	img[1] = IMG_Load("numbers/1.png");
	img[2] = IMG_Load("numbers/2.png");
	img[3] = IMG_Load("numbers/3.png");
	img[4] = IMG_Load("numbers/4.png");
	img[5] = IMG_Load("numbers/5.png");
	img[6] = IMG_Load("numbers/6.png");
	img[7] = IMG_Load("numbers/7.png");
	img[8] = IMG_Load("numbers/8.png");
	img[9] = IMG_Load("numbers/9.png");
	img[10] = IMG_Load("numbers/colon.png");

	/* tableau qui va contenir l'heure */
	int time[6];

	/* tableau des 8 textures correspondant aux 8 images de l'heuure */
	GLuint textureID[11];
	glGenTextures(11, textureID);

	/* on envoie les textures */
	int j;
	for(j=0;j<11;j++){
		glBindTexture(GL_TEXTURE_2D, textureID[j]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D,	
			0,
			GL_RGBA,
			img[j]->w,
			img[j]->h,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			img[j]->pixels
		);
		glBindTexture(GL_TEXTURE_2D, 0);
	    SDL_FreeSurface(img[j]);
	}	
	
    int loop = 1;
    glClearColor(1, 1, 1 ,1);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        /* on récupère l'heure */
		getTime(&time);

		/* on affiche */
        drawScreen(textureID, time);

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
    glDeleteTextures(1, textureID);

    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
