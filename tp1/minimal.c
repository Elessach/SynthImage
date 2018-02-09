#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

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
  SDL_WM_SetCaption("Anticonstitutionnellement", NULL);
 
  glClear(GL_COLOR_BUFFER_BIT);
    

  /* Boucle d'affichage */
  int loop = 1;
  while(loop) {
    /* Récupération du temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();
    
    /* Placer ici le code de dessin */
    
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
      
      //float posX,posY;

      /* Quelques exemples de traitement d'evenements : */
      switch(e.type) {
        /* Clic souris */
        case SDL_MOUSEBUTTONDOWN:
	        //while(e.button.state==SDL_PRESSED){
	        	glColor3ub(255, 255, 255);
	        	glBegin(GL_POINTS);
	        	glVertex2f(-1 + 2. * e.button.x / WINDOW_WIDTH, -(-1+2.*e.button.y/WINDOW_HEIGHT));
	        	glEnd();
	        //}
        break;

        /*case SDL_MOUSEMOTION:
          //printf("clic en (%d, %d)\n", e.button.x, e.button.y);
          posX=e.button.x;
          posX/=WINDOW_WIDTH;
          posY=e.button.y;
          posY/=WINDOW_HEIGHT;
          glClearColor(posX, posY, 0, 1);
          break;*/
          
        /* Touche clavier */
        case SDL_KEYDOWN:
          //printf("touche pressée (code = %d)\n", e.key.keysym.sym);
          switch(e.key.keysym.sym){
            case 113 :
              loop=0;
            default :
              break;
          }
          break;
        
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
  }
  
  SDL_GL_SwapBuffers();

  /* Liberation des ressources associées à la SDL */ 
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
