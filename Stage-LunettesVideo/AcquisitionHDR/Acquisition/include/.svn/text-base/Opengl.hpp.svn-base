#ifndef  __OPENGL_HPP__
#define __OPENGL_HPP__

#include "Utils.hpp"

#include "Trackball.hpp"


/**
 * \class Display
 * \brief Canevas of drawable classes using OpenGL.
 * \author M. Barnachon
 * \version 0.1
 *
 * Class use to draw with Opengl.
 * Design to be used with webcam, SwissRanger camera, etc.
 * Should work fine with others objects, like graphs, tree, any king of geometry.
 * Display class is pure virtual.
 */
class Display
{
	public:
		/// Constructor.
		Display(unsigned int w=640, unsigned int h=480);
		/// Destructor
		virtual ~Display();
		/// Pre-Initialize.
		virtual void Create();
		/// Release used memory. Close device.
		void Release();
		/// Process keybord/joystick/mouse events.
		virtual void ProcessEvent();
		/// Camera control function. Use to perform trackball computing.
		void CameraControl();
		/**
		 * Main loop function.
		 * Infinity loop until user quit.
		 */
		virtual void Run()
		{
			do
			{
				Draw();
				SDL_GL_SwapBuffers();
				ProcessEvent();
			}while( !m_bExit );
		}
		/**
		 * Display Axes.
		 *
		 * @param Relative size of axes.
		 */
		void DrawAxes ( float fSize  = 1.0f );

	public: // Getter / Setter
		/// Return SwissRanger image grabed width.
		uint GetWidth()	{ return m_uiWidth; 	}
		/// Return SwissRanger image grabed height.
		uint GetHeight()	{ return m_uiHeight; 	}
		/// Accessor to the exit parameter.
		bool IsEnded(){ return m_bExit; }
		/// Accessor to exit the application.
		void Exit( bool bExit = true ) { m_bExit = bExit; }

	protected:
		/// Initialization function. Virtual pure function.
		virtual void Initialize() = 0;
		/// Drawing function. Virtual pure function.
		virtual void Draw() = 0;


		SDL_Surface* 			m_pScreen;			///< SDL Screen representation.

		TrackballCamera*		m_pCamera;			///< TrackBall Camera.

		float				m_pObjectXForm[4][4];	///< Object's matrix transformation.
		float				m_fAngle;				///< Camera angle used during rotation.
		float				m_fFar;				///< Camera far clipping plane
		float				m_fNear;				///< Camera near clipping plane.
		float				m_fXMin;				///<
		float				m_fXMax;				///<
		float				m_fYMin;				///<
		float				m_fYMax;				///<
		float				m_pAxis[ 3 ];			///<
		float				m_pLastPos[3];		///<
		int					m_iLastTime;			///<
		uint					m_uiWidth;			///< 3D panel width.
		uint					m_uiHeight;			///< 3D panel height.
		bool					m_bExit;				///< "Should we stop the application?" flag.

};

/**
 * \class Displayable
 * \brief Template to instantiate a drawable object.
 * \author M. Barnachon
 * \version 0.1
 *
 * This template is used to produce a drawable object (in OpenGL).
 * It herites from Display class.
 * All you have to do is:
 * 	1) creating a "Draw" method, with OpenGL functionnality (any of them);
 * 	2) creating a "Create" method (allocation can be thrown here, not in constructor);
 * 	3) Call the create method after object allocation.
 * 	4) Call "Run" method (instantated by Display class). Actually, you can't overloaded this method (in fact, it has not effect if you do this).
 * 		-> This method is a standard loop, \see Display class.
 * 	5) The "Initialize" method isn't called by create. You have to call it yourselft in you object.
 *
 * \todo Allow overloading of Run methode.
 */
template <class Object >
class Displayable : public Display
{
	public:
		/// Standard constructor.
		Displayable() : m_pODisplayalebject( NULL )
		{
		}
		/// Standard destructor. Call "Release" method.
		~Displayable()
		{
			Release();
		}
		/**
		 * \brief Create method.
		 * Create templated object. (with new).
		 * Call its "Create" method (Must exist).
		 * Call pure virtu Display class "Create" method.
		 */
		void Create()
		{
			m_pODisplayalebject = new Object();
			m_pODisplayalebject->Create();
			Display::Create();
		}
		/**
		 * \brief Release method.
		 * If templated object isn't NULL, delete it and put it to NULL.
		 */
		void Release()
		{
			if( NULL != m_pODisplayalebject )
			{
				delete m_pODisplayalebject;
				m_pODisplayalebject = NULL;
			}
		}
		/**
		 * \brief Initialize mathod.
		 * Do nothing. Use to allowed Display instantiation.
		 */
		void Initialize()
		{
			m_pODisplayalebject->Start();
		}
		/**
		 * \brief Dawing method.
		 * Clear Color buffer and depth buffer (standard actions...)
		 * Call templated object's "Draw" method.
		 */
		virtual void Draw()
		{
			m_pODisplayalebject->Draw();
		}
		/**
		 * \brief Main loop method.
		 * Just call Display class "Run" method. \see Display class.
		 */
		void Run()
		{
			Display::Run();
		}
		/**
		 * \brief Process keybord, mouse, and/or joystick events.
		 * Templated object must have such a function.
		 * This method call object function, and call Display function (trackball solution implemented in).
		 */
		virtual void ProcessEvent()
		{
			m_pODisplayalebject->ProcessEvent();
			Display::ProcessEvent();
		}
	private:
		Object* m_pODisplayalebject; ///< Pointer to templated object.
};


#endif // __OPENGL_HPP__
