#ifndef	__DISPLAYWEBCAM_HPP__
#define	__DISPLAYWEBCAM_HPP__

#include "Utils.hpp"

#include "Camera.hpp"

namespace LA
{

class Webcam : public Camera
{
	public:
		/// Default constructor. Arbitrary size.
		Webcam( uint uiWidth = 640, uint uiHeight = 480 );
		/// Destructor
		virtual ~Webcam();
		/// Pre-Initialize.
		virtual void Create( uint uiWidth = 640, uint uiHeight = 480, int numCam = 0 );
		/// Release used memory. Close SwissRanger device.
		void Release();
		/// Process keybord/joystick/mouse events.
		virtual void ProcessEvent() { } ///< @todo Add a list of events to process; to be process in derivated classes.
		/// Drawing function for webcam image.
		virtual void Draw();
		/// Drawing function for webcam image (use for polymorphisme and allow SwissRanger to draw Amplitude with this function).
		virtual void DrawImage() { Draw(); }
		/// Lauch thread to grab image from a camera.
		void Start();
		/// 
		float findExtrinsicParameters(uint cbWidth, uint cbHeight, float squareSize, uint imgCountMax);

	public: // Getter/Setter
		/// Return webcam image grabed width.
		uint GetWidth()		{ return m_uiWidth; 		}
		/// Return webcam image grabed height.
		uint GetHeight()		{ return m_uiHeight; 		}
		/// Accessor to the exit parameter.
		bool IsEnded()			{ return m_bExit; 			}
		/// Return Texture ID
		GLuint GetTextureID()	{ return m_iCamTexture;	}

		IplImage* GetImage()	{ return m_pCamImageBack;	}
		float* GetImageHDR()	{ return m_pEXRData;	}
		//IplImage* GetImageHDR()	{ return m_pEXRImage;	}

	//~ private:
		/// Grab an image from the webcam.
		void GrabImage();
		
		void grabLdrImagesInit();
		void grabLdrImages();
		IplImage** getLdrImages(){ return m_pLdrImages;}
		float* getTabTe(){ return m_pTabTe;}
		
		/// Initialize the webcam (thread, mutex, buffers, etc. )
		void Initialize( int numCam );

		CameraUVC*		m_pCapture;			///< OpenCV virtual capture type to camera access.
	private:
		SDL_Thread*		m_pWebcamThread;	///< Camera thread.
		SDL_mutex*		m_pMutexWebCam;		///< Camera image mutex.

		IplImage*		m_pCamImageBack ;	///< OpenCV image back buffer grabed from camera.
		IplImage*		m_pCameraImage;		///< OpenCV image used to grab from camera.
#ifdef CAM_FIREWIRE
		IplImage*		m_pCameraImageBayer;///< OpenCV image use to convert from Bayer in FireWire mode.
#endif
		IplImage**		m_pLdrImages;
		IplImage*		m_pEXRImage;
		
		float*			m_pTabTe;
		float*			m_pEXRData;

		GLuint			m_iCamTexture;		///< Webcam image in OpenGL texture form.com

		uint			m_uiWidth;			///< Webcam image width.
		uint			m_uiHeight;			///< Webcam image height.

		bool			m_bExit;			///< "Should we stop the webcam?"'s flag.
		bool			m_bInitTexture;		///< Is the texture used to render initialized?

	private:

		friend int WebCamLoop( void* pData );
};


}

extern int WebCamLoop( void* pData );
#endif // __DISPLAYWEBCAM_HPP__
