#include "Camera.h"
using namespace std;
using namespace cv;
#include <fstream>

INT initCamera (HIDS *hCam, HWND hWnd)
{	
    INT nRet = is_InitCamera (hCam, hWnd);	
	int ret = is_ParameterSet(*hCam, IS_PARAMETERSET_CMD_LOAD_EEPROM, NULL, 0); //ajout eric
	if (ret != IS_SUCCESS)
    {
			printf("probleme chargement des parametres camera de la EEPROM - ");
	}

    /************************************************************************************************/
    /*                                                                                              */
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*                                                                                              */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */ 
    /*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
    /*                                                                                              */                        
    /************************************************************************************************/
    if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
    {
        // Time for the firmware upload = 25 seconds by default
        INT nUploadTime = 25000;
        is_GetDuration (*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);

        char Str1[] = { "This camera requires a new firmware. The upload will take about" };
        char Str2[] = { "seconds. Please wait ..." };
        char Str3[100];
        sprintf (Str3, "%s %d %s", Str1, nUploadTime / 1000, Str2);
        
		// This seems to be windows specific.
        MessageBox (NULL, Str3, "", MB_ICONWARNING);

        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS) (((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
        nRet = is_InitCamera (hCam, hWnd);   
    }

    return nRet;
}

//////////////////////////////////////////////////////////////////////////////
/// CONSTRUCTEUR - DESTRUCTEUR
//////////////////////////////////////////////////////////////////////////////

Camera::Camera(int index)
{
	/////////////////////////////////////////////
	// CODE AURELIEN 27/03/2014
	/////////////////////////////////////////////
	if (hCam !=0 ) {
        //free old image mem.
        //is_FreeImageMem (hCam, m_pcImageMemory, m_nMemoryId);
        //is_ExitCamera (hCam);
    }
	
	//m_pcImageMemory = NULL;
	//m_nMemoryId = 0;
	hCam = (HIDS)0;                           // open next camera
	int desiredFrameRate = 60;
	m_bitsPerPixel = 24; //chaque info sur 8 bits
	
    int nRet = initCamera (&hCam, m_hwndDisp);    // init camera
	if (nRet == IS_SUCCESS)
	{
        // Get sensor info
		is_GetSensorInfo(hCam, &m_sInfo);
		getMaxImageSize(&width, &height);
		// allocate an image memory.
		if (is_AllocImageMem(hCam, width, height, m_bitsPerPixel, &m_pcImageMemory, &m_nMemoryId) != IS_SUCCESS)
		{
				cout<<"Memory allocation failed!"<<endl;
				active = false;	
				return ;
		}
		is_SetImageMem(hCam, m_pcImageMemory, m_nMemoryId);
		// set the image size to capture

		IS_SIZE_2D imageSize;
		imageSize.s32Width = width;
		imageSize.s32Height = height;

		is_AOI(hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));	
		
		is_CaptureVideo(hCam, IS_WAIT);
		// check if sequence mode is available
		INT nSequence = 0;
		nRet = is_AOI(hCam, IS_AOI_SEQUENCE_GET_SUPPORTED, &nSequence, sizeof(nSequence));

		if (nSequence ==  IS_AOI_SEQUENCE_INDEX_AOI_1)
		{
			cout<<"AOI sequence is not supported!"<<endl;
			exitCamera();
			active = false;	
			return;
		}
		active = true;	
	}
	else
	{
		 cout<<"No uEye camera could be opened !"<<endl;
		 active = false;	
		 return;
	}

	//Boost
	//nRet = is_SetGainBoost(hCam, IS_SET_GAINBOOST_ON);
	//cout<<"Boost = "<<nRet<<endl;
	
	//Set FrameRate
	setFrameRate(desiredFrameRate);
	
	//Global Shutter
	int nMode = IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL;
	setShutterMode(nMode);

	//HDR mode 	
	enableHdrMode();
	
	cout << "Frame size: " << width << " x " << height << " Status : ";
}

void Camera::setFrameRate(int desiredFrameRate){

	int nRet = is_SetFrameRate(hCam, desiredFrameRate, &m_actualFrameRate);
	if (nRet != IS_SUCCESS) {
		cout << "WARNING! Failed to set frame rate to: " << desiredFrameRate << endl;
		cout << "Current frame rate: " << m_actualFrameRate << endl;
		m_numberOfFrames = m_actualFrameRate;
	}
	else
		m_numberOfFrames = desiredFrameRate;
	cout << "FPS = " << m_numberOfFrames << endl;
}

void Camera::setShutterMode(int nMode){
	INT nSupportedFeatures;
	if (is_DeviceFeature(hCam, IS_DEVICE_FEATURE_CMD_GET_SUPPORTED_FEATURES, (void*)&nSupportedFeatures, sizeof(nSupportedFeatures)) == IS_SUCCESS){
		if (nSupportedFeatures & nMode){
			int nRet = is_DeviceFeature(hCam, IS_DEVICE_FEATURE_CMD_SET_SHUTTER_MODE, (void*)&nMode, sizeof(nMode));
			cout << "Global Shutter code retour (0 = success) = " << nRet << endl;
		}
		else{
			cout << "Global Shutter pas supporte" << endl;
			exit(1);
		}
	}
}

void loadExposureSeq(String path, vector<Mat>& images, vector<float>& times){
	path = path + std::string("/"); 
	ifstream list_file((path + "expositionTimes.txt").c_str());
	if (list_file){
		string name;
		float val;
		while (list_file >> name >> val) {
			cout << "load image " << name << endl;
			Mat img = imread(path + name);
			images.push_back(img);
			times.push_back(val);
		}
	} 
	else {
		cout << "Error load images" << endl;
	}
	list_file.close();
}

void Camera::enableHdrMode(){
	int nRet = is_EnableHdr(hCam, IS_ENABLE_HDR);
	if (nRet != IS_SUCCESS){
		cout << "Invalid HDR Mode IDS for this device, error code = " << nRet << endl;
		cout << "HDR Mode OpenCV activated" << endl;
		//Vérification du ShutterMode
		int nMode;
		is_DeviceFeature(hCam, IS_DEVICE_FEATURE_CMD_GET_SHUTTER_MODE, (void*)&nMode, sizeof(nMode));
		if (nMode != IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL){
			nMode = IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_GLOBAL;
			setShutterMode(nMode);
		}
		String path = "res/HDR_calib-set";
		loadExposureSeq(path, images, times);
		cout << "Initialization HDR ..." << endl;
		calibrate = createCalibrateDebevec();
		calibrate->process(images, response, times);
		merge_debevec = createMergeDebevec();
		tonemap = createTonemapDurand(2.2f);
		cout << "Initialization done !" << endl;
	}
	else {
		cout << "not supported yet" << endl;
	}
}

void Camera::exitCamera()
{
	if( hCam != 0 )
	{
		is_StopLiveVideo( hCam, IS_WAIT );
		if( m_pcImageMemory != NULL )
  			is_FreeImageMem( hCam, m_pcImageMemory, m_nMemoryId );
        
		m_pcImageMemory = NULL;
		is_ExitCamera( hCam );
	}
}

void Camera::getMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
    // Check if the camera supports an arbitrary AOI
    // Only the ueye xs does not support an arbitrary AOI
    INT nAOISupported = 0;
    BOOL bAOISupported = TRUE;
    if (is_ImageFormat(hCam, IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED, (void*)&nAOISupported, sizeof(nAOISupported)) == IS_SUCCESS)
        bAOISupported = (nAOISupported != 0);

    if (bAOISupported)
    {  
        // All other sensors
        // Get maximum image size
	    *pnSizeX = m_sInfo.nMaxWidth;
	    *pnSizeY = m_sInfo.nMaxHeight;
    }
    else
    {
        // Only ueye xs
		// Get image size of the current format
		IS_SIZE_2D imageSize;
		is_AOI(hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

		*pnSizeX = imageSize.s32Width;
		*pnSizeY = imageSize.s32Height;
    }
}

//////////////////////////////////////////////////////////////////////////////
/// OTHER FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

cv::Size Camera::getSize()
{
	double dWidth = width; 
	double dHeight = height;
	return cv::Size(dWidth, dHeight);
}

void Camera::loadDistortionMatrix()
{
	cv::FileStorage fs("calibrationProperties", cv::FileStorage::READ);
}
