
//===========================================================================//
//																			 //
//	Initialisation et lecture des images caméra UEYE						 //
//																			 //
//===========================================================================//
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>

#include <tchar.h>

#include <sys/types.h>
#include <sys/timeb.h>
#include <list>
#include <windows.h>


#include "uEye.h"
#include "uEye_tools.h"
#include "ueye_deprecated.h"


#include <conio.h>

bool CreateDisplayWindow();
bool DeleteDisplayWindow();
bool ToggleLive();
bool ToggleDisplay();

int InitCamera (HIDS *hCam, HWND hWnd);
bool OpenCamera();
bool CloseCamera();
void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);
int InitDisplayMode();
bool ProcessuEyeMessages(UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI ConsoleDispWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#define MSG_QUIT_THREAD         (WM_USER + 123)

#define CONSOLEDISPCLASS         "ConsoleDispClass"
#define CONSOLEDISPWINDOW        "ConsoleDispWindow"

typedef enum _disp_mode 
{
    e_disp_mode_bitmap = 0,
    e_disp_mode_directdraw
};


bool            m_boLive;
bool            m_boDisplay;
HCAM            m_hCam = NULL;
CAMINFO         m_ci;
SENSORINFO      m_si;
int             m_nSizeX, m_nSizeY;
int             m_nDispSizeX, m_nDispSizeY;
char*           m_pcImageMemory;
int             m_nMemoryId;
int             m_nColorMode;
int             m_nBitsPerPixel;
int             m_nDispModeSel;
HWND            m_hwndDisp;
ATOM            m_atom;




bool OpenCamera()
{
    int nRet;

    // init camera
    m_hCam = (HCAM)0;                           // open next camera
    nRet = InitCamera (&m_hCam, m_hwndDisp);    // init camera
	printf ("ID windows de la camera ouverte: %d",m_hCam);	

    if( nRet == IS_SUCCESS )
    {
        is_GetCameraInfo( m_hCam, &m_ci);

        // retrieve original image size
        is_GetSensorInfo( m_hCam, &m_si);

		

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        // calc the display size so that it is a integer fraction
        // of the sensor size and max 640 by 480
        int n = 1;
        do
        {
            m_nDispSizeX = m_nSizeX / n;
            n++;
        }
        //while(m_nDispSizeX > 640);
		while(m_nDispSizeX > 1600);

        m_nDispSizeY = m_nSizeY / (n - 1);

        // setup the bitmap or directdraw display mode
        nRet = InitDisplayMode();

        if(nRet == IS_SUCCESS)
        {
            // Enable Messages
            nRet = is_EnableMessage( m_hCam, IS_DEVICE_REMOVED, m_hwndDisp );
            nRet = is_EnableMessage( m_hCam, IS_DEVICE_RECONNECTED, m_hwndDisp );
            nRet = is_EnableMessage( m_hCam, IS_FRAME, m_hwndDisp );
        }   // end if( nRet == IS_SUCCESS )
    }   // end if( nRet == IS_SUCCESS )
	
    return (nRet == IS_SUCCESS);
}


bool CloseCamera()
{
    bool boRet = FALSE;

    if( m_hCam != 0 )
    {
        is_EnableMessage( m_hCam, IS_FRAME, NULL );
        is_StopLiveVideo( m_hCam, IS_WAIT );
        if( m_pcImageMemory != NULL )
            is_FreeImageMem( m_hCam, m_pcImageMemory, m_nMemoryId );
        m_pcImageMemory = NULL;
        is_ExitCamera( m_hCam );
        m_hCam = NULL;
        boRet = TRUE;
    }

    return boRet;
}

int InitDisplayMode()
{
    int nRet = IS_NO_SUCCESS;

    if (m_hCam == NULL)
    {
        return IS_NO_SUCCESS;
    }

    // if some image memory exist already then free it
    if (m_pcImageMemory != NULL)
    {
        nRet = is_FreeImageMem (m_hCam, m_pcImageMemory, m_nMemoryId);
    }

    m_pcImageMemory = NULL;

    switch(m_nDispModeSel)
    {
    case e_disp_mode_directdraw:
        // if initializiing the direct3d mode succeeded then
        // leave the switch tree else run through to bitmap mode
        nRet = is_SetDisplayMode( m_hCam,
            IS_SET_DM_DIRECT3D );
        if(nRet == IS_SUCCESS )
        {
            // setup the color depth to the current VGA settiqng
            nRet = is_GetColorDepth( m_hCam, &m_nBitsPerPixel, &m_nColorMode );
            break;
        }
        else
            m_nDispModeSel = e_disp_mode_bitmap; 

    case e_disp_mode_bitmap:
        nRet = is_SetDisplayMode( m_hCam, IS_SET_DM_DIB);
        if(nRet == IS_SUCCESS)
        {
            if( m_si.nColorMode == IS_COLORMODE_BAYER )
            {
                // for color camera models use RGB24 mode
                m_nColorMode = IS_CM_BGR8_PACKED;
                m_nBitsPerPixel = 24;
				  _cputs("\nRGB24\n");
				
            }
            else if( m_si.nColorMode == IS_COLORMODE_CBYCRY )
            {
                // for CBYCRY camera models use RGB32 mode
                m_nColorMode = IS_CM_BGRA8_PACKED;
                m_nBitsPerPixel = 32;
            }
            else
            {
                // for monochrome camera models use Y8 mode
                m_nColorMode = IS_CM_MONO8;
                m_nBitsPerPixel = 8;
            }

            // allocate an image memory.
            nRet = is_AllocImageMem(m_hCam,
                m_nSizeX,
                m_nSizeY,
                m_nBitsPerPixel,
                &m_pcImageMemory,
                &m_nMemoryId );
        }   // end if(nRet == IS_SUCCESS)
        break;
    }

    if(nRet == IS_SUCCESS)
    {
        // set the image memory only for the bitmap mode when allocated
        if(m_pcImageMemory != NULL)
            nRet = is_SetImageMem( m_hCam, m_pcImageMemory, m_nMemoryId );
			 _cputs("\nImage capture in bitmap mode\n");
        // set the desired color mode
        nRet = is_SetColorMode( m_hCam, m_nColorMode );
        // set the image size to capture
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        nRet = is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
    }   // end if(nRet == IS_SUCCESS)

    return nRet;
}


bool CreateDisplayWindow()
{
    WNDCLASSEX wcx; 
    HMODULE hInstance = GetModuleHandle(NULL);
    if (GetClassInfoEx (hInstance, CONSOLEDISPCLASS, &wcx) == 0)
    {
        // Fill in the window class structure with parameters that describe the main window. 
        wcx.cbSize        = sizeof(wcx);          // size of structure 
        wcx.style         = CS_HREDRAW | CS_NOCLOSE | CS_SAVEBITS | CS_VREDRAW | WS_OVERLAPPED;
        wcx.lpfnWndProc   = ConsoleDispWndProc;   // points to window procedure 
        wcx.cbClsExtra    = 0;                    // no extra class memory 
        wcx.cbWndExtra    = 0;                    // no extra window memory 
        wcx.hInstance     = hInstance;            // handle to instance 
        wcx.hIcon         = NULL;                 // no icon
        wcx.hCursor       = NULL;                 // no cursor
        wcx.lpszMenuName  = NULL;                 // name of menu resource 
        wcx.lpszClassName = CONSOLEDISPCLASS;     // name of window class 
        wcx.hIconSm       = NULL;                 // small class icon 
        wcx.hbrBackground = NULL;

        // Register the window class. 
        m_atom = RegisterClassEx(&wcx);

        if(m_atom != NULL)
        {
            // create our display window
            m_hwndDisp = CreateWindow(CONSOLEDISPCLASS ,        // name of window class 
                "Camera display window",  // title-bar string 
                WS_OVERLAPPEDWINDOW,      // top-level window 
                CW_USEDEFAULT,            // default horizontal position 
                CW_USEDEFAULT,            // default vertical position 
                m_nDispSizeX,             // default width 
                m_nDispSizeY,             // default height 
                (HWND) NULL,              // no owner window 
                (HMENU) NULL,             // use class menu 
                hInstance,                // handle to application instance 
                (LPVOID) NULL);            // no window-creation data 

            if (!m_hwndDisp)
            {
                DWORD a = GetLastError();
                LPVOID lpMsgBuf;
                FormatMessage( 	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                    FORMAT_MESSAGE_FROM_SYSTEM | 
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, a,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR) &lpMsgBuf,	0, NULL );
                // Free the buffer.
                LocalFree( lpMsgBuf );
                ::UnregisterClass (CONSOLEDISPCLASS, hInstance);
                m_atom = NULL;
            }   // end if (!m_hwndDisp)
        }   // end if(m_atom != NULL)
    }   // end if (GetClassInfoEx (hinstance, 

    return (m_atom != NULL);
}

bool DeleteDisplayWindow()
{
    bool boRet = FALSE;

    if(m_atom != NULL)
    {
        HMODULE hInstance = GetModuleHandle(NULL);

        SendMessage(m_hwndDisp, WM_CLOSE, 0, 0);
        SendMessage(m_hwndDisp, WM_DESTROY, 0, 0);

        UnregisterClass (CONSOLEDISPCLASS, hInstance);

        m_atom = NULL;
        m_hwndDisp = NULL;
    }

    return boRet;
}

bool ToggleLive()
{
    bool boRet = FALSE;

    if(m_hCam != NULL)
    {
        m_boLive ^= 1;

        if(m_boLive)
            is_CaptureVideo(m_hCam, IS_DONT_WAIT);
        else
            is_StopLiveVideo(m_hCam, IS_WAIT);
    }

    return boRet;
}

bool ToggleDisplay()
{
    bool boRet = FALSE;

    m_boDisplay ^= 1;

    return boRet;
}

LRESULT WINAPI ConsoleDispWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rc;

    switch ( msg )
    {
    case IS_UEYE_MESSAGE:
        ProcessuEyeMessages(msg, wParam, lParam);
        return 0;

    case WM_CREATE: 
        // Initialize the window. 
        return 0; 

    case WM_ERASEBKGND:
        // we need to refresh the background only if live or
        // display mode is off
        if(!m_boLive || !m_boDisplay)
        {
            if(GetUpdateRect(hwnd, &rc, FALSE))
            {
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
                EndPaint(hwnd, &ps);
            }
        }
        return 1;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam); 
    }
    return 0;
}


bool ProcessuEyeMessages(UINT msg, WPARAM wParam, LPARAM lParam)
{
    bool boRet = FALSE;

    switch(msg)
    {
    case IS_UEYE_MESSAGE:
        switch(wParam)
        {
        case IS_FRAME:
            // if in bitmap mode and the display flag is enabled then
            // draw the actual image buffer
            if((m_nDispModeSel == e_disp_mode_bitmap) && m_boDisplay)
                is_RenderBitmap(m_hCam, m_nMemoryId, m_hwndDisp, IS_RENDER_FIT_TO_WINDOW);
            break;

        case IS_CAPTURE_STATUS:
            Beep(4000, 10);
            break;

        case IS_DEVICE_RECONNECTED:
            Beep(1000, 200);
            break;

        case IS_MEMORY_MODE_FINISH:
            break;

        case IS_DEVICE_REMOVED:
            Beep(3000, 200);
            break;

        case IS_DEVICE_REMOVAL:
            break;

        case IS_NEW_DEVICE:
            break;

        }   // end switch(pmsg->wParam)

        // tell that this message has been processed
        boRet = TRUE;
        break;  // end case IS_UEYE_MESSAGE:
    }   // end switch(msg)

    return boRet;
}


INT InitCamera (HIDS *hCam, HWND hWnd)
{
		
    INT nRet = is_InitCamera (hCam, hWnd);	

	
	/*//ajout eric Juillet 2014. Semble pas nécessaire...
	int ret = is_ParameterSet(*hCam, IS_PARAMETERSET_CMD_LOAD_EEPROM, NULL, NULL); 
	if (ret != IS_SUCCESS)
    {
			printf("probleme chargement des parametres camera de la EEPROM");
	}
	*/

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
        MessageBox (NULL, Str3, "", MB_ICONWARNING);

        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS) (((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
        nRet = is_InitCamera (hCam, hWnd);   
    }

    return nRet;
}


void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
    // Check if the camera supports an arbitrary AOI
    // Only the ueye xs does not support an arbitrary AOI
    INT nAOISupported = 0;
    bool bAOISupported = TRUE;
    if (is_ImageFormat(m_hCam,
                       IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED, 
                       (void*)&nAOISupported, 
                       sizeof(nAOISupported)) == IS_SUCCESS)
    {
        bAOISupported = (nAOISupported != 0);
    }

    if (bAOISupported)
    {  
        // All other sensors
        // Get maximum image size
	    SENSORINFO sInfo;
	    is_GetSensorInfo (m_hCam, &sInfo);
	    *pnSizeX = sInfo.nMaxWidth;
	    *pnSizeY = sInfo.nMaxHeight;
    }
    else
    {
        // Only ueye xs
		// Get image size of the current format
		IS_SIZE_2D imageSize;
		is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

		*pnSizeX = imageSize.s32Width;
		*pnSizeY = imageSize.s32Height;
    }
}
