#include "Hybrid.hpp"
#include "FrameBufferObject.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "cameraIEEE.h"
#include "Transform.h"

#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImfInputFile.h>
#include <OpenEXR/ImfOutputFile.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfArray.h>
#include <OpenEXR/ImfFrameBuffer.h>

#include <utilitaire_calibrage_HDR.h>

int WriteEXRImage (const char* fileName, int width, int height, float* ColorImPtr, float* ZImPtr, char* ConfidentImPtr )
{
	using namespace Imf; 
	float *rPixels=new float [width*height];
	float *gPixels=new float [width*height];
	float *bPixels=new float [width*height];
	float *xPixels=new float [width*height];
	float *yPixels=new float [width*height];
	float *zPixels=new float [width*height];
	char *cPixels=new char [width*height];

	for (int u = 0; u < width ; ++u)
	{
		for (int v = 0; v < height ; ++v)
		{
			rPixels[v*width+u]=ColorImPtr[3*(v*width+u)+0];
			gPixels[v*width+u]=ColorImPtr[3*(v*width+u)+1];
			bPixels[v*width+u]=ColorImPtr[3*(v*width+u)+2];
			xPixels[v*width+u]=ZImPtr[3*(v*width+u)+0];
			yPixels[v*width+u]=ZImPtr[3*(v*width+u)+1];
			zPixels[v*width+u]=ZImPtr[3*(v*width+u)+2];
			cPixels[v*width+u]=ConfidentImPtr[v*width+u];
		}
	}
	
	Header header (width, height);
	header.channels().insert ("R", Channel (FLOAT));
	header.channels().insert ("G", Channel (FLOAT));
	header.channels().insert ("B", Channel (FLOAT));
	header.channels().insert ("X", Channel (FLOAT));
	header.channels().insert ("Y", Channel (FLOAT));
	header.channels().insert ("Z", Channel (FLOAT));
	header.channels().insert ("C", Channel (FLOAT));
	OutputFile file (fileName, header);
	
	FrameBuffer frameBuffer; 
	frameBuffer.insert ("R",Slice (FLOAT,  (char *) rPixels,sizeof (*rPixels) * 1, sizeof (*rPixels) * width)); 
	frameBuffer.insert ("G",Slice (FLOAT,  (char *) gPixels,sizeof (*gPixels) * 1, sizeof (*gPixels) * width));
	frameBuffer.insert ("B",Slice (FLOAT,  (char *) bPixels,sizeof (*bPixels) * 1, sizeof (*bPixels) * width));  
	frameBuffer.insert ("X",Slice (FLOAT,  (char *) xPixels,sizeof (*xPixels) * 1, sizeof (*xPixels) * width)); 
	frameBuffer.insert ("Y",Slice (FLOAT,  (char *) yPixels,sizeof (*yPixels) * 1, sizeof (*yPixels) * width));  
	frameBuffer.insert ("Z",Slice (FLOAT,  (char *) zPixels,sizeof (*zPixels) * 1, sizeof (*zPixels) * width));   
	frameBuffer.insert ("C",Slice (FLOAT,  (char *) cPixels,sizeof (*cPixels) * 1, sizeof (*cPixels) * width));  


	file.setFrameBuffer (frameBuffer);
	file.writePixels (height);
	return 0;
}

int WriteEXRImage (const char* fileName, int width, int height, IplImage* ColorImPtr, float* ZImPtr, char* ConfidentImPtr )
{
	CvScalar Pix;
	using namespace Imf; 
	float *rPixels=new float [width*height];
	float *gPixels=new float [width*height];
	float *bPixels=new float [width*height];
	float *zPixels=new float [width*height];
	char *cPixels=new char [width*height];

	for (int u = 0; u < width ; ++u)
	{
		for (int v = 0; v < height ; ++v)
		{
			Pix=cvGet2D(ColorImPtr,u,v);
			rPixels[v*width+u]=Pix.val[0];
			gPixels[v*width+u]=Pix.val[1];
			bPixels[v*width+u]=Pix.val[2];
			zPixels[v*width+u]=ZImPtr[3*(v*width+u)+2];
			cPixels[v*width+u]=ConfidentImPtr[v*width+u];
		}
	}
	
	Header header (width, height);
	header.channels().insert ("R", Channel (FLOAT));
	header.channels().insert ("G", Channel (FLOAT));
	header.channels().insert ("B", Channel (FLOAT));
	header.channels().insert ("Z", Channel (FLOAT));
	header.channels().insert ("C", Channel (FLOAT));
	OutputFile file (fileName, header);
	
	FrameBuffer frameBuffer; 
	frameBuffer.insert ("R",Slice (FLOAT,  (char *) rPixels,sizeof (*rPixels) * 1, sizeof (*rPixels) * width)); 
	frameBuffer.insert ("G",Slice (FLOAT,  (char *) gPixels,sizeof (*gPixels) * 1, sizeof (*gPixels) * width));
	frameBuffer.insert ("B",Slice (FLOAT,  (char *) bPixels,sizeof (*bPixels) * 1, sizeof (*bPixels) * width));  
	frameBuffer.insert ("Z",Slice (FLOAT,  (char *) zPixels,sizeof (*zPixels) * 1, sizeof (*zPixels) * width));  
	frameBuffer.insert ("C",Slice (FLOAT,  (char *) cPixels,sizeof (*cPixels) * 1, sizeof (*cPixels) * width));  


	file.setFrameBuffer (frameBuffer);
	file.writePixels (height);
	return 0;
}

int WriteEXRImage (const char* fileName, int width, int height, float* ColorImPtr )
{
	CvScalar Pix;
	using namespace Imf; 
	float *rPixels=new float [width*height];
	float *gPixels=new float [width*height];
	float *bPixels=new float [width*height];

	for (int u = 0; u < width ; ++u)
	{
		for (int v = 0; v < height ; ++v)
		{
			rPixels[v*width+u]=ColorImPtr[3*(v*width+u)+0];
			gPixels[v*width+u]=ColorImPtr[3*(v*width+u)+1];
			bPixels[v*width+u]=ColorImPtr[3*(v*width+u)+2];
		}
	}
	
	Header header (width, height);
	header.channels().insert ("R", Channel (FLOAT));
	header.channels().insert ("G", Channel (FLOAT));
	header.channels().insert ("B", Channel (FLOAT));
	OutputFile file (fileName, header);
	
	FrameBuffer frameBuffer; 
	frameBuffer.insert ("R",Slice (FLOAT,  (char *) rPixels,sizeof (*rPixels) * 1, sizeof (*rPixels) * width)); 
	frameBuffer.insert ("G",Slice (FLOAT,  (char *) gPixels,sizeof (*gPixels) * 1, sizeof (*gPixels) * width));
	frameBuffer.insert ("B",Slice (FLOAT,  (char *) bPixels,sizeof (*bPixels) * 1, sizeof (*bPixels) * width));  


	file.setFrameBuffer (frameBuffer);
	file.writePixels (height);
	return 0;
}


namespace // anonymous namespace (like static...)
{
	float mins[3]={-0.5,-0.5,-0.5};
	float maxs[3]={0.5,0.5,0.5};
}

Hybrid::Hybrid(uint uiWidth3D, uint uiHeight3D,
				uint uiWidthSR, uint uiHeightSR,
				uint uiWidthWC, uint uiHeightWC ) :
	Display(uiWidth3D,uiHeight3D),
	m_uiWidth3D( uiWidth3D ),
	m_uiHeight3D( uiHeight3D ),
	m_uiWidthViewport( (uiWidth3D-25) / 4 ),
	m_uiHeightViewport( (uiHeight3D-25) / 4 ),
	m_uiWidthSR( uiWidthSR ),
	m_uiHeightSR( uiHeightSR ),
	m_uiWidthWC( uiWidthWC ),
	m_uiHeightWC( uiHeightWC ),
	m_uiExpoWC( 300 ),
	m_uiExpoTWC( 300 ),
	m_pSwissRanger( NULL ),
	m_pWebcam( NULL ),
	m_pTrackingCam(NULL),
	m_fSize( 1.0f ),
	m_bParametersToSet( true ),
	m_bStepIm( false ),
	m_bContIm( false ),
	m_bSaveIm( false ),
	m_iSerial( 0 ),
	m_iCameraTexture( 0 ),
	m_iTrackingTexture( 1 )
	
{
	m_pOrigViewport[ 0 ] = 5;
	m_pOrigViewport[ 1 ] = 1*uiWidth3D/4 +5;
	m_pOrigViewport[ 2 ] = 1*uiHeight3D/4 +5;
	m_pOrigViewport[ 3 ] = 2*uiWidth3D/4 +5;
	m_pOrigViewport[ 4 ] = 2*uiHeight3D/4 +5;
	m_pOrigViewport[ 5 ] = 3*uiWidth3D/4 +5;
	m_pOrigViewport[ 6 ] = 3*uiHeight3D/4 +5;
}

Hybrid::~Hybrid()
{
	Release();
}


void Hybrid::Create()
{
	Display::Create();
	Initialize();
}

void Hybrid::Initialize()
{
	/*Display*/
	m_uiWidth3D = Display::GetWidth();
	m_uiHeight3D = Display::GetHeight();	
	
	/*SwissRanger*/
	std::cout<<"SwissRanger creation ... ";
	m_pSwissRanger = new LA::SwissRanger();
	m_pSwissRanger->Create();
	m_uiWidthSR = m_pSwissRanger->GetWidth();
	m_uiHeightSR = m_pSwissRanger->GetHeight();
	std::cout<<"done"<<std::endl<<"> Dimensions: "<<m_uiWidthSR<<", "<<m_uiHeightSR<<std::endl;
	m_pSwissRanger->ComputeFrustum( m_uiWidthSR, m_uiHeightSR );
	std::cout<<std::endl;
	
	/*Webcam*/
	std::cout<<"Webcam creation ... ";
	m_pWebcam = m_pSwissRanger->GetWebcam();
	std::cout<<"done"<<std::endl<<"> Exposure : "<<m_uiExpoWC<<std::endl;
	m_pWebcam->m_pCapture->setParameter(V4L2_CID_EXPOSURE_AUTO, 1);
	m_pWebcam->m_pCapture->setParameter(V4L2_CID_EXPOSURE_ABSOLUTE, m_uiExpoWC);
	m_pWebcam->m_pCapture->setParameter(V4L2_CID_AUTO_WHITE_BALANCE, 1);
	m_uiWidthWC = m_pSwissRanger->GetWidthWC();
	m_uiHeightWC = m_pSwissRanger->GetHeightWC();
	std::cout<<"> Dimensions: "<<m_uiWidthWC<<", "<<m_uiHeightWC<<std::endl;
	m_pSwissRanger->ComputeFrustumWebcam( m_uiWidthWC, m_uiHeightWC );
	std::cout<<std::endl;
	/*std::cout<<"-------------Webcam values------------"<<std::endl;
	m_pWebcam->m_pCapture->listParameters();
	std::cout<<"--------------------------------------"<<std::endl;//*/

	/*Tracking Camera*/
	std::cout<<"Tracking Webcam creation ... ";
	m_pTrackingCam = new LA::Webcam();
	m_pTrackingCam->Create( 960, 720, 1 );
	std::cout<<"done"<<std::endl<<"> Exposure : "<<m_uiExpoWC<<std::endl;
	m_pTrackingCam->m_pCapture->setParameter(V4L2_CID_EXPOSURE_AUTO, 1);
	m_pTrackingCam->m_pCapture->setParameter(V4L2_CID_EXPOSURE_ABSOLUTE, 50);
	m_pTrackingCam->m_pCapture->setParameter(V4L2_CID_AUTO_WHITE_BALANCE, 0);
	/*std::cout<<"-------------Tracking Webcam values------------"<<std::endl;
	m_pTrackingCam->m_pCapture->listParameters();
	std::cout<<"----------------------------------------------"<<std::endl;//*/
	
	m_uiWidthTWC = m_pTrackingCam->GetWidth();
	m_uiHeightTWC = m_pTrackingCam->GetHeight();
	std::cout<<"> Dimensions: "<<m_uiWidthTWC<<", "<<m_uiHeightTWC<<std::endl;
	std::cout<<"Load intrinsic parameters ... ";
	
/*Calibration Cameras*/

	m_pTWCCalibTarget = new ChessboardCalibration(m_pTrackingCam->m_pCapture,100, 4 ,5, 0.0474);
	/*
	m_pTCCalibWorld = new ChessboardCalibration(m_pTrackingCam, 100, 11, 8, 0.12); // calibrage de la camera tracking dans le repère du monde
	m_pWCCalibRelative = new ChessboardCalibration((Camera*)m_pWebcam,100, 4 ,5, 0.056);
	m_pWCCalibWorld = new ChessboardCalibration((Camera*)m_pWebcam,100, 8 ,11, 0.12);
	m_pTCCalibRelative = new ChessboardCalibration((Camera*)m_pTrackingCam, 100, 4 ,5, 0.0474); // calibrage de la camera tracking pour l'acquisition
	//*/
	
	char key;
			
	/*init TTWC_to_target*/
	/*std::cout << "Calibrate Target chessboard in Tracking Webcam frame ?? (press letter+Enter)" << std::endl;
	std::cin >> key ;
	m_pTCCalibRelative->findExtrinsicParameters(0,0,NULL);//*/
	std::cout <<"Load TTWC_to_target transformation"<<std::endl;
	m_pTrackingCam->Load("TWC.internal", "TWC-T.external");
	m_pTrackingCam->m_pCapture->loadIntrinsicParameters("TWC.internal");
	
	TRot=m_pTrackingCam->GetR();
	TTrans=m_pTrackingCam->GetT();
	
	TTWC_to_target = gk::Transform(gk::Matrix4x4(TRot[0], TRot[1], TRot[2], TTrans[0], TRot[3], TRot[4], TRot[5], TTrans[1], TRot[6], TRot[7], TRot[8],TTrans[2], 0.0, 0.0, 0.0, 1.0));
	TTWC_to_target.print();
	std::cout<<"\t\t\tTranslation de "<<sqrt(TTrans[0]*TTrans[0]+TTrans[1]*TTrans[1]+TTrans[2]*TTrans[2])<<" metres"<<std::endl;
	std::cout <<"> inverse"<<std::endl;
	TTWC_to_target.getInverse().print();
	
	/*init TTWC_to_world*/
	/*std::cout << "Calibrate World chessboard in Tracking Webcam frame ?? (press letter+Enter)" << std::endl;
	std::cin >> key ;
	m_pTCCalibWorld->findExtrinsicParameters(0,0,NULL);//*/
	std::cout <<"Load TTWC_to_world transformation"<<std::endl;
	m_pTrackingCam->Load("TWC.internal", "TWC-W.external");
	
	TRot=m_pTrackingCam->GetR();
	TTrans=m_pTrackingCam->GetT();
	
	TTWC_to_world = gk::Transform(gk::Matrix4x4(TRot[0], TRot[1], TRot[2], TTrans[0], TRot[3], TRot[4], TRot[5], TTrans[1], TRot[6], TRot[7], TRot[8],TTrans[2], 0.0, 0.0, 0.0, 1.0));
	TTWC_to_world.print();
	std::cout<<"\t\t\tTranslation de "<<sqrt(TTrans[0]*TTrans[0]+TTrans[1]*TTrans[1]+TTrans[2]*TTrans[2])<<" metres"<<std::endl;
	std::cout <<"> inverse"<<std::endl;
	TTWC_to_world.getInverse().print();

	
	/*init TWC_to_world*/
	/*std::cout << "Calibrate World chessboard in Webcam frame ?? (press letter+Enter)" << std::endl;
	std::cin >> key ;
	m_pWCCalibWorld->findExtrinsicParameters(0,0,NULL);//*/
	std::cout <<"Load TWC_to_world transformation"<<std::endl;
	m_pWebcam->Load("WC.internal", "WC-W.external");

	TRot=m_pWebcam->GetR();
	TTrans=m_pWebcam->GetT();
	
	TWC_to_world = gk::Transform(gk::Matrix4x4(TRot[0], TRot[1], TRot[2], TTrans[0], TRot[3], TRot[4], TRot[5], TTrans[1], TRot[6], TRot[7], TRot[8],TTrans[2], 0.0, 0.0, 0.0, 1.0));
	TWC_to_world.print();
	std::cout<<"\t\t\tTranslation de "<<sqrt(TTrans[0]*TTrans[0]+TTrans[1]*TTrans[1]+TTrans[2]*TTrans[2])<<" metres"<<std::endl;
	std::cout <<"> inverse"<<std::endl;
	TWC_to_world.getInverse().print();
	
	/*init TSR_to_world*/
	/*std::cout << "Calibrate World chessboard in SwissRanger frame ?? (press letter+Enter)" << std::endl;
	std::cin >> key ;
	m_pSRCalibWorld->findExtrinsicParameters(0,0,NULL);//*/
	std::cout <<"Load TSR_to_world transformation"<<std::endl;
	m_pSwissRanger->Load("SR.internal", "SR-W.external");

	TRot=m_pSwissRanger->GetR();
	TTrans=m_pSwissRanger->GetT();
	
	TSR_to_world = gk::Transform(gk::Matrix4x4(TRot[0], TRot[1], TRot[2], TTrans[0], TRot[3], TRot[4], TRot[5], TTrans[1], TRot[6], TRot[7], TRot[8],TTrans[2], 0.0, 0.0, 0.0, 1.0));
	TSR_to_world.print();
	std::cout<<"\t\t\tTranslation de "<<sqrt(TTrans[0]*TTrans[0]+TTrans[1]*TTrans[1]+TTrans[2]*TTrans[2])<<" metres"<<std::endl;
	std::cout <<"> inverse"<<std::endl;
	TSR_to_world.getInverse().print();
	
	/*Transformations*/
	/**World to Target*/
		Tworld_to_target = TTWC_to_world.getInverse() * TTWC_to_target;
		std::cout <<"Tworld_to_target"<<std::endl;
		Tworld_to_target.print();
	/**Target_to_SR*/
		Ttarget_to_SR = Tworld_to_target.getInverse() * TSR_to_world.getInverse();
		std::cout <<"Ttarget_to_SR"<<std::endl;
		Ttarget_to_SR.print();
	/**SR_to_WC*/
		TSR_to_WC = TSR_to_world * TWC_to_world.getInverse();
		std::cout <<"TSR_to_WC"<<std::endl;
		TSR_to_WC.print();
	
	/*Webcam position*/
	Vector3D oWebcamPosition(m_pWebcam->GetPositionVector()[0], m_pWebcam->GetPositionVector()[1], m_pWebcam->GetPositionVector()[2]);
	Vector3D oWebcamDirection(m_pWebcam->GetDirection()[0], m_pWebcam->GetDirection()[1], m_pWebcam->GetDirection()[2]);
	std::cout << "Webcam Direction : (" << oWebcamDirection[0] << ", " << oWebcamDirection[1] << ", " << oWebcamDirection[2] << ")" << std::endl;
		
	/*SwissRanger position*/
	Vector3D oSwissRangerPosition( m_pSwissRanger->GetPositionVector()[0], m_pSwissRanger->GetPositionVector()[1], m_pSwissRanger->GetPositionVector()[2] );
	Vector3D oSwissRangerDirection( m_pSwissRanger->GetDirection()[0], m_pSwissRanger->GetDirection()[1], m_pSwissRanger->GetDirection()[2] );
	std::cout << "SwissRanger Direction : (" << oSwissRangerDirection[0] << ", " << oSwissRangerDirection[1] << ", " << oSwissRangerDirection[2] << ")" << std::endl;

	/*Copy parameters to OpenGL cameras*/
	memcpy( m_oCameraGLWC.frustum, m_pWebcam->GetFrustum(), sizeof( float ) * 6 );
	memcpy( m_oCameraGLWC.direction, m_pWebcam->GetDirection(), sizeof( float ) * 3 );
	memcpy( m_oCameraGLSR.frustum, m_pSwissRanger->GetFrustum(), sizeof( float ) * 6 );
	memcpy( m_oCameraGLSR.direction, m_pSwissRanger->GetDirection(), sizeof( float ) * 3 );
	
	std::cout<<"Initialize file \"Images/realScene_mat.txt\"...";
	std::ofstream matF ("Images/realScene_mat.txt", std::ios::trunc);
	if (matF.is_open())
    {
    	matF	<< "#\terror\ttarget_to_world tranformation matrix [4x4]" << std::endl;
        matF.close();
        std::cout<<" done"<<std::endl;
    }
    else
    {
        std::cout<<" Unable to open file"<<std::endl;
    }
    
    std::cout<<"Initialize file \"Images/realScene_mat_180.txt\"...";
	std::ofstream matF180 ("Images/realScene_mat_180.txt", std::ios::trunc);
	if (matF180.is_open())
    {
    	matF180	<< "#\terror\ttarget_to_world tranformation matrix [4x4]" << std::endl;
        matF180.close();
        std::cout<<" done"<<std::endl;
    }
    else
    {
        std::cout<<" Unable to open file"<<std::endl;
    }
    
   	/*Get Colors images
	m_pWebcam->grabLdrImagesInit();//, 1, 10000);
	calibrate_camera_for_HDR(m_pWebcam->getLdrImages(), 9, m_pWebcam->getTabTe(), 9, 20, "testLdr.txt");//*/
	
	load_G_func_channel("Images/GfonctionPro9000.txt",m_gFunction);
}

void Hybrid::Release()
{
	if( NULL != m_pSwissRanger )
	{
		delete m_pSwissRanger;
		m_pSwissRanger = NULL;
	}
	if( NULL != m_pWebcam )
	{
		delete m_pWebcam;
		m_pWebcam = NULL;
	}
}

void Hybrid::ProcessEvent()
{
	SDL_Event event;
	/* Process an event */
	while ( SDL_PollEvent( &event ) )
	{
		/* Which one? */
		switch (event.type)
		{
			// Key down
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						m_bExit = true;
						break;
					case SDLK_SPACE:
						if(m_bStepIm)
							m_bStepIm = false;
						else
							m_bStepIm = true;
						break;
					case SDLK_s:
						m_bSaveIm = true;
						break;
					case SDLK_c:
						m_bContIm = true;
						break;
					default:
						m_pCamera->OnKeyboard( event.key );
				}
				break;
			case SDL_KEYUP:
				m_pCamera->OnKeyboard( event.key );
				break;
			// Mouse button up
			case SDL_MOUSEBUTTONUP:
			// Mouse button down
			case SDL_MOUSEBUTTONDOWN:
				m_pCamera->OnMouseButton( event.button );
				break;
			// Manage mouse motion
			case SDL_MOUSEMOTION:
				m_pCamera->OnMouseMotion( event.motion );
				break;
		}
	}
}

void Hybrid::Draw()
{
	gk::Matrix4x4 mat;	
	
	if(m_bStepIm)
	{
		
		if(m_bSaveIm)
		{
		/* Enregistrement de l'image Color+Z */
			char filename[32];
			m_iSerial++;
			sprintf(filename,"Images/realScene_test_%.4d.exr", m_iSerial);
			
			/*char WCHDRimage[32];
			sprintf(WCHDRimage,"Images/realScene_testWC_%.4d.exr", m_iSerial);
			WriteEXRImage (WCHDRimage, m_uiWidthWC, m_uiHeightWC, m_pWebcam->GetImageHDR());//*/
			
			std::cout<<"> Save Image \""<<filename<<"\"...";
			WriteEXRImage (filename, m_uiWidthSR , m_uiHeightSR, m_pSwissRanger->GetXYZColorHDR(), m_pSwissRanger->GetXYZCoords(),  m_pSwissRanger->GetConfData() );
			std::cout<<" done"<<std::endl;
		/* Enregistrement de la matrice world_to_target + erreur */
			std::cout<<"> Save matrix in \"Images/realScene_mat.txt\"...";
			std::ofstream matF ("Images/realScene_mat.txt", std::ios::app);
			if (matF.is_open())
		    {
		    	mat = TSR_to_world.matrix();
		    	//TSR_to_world.print();
		    	matF	<< m_iSerial
		    			<< "\t"<<error
		    			<< "\t"<<mat[0] <<" "<<mat[1] <<" "<<mat[2] <<" "<<mat[3]
		    			<<" "<<mat[4] <<" "<<mat[5] <<" "<<mat[6] <<" "<<mat[7]
		    			<<" "<<mat[8] <<" "<<mat[9] <<" "<<mat[10] <<" "<<mat[11]
		    			<<" "<<mat[12] <<" "<<mat[13] <<" "<<mat[14] <<" "<<mat[15]
		    			<<std::endl;
		        matF.close();
		        std::cout<<" done"<<std::endl;
		    }
		    else
		    {
		        std::cout<<" Unable to open file"<<std::endl;
		    }
		    
		    std::cout<<"> Save matrix*180 in \"Images/realScene_mat_180.txt\"...";
			std::ofstream matF180 ("Images/realScene_mat_180.txt", std::ios::app);
			if (matF180.is_open())
		    {
		    	gk::Transform Tmat = gk::RotateZ(180.0) * TSR_to_world;
		    	mat = Tmat.matrix();
		    	//Tmat.print();
		    	matF180	<< m_iSerial
		    			<< "\t"<<error
		    			<< "\t"<<mat[0] <<" "<<mat[1] <<" "<<mat[2] <<" "<<mat[3]
		    			<<" "<<mat[4] <<" "<<mat[5] <<" "<<mat[6] <<" "<<mat[7]
		    			<<" "<<mat[8] <<" "<<mat[9] <<" "<<mat[10] <<" "<<mat[11]
		    			<<" "<<mat[12] <<" "<<mat[13] <<" "<<mat[14] <<" "<<mat[15]
		    			<<std::endl;
		        matF180.close();
		        std::cout<<" done"<<std::endl;
		    }
		    else
		    {
		        std::cout<<" Unable to open file"<<std::endl;
		    }
			
			std::cout<<"------------------------------------------------------"<<std::endl<<std::endl;
		}
		else
		{
			std::cout<<"> Continue to next image"<<std::endl<<"------------------------------------------------------"<<std::endl<<std::endl;
		}
		std::cout<<"Step by step mode"<<std::endl;
	}
	else
	{
		std::cout<<"------------------------------------------------------"<<std::endl<<std::endl<<"Continuous mode"<<std::endl;
	}
	
	/*Get tracking image*/
	//m_pTrackingImage = m_pTrackingCam->GetImage();
	m_pTrackingCam->GrabImage();
	
	/*Get HDR Color image*/
	if(m_bStepIm)
	{
		/*Get Depth image*/
		m_pSwissRanger->GrabDepth();//Median(5);
		m_pWebcam->grabLdrImages();
		create_EXR_channels_from_LDR_image(m_pWebcam->getLdrImages(), m_pWebcam->getTabTe(), 3, m_gFunction, m_uiWidthWC, m_uiHeightWC, m_pWebcam->GetImageHDR());
		/*Compute color image in 3D world position*/
		ComputeGeometryColorHDR();
	}
	else
	{
		/*Get Depth image*/
		m_pSwissRanger->GrabDepth();
		if(  m_pWebcam->m_pCapture->getParameter(V4L2_CID_EXPOSURE_ABSOLUTE)!=m_uiExpoWC)
		{
			std::cout<<"Change exposure to default value"<<std::endl;
			m_pWebcam->m_pCapture->setParameter(V4L2_CID_EXPOSURE_ABSOLUTE, m_uiExpoWC);
		}	
		m_pWebcam->GrabImage();
		/*Compute color image in 3D world position*/
		ComputeGeometryColor();
	}

	/*Compute new transformation*/
	//cvNamedWindow("calib_image", CV_WINDOW_AUTOSIZE);
	error=m_pTWCCalibTarget->findExtrinsicParameters(0,0,NULL);//"calib_image");
	//cvWaitKey(30);
	//cvDestroyWindow("calib_image");
	//error = m_pTrackingCam->findExtrinsicParameters(4, 5, 0.0474, 100);
	std::cout<<"> Calibration :"<<std::endl<<"   error = "<<error<<std::endl;
	if( error < 999998.0)
	{
		/*TRot=m_pTrackingCam->GetR();
		TTrans=m_pTrackingCam->GetT();//*/
		
		TRot=m_pTrackingCam->m_pCapture->extrinsicR;
		TTrans=m_pTrackingCam->m_pCapture->extrinsicT;
		
		TTWC_to_target = gk::Transform(gk::Matrix4x4(TRot[0], TRot[1], TRot[2], TTrans[0], TRot[3], TRot[4], TRot[5], TTrans[1], TRot[6], TRot[7], TRot[8],TTrans[2], 0.0, 0.0, 0.0, 1.0));
		//TTWC_to_target.print();
		Tworld_to_target = TTWC_to_world.getInverse() * TTWC_to_target;
		TSR_to_world = Ttarget_to_SR.getInverse() * Tworld_to_target.getInverse();
		std::cout<<"   SR to world matrix :"<<std::endl;
		TSR_to_world.print();
	}
	
/*3D Display*/
	glClearColor(.2,.2,.2,0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	/*glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);//*/

	float factor = 1.0f;  ///< Brice Adding
	glViewport( 0, m_pOrigViewport[2],m_uiWidth3D, m_uiHeight3D-m_pOrigViewport[2] );
	
	/**Projection mode*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum ( m_oCameraGLWC.frustum[0]*factor, m_oCameraGLWC.frustum[1]*factor, m_oCameraGLWC.frustum[2]*factor,
			m_oCameraGLWC.frustum[3]*factor, m_oCameraGLWC.frustum[4]*factor, m_oCameraGLWC.frustum[5] );
	
	glGetFloatv(GL_PROJECTION_MATRIX, mat);
	TFrustum = gk::Transform(mat);
	
	/**Scene Positionning*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	mat = TTWC_to_world.matrix().Transpose();
	gluLookAt(0., 0., 0., mat[12], mat[13], mat[14], 0., -1., 0. ); 
						/*/m_oCameraGLWC.direction[0], m_oCameraGLWC.direction[1], m_oCameraGLWC.direction[2], 0., -1., 0. );//*/
	glMultMatrixf ( mat ); /*/ m_pWebcam->GetPositionMatrix() );//*/
	
	/*std::cout<<">>MATRICES world_to_WC et PositionMatrix :"<<std::endl;
	Tworld_to_WC.print();
	printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pWebcam->m_pPositionMatrix[0], m_pWebcam->m_pPositionMatrix[1], m_pWebcam->m_pPositionMatrix[2], m_pWebcam->m_pPositionMatrix[3] );
	printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pWebcam->m_pPositionMatrix[4], m_pWebcam->m_pPositionMatrix[5], m_pWebcam->m_pPositionMatrix[6], m_pWebcam->m_pPositionMatrix[7] );
	printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pWebcam->m_pPositionMatrix[8], m_pWebcam->m_pPositionMatrix[9], m_pWebcam->m_pPositionMatrix[10], m_pWebcam->m_pPositionMatrix[11] );
	printf( "[ %.5f\t  %.5f\t  %.5f\t  %.5f ]\n", m_pWebcam->m_pPositionMatrix[12], m_pWebcam->m_pPositionMatrix[13], m_pWebcam->m_pPositionMatrix[14], m_pWebcam->m_pPositionMatrix[15] );//*/
	
	/**OpenGL camera control*/
	Display::CameraControl();
	
	/**World basis*/
	glLineWidth(3.f);
	Display::DrawAxes( .6f );
	glLineWidth(1.f);
	/*glPointSize(10.f);
	glColor3f(1.,1.,1.);
	glBegin(GL_POINTS);
		glVertex3i(0,0,0);
	glEnd();
	glPointSize(1);//*/
	
	
	/*glPushMatrix();
		mat = Tworld_to_target.matrix().Transpose();
		//gluLookAt(0., 0., 0., mat[12], mat[13], mat[14], 0., -1., 0. ); 
		glMultMatrixf(mat);
		//glMultTransposeMatrixf(Tworld_to_target.getGLTransposeMatrix());
	/**Target basis
		Display::DrawAxes( 0.2f );
		glPointSize(10.f);
		glColor3f(0.,1.,0.);
		glBegin(GL_POINTS);
			glVertex3i(0,0,0);
		glEnd();
		glPointSize(1);
		
		glMultMatrixf(Ttarget_to_SR.matrix().Transpose());
	/**Swiss Ranger basis
		Display::DrawAxes( 0.2f );
		glPointSize(10.f);
		glColor3f(1.0,0.,0.);
		glBegin(GL_POINTS);
			glVertex3i(0,0,0);
		glEnd();
		glPointSize(1);
		glPushMatrix();
			glRotatef( 180.0f, 0.0f, 0.0f, 1.0f );
			m_pSwissRanger->DrawColor();
		glPopMatrix();
		
		glMultMatrixf(TSR_to_WC.matrix().Transpose());
	/**Webcam basis
		Display::DrawAxes( 0.2f );
		glPointSize(10.f);
		glColor3f(0.,0.,1.0);
		glBegin(GL_POINTS);
			glVertex3i(0,0,0);
		glEnd();
		glPointSize(1);
	glPopMatrix();//*/
		
	
	glPushMatrix();
		mat = TWC_to_world.inverseMatrix().Transpose();
		//gluLookAt(0., 0., 0., mat[12], mat[13], mat[14], 0., -1., 0. ); 
		glMultMatrixf(mat);
	/**Webcam basis*/
		Display::DrawAxes( 0.2f );
		glPointSize(10.f);
		glColor3f(0.,0.,0.6);
		glBegin(GL_POINTS);
			glVertex3i(0,0,0);
		glEnd();
		glPointSize(1);//*/
	glPopMatrix();
	
	glPushMatrix();
		gk::Transform Tmat = gk::RotateZ(180.0) * TSR_to_world;
		mat = Tmat.inverseMatrix().Transpose();
		//mat = TSR_to_world.inverseMatrix().Transpose();
		glMultMatrixf(mat);
		//glRotatef( 180.0f, 0.0f, 0.0f, 1.0f );
	/**Swiss Ranger basis*/
		Display::DrawAxes( 0.2f );
		glPointSize(10.f);
		glColor3f(0.6,0.,0.);
		glBegin(GL_POINTS);
			glVertex3i(0,0,0);
		glEnd();
		glPointSize(1);//*/

		m_pSwissRanger->DrawColor();
	glPopMatrix();
	
	glPushMatrix();
		mat = TTWC_to_world.inverseMatrix().Transpose();
		//gluLookAt(0., 0., 0., mat[12], mat[13], mat[14], 0., -1., 0. ); 
		glMultMatrixf(mat);
	/**Tracking Webcam basis*/
		Display::DrawAxes( 0.2f );
		glPointSize(10.f);
		glColor3f(0.5,0.5,0.5);
		glBegin(GL_POINTS);
			glVertex3i(0,0,0);
		glEnd();
		glPointSize(1);
	glPopMatrix();

/*Thumbnails display*/
	glDisable(GL_DEPTH_TEST);
	
	/**Webcam Image*/
	glViewport(m_pOrigViewport[0], m_pOrigViewport[0], m_uiWidthViewport, m_uiHeightViewport);
	glMatrixMode ( GL_PROJECTION );
	glPushMatrix();
		glLoadIdentity ();
		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix();
			glLoadIdentity ();
			glScalef(2.0,2.0,1.0);
			glTranslatef(-0.5,-0.5,0.0);
			m_pWebcam->Draw();
		glPopMatrix ();
	glPopMatrix();
	
	/*Affichage repere monde sur vignette (marche pas)
	glMatrixMode ( GL_PROJECTION );
	glPushMatrix();
		//glLoadIdentity();
		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix();
			//Display::DrawAxes( 0.5f );
			glLoadIdentity();
			mat = TWC_to_world.matrix().Transpose();
			gluLookAt(0., 0., 0., mat[12], mat[13], mat[14], 0., -1., 0. );
			glMultMatrixf(mat);
			
			Display::DrawAxes( 1.0f );
			glPointSize(10.f);
			glBegin(GL_POINTS);
				glColor3f(1.0,1.0,1.0);
				glVertex3i(0,0,0);
				glColor3f(1.0,0.0,0.0);
				glVertex3i(2,0,0);
				glColor3f(0.0,1.0,0.0);
				glVertex3i(0,2,0);
				glColor3f(0.0,0.0,1.0);
				glVertex3i(0,0,2);
			glEnd();
			glPointSize(1);
		glPopMatrix();
	glPopMatrix();//*/
	
	/**Depth Image*/
	glViewport(m_pOrigViewport[1], m_pOrigViewport[0], m_uiWidthViewport, m_uiHeightViewport);
	glMatrixMode ( GL_PROJECTION );
	glPushMatrix();
		glLoadIdentity ();
		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix();
			glLoadIdentity ();
			glScalef(2.0,2.0,1.0);
			glTranslatef(-0.5,-0.5,0);
			m_pSwissRanger->DrawImage();
		glPopMatrix ();
	glPopMatrix ();
	
	/**Tracking Image*/
	glViewport(m_pOrigViewport[3], m_pOrigViewport[0], m_uiWidthViewport, m_uiHeightViewport);
	glMatrixMode ( GL_PROJECTION );
	glPushMatrix();
		glLoadIdentity ();
		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix();
			glLoadIdentity ();
			glScalef(2.0,2.0,1.0);
			glTranslatef(-0.5,-0.5,0);
			m_pTrackingCam->Draw();
		glPopMatrix ();
	glPopMatrix();
	
	/*glMatrixMode ( GL_PROJECTION );
	glPushMatrix();
		//glLoadIdentity();
		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix();
			glLoadIdentity ();
			//Display::DrawAxes( 0.5f );
			mat = TTWC_to_world.matrix().Transpose();
			gluLookAt(0., 0., 0., mat[12], mat[13], mat[14], 0., -1., 0. );
			glMultMatrixf(mat);
			Display::DrawAxes( 1.0f );
			glPointSize(10.f);
			glBegin(GL_POINTS);
				glColor3f(1.0,1.0,1.0);
				glVertex3i(0,0,0);
				glColor3f(1.0,0.0,0.0);
				glVertex3i(2,0,0);
				glColor3f(0.0,1.0,0.0);
				glVertex3i(0,2,0);
				glColor3f(0.0,0.0,1.0);
				glVertex3i(0,0,2);
			glEnd();
			glPointSize(1);
		glPopMatrix();
	glPopMatrix();//*/
	
/*/
	glViewport(m_pOrigViewport[3], m_pOrigViewport[0], m_uiWidthViewport, m_uiHeightViewport);
	glMatrixMode ( GL_PROJECTION );
	glPushMatrix();
		glLoadIdentity ();
		glMatrixMode ( GL_MODELVIEW );
		glPushMatrix();
			glLoadIdentity ();
			glScalef(2.0,2.0,1.0);
			glTranslatef(-0.5,-0.5,0);
			if( m_pTrackingImage != NULL )
			{
			glEnable( GL_TEXTURE_RECTANGLE_ARB );
			glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iTrackingTexture );
			glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_CAMERA_COLOR_MODE,
					m_pTrackingImage->width, m_pTrackingImage->height, 0,
					GL_CAMERA_COLOR_MODE, GL_UNSIGNED_BYTE, m_pTrackingImage->imageData );
			}

			glColor3f ( 1.f, 1.f, 1.f );
			glBegin ( GL_POLYGON );
				glTexCoord2f ( 0.0f, 480 );		glVertex3f ( 0.0, 0.0, 0.0f );
				glTexCoord2f ( 640, 480 );	glVertex3f ( 1.0, 0.0f, 0.0f );
				glTexCoord2f ( 640, 0.0 );		glVertex3f (1.0f, 1.0f, 0.0f );
				glTexCoord2f ( 0.0f, 0.0f );			glVertex3f ( 0.0f, 1.0, 0.0f );
			glEnd ();
			//~ glBindTexture( GL_TEXTURE_RECTANGLE_ARB, 0 ); // Bind the OpenGL Texture.
			glDisable( GL_TEXTURE_RECTANGLE_ARB );
		glPopMatrix ();
	glPopMatrix();//*/
	
	glViewport( 0, 0,m_uiWidth3D, m_uiHeight3D );
	glEnable(GL_DEPTH_TEST);
}

/** Code Brice **/
void Hybrid::ComputeGeometryColor()
{
	static unsigned int n_channels = 3;
	static int fb_width = m_uiWidthWC;
	static int fb_height = m_uiHeightWC;

	static unsigned char *fb_data = NULL;
	static  FBO* fbo = NULL;
	if (fb_data == NULL)
	{
		fb_data = (unsigned char*) malloc(sizeof(unsigned char)*fb_width*fb_height*n_channels);
		assert( fb_data);
		fbo = FBO_init(fb_width,fb_height);
	}

	FBO_active(fbo);
	glViewport( 0, 0,fb_width, fb_height);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	{
		float factor = 0.1f;

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity( );
		gluLookAt (	0., 0., 0.,/*position*/
					m_oCameraGLWC.direction [ 0 ], m_oCameraGLWC.direction [ 1 ], m_oCameraGLWC.direction [ 2 ],/*direction*/
					0., -1., 0. );/*up direction*/
//		Display::DrawAxes( 1.0f );
		glMultMatrixf ( m_pWebcam->GetPositionMatrix() );/*go to camera position*/

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );

		glFrustum (	m_oCameraGLWC.frustum[0]*factor, m_oCameraGLWC.frustum[1]*factor,
					m_oCameraGLWC.frustum[2]*factor, m_oCameraGLWC.frustum[3]*factor,
					1.f*factor, 1000.0f ); //m_oCameraGLWC.frustum[4], m_oCameraGLWC.frustum[5] );

		glPushMatrix();
			glMatrixMode( GL_MODELVIEW );
			glMultMatrixf( m_pSwissRanger->GetPositionMatrixInverse() );/*go to world basis*/
			glRotatef( 180.0f, 0.0f, 0.0f, 1.0f );
			m_pSwissRanger->DrawCodedColor();/*Coded color 3D drawing*/
		glPopMatrix();
	}
	glReadPixels ( 0, 0,fb_width,fb_height, GL_RGB, GL_UNSIGNED_BYTE, fb_data);
	unsigned char* ptr_fb_data = NULL;
	unsigned char* ptr_webcam_dat = NULL;
	unsigned int x_index, y_index;
	for (int i=0; i <fb_height * fb_width; ++i)
	{
		ptr_webcam_dat = (unsigned char*) &m_pWebcam->GetImage()->imageData[i*3];
		ptr_fb_data = &fb_data[i *3];
		x_index=(unsigned int) ptr_fb_data[0];
		y_index=(unsigned int) ptr_fb_data[1];
		m_pSwissRanger->GetXYZColor()[x_index * m_uiWidthSR *3 + y_index * 3]=ptr_webcam_dat[0]/255.0f;
		m_pSwissRanger->GetXYZColor()[x_index * m_uiWidthSR *3 + y_index * 3+1]=ptr_webcam_dat[1]/255.0f;
		m_pSwissRanger->GetXYZColor()[x_index * m_uiWidthSR *3 + y_index * 3+2]=ptr_webcam_dat[2]/255.0f;
	}
	FBO_desactive();
}

void Hybrid::ComputeGeometryColorHDR()
{
	static unsigned int n_channels = 3;
	static int fb_width = m_uiWidthWC;
	static int fb_height = m_uiHeightWC;

	static unsigned char *fb_data = NULL;
	static  FBO* fbo = NULL;
	if (fb_data == NULL)
	{
		fb_data = (unsigned char*) malloc(sizeof(unsigned char)*fb_width*fb_height*n_channels);
		assert( fb_data);
		fbo = FBO_init(fb_width,fb_height);
	}
	else
	{
		unsigned char a = (unsigned char)0;
		for (int i=0; i<fb_width*fb_height*n_channels; i++)
		{
			fb_data[i]=a;
		}
	}
	for(int i=0; i<m_uiWidthSR*m_uiHeightSR*3; i++)
	{
		m_pSwissRanger->GetXYZColorHDR()[i]=-1.0;
	}//*/

	FBO_active(fbo);
	glViewport( 0, 0,fb_width, fb_height);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	{
		float factor = 0.1f;

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity( );
		gluLookAt (	0., 0., 0.,/*position*/
					m_oCameraGLWC.direction [ 0 ], m_oCameraGLWC.direction [ 1 ], m_oCameraGLWC.direction [ 2 ],/*direction*/
					0., -1., 0. );/*up direction*/
//		Display::DrawAxes( 1.0f );
		glMultMatrixf ( m_pWebcam->GetPositionMatrix() );/*go to camera position*/

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );

		glFrustum (	m_oCameraGLWC.frustum[0]*factor, m_oCameraGLWC.frustum[1]*factor,
					m_oCameraGLWC.frustum[2]*factor, m_oCameraGLWC.frustum[3]*factor,
					1.f*factor, 1000.0f ); //m_oCameraGLWC.frustum[4], m_oCameraGLWC.frustum[5] );

		glPushMatrix();
			glMatrixMode( GL_MODELVIEW );
			glMultMatrixf( m_pSwissRanger->GetPositionMatrixInverse() );/*go to world basis*/
			glRotatef( 180.0f, 0.0f, 0.0f, 1.0f );
			m_pSwissRanger->DrawCodedColor();/*Coded color 3D drawing*/
		glPopMatrix();
	}
	glReadPixels ( 0, 0,fb_width,fb_height, GL_RGB, GL_UNSIGNED_BYTE, fb_data);
	unsigned char* ptr_fb_data = NULL;
	float* ptr_webcam_dat = NULL;
	unsigned int x_index, y_index;
	for (int i=0; i <fb_height * fb_width; ++i)
	{
		ptr_webcam_dat = (float*) &m_pWebcam->GetImageHDR()[i*3];
		ptr_fb_data = &fb_data[i *3];
		x_index=(unsigned int) ptr_fb_data[0];
		y_index=(unsigned int) ptr_fb_data[1];
		m_pSwissRanger->GetXYZColorHDR()[x_index * m_uiWidthSR *3 + y_index * 3]=ptr_webcam_dat[0];
		m_pSwissRanger->GetXYZColorHDR()[x_index * m_uiWidthSR *3 + y_index * 3+1]=ptr_webcam_dat[1];
		m_pSwissRanger->GetXYZColorHDR()[x_index * m_uiWidthSR *3 + y_index * 3+2]=ptr_webcam_dat[2];
	}
	FBO_desactive();
}

void Hybrid::DrawCamera( LA::Camera* pCamera )
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		gluLookAt ( 0., 0., 0., 0., 0., 1., 0., -1.0, 0. );
		glMultMatrixf ( pCamera->GetPositionMatrix() );

		Display::DrawAxes( 10.0f );
		pCamera->DrawImage();
	}
	glPopMatrix();
}

void Hybrid::DrawForeground( )
{
//cvShowImage("Foreground", m_pSwissRanger->GetForeground()) ;

cvSaveImage("rada.bmp",m_pSwissRanger->GetForeground());

//  std::cout<<"RESULT::::::::::"<<res<<std::endl;
    // Idle until the user hits the “Esc” key.


}
