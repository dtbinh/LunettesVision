#pragma once

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

