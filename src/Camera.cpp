#include "Camera.h"


INT InitCamera (HIDS *hCam, HWND hWnd)
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
        //MessageBox (NULL, Str3, "", MB_ICONWARNING);

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

Camera::Camera(int index, cv::Size camSize)
{
	/////////////////////////////////////////////
	// TEST ERIC 03/09/2014
	/////////////////////////////////////////////
	hCam = 0;
	HWND    m_hwndDisp;
	hCam = (HCAM)0;                           // open next camera
    int nRet = InitCamera (&hCam, m_hwndDisp);    // init camera
	nRet = is_AllocImageMem(hCam,
                camSize.width,
                camSize.height,
                24,// Nb de bits par pixel
                &m_pcImageMemory,
                &m_nMemoryId ); //Alloue emplacement mémoire pour image
	is_SetImageMem(hCam,m_pcImageMemory,m_nMemoryId); //active la zone mémoire
	if(is_EnableHdr(hCam, IS_ENABLE_HDR) != IS_SUCCESS)
		cout << "HDR uEye impossible" << endl;
	int rep = is_CaptureVideo(hCam, IS_DONT_WAIT);
	if(rep != 0){ //capture l'image live
		cout << "Camera non active - Error " << endl;
		active = false;
		if(rep == 140){ //Error Code : Camera already running
			is_StopLiveVideo(hCam, IS_FORCE_VIDEO_STOP); //force l'arret de la capture
			cout << "Camera arrete " << endl;
			if(is_CaptureVideo(hCam, IS_DONT_WAIT) == IS_SUCCESS){ //capture l'image live
				cout << "Camera redemarre " << endl;
				active = true;
			}
		}
	}else 
		active = true;
	cout << "Frame size: " << camSize.width << " x " << camSize.height << " Status : ";
	//////// FIN TEST ////////////////////
	
	/*
	videoStream = new cv::VideoCapture(index);
	if(!videoStream->isOpened()) { active = false; return; }
	else active = true;

	videoStream->set(CV_CAP_PROP_FRAME_WIDTH,camSize.width);
	videoStream->set(CV_CAP_PROP_FRAME_HEIGHT,camSize.height);
	videoStream->set(CV_CAP_PROP_FPS,60);

	double dWidth = videoStream->get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = videoStream->get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the  video
	cout << "Frame size: " << dWidth << " x " << dHeight << " Status : ";
	*/
}

Camera::~Camera(void)
{
	//delete videoStream;
	is_StopLiveVideo (hCam,IS_FORCE_VIDEO_STOP);
}

//////////////////////////////////////////////////////////////////////////////
/// OTHER FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

cv::Size Camera::getSize()
{
//	double dWidth = videoStream->get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
//	double dHeight = videoStream->get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the  video
	double dWidth = 1600; 
	double dHeight = 1200;
	return cv::Size(dWidth, dHeight);
}

void Camera::loadDistortionMatrix()
{
	cv::FileStorage fs("calibrationProperties", cv::FileStorage::READ);
	// Load matrices with the corresponding camera Id
}
