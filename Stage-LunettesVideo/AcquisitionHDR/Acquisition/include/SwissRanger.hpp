#ifndef	__DISPLAYSR_HPP__
#define	__DISPLAYSR_HPP__

#include <math.h>
#include "Utils.hpp"

#include "Camera.hpp"
#include "Webcam.hpp"

#include "ConnectedComponent.hpp"	///< From Brice.
#include "Gaussmix.hpp"				///< From Brice.

#include "VoxelsGrid.hpp"

namespace LA
{
/**
 * \class SwissRanger
 * \brief SwissRanger image grabing & showing class.
 * \author M. Barnachon
 * \version 0.1
 *
 * This class is drawable in Drawable template meaning.
 * \see Display class to understand principles.
 * \see Drawable class for an example.
 *
 * Actually, it only draw geometry 'as it captured'.
 *
 * \todo Add accessors to SwissRanger standard parameters, like frequency, median filter, etc. -> Getter and Setter! (may be the camera must be restart for some of them).
 * \todo Add this new "callback" to OASIS API (to complete camera class).
 */
class SwissRanger : public Camera
{
public:
	/// Default constructor.
	SwissRanger();
	/// Destructor
	virtual ~SwissRanger();
	/// Pre-Initialize.
	virtual void Create();
	/// Release used memory. Close SwissRanger device.
	void Release();
	/// Drawing function for SwissRanger image.
	void Draw();
	/// Draw Coded Color for geometry. Use to link Z-geometry to RGB color.
	void DrawCodedColor();
	/// Draw geometry with right color from webcam.
	void DrawColor();
	/// Draw a quad with "scientist" z-map image representation.
	virtual void DrawImage();
	/// Process keybord/joystick/mouse events.
	void ProcessEvent(); ///< @todo Add a list of events to process; to be process in derivated classes.
	/// Compute frustum of Webcam. SwissRanger frustum is directly computable by super class Camera.
	void ComputeFrustumWebcam( unsigned int uiWidth, unsigned int uiHeight ) 		
	{ m_pWebcam->ComputeFrustum( uiWidth, uiHeight ); }

	// RADA
	/// Compute the background model and the foreground image extracted from it
	void ComputeBackgroundForeground();
	// can be used to draw instead of the webcam
    /// Draw the foreground image
	void DrawForeground();

	void DrawForegroundDepth();

	void DrawForegroundColor();

	void DrawHistogram();
	/// Get the foreground image
	IplImage*  GetForeground();

public: // Getter / Setter
	/// Return SwissRanger image grabed width.
	uint GetWidth()		{ return m_uiWidth; 	}
	/// Return SwissRanger image grabed height.
	uint GetHeight()		{ return m_uiHeight; 	}
	/// Return Webcam image grabed width.
	uint GetWidthWC()		{ return m_uiWidthWC; 	}
	/// Return Webcam image grabed height.
	uint GetHeightWC()		{ return m_uiHeightWC; }
	/// Accessor to the exit parameter.
	bool IsEnded()			{ return m_bExit; 		}
	/// Return Webcam Instance.
	Webcam*	GetWebcam()	{ return m_pWebcam;	}
	/// Return pointer to color buffer computed.
	float*	GetXYZColor()	{ return m_pXYZColor; 	}
	float*	GetXYZColorHDR()	{ return m_pXYZColorHDR; 	}
	float*	GetXYZCoords()	{ return m_pXYZCoords; 	}
	IplImage*	GetDistData()	{ return m_pSR4000Dist; }
	char*	GetConfData()	{ return m_pSR4000Conf->imageData; }
	void GrabBoth();
	/// Grab a SwissRanger image.
	void GrabDepth();
	void GrabDepthMedian(uint nbIm);
	
private:
	/// Initialize SwissRanger (thread, mutex, buffers, etc. )
	void Initialize();
	/// Initialiaze the map to "scientist" visualisation.
	void InitColorMap();
	/// Extract interesting points with Brice's algorithm.
	void ExtractInterestingPoints();

	void lineBresenham(int p1x, int p1y, int p2x, int p2y,VoxelsGrid* m_pVoxelsGrid, int line);

private:
	CMesaDevice*	m_pSwissRangerCapture;	///< Mesa SwissRanger capture object.
	ImgEntry*	m_pImgEntryArray;	///< Array of captured image from SwissRanger.
	ModulationFrq	m_pFrequency;	///< SwissRanger camera frenquency.

	Webcam*	m_pWebcam;	///< Webcam instance.

	IplImage*	m_pSR4000Dist;	///< Distance map grabed from SwissRanger.
	IplImage*	m_pSR4000Conf;	///< Confidente map grabed from SwissRanger.
	IplImage*	m_pSR4000Ampl;	///< Amplitude map grabed from SwissRanger.
	IplImage*	m_pSR4000AmplBack;	///< Amplitude map back buffer.
	IplImage*	m_pFusion;	///< Fusion buffer (ie Amplitude channel 1, Depth channel 2, and 0 channel 3).
	IplImage*	m_pFusion1;	///< Fusion buffer (ie Amplitude channel 1, Depth channel 2, and 0 channel 3).
	IplImage*	m_pFusion2;	///< Fusion buffer (ie Amplitude channel 1, Depth channel 2, and 0 channel 3).
	IplImage*	m_pTemp;

	IplImage* m_pMapX;	///< Undistort map for X channel.
	IplImage* m_pMapY;	///< Undistort map for Y channel.

	//RADA
	IplImage*	m_pForegroundBack;	///< foreground back buffer
	IplImage*	m_pForegroundColorBack;	///< foreground back buffer
	IplImage*	m_pForegroundDepthBack;	///< foreground back buffer
	IplImage*	m_pForegroundHistogram;	///< foreground back buffer


	CvBGStatModel*	m_pBackgroundModel;	///< Model used to background substraction.
	CvBGStatModel*	m_pBackgroundModel1;	///< Model used to background substraction.
	CvBGStatModel*	m_pBackgroundModel2;	///< Model used to background substraction.

	ImageAnalysis*	m_pImageAnalisis;	///< Stuff from Brice...enjoy...
	CvGaussBGStatModelParams	m_oBGParams;	///< Background substraction parameters.
	IplConvKernel*	m_pConvKern;	///< Kernel used in background substraction.

	float*	m_pXYZCoords;	///< Buffer of 3D points transformed from SwissRanger raw image.
	float*	m_pXYZColor;	///< Buffer of Colors associated to each point
	float*	m_pXYZColorHDR;
	float*	m_p3DInterestingPoints;	///< Buffer of 3D interesting points.
	float*	m_p3DInterestingColor;	///< Buffer of 3D interesting points.

	int*	m_pXYZIndex;	///< Buffer of 3D points index transformed from SwissRanger raw image, used to draw like an heightmap.

	unsigned char*	m_pXYZCodedColor;	///< Coded color of each 3D point.

	float	m_pCenterOfGravity[3];	///< Center of gravity of principal connected component.
	float	m_pDispertion[3];	///< Dispertion of principal connected component.

	float	m_fDmax;	///< Distance max, linked to frenquency. (Max SwissRanger vision field).
	float	m_fNoiseThreshold;	///< Noise Threshold used to filtering.

	GLuint	m_iCamTexture;	///< SwissRanger Amplitude image in OpenGL texture form.
	
	// RADA
	// To be able to draw the foreground image, hopefully :)
	GLuint	m_iCamTexture2;

	GLuint	m_uiZMapVBO;	///< SwissRanger VBO used to draw.
	GLuint	m_uiZMapVBOIndex;	///< SwissRanger VBO Index used to draw like an heightmap.
	uint	m_uiWidth;	///< SwissRanger image width
	int	m_iUpdateCounter;	///< Counter to stop update background model.
	int	m_iUpdateThreshold;	///< Threshold to stop update background model.
	int	m_iNbInterestingPoints;	///< Number of interesting points extracted (use to compute center of gravity).
	int	m_iConfidentThreshold;	///< Confident threshold used to keep (or not) a point.
	uint	m_uiHeight;	///< SwissRanger image height.
	uint	m_uiWidthWC;	///< Width of webcam images grabed.
	uint	m_uiHeightWC;	///< Height of webcam images grabed.

	bool	m_bExit;	///< "Should we stop the SwissRanger?"'s flag.
	bool	m_bInitTexture;	///< Is the texture used to render initialized?

	void*	m_pBuffer;	///< Buffer to save data from SwissRanger.

	float	m_pColorMap[255][3];	///< Color map to "scientist" visualisation.

	//RADA
	bool	m_bBackgroundFirstCall;	///< to know if the background model is called for the first time
	//I think I canhttp://start.ubuntu.com/9.10/ check if it is nothing "the model" instead but for now it is ok
	//this I may use it outside the class so I should have a get function or simply put it public
	VoxelsGrid*	m_pVoxelsGrid; /// the grid of voxels to be filled
	uint	m_uiNumOfPixelsInVoxel; /// the number of pixels per voxel
	float	m_fPixelsPersentageInVoxel; /// the percentage of the number of white projeted pixels in the voxel to the total number ot voxels
	// so the voxel will be filled or no.

	// CvMat*	m_XProjectionHistogram;	//Projection histogram on X
	// CvMat*	m_YProjectionHistogram;	//Projection histogram on Y
};

/**
 * USB callback to use with usb library (SwissRanger drivers like...)
 */
static inline int LibusbCallback( SRCAM srCam, unsigned int msg, unsigned int param, void* data )
{
	return 0;
	//~ switch( msg )
	//~ {
		//~ case CM_MSG_DISPLAY: // redirects all output to console
		//~ {
			//~ return 0;
		//~ }
		//~ case CM_PROGRESS:
		//~ {
			//~ return 0;
		//~ }
		//~ default:
		//~ {
			//~ //default handling
			//~ return SR_GetDefaultCallback()(0,msg,param,data);
		//~ }
	//~ }
}

} //End namespace

#endif //__DISPLAYSR_HPP__
