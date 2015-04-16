#include "Trackball.hpp"

TrackballCamera::TrackballCamera():
	m_ppJoystick( NULL )
{
	const char *hand1[] =
	{
		/* width height num_colors chars_per_pixel */
		" 16 16 3 1 ",
		/* colors */
		"X c #000000",
		". c #ffffff",
		"  c None",
		/* pixels */
		"       XX       ",
		"   XX X..XXX    ",
		"  X..XX..X..X   ",
		"  X..XX..X..X X ",
		"   X..X..X..XX.X",
		"   X..X..X..X..X",
		" XX X.......X..X",
		"X..XX..........X",
		"X...X.........X ",
		" X............X ",
		"  X...........X ",
		"  X..........X  ",
		"   X.........X  ",
		"    X.......X   ",
		"     X......X   ",
		"     X......X   ",
		"0,0"
	};

	const char *hand2[] =
	{
		/* width height num_colors chars_per_pixel */
		" 16 16 3 1 ",
		/* colors */
		"X c #000000",
		". c #ffffff",
		"  c None",
		/* pixels */
		"                ",
		"                ",
		"                ",
		"                ",
		"    XX XX XX    ",
		"   X..X..X..XX  ",
		"   X........X.X ",
		"    X.........X ",
		"   XX.........X ",
		"  X...........X ",
		"  X...........X ",
		"  X..........X  ",
		"   X.........X  ",
		"    X.......X   ",
		"     X......X   ",
		"     X......X   ",
		"0,0"
	};

	_hand1 = cursorFromXPM( hand1 ); //création du curseur normal
	_hand2 = cursorFromXPM(hand2); //création du curseur utilisé quand le bouton est enfoncé
	SDL_SetCursor(_hand1); //activation du curseur normal
	_hold = false; //au départ on part du principe que le bouton n'est pas maintenu
	_angleX = 0;
	_angleY = 90;
	_angleZ = 0;
	_distance = 0.5; //distance initiale de la caméra avec le centre de la scène
	_motionSensivity = 0.5;
	_scrollSensivity = 0.01;

	Initialize();
}

void TrackballCamera::Initialize()
{
	m_iNbJoysticks = SDL_NumJoysticks();
	m_ppJoystick = new SDL_Joystick*[ m_iNbJoysticks ];

	std::clog << m_iNbJoysticks << " Joystick connected." << std::endl;

	for( int i = 0 ; i < m_iNbJoysticks ; i++ )
	{
		m_ppJoystick[ i ] = SDL_JoystickOpen( i );
		std::clog << "Joystick " << i << ": " << SDL_JoystickName( i ) << std::endl;
		//~ printf("Axes: %d\n", SDL_JoystickNumAxes(joy));
		//~ printf("Boutons: %d\n", SDL_JoystickNumButtons(joy));
		//~ printf("Trackballs: %d\n", SDL_JoystickNumBalls(joy));
		//~ printf("Chapeaux: %d\n\n", SDL_JoystickNumHats(joy));
	}
}

TrackballCamera::~TrackballCamera()
{
	//~ if( _hand1 != NULL )
		//~ SDL_FreeCursor(_hand1); //destruction du curseur normal
	//~ if( _hand1 != NULL )
		//~ SDL_FreeCursor(_hand2); //destruction du curseur spécial
	SDL_SetCursor(NULL); //on remet le curseur par défaut.
	Release();
}

void TrackballCamera::Release()
{
	///@bug didn't work, may be SDL context close before...
	//~ for( int i = 0 ; i < m_iNbJoysticks ; i++ )
	//~ {
		//~ SDL_JoystickClose( m_ppJoystick[ i ] );
	//~ }
}

void TrackballCamera::OnMouseMotion(const SDL_MouseMotionEvent & event)
{
	if( _hold && _type == SDL_BUTTON_RIGHT ) //si nous maintenons le bouton gauche enfoncé
	{
		_angleZ += event.xrel*_motionSensivity; //mouvement sur X de la souris -> changement de la rotation horizontale
	}
	else if( _hold && _type == SDL_BUTTON_LEFT )
	{
		_angleY += event.xrel*_motionSensivity; //mouvement sur Y de la souris -> changement de la rotation verticale
		//pour éviter certains problèmes, on limite la rotation verticale à des angles entre -90° et 90°
		if (_angleY > 360 )
			_angleY = 0;
		else if (_angleY < -360 )
			_angleY = 0;
		//~ _angleX += event.xrel * _motionSensivity; // mouvement sur X de la souris -> changement de la rotation sur X (3D)
	}
}

void TrackballCamera::OnMouseButton(const SDL_MouseButtonEvent & event)
{
	_type = event.button;
	if (event.button == SDL_BUTTON_LEFT) //l'événement concerne le bouton gauche
	{
		if( (_hold) &&(event.type == SDL_MOUSEBUTTONUP) ) //relâchement alors qu'on était enfoncé
		{
			_hold = false; //le mouvement de la souris ne fera plus bouger la scène
			SDL_SetCursor(_hand1); //on met le curseur normal
		}
		else if( (!_hold)&&(event.type == SDL_MOUSEBUTTONDOWN) ) //appui alors qu'on était relâché
		{
			_hold = true; //le mouvement de la souris fera bouger la scène
			SDL_SetCursor(_hand2); //on met le curseur spécial
		}
	}
	if (event.button == SDL_BUTTON_RIGHT) //l'événement concerne le bouton gauche
	{
		if( (_hold) &&(event.type == SDL_MOUSEBUTTONUP) ) //relâchement alors qu'on était enfoncé
		{
			_hold = false; //le mouvement de la souris ne fera plus bouger la scène
			SDL_SetCursor(_hand1); //on met le curseur normal
		}
		else if( (!_hold)&&(event.type == SDL_MOUSEBUTTONDOWN) ) //appui alors qu'on était relâché
		{
			_hold = true; //le mouvement de la souris fera bouger la scène
			SDL_SetCursor(_hand2); //on met le curseur spécial
		}
	}
	else if ((event.button == SDL_BUTTON_WHEELUP)&&(event.type == SDL_MOUSEBUTTONDOWN)) //coup de molette vers le haut
	{
		_distance -= _scrollSensivity; //on zoome, donc rapproche la caméra du centre
		//~ if (_distance < 0.1) //distance minimale, à changer si besoin (avec un attribut par exemple)
			//~ _distance = 0.1;
	}
	else if ((event.button == SDL_BUTTON_WHEELDOWN)&&(event.type == SDL_MOUSEBUTTONDOWN)) //coup de molette vers le bas
	{
		_distance += _scrollSensivity; //on dézoome donc éloigne la caméra
	}
}

void TrackballCamera::OnKeyboard(const SDL_KeyboardEvent & event)
{
	if ((event.type == SDL_KEYDOWN)&&(event.keysym.sym == SDLK_HOME)) //appui sur la touche HOME
	{
		//~ printf "Appuie sur HOME\n" );
		_angleY = 90; //remise à zéro des angles
		_angleZ = 0;
		_distance = 0.5;
	}
	if ((event.type == SDL_KEYDOWN)&&(event.keysym.sym == SDLK_p)) //appui sur la touche HOME
	{
		std::clog << "Camera : ( " << _angleY << ", " << _angleZ << " @ " << _distance << " )" << std::endl;
	}
}

void TrackballCamera::look()
{
	//~ gluLookAt(0,0,0,
		//~ 0,0,0,
		//~ 0,1,0); // la caméra regarde le centre (0,0,0) et est sur l'axe X à une certaine distance du centre donc (_distance,0,0)
	glTranslatef( 0.0f, 0.0f, _distance );
	glRotated(_angleY,1,0,0); //la scène est tournée autour de l'axe Y
	glRotated(_angleZ,0,0,1); //la scène est tournée autour de l'axe Z
}

//~ bool TrackballCamera::OnJoystickMotion( const SDL_Event& oEvent )
//~ {
	//~ switch( oEvent.type )
	//~ {
		//~ case SDL_JOYAXISMOTION :
			//~ m_iJoyAxis = oEvent.jaxis.axis;
			//~ m_fJoyValue = oEvent.jaxis.value;
			//~ break;
		//~ case SDL_JOYBUTTONDOWN :
			//~ if( oEvent.jbutton.button == 10 )
			//~ {
				//~ return false;
			//~ }
			//~ break;
		//~ default:
			//~ break;
	//~ }

	//~ return true;
//~ }

void TrackballCamera::animate(Uint32 timestep)
{
	for( int i = 0 ; i < m_iNbJoysticks ; i++ )
	{
		m_iMove = SDL_JoystickGetAxis( m_ppJoystick[i], 4 );
		if( !( m_iMove < 8000 && m_iMove > -8000 ) )
		{
			_angleY += ( ( m_iMove > 0 ) ? 1 : -1 ) * _motionSensivity;
			if (_angleY > 360 )
				_angleY = 0;
			else if (_angleY < -360 )
				_angleY = 0;
		}
		m_iMove = SDL_JoystickGetAxis( m_ppJoystick[i], 3 );
		if( !( m_iMove < 8000 && m_iMove > -8000 ) )
		{
			_angleZ += ( ( m_iMove > 0 ) ? 1 : -1 ) * _motionSensivity;
		}
		m_iMove = SDL_JoystickGetAxis( m_ppJoystick[i], 2 );
		if( !( m_iMove < 8000 && m_iMove > -8000 ) )
		{
			_distance += _scrollSensivity;
		}
		m_iMove = SDL_JoystickGetAxis( m_ppJoystick[i], 0 );
		if( !( m_iMove < 20000 && m_iMove > -20000 ) )
		{
			_distance -= _scrollSensivity;
		}
		m_iMove = SDL_JoystickGetAxis( m_ppJoystick[i], 5 );
		if( !( m_iMove < 20000 && m_iMove > -20000 ) )
		{
			_distance += ( (m_iMove > 0 ) ? 1 : -1 ) * _scrollSensivity;
		}
		m_iMove = SDL_JoystickGetAxis( m_ppJoystick[i], 1 );
		if( !( m_iMove < 20000 && m_iMove > -20000 ) )
		{
			_distance += ( (m_iMove > 0 ) ? 1 : -1 ) * _scrollSensivity;
		}
	}
}


SDL_Cursor * cursorFromXPM(const char * xpm[])
{
	int i, row, col;
	int width, height;
	Uint8 * data;
	Uint8 * mask;
	int hot_x, hot_y;
	SDL_Cursor * cursor = NULL;

	sscanf(xpm[0], "%d %d", &width, &height);
	data = (Uint8*)calloc(width/8*height,sizeof(Uint8));
	mask = (Uint8*)calloc(width/8*height,sizeof(Uint8));

	i = -1;
	for ( row=0; row<height; ++row )
	{
		for ( col=0; col<width; ++col )
		{
			if ( col % 8 )
			{
				data[i] <<= 1;
				mask[i] <<= 1;
			}
			else
			{
				++i;
				data[i] = mask[i] = 0;
			}
			switch (xpm[4+row][col])
			{
				case 'X':
					data[i] |= 0x01;
					mask[i] |= 0x01;
					break;
				case '.':
					mask[i] |= 0x01;
					break;
				case ' ':
					break;
			}
		}
	}
	sscanf(xpm[4+row], "%d,%d", &hot_x, &hot_y);
	cursor = SDL_CreateCursor(data, mask, width, height, hot_x, hot_y);
	free(data);
	free(mask);
	return cursor;
}
