#ifndef UNIT_TEST_GOOGLE

#include "Graph.hpp"
#include "Opengl.hpp"

#include <vector>

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include <GL/glut.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Hybrid.hpp"


Hybrid* m_pMyDisplayHybrid; /// Because I cleaned my room!
///Cleaning my room...
void Cleaning( void )
{
	delete m_pMyDisplayHybrid;
}

/// Main function
int main( int argc, char** argv )
{

	atexit( Cleaning );

	m_pMyDisplayHybrid = new Hybrid();
	m_pMyDisplayHybrid->Create();
	m_pMyDisplayHybrid->Run();

	return 0;
}


#endif // UNIT_TEST_GOOGLE
