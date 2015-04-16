#include "Webcam.hpp"
#include <pthread.h>

using namespace LA;

Webcam::Webcam( uint uiWidth, uint uiHeight ) :
	m_pWebcamThread( NULL ),
	m_pMutexWebCam( NULL ),
	m_pCapture( NULL ),
	m_pCamImageBack( NULL ),
	m_pCameraImage( NULL ),
	m_pLdrImages( NULL ),
	m_pTabTe( NULL ),
	m_uiWidth( uiWidth ),
	m_uiHeight( uiHeight ),
	m_iCamTexture( 0 ),
	m_bInitTexture( false ),
	m_bExit( false )
{
}

Webcam::~Webcam()
{
	Release();
}

void Webcam::Create( uint uiWidth, uint uiHeight, int numCam )
{
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_pEXRData = new float[m_uiWidth*m_uiHeight*3];
	m_pLdrImages = new IplImage*[20];
	m_pTabTe=new float[20];
	Initialize(numCam);
}

void Webcam::Release()
{
	m_bExit = true;
	SDL_KillThread( m_pWebcamThread );
	//~ SDL_WaitThread( m_pWebcamThread, NULL );
	SDL_DestroyMutex( m_pMutexWebCam );

	cvReleaseCapture( &m_pCapture ); /// Actually, we are unable to test the result... OpenCV should do it...
}

void Webcam::Initialize(int numCam)
{
	/// @todo Add texture which include OpenCV image in OpenGL window.

	// Start capture from camera.
	//cvCaptureFromCAMExt(m_pCapture,0);
    m_pCapture = new CameraUVC();
    m_pCapture->open(numCam, m_uiWidth, m_uiHeight, 15, V4L2_PIX_FMT_MJPEG);
	ASSERT_TRUE( m_pCapture != NULL ) << "\t ==> Maybe not webcam pluged???";


	//cvSetCaptureProperty( m_pCapture, CV_CAP_PROP_FRAME_WIDTH, m_uiWidth );
	//cvSetCaptureProperty( m_pCapture, CV_CAP_PROP_FRAME_HEIGHT, m_uiHeight );

	// Grab the first frame
#ifdef CAM_FIREWIRE
	m_pCameraImage = cvCreateImage( cvSize( m_uiWidth, m_uiHeight ), IPL_DEPTH_8U, 3 );
	m_pCameraImageBayer = cvCreateImage( cvSize( m_uiWidth, m_uiHeight ), IPL_DEPTH_8U, 1 );
	cvQueryFrameExt( m_pCapture, m_pCameraImageBayer );
#endif
#ifndef CAM_FIREWIRE
	//cvQueryFrameExt( m_pCapture, m_pCameraImage );
	m_pCameraImage = m_pCapture->get1Frame();
	// Convert the frame to use as texture.
	cvCvtColor( m_pCameraImage , m_pCameraImage, CV_BGR2RGB );
#endif
	ASSERT_TRUE( m_pCameraImage != NULL );

#ifdef CAM_FIREWIRE
	cvCvtColor( m_pCameraImageBayer, m_pCameraImage, CV_BayerBG2RGB );
#endif

	m_pMutexWebCam = SDL_CreateMutex();

	//~ Load( "SonyLeft.internal", "SonyLeft.external" );
//	Load( "SonyLeft-Pentax.internal", "SonyLeft-Pentax.external" );
	Load( "WC.internal", "WC-W.external" );


	glEnable( GL_TEXTURE_RECTANGLE_ARB );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures( 1, &m_iCamTexture );
	ASSERT_GE( m_iCamTexture, 0 );

	glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
	LOG << "Webcam Texture ID: " << m_iCamTexture << ENDLOG;

	glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	GrabImage();

	glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_CAMERA_COLOR_MODE,
		m_pCamImageBack->width, m_pCamImageBack->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, m_pCamImageBack->imageData );
}

void Webcam::Start()
{
	m_pWebcamThread = SDL_CreateThread( WebCamLoop, this );
	//~ LOGHERE();
}

void Webcam::Draw()
{
	if( m_pCamImageBack != NULL )
	{
		//~ if( !m_bInitTexture )
		//~ {
			//~ glEnable( GL_TEXTURE_RECTANGLE_ARB );

			//~ glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
			//~ glGenTextures( 1, &m_iCamTexture );
			//~ ASSERT_GE( m_iCamTexture, 0 );

			//~ glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
			//~ LOG << "Webcam Texture ID: " << m_iCamTexture << ENDLOG;

			//~ glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			//~ glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			//~ glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			//~ glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_CAMERA_COLOR_MODE,
				//~ m_pCamImageBack->width, m_pCamImageBack->height,
				//~ 0, GL_RGB, GL_UNSIGNED_BYTE, m_pCamImageBack->imageData );
			//~ m_bInitTexture = true;
		//~ }
		//~ else
		{
			glEnable( GL_TEXTURE_RECTANGLE_ARB );
			glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
			
			glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_CAMERA_COLOR_MODE,
				m_pCamImageBack->width, m_pCamImageBack->height, 0, GL_CAMERA_COLOR_MODE,
				GL_UNSIGNED_BYTE, m_pCamImageBack->imageData );
		}
	}
	
	glColor3f ( 1.f, 1.f, 1.f );
	glBegin ( GL_POLYGON );
	{
		glTexCoord2f ( 0.0f, m_uiHeight );		glVertex3f ( 0.0, 0.0, 0.0f );
		glTexCoord2f ( m_uiWidth, m_uiHeight );	glVertex3f ( 1.0, 0.0f, 0.0f );
		glTexCoord2f ( m_uiWidth, 0.0 );		glVertex3f (1.0f, 1.0f, 0.0f );
		glTexCoord2f ( 0.0f, 0.0f );			glVertex3f ( 0.0f, 1.0, 0.0f );
	}
	glEnd ();
	//~ glBindTexture( GL_TEXTURE_RECTANGLE_ARB, 0 ); // Bind the OpenGL Texture.
	glDisable( GL_TEXTURE_RECTANGLE_ARB );
}

void Webcam::GrabImage()
{
	ASSERT_TRUE( m_pCapture != NULL );
	// Begin Critical Section for WebCam image
	SDL_LockMutex( m_pMutexWebCam );

#ifdef CAM_FIREWIRE
		//~ cvSetImageCOI( m_pCameraImage,1 );
		//~ cvCopy( m_pCameraImage, m_pCameraImageBayer, NULL );
		//~ cvCvtColor( m_pCameraImageBayer, m_pCamImageBack, CV_BayerGB2RGB );
		cvQueryFrameExt( m_pCapture, m_pCameraImageBayer );
		cvCvtColor( m_pCameraImageBayer, m_pCameraImage, CV_BayerGB2BGR );
#else
		cvQueryFrameExt( m_pCapture, m_pCameraImage );
		cvCvtColor( m_pCameraImage , m_pCameraImage, CV_BGR2RGB);
#endif
	ASSERT_TRUE( m_pCameraImage != NULL );
	if( NULL == m_pCamImageBack )
	{
		m_pCamImageBack = cvCreateImage( cvGetSize( m_pCameraImage ), m_pCameraImage->depth, m_pCameraImage->nChannels );
	}
	else
	{
#ifdef CAM_FIREWIRE
		//~ cvSetImageCOI( m_pCameraImage,1 );
		//~ cvCopy( m_pCameraImage, m_pCameraImageBayer, NULL );
		//~ cvCvtColor( m_pCameraImageBayer, m_pCamImageBack, CV_BayerGB2BGR );
		/*cvCvtColor( m_pCameraImageBayer, m_pCamImageBack, CV_BayerGB2BGR );*/
		cvConvertImage(m_pCameraImage, m_pCamImageBack,CV_CVTIMG_FLIP);

		//~ cvCvtColor( m_pCameraImageBayer, m_pCameraImage, CV_BayerBG2RGB );
#else
//		cvCopyImage( m_pCameraImage, m_pCamImageBack );
    cvConvertImage(m_pCameraImage, m_pCamImageBack,CV_CVTIMG_FLIP);
#endif
	}
	ASSERT_TRUE( m_pCamImageBack != NULL );
	ASSERT_TRUE( m_pCamImageBack->nChannels == 3 ) << "\t ==> Not a color image!";

	// End Critical Section for WebCam image
	SDL_UnlockMutex( m_pMutexWebCam );
}

void Webcam::grabLdrImagesInit()
{
	int exp=32;
	char s[128];
	m_pCapture->setParameter(V4L2_CID_EXPOSURE_ABSOLUTE, exp);
	for(uint jj=0; jj<3; jj++)
		GrabImage();
	for(uint ii=0; ii<9; ii++)
	{
		std::cout<<"Change exposure time "<<exp<<"->";
		m_pCapture->setParameter(V4L2_CID_EXPOSURE_ABSOLUTE, exp);
		while( m_pCapture->getParameter(V4L2_CID_EXPOSURE_ABSOLUTE)!=exp)
		{std::cout<<".";}
		std::cout<<m_pCapture->getParameter(V4L2_CID_EXPOSURE_ABSOLUTE)<<" done"<<std::endl;
		
		for(uint jj=0; jj<10; jj++)
			GrabImage();
		
		/*sprintf(s, "%d", ii);
		cvNamedWindow( s, CV_WINDOW_AUTOSIZE );
		cvShowImage( s, GetImage() );//*/
		m_pLdrImages[ii] = cvCloneImage (m_pCamImageBack);
		m_pTabTe[ii] = (float)1/exp;
		exp *= 2;
		
	}
	/*cvWaitKey(0);
	for(uint ii=0; ii<nbIm; ii++)
	{
		sprintf(s, "%d", ii);
		cvDestroyWindow( s );
	}//*/
}
void Webcam::grabLdrImages()//, float bInf, float bSup)
{
	int exp=128;
	char s[128];
	m_pCapture->setParameter(V4L2_CID_EXPOSURE_ABSOLUTE, exp);
	for(uint jj=0; jj<3; jj++)
		GrabImage();
	for(uint ii=0; ii<3; ii++)
	{
		std::cout<<"Change exposure time "<<exp<<"->";
		m_pCapture->setParameter(V4L2_CID_EXPOSURE_ABSOLUTE, exp);
		while( m_pCapture->getParameter(V4L2_CID_EXPOSURE_ABSOLUTE)!=exp)
		{std::cout<<".";}
		std::cout<<m_pCapture->getParameter(V4L2_CID_EXPOSURE_ABSOLUTE)<<" done"<<std::endl;
		
		for(uint jj=0; jj<10; jj++)
			GrabImage();
		
		m_pLdrImages[ii] = cvCloneImage (m_pCamImageBack);
		m_pTabTe[ii] = (float)1/exp;
		exp *= 8;
	}
}

float Webcam::findExtrinsicParameters(uint cbWidth, uint cbHeight, float squareSize, uint imgCountMax)
{
	int cornerCount = (cbHeight-1)*(cbWidth-1);		// inner corner count
	CvPoint2D32f* cb2DCorners = new CvPoint2D32f[imgCountMax*cornerCount];	// 2D coordinates of inner chessboard corners
	if( cb2DCorners == NULL )
	{
		printf("ERROR in ChessboardCalibration::ChessboardCalibration : memory allocation for 2D points failed.\n");
		exit(1);
	}
	CvPoint3D32f* cb3DCorners = new CvPoint3D32f[imgCountMax*cornerCount];	// 3D coordinates of inner chessboard corners
	if( cb3DCorners == NULL )
	{
		printf("ERROR in ChessboardCalibration::ChessboardCalibration : memory allocation for 3D points failed.\n");
		exit(1);
	}
	CvSize imageSize;	// image size in pixels
	unsigned int cb2DCornersSetIndex;	// index of current 2D points set
	bool boool=false;
	float err = 0, err3 = 0;
	
	/* get frame */
	GrabImage();
	
	/* look for one set of 2D points */
	
	// resetCb2DCornersSetIndex();
	cb2DCornersSetIndex = 0;
	// END resetCb2DCornersSetIndex();
	
	// if( add2DCornersSet(currentImage) )
	if( cb2DCornersSetIndex < imgCountMax  )
	{		
		CvPoint2D32f* pcb2DCorners= &(cb2DCorners[cb2DCornersSetIndex*cornerCount]);
		int cornerCountFounded = 0;
	
		//if( cvFindChessboardCorners( this->m_pCamImageBack, cvSize( cbWidth-1, cbHeight-1), pcb2DCorners) == 0 )/*/
		if( cvFindChessboardCorners( this->m_pCamImageBack, cvSize( cbWidth-1, cbHeight-1), pcb2DCorners, &cornerCountFounded, CV_CALIB_CB_ADAPTIVE_THRESH) == 0
			&& 	cvFindChessboardCorners( this->m_pCamImageBack, cvSize( cbWidth-1, cbHeight-1), pcb2DCorners, &cornerCountFounded, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS | CV_CALIB_CB_NORMALIZE_IMAGE) == 0
			&&  cvFindChessboardCorners( this->m_pCamImageBack, cvSize( cbWidth-1, cbHeight-1), pcb2DCorners, &cornerCountFounded, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS) == 0
			&&  cvFindChessboardCorners( this->m_pCamImageBack, cvSize( cbWidth-1, cbHeight-1), pcb2DCorners, &cornerCountFounded, CV_CALIB_CB_ADAPTIVE_THRESH) == 0
			&&  cvFindChessboardCorners( this->m_pCamImageBack, cvSize( cbWidth-1, cbHeight-1), pcb2DCorners, &cornerCountFounded, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE) == 0 )//*/
		{
			// extraction failed
			cvDrawChessboardCorners( this->m_pCamImageBack, cvSize( cbWidth-1, cbHeight-1), pcb2DCorners, cornerCountFounded, 0);
			boool=false;
		}
		else
		{
			// refines the corner locations
			IplImage* grayImage = cvCreateImage( cvGetSize(this->m_pCamImageBack), IPL_DEPTH_8U, 1);
			if( this->m_pCamImageBack->nChannels == 3 )
				cvCvtColor( this->m_pCamImageBack, grayImage, CV_BGR2GRAY);
			else
				cvCopyImage( this->m_pCamImageBack, grayImage);
		
			cvFindCornerSubPix( grayImage, pcb2DCorners, cornerCount, cvSize( 5, 5), cvSize( 5, 5), cvTermCriteria( CV_TERMCRIT_ITER, 100, 0.1));
			
			cvReleaseImage(&grayImage);
			
			// draw corners
			cvDrawChessboardCorners( this->m_pCamImageBack, cvSize( cbWidth-1, cbHeight-1), pcb2DCorners, cornerCount, 1);
			boool=true;
		}
	// END if( add2DCornersSet(currentImage) )	
	
		if( boool )
		{
			cb2DCornersSetIndex++;
			imageSize = cvGetSize(this->m_pCamImageBack);
			
			/* calculate chessboard points 3D position */
			
			// calc3DPoints( dx, dy);
			unsigned int imgCount = cb2DCornersSetIndex;
			int h = cbHeight-1;		// vertical inner corner count
			int w = cbWidth-1;		// horizontal inner corner count
			for( int iImg = 0; iImg < imgCount; iImg++)
				for( int i = 0; i < h; i++)
					for( int j = 0; j < w; j++)
						cb3DCorners[ iImg*h*w + i*w + j ] = cvPoint3D32f( j*squareSize, i*squareSize, 0 );	
			// END calc3DPoints( dx, dy);
	
			/* calculate intrinsic parameters */
			
			// camera->extrinsicError = calcExtrinsicParameters();
			CvPoint2D32f* ip = new CvPoint2D32f[cornerCount];
			CvPoint3D32f* op = new CvPoint3D32f[cornerCount];
			VEC3  Rv;
			CvMat _dist = cvMat( 1, 4, CV_32FC1, this->m_pInternalParameters.k);
			MAT33 tempA;
			//MAT44 tempA44;
			//mat44_transpose(&tempA44[0], &this->m_pInternalParameters.A[0]);
			mat44_to33(&tempA[0], &this->m_pInternalParameters.A[0]);
			CvMat _A = cvMat( 3, 3, CV_32FC1, tempA);
			CvMat _r = cvMat( 1, 3, CV_32FC1, Rv);
			CvMat _t = cvMat( 1, 3, CV_32FC1, this->m_pExternalParameters.T);
			CvMat _ngrid = cvMat( 1, 1, CV_32SC1, &cornerCount);
			CvMat cr2 = cvMat( cornerCount, 1, CV_32FC2, cb2DCorners);
			CvMat cr3 = cvMat( cornerCount, 1, CV_32FC3, cb3DCorners);
		
			/* calibrate */
			//vec4_print(this->m_pInternalParameters.k);
			//mat33_print(tempA);
			//vec3_print(Rv);
			cvFindExtrinsicCameraParams2( &cr3, &cr2, &_A, &_dist, &_r, &_t);
			//vec3_print(Rv);
			CvMat _R = cvMat( 3, 3, CV_32FC1, this->m_pExternalParameters.R);
			//mat33_print(this->m_pExternalParameters.R);
			cvRodrigues2( &_r, &_R, NULL);
			//mat33_print(this->m_pExternalParameters.R);
			/* calculate calibration error */
			
				// back2DProjection( cornerCount, cb3DCorners, ip, true);
				float pt[3];
				for( int i = 0; i < cornerCount; i++) 
				{
					pt[0] = this->m_pExternalParameters.R[0]*cb3DCorners[i].x + this->m_pExternalParameters.R[1]*cb3DCorners[i].y + this->m_pExternalParameters.R[2]*cb3DCorners[i].z + this->m_pExternalParameters.T[0];
					pt[1] = this->m_pExternalParameters.R[3]*cb3DCorners[i].x + this->m_pExternalParameters.R[4]*cb3DCorners[i].y + this->m_pExternalParameters.R[5]*cb3DCorners[i].z + this->m_pExternalParameters.T[1];
					pt[2] = this->m_pExternalParameters.R[6]*cb3DCorners[i].x + this->m_pExternalParameters.R[7]*cb3DCorners[i].y + this->m_pExternalParameters.R[8]*cb3DCorners[i].z + this->m_pExternalParameters.T[2];
					
					//std::cout<<"pt2 = "<<pt[2]<<std::endl;
					pt[0] = pt[0] / pt[2];
					pt[1] = pt[1] / pt[2];
					pt[2] = 1.0f;
					
				
					// distortion( pt, this->m_pInternalParameters.k);
					float u = pt [ 0 ];
					float v = pt [ 1 ];
					float r = u*u+v*v;
					
					pt[0] = u*(1.0f + this->m_pInternalParameters.k[0]*r + this->m_pInternalParameters.k[1]*r*r) + 2*u*v*this->m_pInternalParameters.k[2] + this->m_pInternalParameters.k[3]*(r+2*u*u);
					pt[1] = v*(1.0f + this->m_pInternalParameters.k[0]*r + this->m_pInternalParameters.k[1]*r*r) + 2*u*v*this->m_pInternalParameters.k[3] + this->m_pInternalParameters.k[2]*(r+2*v*v);
					/// END distortion( pt, this->m_pInternalParameters.k);

					ip[i].x = this->m_pInternalParameters.A[0]*pt[0] + this->m_pInternalParameters.A[1]*pt[1] + this->m_pInternalParameters.A[2];
					ip[i].y = this->m_pInternalParameters.A[3]*pt[0] + this->m_pInternalParameters.A[4]*pt[1] + this->m_pInternalParameters.A[5];
				}
				/// END back2DProjection( cornerCount, cb3DCorners, ip, true);
				
				// back3DProjection( cornerCount, op, cb2DCorners);
				VEC4  pc, pg;
				VEC2  pi;
				MAT44  Rcg; 
				VEC4  Tcg;
				MAT44  extR44;
				VEC4  extT4;
				
				mat33_to44( Rcg, this->m_pExternalParameters.R);
				vec4_init_v3dir( extT4, this->m_pExternalParameters.T);
				
				mat44_mul_vec( Tcg, Rcg, extT4);
				vec4_const_mul( Tcg, -1.0f, Tcg);
				for( int i = 0; i < cornerCount; i++)
				{
					pi[0] = cb2DCorners[i].x;
					pi[1] = cb2DCorners[i].y;
					
					// pixel_to_camera( pc, pi);
						// undistortion( camera->intrinsicA, camera->intrinsicK, pi [ 0 ], pi [ 1 ], pc [ 0 ], pc [ 1 ] );
						float u0, v0, fx, fy, _fx, _fy, k1, k2, p1, p2;
						u0 = this->m_pInternalParameters.A[2]; v0 = this->m_pInternalParameters.A[5];
						fx = this->m_pInternalParameters.A[0]; fy = this->m_pInternalParameters.A[4];
						_fx = 1.f/fx; _fy = 1.f/fy;
						k1 = this->m_pInternalParameters.k[0]; k2 = this->m_pInternalParameters.k[1];
						p1 = this->m_pInternalParameters.k[2]; p2 = this->m_pInternalParameters.k[3];

						float y = (pi[1] - v0)*_fy;
						float x = (pi[0] - u0)*_fx;

						float y2 = y*y;
						float _2p1y = 2*p1*y;
						float _3p1y2 = 3*p1*y2;
						float p2y2 = p2*y2;

						float x2 = x*x;
						float r2 = x2 + y2;
						float d = 1 + (k1 + k2*r2)*r2;

						pc[0] = (x*(d + _2p1y) + p2y2 + (3*p2)*x2);
						pc[1] = (y*(d + (2*p2)*x) + _3p1y2 + p1*x2);
						/// END undistortion( camera->intrinsicA, camera->intrinsicK, pi [ 0 ], pi [ 1 ], pc [ 0 ], pc [ 1 ] );
					pc [ 2 ] = 1.0f;
					/// END pixel_to_camera( pc, pi);
					
					mat44_mul_vec( pg, Rcg, pc);
					vec4_norm( pg, pg);
					float t = - Tcg[2] / pg[2];
					op[i].x = Tcg[0] + pg[0]*t;
					op[i].y = Tcg[1] + pg[1]*t;
					op[i].z = Tcg[2] + pg[2]*t;
				}
				/// END back3DProjection( cornerCount, op, cb2DCorners);
				
				// err = error2D( cornerCount, cb2DCorners, ip);
				for ( int i = 0 ; i < cornerCount ; i ++ )
				{
					float dx = cb2DCorners [ i ].x - ip [ i ].x;
					float dy = cb2DCorners [ i ].y - ip [ i ].y;
					err += sqrtf ( dx * dx + dy * dy );
				}
				err = err / cornerCount;
				/// END err = error2D( cornerCount, cb2DCorners, ip);
				
				// err3 = error3D( cornerCount, cb3DCorners, op);
				for ( int i = 0 ; i < cornerCount ; i ++ )
				{
					float dx = cb3DCorners [ i ].x - op [ i ].x;
					float dy = cb3DCorners [ i ].y - op [ i ].y;
					float dz = cb3DCorners [ i ].z - op [ i ].z;
					err3 += sqrtf ( dx * dx + dy * dy + dz * dz );
				}
				err3 = err3 / cornerCount;
				/// END err3 = error3D( cornerCount, cb3DCorners, op);
			
			delete [] op;
			delete [] ip;
			// END camera->extrinsicError = calcExtrinsicParameters();
		}
		else	// no 2D points set found
			err = 999999.0;
	}
	return  err;
}


int WebCamLoop( void* pData )
{
	Webcam* oWebcam = reinterpret_cast<Webcam*>( pData );

	while( !oWebcam->IsEnded() )
	{
		oWebcam->GrabImage();
		SDL_Delay( 100/ 3 );
	}

	return 0;
}
