#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "main.h"
#include "RubixCube.h"

#define TRUE  1
#define FALSE 0

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define my_assert(X,Y) ((X)?(void) 0:(printf("error:%s in %s at %d", Y, __FILE__, __LINE__), myabort()))

#define DELTA_TIME 10  /* defined to be 10 msec */
#define CUBE_ROTATE_SPEED 500 /* msec to take for the cube to turn 90 degrees*/
#define GAME_UPDATE_SPEED 25 /* in fps, how many times a second the game should update. */
#define MAX_FRAME_SKIP 5 /* Max number of frames that the program can skip to update game mechanics */

void myabort(void);
void glut_setup(void) ;
void gl_setup(void) ;
void my_setup(int argc, char **argv);
void my_display(void) ;
void my_reshape(int w, int h) ;
void my_keyboard(unsigned char key, int x, int y);
void my_mouse_drag(int x, int y);
void my_mouse(int button, int state, int mousex, int mousey);
void my_idle(void);

// Variables.
const int SKIP_TICKS = 1000 / GAME_UPDATE_SPEED;

RubixCube rc;
time_t next_game_tick;
float interpolation;
int loops;
int theta;

void myabort(void) {
	abort();
	exit(1); /* exit so g++ knows we don't return. */
} 

int main(int argc, char **argv) {
	/* General initialization for GLUT and OpenGL
     Must be called first */
	glutInit( &argc, argv ) ;
	
	/* we define these setup procedures */
	glut_setup() ;  
	gl_setup() ;
	my_setup(argc, argv);
	
	/* go into the main event loop */
	glutMainLoop() ;
	
	return(0) ;
}

/* This function sets up the windowing related glut calls */
void glut_setup(void) {
	
	/* specify display mode -- here we ask for a double buffer and RGB coloring */
	glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB |GLUT_DEPTH);
	
	/* make a 400x300 window with the title of "GLUT Skeleton" placed at the top left corner */
	glutInitWindowSize(800,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("GLUT Demo 1");
	
	/*initialize callback functions */
	glutDisplayFunc( my_display );
	glutReshapeFunc( my_reshape ); 
	glutMouseFunc( my_mouse);
	glutMotionFunc( my_mouse_drag);
	glutKeyboardFunc (my_keyboard);
	glutIdleFunc( my_idle );	
	
	
	return ;
}

/* This function sets up the initial states of OpenGL related enivornment */
void gl_setup(void) {
	
	/* specifies a background color: black in this case */
	glClearColor(0,0,0,0) ;
        
        glEnable(GL_DEPTH_TEST);
	
	/* setup for simple 2d projection */
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
        /*gluLookAt(0, 5, 15,
                  0, 0, 0,
                  0, 1, 0);*/
	/* set the viewable portion  */
	gluPerspective(20.0, 4.0/3.0, 1.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
	return;
}

void my_setup(int argc, char **argv) {
    next_game_tick = time(NULL);
    rc.print_debug();
    theta = 0;
    return;
}

void my_reshape(int w, int h) {
	/* define viewport -- x, y, (origin is at lower left corner) width, height */
	glViewport (0, 0, w, h); 
}

/* Sample keyboard callback function.  
 This allows the user to quit the program
 by typing the letter q.
 */
void my_keyboard( unsigned char key, int x, int y ) {
	
	switch( key ) {
            case 't':
                rc.shuffle_cube(1);
                break;
            case 'r':
                rc.shuffle_cube(2);
                break;
            case 'e':
                rc.shuffle_cube(3);
                break;
            case 'w':
                rc.shuffle_cube(4);
                break;
		case 'q': 
		case 'Q':
			exit(0) ;
			break ;	
		default: break;
	}
	return ;
}

/* currently doesn't do anything */
void my_mouse_drag(int x, int y) {
	return;
}

/* stubbed but doesn't do anything either 
 (mousex, mousey) are in screen coordinates
 that is, (0,0) is at the upper left corner
 */
void my_mouse(int button, int state, int mousex, int mousey) {
}

void my_display(void) {
	/* clear the buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor3f(1,0, 0) ;
        
        glMatrixMode(GL_MODELVIEW) ;
        glLoadIdentity();
        gluLookAt(0.0, 5.0, 15.0,  // x,y,z coord of the camera 
	    0.0, 0.0, 0.0,  // x,y,z coord of the origin
	    0.0, 1.0, 0.0); // the direction of up (default is y-axis)
	
        // Setup placement of the Rubix Cube. Will be located at -1.5, -1.5
        glRotatef(theta++, 0, 1, 0);
        glPushMatrix();
        glTranslatef(-1.5, -1.5, -1.5);
	// Setup Interpolation
	interpolation = (float)(time(NULL)+SKIP_TICKS-next_game_tick) / (float)(SKIP_TICKS);
	// Draw Cube
        rc.draw(interpolation);
        glPopMatrix();
	/* buffer is ready */
	glutSwapBuffers();
	
	return ;
}


void my_idle(void) {
    loops = 0;
    while(time(NULL) > next_game_tick && loops < MAX_FRAME_SKIP){
        // Update Game Stuff
        rc.update_cube(SKIP_TICKS);
        
        // Update Timing Stuff
        next_game_tick += SKIP_TICKS;
        loops++;
    }
    
    // Display Screen
    my_display();
    return ;
}
