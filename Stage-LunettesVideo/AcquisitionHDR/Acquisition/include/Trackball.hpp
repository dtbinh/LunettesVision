#ifndef	__TRACKBALL_HPP__
#define	__TRACKBALL_HPP__

#include "Utils.hpp"

SDL_Cursor * cursorFromXPM(const char * xpm[]);

class TrackballCamera
{
	public:
		TrackballCamera();

		virtual void OnMouseMotion(const SDL_MouseMotionEvent & event);
		virtual void OnMouseButton(const SDL_MouseButtonEvent & event);
		virtual void OnKeyboard(const SDL_KeyboardEvent & event);

		virtual void look();
		virtual void setMotionSensivity(double sensivity) { _motionSensivity = sensivity; };
		virtual void setScrollSensivity(double sensivity) { _scrollSensivity = sensivity; };

		virtual ~TrackballCamera();
		
		// Accessors
		void SetWebcamPosition( const Vector3D& oPosition, const Vector3D& oDirection )
		{
			//~ std::cout << "Webcam Position: " << oPosition[0] << ", " << oPosition[1] << ", " << oPosition[2] << std::endl;
			m_WebcamPosition = Vector3D( oPosition );
			m_WebcamDirection = Vector3D( oDirection );
			//~ std::cout << "Webcam: " << m_WebcamPosition[0] << ", " << m_WebcamPosition[1] << ", " << m_WebcamPosition[2] << std::endl;
		}
		void SetSwissRangerPosition( const Vector3D& oPosition, const Vector3D& oDirection )
		{
			//~ std::cout << "SwissRanger Position: " << oPosition[0] << ", " << oPosition[1] << ", " << oPosition[2] << std::endl;
			m_SwissRangerPosition = Vector3D( oPosition );
			m_SwissRangerDirection = Vector3D( oDirection );
			//~ std::cout << "SwissRanger: " << m_SwissRangerPosition[0] << ", " << m_SwissRangerPosition[1] << ", " << m_SwissRangerPosition[2] << std::endl;
		}
		void setSpeed(double speed)
		{
			_motionSensivity = speed;
		}

		void setSensivity(double sensivity)
		{
			_scrollSensivity = sensivity;
		}

		void setPosition(const Vector3D & position)
		{
			_position = position;
		}
		//~ virtual bool OnJoystickMotion( const SDL_Event& event );
		virtual void animate(Uint32 timestep);
		
	private:
		void SaveCameraPosition( const char* pCameraPosition )
		{
			FILE* f;
			f = fopen ( pCameraPosition, "w" );
			if ( f != NULL )
			{
				fprintf( f, "Position\n%lf %lf %lf\n", _position[0], _position[1], _position[2] );
				fprintf( f, "Target\n%lf %lf %lf\n", _target[0], _target[1], _target[2] );
				fprintf( f, "Forward\n%lf %lf %lf\n", _forward[0], _forward[1], _forward[2] );
				fprintf( f, "Left\n%lf %lf %lf\n", _left[0], _left[1], _left[2] );
				fclose( f );
				std::clog << "Camera Position saved in [" << pCameraPosition << "]" << std::endl;
			}
		}
		void LoadCameraPosition( const char* pCameraPosition )
		{
			FILE* f;
			f = fopen( pCameraPosition, "r" );
			if( f != NULL )
			{
				int iNbToSave = 255;
				char* pBuff = new char[ iNbToSave ];

				fgets( pBuff, iNbToSave, f );
				fgets( pBuff, iNbToSave, f );
				sscanf( pBuff, "%lf %lf %lf", &_position.X, &_position.Y, &_position.Z );
				fgets( pBuff, iNbToSave, f );
				fgets( pBuff, iNbToSave, f );
				sscanf( pBuff, "%lf %lf %lf", &_target.X, &_target.Y, &_target.Z );
				fgets( pBuff, iNbToSave, f );
				fgets( pBuff, iNbToSave, f );
				sscanf( pBuff, "%lf %lf %lf", &_forward.X, &_forward.Y, &_forward.Z );
				fgets( pBuff, iNbToSave, f );
				fgets( pBuff, iNbToSave, f );
				sscanf( pBuff, "%lf %lf %lf", &_left.X, &_left.Y, &_left.Z );
				
				//VectorsFromAngles();

				fclose( f );
				
				std::clog << "Camera Position loaded from [" << pCameraPosition << "]" << std::endl;
			}
		}
		
	protected:
		
		void Initialize();
		void Release();
	
		double _motionSensivity;
		double _scrollSensivity;
		bool _hold;
		Uint8 _type;
		double _distance;
		double _angleX;
		double _angleY;
		double _angleZ;
		SDL_Cursor * _hand1;
		SDL_Cursor * _hand2;
	
		SDL_Joystick**  	m_ppJoystick;			///< Joysticks
		int				m_iNbJoysticks;
		Sint16			m_iMove;
		
		Vector3D _position;
		Vector3D _target;
		Vector3D _forward;
		Vector3D _left;
		Vector3D m_WebcamPosition;
		Vector3D m_WebcamDirection;
		Vector3D m_SwissRangerPosition;
		Vector3D m_SwissRangerDirection;
};


#endif // __TRACKBALL_HPP__
