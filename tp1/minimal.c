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

/* entier définissant le PRIM de dessin. PRIM point par défaut */
static unsigned int PRIM = 1;
/* Définit si on dessine 'd' ou choisit une couleur 'c' */
static unsigned char MODE = 'd';
/* entier pour booléen pour savoir si on dessine une forme ou pas. */
/* activé par défaut sauf si espace activé pour couleur. */
static unsigned int POINTS[3][2];
static unsigned int NB_PTS = 0;

/* entiers définissant la couleur utilisée. Défaut : blanc */
static unsigned int RED = 255;
static unsigned int GREEN = 255;
static unsigned int BLUE = 255;

void resize(){
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1., 1., -1., 1.);
}

void drawShape(){
    glColor3ub(RED, GREEN, BLUE);
	switch(PRIM){
	    case 1:
		    glBegin(GL_POINTS);
		    glVertex2f(-1 + 2. * POINTS[0][0] / WINDOW_WIDTH, -(-1 + 2. * POINTS[0][1] / WINDOW_HEIGHT));
		    break;
		case 2:
		    glBegin(GL_LINES);
		    glVertex2f(-1 + 2. * POINTS[0][0] / WINDOW_WIDTH, -(-1 + 2. * POINTS[0][1] / WINDOW_HEIGHT));
			glVertex2f(-1 + 2. * POINTS[1][0] / WINDOW_WIDTH, -(-1 + 2. * POINTS[1][1] / WINDOW_HEIGHT));
		    break;
		case 3:
		    glBegin(GL_TRIANGLES);
		    glVertex2f(-1 + 2. * POINTS[0][0] / WINDOW_WIDTH, -(-1 + 2. * POINTS[0][1] / WINDOW_HEIGHT));
			glVertex2f(-1 + 2. * POINTS[1][0] / WINDOW_WIDTH, -(-1 + 2. * POINTS[1][1] / WINDOW_HEIGHT));
			glVertex2f(-1 + 2. * POINTS[2][0] / WINDOW_WIDTH, -(-1 + 2. * POINTS[2][1] / WINDOW_HEIGHT));
		    break;
	    default:
	   		break;
	}
	glEnd();
}

void reset(){
	int i, j;
	for(i=0;i<3;i++){
		for(j=0;j<2;j++){
			POINTS[i][j]=0;
		}
	}
	NB_PTS = 0;
}

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

	glColor3ub(255, 55, 145);
	glBegin(GL_QUADS);
	glVertex2f(-1 + 2. * 800 / WINDOW_WIDTH, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 1, -(-1 + 2. * 0));
	glVertex2f(-1 + 2. * 1, -(-1 + 2. * 1));
	glVertex2f(-1 + 2. * 800 / WINDOW_WIDTH, -(-1 + 2. * 1));
	glEnd();
}

void changeColors(int x){
	if(x<200){	RED = 255; GREEN = 255; BLUE = 255; }
	else if(x<400){ RED = 0; GREEN = 205; BLUE = 255; }
	else if(x<600){ RED = 85; GREEN = 255; BLUE = 0; }
	else if(x<800){ RED = 255; GREEN = 155; BLUE = 55; }
	else if(x<WINDOW_WIDTH){ RED = 255; GREEN = 55; BLUE = 145; }
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

      /* Quelques exemples de traitement d'evenements : */
      switch(e.type) {
        /* Clic souris */
        case SDL_MOUSEBUTTONUP:
        	switch(MODE){
        		case 'd':
        			POINTS[NB_PTS][0]=e.button.x;
        			POINTS[NB_PTS][1]=e.button.y;
        			NB_PTS++;
        			if(NB_PTS==PRIM){
        				drawShape();
        				reset();
        			}        			
        			break;
        		case 'c':
        			changeColors(e.button.x);
        			break;
        		default:
        			break;
        	}
        	break;

        /* Touche clavier */
        case SDL_KEYDOWN:
          //printf("touche pressée (code = %d)\n", e.key.keysym.sym);
          switch(e.key.keysym.sym){
          	case 32: // touche espace fait apparaitre la palette
          		showColors();
          		MODE='c';
          		break;
          	case 108: // touche l pour dessiner des lignes
          		PRIM = 2;
          		break;
          	case 112: // touche p pour dessiner des points
          		PRIM = 1;
          		break;
            case 113: // touche q pour quitter
              loop=0;
              break;
            case 116: // touche t pour dessiner des triangles
            	PRIM = 3;
            	break;
            default:
              break;
          }
          break;

        case SDL_KEYUP:
        	if(e.key.keysym.sym==32){
        		MODE='d';
        		glColor3ub(0,0, 0);
				glBegin(GL_QUADS);
				glVertex2f(-1 + 2. * 0, -(-1 + 2. * 0));
				glVertex2f(-1 + 2. * 1, -(-1 + 2. * 0));
				glVertex2f(-1 + 2. * 1, -(-1 + 2. * 1));
				glVertex2f(-1 + 2. * 0, -(-1 + 2. * 1));
				glEnd();
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
