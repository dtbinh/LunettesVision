#include "SwissRanger.hpp"
#include <algorithm>

#define  _UNDISTORT_MAP

using namespace LA;


namespace // Static like namespace
{
	int iResultat = 0;
	int iTextureIDActivated = -1;
	float fValue;
	float fZMean;
	float fPosition[ 3 ];
	float* pZMapVBO = NULL;
    double maxDistance = 0;
}

SwissRanger::SwissRanger() :
	m_pSwissRangerCapture( NULL ),
	m_pImgEntryArray( NULL ),
	m_pBackgroundModel( NULL ),
	m_pBackgroundModel1( NULL ),
	m_pBackgroundModel2( NULL ),
	m_pImageAnalisis( NULL ),
	m_pConvKern( NULL ),
	m_pSR4000Dist( NULL ),
	m_pSR4000Conf( NULL ),
	m_pSR4000Ampl( NULL ),
	m_pFusion( NULL ),
	m_pXYZCoords( NULL ),
	m_pXYZCodedColor( NULL ),
	m_pXYZColor( NULL ),
	m_pXYZColorHDR( NULL ),
	m_pXYZIndex( NULL ),
	m_p3DInterestingPoints( NULL ),
	m_p3DInterestingColor( NULL ),
	m_pBuffer( NULL ),
	m_iCamTexture( 0 ),
	m_bInitTexture( false ),
	m_bExit( false ),
	m_uiZMapVBO( 0 ),
	m_uiZMapVBOIndex( 0 ),
	m_iUpdateCounter( 0 ),
	m_iUpdateThreshold( 60 ),
	m_iConfidentThreshold( 200 ),
	m_fNoiseThreshold( 0 ),
	m_pMapX( NULL ),
	m_pMapY( NULL )
{
	//~ Create();
}
SwissRanger::~SwissRanger()
{
	Release();
}

void SwissRanger::Create()
{
	// Set SwissRanger callback. See SR doc for more details.
	SR_SetCallback( LibusbCallback );

	Initialize();
}

void SwissRanger::Release()
{
	m_bExit = false;
	if( NULL != m_pXYZCoords )
		delete[] m_pXYZCoords;
	if( NULL != m_pXYZIndex )
		delete[] m_pXYZIndex;

	iResultat = SR_Close( m_pSwissRangerCapture );
	ASSERT_GE( iResultat, 0 );

	glDeleteBuffers( 1, &m_uiZMapVBO );
}

void SwissRanger::Initialize()
{
#ifdef DEBUG
	//~ freopen( "SwissRanger.log", "w", stderr );	///< Redirect log from SwissRanger to a file.
#else
	freopen( "/dev/null", "w", stderr );			///< Redirect log from SwissRanger to /dev/null!
#endif

	//returns the device ID used in other calls.
	iResultat=SR_OpenUSB(&m_pSwissRangerCapture,0);
	ASSERT_GE( iResultat, 0 );

	iResultat = SR_SetMode( m_pSwissRangerCapture,
						AM_COR_FIX_PTRN 	 	///< Turns on fix pattern noise correction this should always be enabled for good distance measurement.
						//~ | AM_CONV_GRAY		///< Converts the amplitude image by multiplying by the square of the distance, resulting in image more like a conventional camera.
						| AM_DENOISE_ANF	    ///< For sr4k: Turns on the 3x3 hardware adaptive neighborhood filter.
						| AM_CONF_MAP			///< For sr4k: process a confidencemap. this map is accesssible with SR_GetImageList.
//						| AM_MEDIAN 			///< Turns on a 3x3 median filter.
//						| AM_RESERVED1 		    ///< Was AM_SHORT_RANGE For sr4k: this flag results in more precise coordinate transformations for small distances(<1m) works only for SR_CoordTrfFlt()
//						AM_SW_ANF			    ///< Turns on the 7x7 software adaptive neighborhood filter This mode is for internal testing and should not be used.
	);
	ASSERT_GE( iResultat, 0 ) << "\t ==>Maybe the SwissRanger isn't plug???";

	SR_SetModulationFrequency( m_pSwissRangerCapture, MF_15MHz );

	iResultat = SR_Acquire( m_pSwissRangerCapture );
	ASSERT_GE( iResultat, 0 );

	iResultat = SR_GetImageList( m_pSwissRangerCapture, &m_pImgEntryArray );
	ASSERT_NE( iResultat, 0 );


	m_uiWidth = SR_GetCols( m_pSwissRangerCapture );
	m_uiHeight = SR_GetRows( m_pSwissRangerCapture );

	m_iNbInterestingPoints = m_uiWidth * m_uiHeight;

	m_pFrequency = SR_GetModulationFrequency( m_pSwissRangerCapture );

	//~ m_pXCoords = new float[ m_uiWidth * m_uiHeight ];
	//~ m_pYCoords = new float[ m_uiWidth * m_uiHeight ];
	//~ m_pZCoords = new float[ m_uiWidth * m_uiHeight ];
	m_pXYZCoords = new float[ m_uiWidth * m_uiHeight * 3 ];
	m_pXYZCodedColor = new unsigned char[ m_uiWidth * m_uiHeight * 3 ];
	m_pXYZColor = new float[ m_uiWidth * m_uiHeight * 3 ];
	m_pXYZColorHDR = new float[ m_uiWidth * m_uiHeight * 3 ];
	m_p3DInterestingPoints = new float[ m_uiWidth * m_uiHeight * 3 ];
	m_p3DInterestingColor = new float[ m_uiWidth * m_uiHeight * 3 ];
	//~ m_pXYZIndex = new int[ m_uiWidth * m_uiHeight * 3 * 4 ];

	//~ for( unsigned int  i = 0 ; i < m_uiWidth * m_uiHeight ; i ++ )
	//~ {
			//~ m_pXYZIndex[ 4 * i + 0 ] = 3 * i;
			//~ m_pXYZIndex[ 4 * i + 1 ] = 3 * i + 1;
			//~ m_pXYZIndex[ 4 * i + 2 ] = 3 * i + m_uiHeight;
			//~ m_pXYZIndex[ 4 * i + 3 ] = 3 * i + m_uiHeight + 1;
	//~ }

	m_pSR4000Dist = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 1);
	m_pSR4000Conf = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 1);
	m_pSR4000Ampl = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 3);
	m_pSR4000AmplBack = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 3);

    //RADA:
	// replacing the number of channels with 4 to be able to add color info, with removing the intensity info
	// note that opencv might not allow more the four channels, otherwise I may use the intensity of z-cam too

	// m_pFusion = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 3);

	 m_pFusion = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 3);
    m_pFusion1 = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 3);
    m_pFusion2 = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 1);

    m_pForegroundHistogram = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 1);

    m_pTemp = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 3);

	//m_pFusion = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 2);
   // m_pFusion2 = cvhttp://start.ubuntu.com/9.10/CreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 3);

	//RADA
    m_pForegroundBack = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 1);
    m_pForegroundColorBack = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 1);
    m_pForegroundDepthBack = cvCreateImage( cvSize(m_uiWidth,m_uiHeight), IPL_DEPTH_8U, 1);

	cvZero( m_pFusion );
	cvZero( m_pFusion1 );
    cvZero( m_pFusion2 );


	m_fDmax = Distance[ m_pFrequency ];

	glGenBuffers( 1, &m_uiZMapVBO );
	//~ glGenBuffers( 1, &m_uiZMapVBOIndex );

	glBindBuffer( GL_ARRAY_BUFFER, m_uiZMapVBO );
	glBufferData( GL_ARRAY_BUFFER,					/* Target */
		( sizeof( float ) * m_uiWidth * m_uiHeight * 3 ),	/* Vertex size */
		m_pXYZCoords,							/* Data */
		GL_STREAM_DRAW );						/* Mode */

	//~ glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_uiZMapVBO );
	//~ glBufferData( GL_ELEMENT_ARRAY_BUFFER,			/* Target */
		//~ ( sizeof( int ) * m_uiWidth * m_uiHeight * 3 * 4 ),	/* Vertex size */
		//~ m_pXYZIndex,								/* Data */
		//~ GL_STATIC_DRAW );							/* Mode */


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//~ m_fFactor = ( (float) (1<<8) / fDmax );

	//~ freopen( "/dev/stderr", "w", stderr ); 	///< Retrieve normal stderr.
	InitColorMap();

/*
    m_oBGParams.win_size = 10;
    m_oBGParams.bg_threshold = 0.007f;
    m_oBGParams.std_threshold = 2.7f;
    m_oBGParams.weight_init = 0.05;
    m_oBGParams.variance_init = 30*30;
    m_oBGParams.minArea = 1000.f;
    m_oBGParams.n_gauss = 3;
*/


    m_oBGParams.win_size =10 ; // 10
    m_oBGParams.bg_threshold =  0.07f ; // 0.07
    m_oBGParams.std_threshold = 3.5f; // 2.5
    m_oBGParams.weight_init =  0.09; // not used as I guess
    m_oBGParams.variance_init = 100; // 100 // now I am using different variances for different features
	// which is in variance_vector in GaussMix
	// I changed a little in GaussMix too regarding the variances
    m_oBGParams.minArea =  1000.f; // it controls the size of the foreground objects to be kept ,  1000 , and it needs to be checked
    m_oBGParams.n_gauss = 3; //3
    
    

	Load( "SR.internal", "SR-W.external" );


	glEnable( GL_TEXTURE_RECTANGLE_ARB );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );


	glGenTextures( 1, &m_iCamTexture );
	ASSERT_GE( m_iCamTexture, 0 );

	glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
	LOG << "SwissRanger Texture ID: " << m_iCamTexture << ENDLOG;

	glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB,
		m_pSR4000AmplBack->width, m_pSR4000AmplBack->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, m_pSR4000AmplBack->imageData );

		//~ glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE,
			//~ m_pSR4000AmplBack->width, m_pSR4000AmplBack->height, 0, GL_LUMINANCE,
			//~ GL_UNSIGNED_BYTE, m_pSR4000AmplBack->imageData );

    // RADA
	// the new texture to draw the foreground image
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures( 1, &m_iCamTexture2 );
	ASSERT_GE( m_iCamTexture2, 0 );

	glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture2 ); // Bind the OpenGL Texture.

	glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//

	for (unsigned int i=0; i<m_uiHeight; ++i)
		for (unsigned int j=0; j<m_uiWidth; ++j)
		{
			m_pXYZCodedColor[i * m_uiWidth * 3 +j *3] = i;
			m_pXYZCodedColor[i * m_uiWidth * 3 +j *3 +1 ] = j;
			m_pXYZCodedColor[i * m_uiWidth * 3 +j *3 +2 ] = 0;
			m_pXYZColor[i * m_uiWidth * 3 +j *3] = 1.0f;
			m_pXYZColor[i * m_uiWidth * 3 +j *3+1] = 1.0f;
			m_pXYZColor[i * m_uiWidth * 3 +j *3+2] = 1.0f;
		}

    m_fNoiseThreshold = m_uiWidth * m_uiHeight * 0.05f;

	m_pWebcam = new Webcam();
	m_pWebcam->Create( 640, 480, 0 );
	m_uiWidthWC = m_pWebcam->GetWidth();
	m_uiHeightWC = m_pWebcam->GetHeight();

	std::cerr << "Taille Sony: " << m_uiWidthWC << " " << m_uiHeightWC << std::endl;


	// RADA
	m_bBackgroundFirstCall = true;

    // creating the grid of voxels
    // should I decide the size of the voxel depending on the size of the image
    // it makes more sense.
    // or the size of the voxel is not relateed to the size of the image
    // need more information about that

    m_pVoxelsGrid = new VoxelsGrid( 64,64,64, 1,1,1);


    // calculation of the number of pixels contained in each voxel
    // here if I stayed in my idea of the voxel size
    // the number of pixels would not be true in all cases (in the boundries of the grid)
    // but I guess it won't pose a problem, it is better than calculating them in each loop ??
    // needed to be checked

   // m_uiNumOfPixelsInVoxel = m_pVoxelsGrid->GetVoxelSize() *m_pVoxelsGrid->GetVoxelSize() *  m_pVoxelsGrid->GetGridDepth();
    m_uiNumOfPixelsInVoxel = m_uiWidth / (m_pVoxelsGrid->GetVoxelWidth()  * m_pVoxelsGrid->GetGridWidth());

    m_fPixelsPersentageInVoxel =0.18;


	//m_pWebcam->Start();
}

void SwissRanger::GrabDepth()
{
	ASSERT_TRUE( m_pSwissRangerCapture != NULL );

	iResultat = SR_Acquire( m_pSwissRangerCapture );
	ASSERT_GE( iResultat, 0 );
	iResultat = SR_GetImageList( m_pSwissRangerCapture, &m_pImgEntryArray );
	ASSERT_GE( iResultat, 0 );

	m_pBuffer = m_pImgEntryArray[ 0 ].data;

	iResultat = SR_CoordTrfFlt( m_pSwissRangerCapture, &m_pXYZCoords[0], &m_pXYZCoords[1], &m_pXYZCoords[2], sizeof(float[3]) , sizeof(float[3]) , sizeof(float[3]) );
	unsigned char* pImageData = (uchar*)m_pSR4000Dist->imageData;

    maxDistance = m_fDmax*100;
	for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++ )
	{
		fValue = (m_pXYZCoords[ 3 * i + 2 ] * ( (float) ( 1 << 8 ) / Distance[ m_pFrequency ] ) );

		pImageData[ i ] = (uchar) CLAMP( fValue, 0, 255 );



		m_p3DInterestingPoints[ 3 * i + 0 ] = m_pXYZCoords[ 3 * i + 0 ];
        m_p3DInterestingPoints[ 3 * i + 1 ] = m_pXYZCoords[ 3 * i + 1 ];
        m_p3DInterestingPoints[ 3 * i + 2 ] = m_pXYZCoords[ 3 * i + 2 ];
	}


	m_pBuffer=(short int*)m_pImgEntryArray[ 2 ].data;
	unsigned short int* pConfidentBuffer = (unsigned short int*)m_pImgEntryArray[ 2 ].data;
	uchar* pConfidentData = (uchar*)m_pSR4000Conf->imageData;

	for( uint i = 0 ; i < m_uiWidth*m_uiHeight ; i++  )
	{
		pConfidentData[ i ] = (uchar) (pConfidentBuffer[ i ] >> 8 );
	}


	unsigned short int* pAmplitudeBuffer  =(unsigned short int*)m_pImgEntryArray[ 1 ].data;
	uchar* pAmplitudeData = (uchar*)m_pSR4000Ampl->imageData;

	for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++  )
	{
//		pAmplitudeData[ 3 * i + 0 ] = (uchar) (pAmplitudeBuffer[ 3 * i ] >> 8 );
//		pAmplitudeData[ 3 * i + 1 ] = (uchar) (pAmplitudeBuffer[ 3 * i ] >> 8 );
//		pAmplitudeData[ 3 * i + 2 ] = (uchar) (pAmplitudeBuffer[ 3 * i ] >> 8 );
//*
		pAmplitudeData[ 3 * i + 0 ] = (char) CLAMP( m_pColorMap[(int)((m_pXYZCoords[ 3 * i + 2 ] / m_fDmax) * 255 )][0] * 255.0f, 0, 255 );
		pAmplitudeData[ 3 * i + 1 ] = (char) CLAMP( m_pColorMap[(int)((m_pXYZCoords[ 3 * i + 2 ] / m_fDmax) * 255 )][1] * 255.0f, 0, 255 );
		pAmplitudeData[ 3 * i + 2 ] = (char) CLAMP( m_pColorMap[(int)((m_pXYZCoords[ 3 * i + 2 ] / m_fDmax) * 255 )][2] * 255.0f, 0, 255 );
//*/
	// modified by Rada
	// addding just the depth information, then adding the color information in 'DrawColor' function
/*
		m_pFusion->imageData[ 3* i + 0 ] = (char) (pAmplitudeBuffer[ i ] >> 8 );
		m_pFusion->imageData[ 3* i + 1 ] = (char) pImageData[ i ];
		m_pFusion->imageData[ 3* i + 2 ] = 0;
//*/

    	//m_pFusion->imageData[ 3* i + 0  ] = (char) (pAmplitudeBuffer[ i ] >> 8 );
/*		m_pFusion->imageData[ 3* i + 2 ] = (char) (pImageData[ i ] ); //* (pAmplitudeBuffer[ i ] >> 8) / 2) ;
       // m_pFusion2->imageData[ i ] = (char) (pImageData[ i ] ); //* (pAmplitudeBuffer[ i ] >> 8) / 2) ;
	 // m_pFusion->imageData[ 3 * i + 0 ] = pAmplitudeData[ 3 * i + 0 ];
      //m_pFusion->imageData[ 3 * i + 1 ] = pAmplitudeData[ 3 * i + 1 ];
      // m_pFusion->imageData[ 3 * i + 2 ] = pAmplitudeData[ 3 * i + 2 ];


//    m_pFusion->imageData[ 2*i  + 1 ] =	pImageData[ i ] ;
//*/
	}
	
	
	
#ifdef _UNDISTORT_MAP		
	///// Undistort maps!
	
	//~ cvSaveImage( "SR.jpg", m_pSR4000Dist );

	CvMat *intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
	CvMat *distortion = (CvMat*)cvLoad("Distortion.xml");
	
	
	m_pMapX = cvCreateImage( cvGetSize( m_pSR4000Dist ), IPL_DEPTH_32F, 1 );
	m_pMapY = cvCreateImage( cvGetSize( m_pSR4000Dist ), IPL_DEPTH_32F, 1 );
	
	
	cvInitUndistortMap( intrinsic, distortion, m_pMapX, m_pMapY );
	
	
	IplImage *t = cvCloneImage( m_pSR4000Dist );
	cvSaveImage( "SR-preunmap.jpg", t );
	cvRemap( t, m_pSR4000Dist, m_pMapX, m_pMapY );     // Undistort image
	cvReleaseImage(&t);
	cvSaveImage( "SR-unmap.jpg", m_pSR4000Dist );
	
	t = cvCloneImage( m_pSR4000Conf );
	cvRemap( t, m_pSR4000Conf, m_pMapX, m_pMapY );     // Undistort image
	cvReleaseImage(&t);
	
	t = cvCloneImage( m_pSR4000Ampl );
	cvRemap( t, m_pSR4000Ampl, m_pMapX, m_pMapY );     // Undistort image
	cvReleaseImage(&t);
	
	
	IplImage* tx = cvCreateImage( cvSize( m_uiWidth, m_uiHeight ), IPL_DEPTH_32F, 1 );
	IplImage* ty = cvCreateImage( cvSize( m_uiWidth, m_uiHeight ), IPL_DEPTH_32F, 1 );
	IplImage* tz = cvCreateImage( cvSize( m_uiWidth, m_uiHeight ), IPL_DEPTH_32F, 1 );
	IplImage* tX = cvCreateImage( cvSize( m_uiWidth, m_uiHeight ), IPL_DEPTH_32F, 1 );
	IplImage* tY = cvCreateImage( cvSize( m_uiWidth, m_uiHeight ), IPL_DEPTH_32F, 1 );
	IplImage* tZ = cvCreateImage( cvSize( m_uiWidth, m_uiHeight ), IPL_DEPTH_32F, 1 );
	
	for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++ )
	{
		tx->imageData[ i ] = (uchar) m_pXYZCoords[ 3 * i + 0 ];
		ty->imageData[ i ] = (uchar) m_pXYZCoords[ 3 * i + 1 ];
		tz->imageData[ i ] = (uchar) m_pXYZCoords[ 3 * i + 2 ];
	}
	cvRemap( tx, tX, m_pMapX, m_pMapY );     // Undistort image
	cvRemap( ty, tY, m_pMapX, m_pMapY );     // Undistort image
	cvRemap( tz, tZ, m_pMapX, m_pMapY );     // Undistort image
	
	
	for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++ )
	{
		m_pXYZCoords[ 3 * i + 0 ] = (float) tX->imageData[ i ];
		m_pXYZCoords[ 3 * i + 1 ] = (float) tY->imageData[ i ];
		m_pXYZCoords[ 3 * i + 0 ] *= (float) tZ->imageData[ i ]/tz->imageData[ i ];
		m_pXYZCoords[ 3 * i + 1 ] *= (float) tZ->imageData[ i ]/tz->imageData[ i ];
		m_pXYZCoords[ 3 * i + 2 ] = (float) tZ->imageData[ i ];
	}
	
	
	cvReleaseImage(&tx);
	cvReleaseImage(&ty);
	cvReleaseImage(&tz);
	cvReleaseImage(&tX);
	cvReleaseImage(&tY);
	cvReleaseImage(&tZ);
#endif
	
	

//#if 0

	// End Critical Section for Amplitude Map
	//~ SDL_UnlockMutex( m_pMutexAmplitude );
	//~ OUTf();//*/
}

/*void SwissRanger::GrabDepthMedian(uint nbIm)
{
	//IplImage* DepthMedian = cvCreateImage(cvSize(m_uiWidth,m_uiHeight),IPL_DEPTH_8U, 1);
	CvScalar pix;
	vector<int> liste;
	IplImage**	depthImages = new IplImage* [nbIm];
	for(uint ii=0; ii<nbIm; ii++)
	{
		/sleep(0.1);
		//for(uint jj=0; jj<3; jj++)
			GrabDepth();//
		depthImages[ii] = cvCloneImage( GetDistData() );
	}
	for(uint i=0; i<m_uiWidth; i++)
		for(uint j=0; j<m_uiHeight; j++)
		{	
			liste.clear();
			for(uint ii=0; ii<nbIm; ii++)
			{
				pix=cvGet2D(depthImages[ii],j,i);
				liste.push_back((int)pix.val[0]);
			}
			sort(liste.begin(),liste.end());
			pix.val[0]=(char)liste.at(ceil((float)nbIm/2.0));
			cvSet2D(GetDistData(),j,i,pix);
		}
	delete[] depthImages;
}*/
bool sortfunction (float* i,float* j) { return (i[2]<j[2]); };

void SwissRanger::GrabDepthMedian(uint nbIm)
{
	//IplImage* DepthMedian = cvCreateImage(cvSize(m_uiWidth,m_uiHeight),IPL_DEPTH_8U, 1);
	float pix[3];
	vector<float*> liste;
	float**	depthImages = new float* [nbIm];
	for (int i=0 ; i<nbIm ; ++i)
	{
		depthImages[i]=new float [m_uiWidth*m_uiHeight*3];
	}
	
	for(uint ii=0; ii<nbIm; ii++)
	{
		ASSERT_TRUE( m_pSwissRangerCapture != NULL );
		iResultat = SR_Acquire( m_pSwissRangerCapture );
		ASSERT_GE( iResultat, 0 );
		iResultat = SR_GetImageList( m_pSwissRangerCapture, &m_pImgEntryArray );
		ASSERT_GE( iResultat, 0 );
		iResultat = SR_CoordTrfFlt( m_pSwissRangerCapture, &depthImages[ii][0], &depthImages[ii][1], &depthImages[ii][2], sizeof(float[3]) , sizeof(float[3]) , sizeof(float[3]) );
	}
	
	for(uint i=0; i<m_uiWidth; i++)
	{
		for(uint j=0; j<m_uiHeight; j++)
		{	
			liste.clear();
			for(uint ii=0; ii<nbIm; ii++)
			{
				pix[0]=depthImages[ii][3*(j*m_uiWidth+i)];
				pix[1]=depthImages[ii][3*(j*m_uiWidth+i)+1];
				pix[2]=depthImages[ii][3*(j*m_uiWidth+i)+2];
				liste.push_back(new float[3]);
				liste.back()[0]=pix[0];
				liste.back()[1]=pix[1];
				liste.back()[2]=pix[2];
			}
			sort(liste.begin(),liste.end(),sortfunction);
			pix[0]=liste.at(ceil((float)nbIm/2.0))[0];
			pix[1]=liste.at(ceil((float)nbIm/2.0))[1];
			pix[2]=liste.at(ceil((float)nbIm/2.0))[2];
			m_pXYZCoords[3*(j*m_uiWidth+i)]=pix[0];
			m_pXYZCoords[3*(j*m_uiWidth+i)+1]=pix[1];
			m_pXYZCoords[3*(j*m_uiWidth+i)+2]=pix[2];
		}
	}
		
	for (int i=0 ; i<nbIm ; ++i)
	{
		delete[] depthImages[i];
	}
	delete[] depthImages;
}


void SwissRanger::ExtractInterestingPoints()
{
    m_iNbInterestingPoints = 0;

    /// Compute Center Of Gravity
    for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++  )
	{
	    if ( ( m_pBackgroundModel->foreground->imageData[ i ] ) && ( (unsigned char)(m_pSR4000Conf->imageData[ i ]) > m_iConfidentThreshold ) )
	    {
	        m_pCenterOfGravity[ 0 ] += m_pXYZCoords[ i * 3 + 0 ];
	        m_pCenterOfGravity[ 1 ] += m_pXYZCoords[ i * 3 + 1 ];
	        m_pCenterOfGravity[ 2 ] += m_pXYZCoords[ i * 3 + 2 ];
	        m_iNbInterestingPoints++;
	    }
	}
	if( m_iNbInterestingPoints )
	{
	    m_pCenterOfGravity[ 0 ] /= m_iNbInterestingPoints;
	    m_pCenterOfGravity[ 1 ] /= m_iNbInterestingPoints;
	    m_pCenterOfGravity[ 2 ] /= m_iNbInterestingPoints;
	}

	/// Filtering noise
    if( m_iNbInterestingPoints > m_fNoiseThreshold )
	{
        m_iNbInterestingPoints = 0;
	    for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++  )
        {
            if( !( ( m_pXYZCoords[ 3 * i + 2 ] < ( m_pCenterOfGravity[ 2 ] + 1.25f ) )
                && ( m_pXYZCoords[ 3 * i + 2 ] > ( m_pCenterOfGravity[ 2 ] - 1.25f ) ) ) )
            {
                m_pBackgroundModel->foreground->imageData[ i ] = 0;
            }
        }
        /// Cleaning edge
        for( uint i = m_uiWidth + 1 ; i < m_uiWidth * m_uiHeight - m_uiWidth - 2 ; i++  )
        {
            unsigned char* pPointUp = (unsigned char*) & (m_pBackgroundModel->foreground->imageData[ i - m_uiWidth ] );
            unsigned char* pPointPrev = (unsigned char*) & (m_pBackgroundModel->foreground->imageData[ i - 1 ] );
            unsigned char* pPoint = (unsigned char*) & (m_pBackgroundModel->foreground->imageData[ i ] );
            unsigned char* pPointNext= (unsigned char*) & (m_pBackgroundModel->foreground->imageData[ i + 1 ] );
            unsigned char* pPointDown = (unsigned char*) & (m_pBackgroundModel->foreground->imageData[ i + m_uiWidth ] );
            if( pPoint[ 0 ] && pPointPrev[ 0 ] && pPointUp[ 0 ] && pPointNext[ 0 ] && pPointDown[ 0 ] )
            {

                fZMean = 0.0f;
                fZMean += m_pXYZCoords[ 3 * i + 2 - 3* m_uiWidth ];
                fZMean += m_pXYZCoords[ 3 * i + 2 - (3) ];
                fZMean += m_pXYZCoords[ 3 * i + 2 ];
                fZMean += m_pXYZCoords[ 3 * i + 2 + 3 ];
                fZMean += m_pXYZCoords[ 3 * i + 2 + 3* m_uiWidth ];
                fZMean /= 5.0f;

                m_p3DInterestingPoints[ 3 * m_iNbInterestingPoints + 0 ] = m_pXYZCoords[ 3 * i + 0 ];
                m_p3DInterestingPoints[ 3 * m_iNbInterestingPoints + 1 ] = m_pXYZCoords[ 3 * i + 1 ];
                m_p3DInterestingPoints[ 3 * m_iNbInterestingPoints + 2 ] = fZMean;//m_pXYZCoords[ 3 * i + 2 ];
                //m_p3DInterestingPoints[ 3 * m_iNbInterestingPoints + 2 ] = m_pXYZCoords[ 3 * i + 2 ];

                m_p3DInterestingColor[ 3 * m_iNbInterestingPoints + 0 ] = m_pXYZColor[ 3 * i + 0 ];
                m_p3DInterestingColor[ 3 * m_iNbInterestingPoints + 1 ] = m_pXYZColor[ 3 * i + 1 ];
                m_p3DInterestingColor[ 3 * m_iNbInterestingPoints + 2 ] = m_pXYZColor[ 3 * i + 2 ];
                m_iNbInterestingPoints++;
            }
        }
	}
}

void SwissRanger::Draw()
{
	glPointSize( 5.0f );

	glBindBuffer( GL_ARRAY_BUFFER, m_uiZMapVBO );
	pZMapVBO = (float*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
	if( NULL == pZMapVBO )
	{
		std::cerr << "Error, unable to retrive VBO!" << std::endl;
		throw new std::exception;
	}

	memcpy( pZMapVBO, m_pXYZCoords, m_uiWidth * m_uiHeight * sizeof( float [3]) );

	glUnmapBuffer( GL_ARRAY_BUFFER );
	pZMapVBO = NULL;


	glBindBuffer( GL_ARRAY_BUFFER, m_uiZMapVBO );
	glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET(0) );

	// Enable vertex array and color array
	glEnableClientState( GL_VERTEX_ARRAY );

	glActiveTexture( GL_TEXTURE0 + m_pWebcam->GetTextureID() );
	glEnable( GL_TEXTURE_RECTANGLE_EXT );
	glBindTexture( GL_TEXTURE_RECTANGLE_EXT, m_pWebcam->GetTextureID() );

	glDrawArrays( GL_POINTS, 0, m_uiWidth * m_uiHeight );

	glDisable( GL_TEXTURE_RECTANGLE_EXT ); int region_count = 0;
	CvSeq *first_seq = NULL, *prev_seq = NULL, *seq = NULL;


	glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 );
	glDisable( GL_TEXTURE_RECTANGLE_EXT );
	glActiveTexture( GL_TEXTURE0 );

	glDisableClientState( GL_VERTEX_ARRAY );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPointSize( 1.0f );
}

void SwissRanger::DrawCodedColor()
{
	/* Immediate mode*/
	glPointSize(3.0f );
	glBegin(GL_POINTS);
	for (unsigned int i=0; i<m_uiHeight; ++i)
		for (unsigned int j=0; j<m_uiWidth; ++j)
		{
			glColor3f(m_pXYZCodedColor[i * m_uiWidth * 3 + j*3]/255.0f,
					m_pXYZCodedColor[i * m_uiWidth * 3 + j*3+1]/255.0f,
					m_pXYZCodedColor[i * m_uiWidth * 3 + j*3+2]/255.0f);
			glVertex3fv(&m_pXYZCoords[i * m_uiWidth * 3 + j*3]);
		}
	glEnd();
	glPointSize( 1.0f );
}

void SwissRanger::GrabBoth()
{
/*/#ifdef CAM_FIREWIRE
	m_pWebcam->GrabImage();
	m_pWebcam->GrabImage();
	m_pWebcam->GrabImage();
//    m_pWebcam->GrabImage();
//    m_pWebcam->GrabImage();
//#endif//*/

	m_pWebcam->GrabImage();
//
//      m_pWebcam->GrabImage();
//      m_pWebcam->GrabImage();
//
//      m_pWebcam->GrabImage();
//      m_pWebcam->GrabImage();

	GrabDepth();

}

// Here is the grid building process
void SwissRanger::DrawColor()
{
    GrabDepth();
    m_pWebcam->GrabImage();
	/* Immediate mode*/
	glPointSize( 5.0f );
	glBegin(GL_POINTS);
//	for (unsigned int i=0; i<m_uiHeight; ++i)
//		for (unsigned int j=0; j<m_uiWidth; ++j)
//		{
//
//			if (m_pXYZCoords[i * m_uiWidth * 3 +j *3+2] <2.5)
//			{
//				glColor3fv(&m_pXYZColor[i * m_uiWidth * 3 +j *3]);
//				glVertex3fv(&m_pXYZCoords[i * m_uiWidth * 3 +j *3]);
//			}
//		}
    for( unsigned int i = 0 ; i < m_iNbInterestingPoints ; i ++ )
    {
        glColor3fv(&(m_pXYZColor[ 3 * i ]));
//        glColor3fv(&(m_p3DInterestingColor[ 3 * i ]));
        glVertex3fv( &(m_p3DInterestingPoints[ 3 * i ]) );
    }
	glEnd();
	glPointSize( 1.0f );
}

void SwissRanger::DrawImage()
{
	if( m_pSR4000AmplBack != NULL )
	{
		glEnable( GL_TEXTURE_RECTANGLE_ARB );
		glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
		glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB,
			m_pSR4000AmplBack->width, m_pSR4000AmplBack->height,
			0, GL_RGB, GL_UNSIGNED_BYTE,
			m_pSR4000Ampl->imageData );
		//~ glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB,
			//~ m_pSR4000Dist->width, m_pSR4000Dist->height,
			//~ 0, GL_RGB, GL_UNSIGNED_BYTE,
			//~ m_pSR4000Dist->imageData );

	/*		glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE,
		m_pForegroundBack->width,
		m_pForegroundBack->height,
		0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
		m_pForegroundBack->imageData );
	*/
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
	//~ glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 ); // Bind the OpenGL Texture.
	glDisable( GL_TEXTURE_RECTANGLE_EXT );
}

void SwissRanger::ProcessEvent()
{
	/// @todo Add events corresponding to SwissRanger actions.
}

void SwissRanger::InitColorMap()
{
	for( int i = 0 ; i < 255 ; i ++ )
	{
		m_pColorMap[i][0] = (255 - i) / 255.0f;
		m_pColorMap[i][1] = (255-(abs((2*i)-255))) / 255.0f;
		m_pColorMap[i][2]  = i / 255.0f;
	}
}

// RADA
// here I am getting 3 foregrounds by applying 3 background models on different features
// useful to compare so we can enhance
void SwissRanger::ComputeBackgroundForeground()
{
    unsigned char* pImageData = (uchar*)m_pSR4000Dist->imageData;
    for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++ )
    {
        m_pTemp->imageData[ 3 * i + 0]   = m_pXYZColor[ 3 * i + 0 ] * 255;
        m_pTemp->imageData[ 3 * i + 1]   = m_pXYZColor[ 3 * i + 1 ] * 255;
        m_pTemp->imageData[ 3 * i + 2]   = m_pXYZColor[ 3 * i + 2 ] * 255;

    }

    cvCvtColor(m_pTemp, m_pTemp, CV_BGR2RGB);
    cvSaveImage("radaColors.jpg",m_pTemp);

   cvCvtColor(m_pTemp, m_pTemp, CV_RGB2YCrCb);

    for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++ )
    {
        m_pFusion->imageData[ 3 * i + 0 ] = m_pTemp->imageData[ 3 * i + 1 ];
        m_pFusion->imageData[ 3 * i + 1 ] = m_pTemp->imageData[ 3 * i + 2 ];


        m_pFusion1->imageData[ 3 * i +0 ] = m_pTemp->imageData[ 3 * i + 0 ];
        m_pFusion1->imageData[ 3 * i +1 ] = m_pTemp->imageData[ 3 * i + 1 ];
        m_pFusion1->imageData[ 3 * i +2 ] = m_pTemp->imageData[ 3 * i + 2 ];

        m_pFusion2->imageData[ i ] = (char) (pImageData[ i ] );
    }

// RADA
// this code is for getting the background model and update it

if (m_bBackgroundFirstCall)
{
  m_pBackgroundModel = cvCreateGaussianBGModelGaussMix( m_pFusion,  &m_oBGParams);
    m_pBackgroundModel1 = cvCreateGaussianBGModelGaussMix( m_pFusion1, &m_oBGParams);
    m_pBackgroundModel2 = cvCreateGaussianBGModelGaussMix( m_pFusion2, &m_oBGParams);

    // I don't think we need to control the call of update anymore
    // so I think this call should be removed
    cvSetUpdateState(1);
    m_bBackgroundFirstCall = false;
}
// is it the function that calls the update from the background model
cvUpdateBGStatModel( m_pFusion, m_pBackgroundModel );
cvUpdateBGStatModel( m_pFusion1, m_pBackgroundModel1 );
cvUpdateBGStatModel( m_pFusion2, m_pBackgroundModel2 );

    cvCopyImage( m_pSR4000Ampl, m_pSR4000AmplBack );

     for( uint i = 0 ; i < m_uiWidth * m_uiHeight ; i++ )
	{
	    int val =0;
         m_pForegroundColorBack->imageData[ i ]   =  m_pBackgroundModel->background->imageData [ 3 * i +  0 ];
        val   +=  m_pBackgroundModel->background->imageData [ 3 * i + 1 ];
        val   +=  m_pBackgroundModel->background->imageData [ 3 * i + 2 ];
        m_pForegroundBack->imageData[ i ]   = val/2;
	}

    cvSaveImage("background.bmp", m_pForegroundBack);
    cvSaveImage("backgroundDepth.bmp", m_pForegroundColorBack);

    cvCopyImage( m_pBackgroundModel->foreground , m_pForegroundBack );
    cvCopyImage( m_pBackgroundModel1->foreground , m_pForegroundColorBack );
    cvCopyImage( m_pBackgroundModel2->foreground , m_pForegroundDepthBack );
  // cvCopyImage( m_pBackgroundModel2->foreground , m_pForegroundColorBack );
    cvSaveImage("foreground.bmp",  m_pBackgroundModel->foreground );
    cvSaveImage("foreground_color.bmp", m_pBackgroundModel1->foreground);
    cvSaveImage("foreground_depth.bmp",  m_pBackgroundModel2->foreground);

    //cvSaveImage("color.bmp",m_pForegroundColorBack);
}

//RADA
// this function is used to draw the foreground image
// I can call it instead of calling the webcam one
void SwissRanger::DrawForeground()
{
  	if( m_pForegroundBack != NULL )
	{
		glEnable( GL_TEXTURE_RECTANGLE_ARB );
		glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
        glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE,
		m_pForegroundBack->width,
		m_pForegroundBack->height,
		0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
		m_pForegroundBack->imageData );

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
	//~ glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 ); // Bind the OpenGL Texture.
	glDisable( GL_TEXTURE_RECTANGLE_EXT );

}


void SwissRanger::DrawForegroundDepth()
{
  	if( m_pForegroundDepthBack != NULL )
	{
		glEnable( GL_TEXTURE_RECTANGLE_ARB );
		glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.

        glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE,
		m_pForegroundDepthBack->width,
		m_pForegroundDepthBack->height,
		0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
		m_pForegroundDepthBack->imageData );

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
	//~ glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 ); // Bind the OpenGL Texture.
	glDisable( GL_TEXTURE_RECTANGLE_EXT );

}


void SwissRanger::DrawForegroundColor()
{
    #if 1
  	if( m_pForegroundColorBack != NULL )
	{
		glEnable( GL_TEXTURE_RECTANGLE_ARB );
		glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
        glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE,
		m_pForegroundColorBack->width,
		m_pForegroundColorBack->height,
		0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
		m_pForegroundColorBack->imageData );

	}
	#else
	if( m_pTemp != NULL )
	{
		glEnable( GL_TEXTURE_RECTANGLE_ARB );
		glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
        glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB,
		m_pTemp->width,
		m_pTemp->height,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		m_pTemp->imageData );

	}

 #endif
	glColor3f ( 1.f, 1.f, 1.f );
	glBegin ( GL_POLYGON );
	{
		glTexCoord2f ( 0.0f, m_uiHeight );		glVertex3f ( 0.0, 0.0, 0.0f );
		glTexCoord2f ( m_uiWidth, m_uiHeight );	glVertex3f ( 1.0, 0.0f, 0.0f );
		glTexCoord2f ( m_uiWidth, 0.0 );		glVertex3f (1.0f, 1.0f, 0.0f );
		glTexCoord2f ( 0.0f, 0.0f );			glVertex3f ( 0.0f, 1.0, 0.0f );
	}
	glEnd ();
	//~ glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 ); // Bind the OpenGL Texture.
	glDisable( GL_TEXTURE_RECTANGLE_EXT );

}


void SwissRanger::DrawHistogram()
{
  	if( m_pForegroundHistogram != NULL )
	{
		glEnable( GL_TEXTURE_RECTANGLE_ARB );
		glBindTexture( GL_TEXTURE_RECTANGLE_ARB, m_iCamTexture ); // Bind the OpenGL Texture.
		glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE,
			m_pForegroundHistogram->width, m_pForegroundHistogram->height,
			0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pForegroundHistogram->imageData );
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
	//~ glBindTexture( GL_TEXTURE_RECTANGLE_EXT, 0 ); // Bind the OpenGL Texture.
	glDisable( GL_TEXTURE_RECTANGLE_EXT );
}
//RADA
// this function is used to return the foreground image
IplImage*  SwissRanger::GetForeground()
{
    return m_pForegroundBack;
}



/**
 * Draws a line between two points p1(p1x,p1y) and p2(p2x,p2y).
 * This function is based on the Bresenham's line algorithm and is highly
 * optimized to be able to draw lines very quickly. There is no floating point
 * arithmetic nor multiplications and divisions involved. Only addition,
 * subtraction and bit shifting are used.
 *
 * Note that you have to define your own customized setPixel(x,y) function,
 * which essentially lights a pixel on the screen.
 */
void  SwissRanger::lineBresenham(int p1x, int p1y, int p2x, int p2y,VoxelsGrid* m_pVoxelsGrid, int line)
{
    int F, x, y;

    if (p1x > p2x)  // Swap points if p1 is on the right of p2
    {
        swap(p1x, p2x);
        swap(p1y, p2y);
    }

    // Handle trivial cases separately for algorithm speed up.
    // Trivial case 1: m = +/-INF (Vertical line)
    if (p1x == p2x)
    {
        if (p1y > p2y)  // Swap y-coordinates if p1 is above p2
        {
            swap(p1y, p2y);
        }

        x = p1x;
        y = p1y;

        while ( x<0)
                x++;

        while ((y <= p2y) && (y<m_pVoxelsGrid->GetGridDepth()) )
        {
             m_pVoxelsGrid->pGrid[line][x][y] = 255;
            y++;
        }
        return;
    }
    // Trivial case 2: m = 0 (Horizontal line)
    else if (p1y == p2y)
    {
        x = p1x;
        y = p1y;

        while ((x <= p2x)  && (x < m_pVoxelsGrid->GetGridWidth()))
        {
            if (x>=0)
                m_pVoxelsGrid->pGrid[line][x][y] = 255;
            x++;
        }
        return;
    }


    int dy            = p2y - p1y;  // y-increment from p1 to p2
    int dx            = p2x - p1x;  // x-increment from p1 to p2
    int dy2           = (dy << 1);  // dy << 1 == 2*dy
    int dx2           = (dx << 1);
    int dy2_minus_dx2 = dy2 - dx2;  // precompute constant for speed up
    int dy2_plus_dx2  = dy2 + dx2;


    if (dy >= 0)    // m >= 0
    {
        // Case 1: 0 <= m <= 1 (Original case)
        if (dy <= dx)
        {
            F = dy2 - dx;    // initial F

            x = p1x;
            y = p1y;
            while ((x <= p2x) && (x < m_pVoxelsGrid->GetGridWidth()))
            {
                if (x>=0)
                    m_pVoxelsGrid->pGrid[line][x][y] = 255;
                if (F <= 0)
                {
                    F += dy2;
                }
                else
                {
                    y++;
                    F += dy2_minus_dx2;
                }
                x++;
            }
        }
        // Case 2: 1 < m < INF (Mirror about y=x line
        // replace all dy by dx and dx by dy)
        else
        {
            F = dx2 - dy;    // initial F

            y = p1y;
            x = p1x;

            while ( x<0)
                x++;
            while ((y <= p2y) && (y<m_pVoxelsGrid->GetGridDepth()))
            {
                 m_pVoxelsGrid->pGrid[line][x][y] = 255;
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F -= dy2_minus_dx2;
                }
                y++;
            }
        }
    }
    else    // m < 0
    {
        // Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy)
        if (dx >= -dy)
        {
            F = -dy2 - dx;    // initial F

            x = p1x;
            y = p1y;
            while ((x <= p2x)  && (x < m_pVoxelsGrid->GetGridWidth()) )
            {
                if (x>=0)
                    m_pVoxelsGrid->pGrid[line][x][y] = 255;
                if (F <= 0)
                {
                    F -= dy2;
                }
                else
                {
                    y--;
                    F -= dy2_plus_dx2;
                }
                x++;
            }
        }
        // Case 4: -INF < m < -1 (Mirror about x-axis and mirror
        // about y=x line, replace all dx by -dy and dy by dx)
        else
        {
            F = dx2 + dy;    // initial F

            y = p1y;
            x = p1x;
            while ( x<0)
                x++;
            while ((y >= p2y) && (y<m_pVoxelsGrid->GetGridDepth()))
            {
                m_pVoxelsGrid->pGrid[line][x][y] = 255;
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F += dy2_plus_dx2;
                }
                y--;
            }
        }
    }
}




