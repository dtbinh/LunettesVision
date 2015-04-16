/*!
 * \file    CDuEyeCam.cpp
 * \date    19.07.2011
 * \author  IDS - Imaging Development Systems GmbH
 */
#include "stdafx.h"
#include "cdueyecam.h"

/////////////////////////////////////////////////////////////////////////////
// CuEyeCam

IMPLEMENT_DYNCREATE(CuEyeCam, CWnd)

/////////////////////////////////////////////////////////////////////////////
// Eigenschaften CuEyeCam 

BOOL CuEyeCam::GetLiveAtStartup()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetLiveAtStartup(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetActivate1stCamera()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetActivate1stCamera(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetAllowPopupMenu()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetAllowPopupMenu(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetEnableErrorReport()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableErrorReport(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetEnableBadPixelCorrection()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableBadPixelCorrection(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

long CuEyeCam::GetInitBrightness()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void CuEyeCam::SetInitBrightness(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

long CuEyeCam::GetInitContrast()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void CuEyeCam::SetInitContrast(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

long CuEyeCam::GetInitGamma()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void CuEyeCam::SetInitGamma(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

BOOL CuEyeCam::GetInitFitToWindow()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetInitFitToWindow(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetTransferFailedSound()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetTransferFailedSound(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetEnableEvents()
{
	BOOL result;
	GetProperty(0x5a, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableEvents(BOOL propVal)
{
	SetProperty(0x5a, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetEnableColorCorrection()
{
	BOOL result;
	GetProperty(0x5b, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableColorCorrection(BOOL propVal)
{
	SetProperty(0x5b, VT_BOOL, propVal);
}

long CuEyeCam::GetSequenceLength()
{
	long result;
	GetProperty(0x5c, VT_I4, (void*)&result);
	return result;
}

void CuEyeCam::SetSequenceLength(long propVal)
{
	SetProperty(0x5c, VT_I4, propVal);
}

BOOL CuEyeCam::GetDisplayEnable()
{
	BOOL result;
	GetProperty(0x6b, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetDisplayEnable(BOOL propVal)
{
	SetProperty(0x6b, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetEnableDirectDraw()
{
	BOOL result;
	GetProperty(0x7e, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableDirectDraw(BOOL propVal)
{
	SetProperty(0x7e, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetEnableDirect3D()
{
	BOOL result;
	GetProperty(0xAc, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableDirect3D(BOOL propVal)
{
	SetProperty(0xAc, VT_BOOL, propVal);
}

unsigned long CuEyeCam::GetDDKeyColor()
{
	unsigned long result;
	GetProperty(0x81, VT_I4, (void*)&result);
	return result;
}

void CuEyeCam::SetDDKeyColor(unsigned long propVal)
{
	SetProperty(0x81, VT_I4, propVal);
}

BOOL CuEyeCam::GetEnableMaxExposure()
{
	BOOL result;
	GetProperty(0x82, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableMaxExposure(BOOL propVal)
{
	SetProperty(0x82, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetEnableVerticalFlipping()
{
	BOOL result;
	GetProperty(0x83, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableVerticalFlipping(BOOL propVal)
{
	SetProperty(0x83, VT_BOOL, propVal);
}

BOOL CuEyeCam::GetEnableHorizontalFlipping()
{
	BOOL result;
	GetProperty(0x84, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableHorizontalFlipping(BOOL propVal)
{
	SetProperty(0x84, VT_BOOL, propVal);
}

OLE_COLOR CuEyeCam::GetBackColor()
{
	OLE_COLOR result;
	GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
	return result;
}

void CuEyeCam::SetBackColor(OLE_COLOR propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Operationen CuEyeCam 

void CuEyeCam::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CuEyeCam::InitCamera(long hb)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hb);
	return result;
}

void CuEyeCam::ExitCamera()
{
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CuEyeCam::FreezeImage(long timeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		timeout);
	return result;
}

long CuEyeCam::StartLiveVideo(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::StartLiveVideoWait(long nTimeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13D, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nTimeout);
	return result;
}

long CuEyeCam::StopLiveVideo(long nWait)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nWait);
	return result;
}

long CuEyeCam::GetRenderMode()
{
	long result;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetRenderMode(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::GetColorMode()
{
	long result;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetColorMode(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::GetBrightness()
{
	long result;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetBrightness(long nBrightness)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nBrightness);
	return result;
}

long CuEyeCam::GetContrast()
{
	long result;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetContrast(long nContrast)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nContrast);
	return result;
}

long CuEyeCam::GetGamma()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetGamma(long nGamma)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nGamma);
	return result;
}

long CuEyeCam::GetMasterGain()
{
	long result;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetRedGain()
{
	long result;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetGreenGain()
{
	long result;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetBlueGain()
{
	long result;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetHardwareGain(long nMaster, long nRed, long nGreen, long nBlue)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMaster, nRed, nGreen, nBlue);
	return result;
}

long CuEyeCam::GetImagePosition(long* xPos, long* yPos)
{
	long result;
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		xPos, yPos);
	return result;
}

long CuEyeCam::SetImagePosition(long xPos, long yPos)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		xPos, yPos);
	return result;
}

long CuEyeCam::GetImageSize(long* nWidth, long* nHeight)
{
	long result;
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nWidth, nHeight);
	return result;
}

long CuEyeCam::SetImageSize(long nWidth, long nHeight)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nWidth, nHeight);
	return result;
}

long CuEyeCam::SetImageAOI(long lPosX, long lPosY, long lWidth, long lHeight)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x78, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lPosX, lPosY, lWidth, lHeight);
	return result;
}

long CuEyeCam::GetRopEffect()
{
	long result;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetRopEffect(long nEffect, long nParam)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nEffect, nParam);
	return result;
}

long CuEyeCam::GetExternalTrigger()
{
	long result;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetExternalTrigger(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::GetWhiteBalance()
{
	long result;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetWhiteBalance(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::GetWhiteBalanceMultipliers(double* nRed, double* nGreen, double* nBlue)
{
	long result;
	static BYTE parms[] =
		VTS_PR8 VTS_PR8 VTS_PR8;
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nRed, nGreen, nBlue);
	return result;
}

long CuEyeCam::SetWhiteBalanceMultipliers(double nRed, double nGreen, double nBlue)
{
	long result;
	static BYTE parms[] =
		VTS_R8 VTS_R8 VTS_R8;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nRed, nGreen, nBlue);
	return result;
}

CString CuEyeCam::ReadEEProm()
{
	CString result;
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CuEyeCam::WriteEEProm(LPCTSTR pcString)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pcString);
	return result;
}

long CuEyeCam::SaveImage(LPCTSTR strFileName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strFileName);
	return result;
}

long CuEyeCam::GetError()
{
	long result;
	InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CuEyeCam::GetErrorText()
{
	CString result;
	InvokeHelper(0x31, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CuEyeCam::IsErrorReportEnabled()
{
	long result;
	InvokeHelper(0x32, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetErrorReport(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x33, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::GetPixelClock()
{
	long result;
	InvokeHelper(0x34, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetPixelClock(long nMhz)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMhz);
	return result;
}

double CuEyeCam::GetFrameRate()
{
	double result;
	InvokeHelper(0x36, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
	return result;
}

double CuEyeCam::SetFrameRate(double dFps)
{
	double result;
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x37, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		dFps);
	return result;
}

double CuEyeCam::GetExposureTime()
{
	double result;
	InvokeHelper(0x38, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
	return result;
}

double CuEyeCam::SetExposureTime(double dExp)
{
	double result;
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x39, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		dExp);
	return result;
}

long CuEyeCam::GetEdgeEnhancement()
{
	long result;
	InvokeHelper(0x3a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetEdgeEnhancement(long nEnable)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nEnable);
	return result;
}

long CuEyeCam::GetColorCorrection()
{
	long result;
	InvokeHelper(0x3c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetColorCorrection(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3d, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::IsColorCorrectionEnabled()
{
	long result;
	InvokeHelper(0x3e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetBlOffset()
{
	long result;
	InvokeHelper(0x3f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetBlCompensation(long nEnable, long nOffset)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x40, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nEnable, nOffset);
	return result;
}

long CuEyeCam::IsBlCompensationEnabled()
{
	long result;
	InvokeHelper(0x41, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetBadPixelCorrection()
{
	long result;
	InvokeHelper(0x42, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetBadPixelThreshold()
{
	long result;
	InvokeHelper(0x43, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetBadPixelCorrection(long nEnable, long nTreshold)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x44, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nEnable, nTreshold);
	return result;
}

long CuEyeCam::IsHardwareBPCEnabled()
{
	long result;
	InvokeHelper(0x45, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::IsSoftwareBPCEnabled()
{
	long result;
	InvokeHelper(0x46, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::LoadBPCTable(LPCTSTR FILE)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x47, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		FILE);
	return result;
}

long CuEyeCam::GetDC()
{
	long result;
	InvokeHelper(0x48, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::ReleaseDC(long hDC)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x49, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hDC);
	return result;
}

VARIANT CuEyeCam::GetImageMem()
{
	VARIANT result;
	InvokeHelper(0x4a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

long CuEyeCam::InquireImageMem(long* nWidth, long* nHeight, long* nBits, long* nPitch)
{
	long result;
	static BYTE parms[] =
		VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0x4b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nWidth, nHeight, nBits, nPitch);
	return result;
}

long CuEyeCam::PropertyDialog()
{
	long result;
	InvokeHelper(0x4c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::HasVideoStarted()
{
	long result;
	InvokeHelper(0x4d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::IsVideoFinish()
{
	long result;
	InvokeHelper(0x4e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetDLLVersion()
{
	long result;
	InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetControlVersion()
{
	long result;
	InvokeHelper(0x50, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetOSVersion()
{
	long result;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetCameraType()
{
	long result;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetSensorType()
{
	long result;
	InvokeHelper(0x53, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CuEyeCam::GetSerialNumber()
{
	CString result;
	InvokeHelper(0x54, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CuEyeCam::GetCameraVersion()
{
	CString result;
	InvokeHelper(0x55, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetCameraStatus(long nInfo)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x56, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nInfo);
	return result;
}

long CuEyeCam::GetCameraID()
{
	long result;
	InvokeHelper(0x57, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetDeviceID()
{
	long result;
	InvokeHelper(0x107, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

double CuEyeCam::GetFramesPerSecond()
{
	double result;
	InvokeHelper(0x58, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
	return result;
}

long CuEyeCam::EnableEvent(long nWhich, long bEnable)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x59, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nWhich, bEnable);
	return result;
}

long CuEyeCam::SetFlashStrobe(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5d, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::SetSubSampling(long nMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMode);
	return result;
}

long CuEyeCam::GetSubSampling()
{
	long result;
	InvokeHelper(0x5f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetMemoryMode(long nCount, long nDelay)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nCount, nDelay);
	return result;
}

long CuEyeCam::GetLastMemorySequence()
{
	long result;
	InvokeHelper(0x61, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetNumberOfMemoryImages(long nSeqID)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x62, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nSeqID);
	return result;
}

long CuEyeCam::TransferImage(long nMemID, long nSeqID, long nImgNr)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x63, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMemID, nSeqID, nImgNr);
	return result;
}

long CuEyeCam::TransferMemorySequence(long nSeqID, long nStartNr, long nCount, long nSeqPos)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nSeqID, nStartNr, nCount, nSeqPos);
	return result;
}

BOOL CuEyeCam::IsMemoryBoardConnected()
{
	BOOL result;
	InvokeHelper(0x65, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CuEyeCam::ForceTrigger()
{
	long result;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetFlashStrobe()
{
	long result;
	InvokeHelper(0x67, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::MemoryFreezeImage(long timeout)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		timeout);
	return result;
}

void CuEyeCam::UpdateDisplay()
{
	InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CuEyeCam::GetNumberOfCameras()
{
	long result;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetImageWidth()
{
	long result;
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetImageHeight()
{
	long result;
	InvokeHelper(0x6d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CuEyeCam::GetSensorName()
{
	CString result;
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetCameraId(long lid)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lid);
	return result;
}

long CuEyeCam::SetFlashDelay(unsigned long ulDelay)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x70, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ulDelay);
	return result;
}

unsigned long CuEyeCam::GetFlashDelay()
{
	unsigned long result;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetFlashDuration(unsigned long ulDuration)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x72, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ulDuration);
	return result;
}

unsigned long CuEyeCam::GetFlashDuration()
{
	unsigned long result;
	InvokeHelper(0x73, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetGlobalFlashDelays()
{
	long result;
	InvokeHelper(0x74, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CuEyeCam::ReadTriggerPort()
{
	BOOL result;
	InvokeHelper(0x75, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetBinning()
{
	long result;
	InvokeHelper(0x76, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetBinning(long lMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x77, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lMode);
	return result;
}

long CuEyeCam::StealVideo(long lWait)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x79, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lWait);
	return result;
}

long CuEyeCam::LoadParameters(LPCTSTR strFileName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strFileName);
	return result;
}

long CuEyeCam::SaveParameters(LPCTSTR strFileName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strFileName);
	return result;
}

long CuEyeCam::CopyImage(ULONGLONG pBuffer, long lSize)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x85, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pBuffer, lSize);
	return result;
}

long CuEyeCam::SetAllocatedImageMem(ULONGLONG pBuffer, long lSize)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x86, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pBuffer, lSize);
	return result;
}

long CuEyeCam::SetViewWindow (OLE_HANDLE WindowHandle)
{
	long result;
	static BYTE parms[] =
		VTS_HANDLE;  // OLE_HANDLE??
	InvokeHelper(0xA8, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		WindowHandle);
	return result;
}

long CuEyeCam::SaveImageEx(LPCSTR strFileName, long fileformat, long param)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0xA9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strFileName, fileformat, param);
	return result;
}

long CuEyeCam::GetDuration (long nMode, long *pnTime)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0xAB, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nMode, pnTime);
	return result;
}

long CuEyeCam::DirectRenderer (long nMode, void *pParam, long SizeOfParam)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_PUI1 VTS_I4;
	InvokeHelper(0xAD, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nMode, pParam, SizeOfParam);
	return result;
}

long CuEyeCam::GetUsedBandwidth (void)
{
	long result;
    InvokeHelper(0xAE, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::ClearDirect3DOverlay (void)
{
	long result;
    InvokeHelper(0xB0, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::ReadI2C(long nDeviceAddr, long nRegisterAddr, BYTE *pdDATA, long nLen)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PUI1 VTS_I4;
	InvokeHelper(0xB2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nDeviceAddr, nRegisterAddr, pdDATA, nLen);
	return result;
}

long CuEyeCam::GetCameraLUT(unsigned long Mode, unsigned long NumberOfEntries, double *pRed_Grey, double *pGreen, double *pBlue)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PR8 VTS_PR8 VTS_PR8;
	InvokeHelper(0xB3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        Mode, NumberOfEntries, pRed_Grey, pGreen, pBlue);
	return result;
}

long CuEyeCam::GetColorConverter(long ColorMode, long *pCurrentConvertMode, long *pDefaultConvertMode, long *pSupportedConvertModes)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0xB4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        ColorMode, pCurrentConvertMode, pDefaultConvertMode, pSupportedConvertModes);
	return result;
}

long CuEyeCam::GetSupportedTestImages(long *SupportedTestImages)
{
    long result;
    static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0xB5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        SupportedTestImages);
	return result;
}

long CuEyeCam::EnableHdr(long Enable)
{
    long result;
    static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xB6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        Enable);
	return result;
}

long CuEyeCam::GetComportNumber(unsigned long *pComportNumber)
{
    long result;
    static BYTE parms[] =
		VTS_PUI4;
	InvokeHelper(0xB7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        pComportNumber);
	return result;
}

long CuEyeCam::GetHdrMode(long *Mode)
{
    long result;
    static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0xB8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        Mode);
	return result;
}

VARIANT CuEyeCam::GetImageHistogram(long ColorMode)
{
    VARIANT result;
    static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xB9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, 
       ColorMode);
	return result;
}

long CuEyeCam::ResetCaptureErrorInfo(void)
{
    long result;
    InvokeHelper(0xBA, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetAOI(long type, long *pXPos, long *pYPos, long *pWidth, long *pHeight)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0xBB, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        type, pXPos, pYPos, pWidth, pHeight);
	return result;
}

long CuEyeCam::SetCameraLUT(unsigned long Mode, unsigned long NumberOfEntries, double *pRed_Grey, double *pGreen, double *pBlue)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PR8 VTS_PR8 VTS_PR8;
	InvokeHelper(0xBC, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        Mode, NumberOfEntries, pRed_Grey, pGreen, pBlue);
	return result;
}

long CuEyeCam::SetColorConverter(long ColorMode, long ConvertMode)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xBD, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        ColorMode, ConvertMode);
	return result;
}

long CuEyeCam::SetGlobalShutter(long mode)
{
    long result;
    static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xBE, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        mode);
	return result;
}

long CuEyeCam::SetLED(long nValue)
{
    long result;
    static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xBF, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nValue);
	return result;
}

long CuEyeCam::SetOptimalCameraTiming(long Mode, long Timeout_, long *pMaxPxlClk, double *pMaxFrameRate)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PI4 VTS_PR8;
	InvokeHelper(0xC0, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        Mode, Timeout_, pMaxPxlClk, pMaxFrameRate);
	return result;
}

long CuEyeCam::SetSensorTestImage(long TestImage, long Param)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xC1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        TestImage, Param);
	return result;
}

long CuEyeCam::SetTimeout(unsigned long nMode, unsigned long Timeout)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xC2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nMode, Timeout);
	return result;
}

long CuEyeCam::WriteI2C(long nDeviceAddr, long nRegisterAddr, BYTE *pbData, long nLen)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PUI1 VTS_I4;
	InvokeHelper(0xC3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nDeviceAddr, nRegisterAddr, pbData, nLen);
	return result;
}

long CuEyeCam::GetTestImageValueRange(long TestImage, long *TestImageValueMin, long *TestImageValueMax)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_PI4 VTS_PI4;
	InvokeHelper(0xC4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        TestImage, TestImageValueMin, TestImageValueMax);
	return result;
}

long CuEyeCam::GetTimeout(unsigned long nMode, unsigned long *pTimeout)
{
    long result;
    static BYTE parms[] =
		VTS_UI4 VTS_PUI4;
	InvokeHelper(0xC5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nMode, pTimeout);
	return result;
}

long CuEyeCam::GetColorDepth(long *pnCol, long *pnColMode)
{
    long result;
    static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0xC6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        pnCol, pnColMode);
	return result;
}

long CuEyeCam::SetIO(long nIO)
{
	long result;
    static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xC7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nIO);
	return result;
}
long CuEyeCam::GetIO(void)
{
	long result;
    InvokeHelper(0xC8, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::SetIOMask(long nIOMask)
{
	long result;
    static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xC9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nIOMask);
	return result;
}

long CuEyeCam::GetIOMask(void)
{
	long result;
    InvokeHelper(0xCA, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::ResetToDefault(void)
{
	long result;
    InvokeHelper(0xCC, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetBusSpeed(void)
{
	long result;
    InvokeHelper(0xCD, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT CuEyeCam::GetHdrKneepoints(void)
{
	VARIANT result;
	InvokeHelper(0xCF, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT CuEyeCam::GetHdrKneepointInfo(long* NumberOfSupportedKneepoints, double* MinValueX, double* MaxValueX,
							 double* MinValueY, double* MaxValueY)
{
    VARIANT result;
    static BYTE parms[] =
        VTS_PI4 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8;
    InvokeHelper(0xD1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
        NumberOfSupportedKneepoints, MinValueX, MaxValueX, MinValueY, MaxValueY);
    return result;
}


VARIANT CuEyeCam::GetCaptureErrorInfo(unsigned long *dwCapErrCnt_Total)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_PUI4;
	InvokeHelper(0xD0, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		dwCapErrCnt_Total);
	return result;
}

long CuEyeCam::GetExposureTimeRange(double *minExp, double *maxExp, double *intervall)
{
    long result;
    static BYTE parms[] =
		VTS_PR8 VTS_PR8 VTS_PR8;
	InvokeHelper(0xD2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        minExp, maxExp, intervall);
	return result;
}

long CuEyeCam::GetPixelClockRange(long *nMin, long *nMax, long *nDefault)
{
	long result;
    static BYTE parms[] =
		VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0xD3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nMin, nMax, nDefault);
	return result;
}

long CuEyeCam::GetFrameTimeRange(double *minFps, double *maxFps, double *intervall)
{
    long result;
    static BYTE parms[] =
		VTS_PR8 VTS_PR8 VTS_PR8;
	InvokeHelper(0xD4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        minFps, maxFps, intervall);
	return result;
}

// Focus
BOOL CuEyeCam::GetEnableAutoFocus()
{
	BOOL result;
	GetProperty(0xF0, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetEnableAutoFocus(BOOL propVal)
{
	SetProperty(0xF0, VT_BOOL, propVal);
}


long CuEyeCam::GetManualFocus()
{
	long result;
	GetProperty(0xED, VT_I4, (void*)&result);
	return result;
}


void CuEyeCam::SetManualFocus(long propVal)
{
	SetProperty(0xED, VT_I4, propVal);
}


long CuEyeCam::GetFocusDistance()
{
	long result;
	InvokeHelper(0xEE, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetManualFocusRange(long *pMin, long *pMax, long *pInc)
{
    long result;
    static BYTE parms[] =
		VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0xEC, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        pMin, pMax, pInc);
	return result;
}


// Face detection
BOOL CuEyeCam::GetEnableFaceDetection()
{
	BOOL result;
	GetProperty(0xEB, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetEnableFaceDetection(BOOL propVal)
{
	SetProperty(0xEB, VT_BOOL, propVal);
}


BOOL CuEyeCam::GetSuspendFaceDetection()
{
	BOOL result;
	GetProperty(0xEA, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetSuspendFaceDetection(BOOL propVal)
{
	SetProperty(0xEA, VT_BOOL, propVal);
}



VARIANT CuEyeCam::GetFDTFaceList()
{
    VARIANT result;
	InvokeHelper(0xE7, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetFDTMaxNumOvl()
{
    long result;
	GetProperty(0xE4, VT_I4, (void*)&result);
	return result;
}


void CuEyeCam::SetFDTMaxNumOvl(long propVal)
{
    SetProperty(0xE4, VT_I4, propVal);
}


long CuEyeCam::GetFDTMaxNumFaces()
{
    long result;
	InvokeHelper(0xE5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetFDTNumberFaces()
{
    long result;
	InvokeHelper(0xE6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


BOOL CuEyeCam::GetEnableFDTSearchAngle()
{
	BOOL result;
	GetProperty(0xE9, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetEnableFDTSearchAngle(BOOL propVal)
{
	SetProperty(0xE9, VT_BOOL, propVal);
}


long CuEyeCam::GetFDTSearchAngle()
{
    long result;
	GetProperty(0xE8, VT_I4, (void*)&result);
	return result;
}


void CuEyeCam::SetFDTSearchAngle(long propVal)
{
    SetProperty(0xE8, VT_I4, propVal);
}


long CuEyeCam::GetFDTLineWidthOvl()
{
    long result;
	GetProperty(0xE3, VT_I4, (void*)&result);
	return result;
}


void CuEyeCam::SetFDTLineWidthOvl(long propVal)
{
    SetProperty(0xE3, VT_I4, propVal);
}


long CuEyeCam::GetFDTResolution(unsigned long * pulHorizRes, unsigned long *pulVertRes)
{
    long result;
    static BYTE parms[] =
		VTS_PUI4 VTS_PUI4;
	InvokeHelper(0x133, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        pulHorizRes, pulVertRes);
	return result;
}


// Image format
long CuEyeCam::GetIFFormat()
{
    long result;
	GetProperty(0xE0, VT_I4, (void*)&result);
	return result;
}


void CuEyeCam::SetIFFormat(long propVal)
{
    SetProperty(0xE0, VT_I4, propVal);
}


long CuEyeCam::GetIFNumberFormats()
{
    long result;
	InvokeHelper(0xE2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


VARIANT CuEyeCam::GetIFFormatList()
{
    VARIANT result;
	InvokeHelper(0xE1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}


// Image stabilization
BOOL CuEyeCam::GetEnableImageStabilization()
{
	BOOL result;
	GetProperty(0xDE, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetEnableImageStabilization(BOOL propVal)
{
	SetProperty(0xDE, VT_BOOL, propVal);
}


BOOL CuEyeCam::IsIFArbitraryAOISupported()
{
    BOOL result;
	InvokeHelper(0xDF, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}


// Sensor auto features
BOOL CuEyeCam::GetEnableAutoSensorWB()
{
	BOOL result;
	GetProperty(0xDD, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetEnableAutoSensorWB(BOOL propVal)
{
	SetProperty(0xDD, VT_BOOL, propVal);
}


BOOL CuEyeCam::GetEnableAutoSensorShutter()
{
	BOOL result;
	GetProperty(0xDC, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetEnableAutoSensorShutter(BOOL propVal)
{
	SetProperty(0xDC, VT_BOOL, propVal);
}


BOOL CuEyeCam::GetEnableAutoSensorGain()
{
	BOOL result;
	GetProperty(0xD8, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetEnableAutoSensorGain(BOOL propVal)
{
	SetProperty(0xD8, VT_BOOL, propVal);
}


long CuEyeCam::GetAutoSensorShutterMode()
{
    long result;
	GetProperty(0xDB, VT_I4, (void*)&result);
	return result;
}


void CuEyeCam::SetAutoSensorShutterMode(long propVal)
{
    SetProperty(0xDB, VT_I4, propVal);
}


long CuEyeCam::GetAutoSensorShutterDefault()
{
    long result;
	GetProperty(0xDA, VT_I4, (void*)&result);
	return result;
}


long CuEyeCam::GetAutoSensorShutterCapabilities()
{
    long result;
	GetProperty(0xD9, VT_I4, (void*)&result);
	return result;
}


long CuEyeCam::GetAutoSensorGainMode()
{
    long result;
	GetProperty(0xD7, VT_I4, (void*)&result);
	return result;
}


void CuEyeCam::SetAutoSensorGainMode(long propVal)
{
    SetProperty(0xD7, VT_I4, propVal);
}


long CuEyeCam::GetAutoSensorGainDefault()
{
    long result;
	GetProperty(0xD6, VT_I4, (void*)&result);
	return result;
}


long CuEyeCam::GetAutoSensorGainCapabilities()
{
    long result;
	GetProperty(0xD5, VT_I4, (void*)&result);
	return result;
}

long CuEyeCam::GetAutoSensorWBMode()
{
    long result;
    GetProperty(0x106, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoSensorWBMode(long propVal)
{
    SetProperty(0x106, VT_I4, propVal);
}

long CuEyeCam::GetAutoSensorWBDefault()
{
    long result;
    GetProperty(0x105, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetAutoSensorWBCapabilities()
{
    long result;
    GetProperty(0x104, VT_I4, (void*)&result);
    return result;
}

BOOL CuEyeCam::IsAntiFlickerSupported()
{
	BOOL result;
	InvokeHelper(0x128, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetAntiFlickerMode()
{
    long result;
    GetProperty(0x103, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAntiFlickerMode(long propVal)
{
    SetProperty(0x103, VT_I4, propVal);
}

long CuEyeCam::GetAntiFlickerModeDefault()
{
    long result;
    GetProperty(0x101, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetAntiFlickerSupportedModes()
{
    long result;
    GetProperty(0x102, VT_I4, (void*)&result);
    return result;
}

BOOL CuEyeCam::IsAutoBacklightCompSupported()
{
	BOOL result;
	GetProperty(0x129, VT_BOOL, (void*)&result);
	return result;
}

BOOL CuEyeCam::GetEnableAutoBacklightComp()
{
	BOOL result;
	GetProperty(0x100, VT_BOOL, (void*)&result);
	return result;
}


void CuEyeCam::SetEnableAutoBacklightComp(BOOL propVal)
{
	SetProperty(0x100, VT_BOOL, propVal);
}

BOOL CuEyeCam::IsAutoContrastCorrSupported()
{
	BOOL result;
	GetProperty(0x12A, VT_BOOL, (void*)&result);
	return result;
}

double CuEyeCam::GetAutoContrastCorrectionValue()
{
	double result;
	GetProperty(0xFF,VT_R8, (void*)&result);
	return result;
}

void CuEyeCam::SetAutoContrastCorrectionValue(double propVal)
{
    SetProperty(0xFF, VT_R8, propVal);
}

double CuEyeCam::GetAutoContrastCorrectionValueDefault()
{
	double result;
	GetProperty(0xFE,VT_R8, (void*)&result);
	return result;
}

long CuEyeCam::GetAutoContrastCorrectionRange(double* pdMin, double* pdMax, double* pdInc)
{
    long result;
    static BYTE parms[] =
		VTS_PR8 VTS_PR8 VTS_PR8;
	InvokeHelper(0xFD, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        pdMin, pdMax, pdInc);
	return result;
}

BOOL CuEyeCam::GetEnableUseFDTAOIforAESAGC()
{
	BOOL result;
	GetProperty(0xFC, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetEnableUseFDTAOIforAESAGC(BOOL propVal)
{
	SetProperty(0xFC, VT_BOOL, propVal);
}

BOOL CuEyeCam::IsScenePresetSupported()
{
	BOOL result;
	GetProperty(0x12B, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetScenePreset(long propVal)
{
	SetProperty(0xFB, VT_I4, propVal);
}

long CuEyeCam::GetScenePreset()
{
	long result;
	GetProperty(0xFB, VT_I4, (void*)&result);
	return result;
}

long CuEyeCam::GetScenePresetDefault()
{
	long result;
	GetProperty(0xFA, VT_I4, (void*)&result);
	return result;
}

long CuEyeCam::GetSceneSupportedPresets()
{
	long result;
	GetProperty(0xF9, VT_I4, (void*)&result);
	return result;
}

BOOL CuEyeCam::IsDigitalZoomSupported()
{
	BOOL result;
	GetProperty(0x12C, VT_BOOL, (void*)&result);
	return result;
}

VARIANT CuEyeCam::GetDigitalZoomSupportedFactors()
{
    VARIANT result;
	InvokeHelper(0xF8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void CuEyeCam::SetDigitalZoomFactor(double dZoomFactor)
{
    SetProperty(0xF7, VT_R8, dZoomFactor);
}

double CuEyeCam::GetDigitalZoomFactor()
{
    double result;
    GetProperty(0xF7, VT_R8, (void*)&result);
    return result;
}

BOOL CuEyeCam::IsSharpnessSupported()
{
	BOOL result;
	GetProperty(0x12D, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetSharpnessValue(long nSharpness)
{
    SetProperty(0xF6, VT_I4, (void*)nSharpness);
}

long CuEyeCam::GetSharpnessValue()
{
    long result;
    GetProperty(0xF6, VT_I4, &result);
    return result;
}

long CuEyeCam::GetSharpnessValueDefault()
{
    long result;
    GetProperty(0xF5, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetSharpnessRange(long* pnMin, long* pnMax, long* pnInc)
{
    long result;
    static BYTE parms[] =
		VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0xF4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        pnMin, pnMax, pnInc);
	return result;
}

BOOL CuEyeCam::IsSaturationSupported()
{
	BOOL result;
	GetProperty(0x12E, VT_BOOL, (void*)&result);
	return result;
}

void CuEyeCam::SetSaturationValue(long nSaturation)
{
    SetProperty(0xF3, VT_I4, nSaturation);
}

long CuEyeCam::GetSaturationValue()
{
    long result;
    GetProperty(0xF3, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetSaturationValueDefault()
{
    long result;
    GetProperty(0xF2, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetSaturationRange(long* pnMin, long* pnMax, long* pnInc)
{
    long result;
    static BYTE parms[] =
		VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0xF1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        pnMin, pnMax, pnInc);
	return result;
}

void CuEyeCam::SetRGBColorModel(long nColorModel)
{
    SetProperty(0x10D, VT_I4, nColorModel);
}

long CuEyeCam::GetRGBColorModel()
{
    long result;
    GetProperty(0x10D, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetRGBColorModelDefault()
{
    long result;
    GetProperty(0x10C, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetRGBColorModelSupportedModels()
{
    long result;
    GetProperty(0x10B, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetColorTemperature(long nTemperature)
{
    SetProperty(0x10A, VT_I4, nTemperature);
}

long CuEyeCam::GetColorTemperature()
{
    long result;
    GetProperty(0x10A, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetColorTemperatureDefault()
{
    long result;
    GetProperty(0x109, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetColorTemperatureRange(long* pnMin, long* pnMax, long* pnInc)
{
    long result;
    static BYTE parms[] =
		VTS_PI4 VTS_PI4 VTS_PI4;
	InvokeHelper(0x108, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        pnMin, pnMax, pnInc);
	return result;
}

void CuEyeCam::SetTriggerDebounceMode(long nMode)
{
    SetProperty(0x113, VT_I4, nMode);
}

long CuEyeCam::GetTriggerDebounceMode()
{
    long result;
    GetProperty(0x113, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetTriggerDebounceDelay(long nDelay)
{
    SetProperty(0x110, VT_I4, nDelay);
}

long CuEyeCam::GetTriggerDebounceDelay()
{
    long result;
    GetProperty(0x110, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetTriggerDebounceDelayRange(long * pnMin, long * pnMax, long * pnInc)
{
	long result;
	static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 ;
	InvokeHelper(0x10e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pnMin, pnMax, pnInc);
	return result;
}

long CuEyeCam::GetTriggerDebounceDelayDefault()
{
	long result;
	InvokeHelper(0x10f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetTriggerDebounceSupportedMode()
{
	long result;
	InvokeHelper(0x111, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetTriggerDebounceModeDefault()
{
	long result;
	InvokeHelper(0x112, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetCameraHandle()
{
	long result;
	InvokeHelper(0x114, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL CuEyeCam::IsDirectRendererSupported()
{
	BOOL result;
	InvokeHelper(0x115, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CuEyeCam::DRSetHWND(OLE_HANDLE hWnd)
{
	long result;
	static BYTE parms[] = VTS_HANDLE;
	InvokeHelper(0x116, DISPATCH_METHOD, VT_I4, (void*)&result, parms, hWnd);
	return result;
}

long CuEyeCam::DRClearOverlay()
{
	long result;
	InvokeHelper(0x118, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::DRLoadOverlayFromFile(LPCTSTR strFileName)
{
	long result;
	static BYTE parms[] = VTS_BSTR ;
	InvokeHelper(0x119, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strFileName);
	return result;
}

long CuEyeCam::DRGetSyncUserRange(long * pnMin, long * pnMax)
{
	long result;
	static BYTE parms[] = VTS_PI4 VTS_PI4 ;
	InvokeHelper(0x11a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pnMin, pnMax);
	return result;
}

long CuEyeCam::DRSetSyncUser(long nSyncPos)
{
	long result;
	static BYTE parms[] = VTS_I4 ;
	InvokeHelper(0x11b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nSyncPos);
	return result;
}

long CuEyeCam::DREnableSyncAuto(BOOL bNewVal)
{
	long result;
	static BYTE parms[] = VTS_BOOL;
	InvokeHelper(0x11c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bNewVal);
	return result;
}

long CuEyeCam::DREnableSemiTransparentOverlay(BOOL bNewVal)
{
	long result;
	static BYTE parms[] = VTS_BOOL;
	InvokeHelper(0x11d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bNewVal);
	return result;
}

long CuEyeCam::DREnableImageScaling()
{
	long result;
	InvokeHelper(0x11e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::DREnableScaling(BOOL bNewVal)
{
	long result;
	static BYTE parms[] = VTS_BOOL;
	InvokeHelper(0x11f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bNewVal);
	return result;
}

long CuEyeCam::DRShowOverlay(BOOL bNewVal)
{
	long result;
	static BYTE parms[] = VTS_BOOL ;
	InvokeHelper(0x120, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bNewVal);
	return result;
}

long CuEyeCam::DRSetOverlayKeyColor(long nRed, long nGreen, long nBlue)
{
	long result;
	static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
	InvokeHelper(0x121, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nRed, nGreen, nBlue);
	return result;
}

long CuEyeCam::DRGetOverlayKeyColor(long * pnRed, long * pnGreen, long * pnBlue)
{
	long result;
	static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 ;
	InvokeHelper(0x122, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pnRed, pnGreen, pnBlue);
	return result;
}

long CuEyeCam::DRSetOverlayPos(long nX, long nY)
{
	long result;
	static BYTE parms[] = VTS_I4 VTS_I4 ;
	InvokeHelper(0x123, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nX, nY);
	return result;
}

long CuEyeCam::DRSetOverlaySize(long nWidth, long nHeight)
{
	long result;
	static BYTE parms[] = VTS_I4 VTS_I4 ;
	InvokeHelper(0x124, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nWidth, nHeight);
	return result;
}

long CuEyeCam::DRGetMaxOverlaySize(long * pnWidth, long * pnHeight)
{
	long result;
	static BYTE parms[] = VTS_PI4 VTS_PI4 ;
	InvokeHelper(0x125, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pnWidth, pnHeight);
	return result;
}

long CuEyeCam::DRReleaseOverlayDC()
{
	long result;
	InvokeHelper(0x126, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::DRGetOverlayDC(long* hDC)
{
	long result;
	static BYTE parms[] = VTS_I4;//VTS_UNKNOWN ;
	InvokeHelper(0x127, DISPATCH_METHOD, VT_I4, (void*)&result, parms, hDC);
	return result;
}

void CuEyeCam::DRSetStealFormat(long nColorFormat)
{
    SetProperty(0x117, VT_I4, nColorFormat);
}

long CuEyeCam::DRGetStealFormat()
{
    long result;
    GetProperty(0x117, VT_I4, (void*)&result);
    return result;
}

long CuEyeCam::GetTriggerDelay()
{
    long result;
    GetProperty(0x98, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetTriggerDelay(long nDelay)
{
    SetProperty(0x98, VT_I4, nDelay);
}

BOOL CuEyeCam::GetEnableAutoExposure()	
{
    BOOL result;
    GetProperty(0x99, VT_BOOL, (void*)&result);
    return result;
}

void CuEyeCam::SetEnableAutoExposure(BOOL bNewVal)
{
    SetProperty(0x99, VT_BOOL, bNewVal);
}

BOOL CuEyeCam::GetEnableAutoGain()
{
    BOOL result;
    GetProperty(0x9A, VT_BOOL, (void*)&result);
    return result;
}

void CuEyeCam::SetEnableAutoGain(BOOL bNewVal)
{
    SetProperty(0x9A, VT_I4, bNewVal);
}

BOOL CuEyeCam::GetEnableAutoFramerate()	
{
    BOOL result;
    GetProperty(0x9B, VT_BOOL, (void*)&result);
    return result;
}

void CuEyeCam::SetEnableAutoFramerate(BOOL bNewVal)
{
    SetProperty(0x9B, VT_I4, bNewVal);
}

long CuEyeCam::GetAutoBrightSpeed()
{
    long result;
    GetProperty(0x9C, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoBrightSpeed(long nSpeed)
{
    SetProperty(0x9C, VT_I4, nSpeed);
}

long CuEyeCam::GetAutoWBSpeed()
{
    long result;
    GetProperty(0x9D, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoWBSpeed(long nSpeed)
{
    SetProperty(0x9D, VT_I4, nSpeed);
}

long CuEyeCam::GetAutoBrightReference()
{
    long result;
    GetProperty(0x9E, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoBrightReference(long nReference)
{
    SetProperty(0x9E, VT_I4, nReference);
}

long CuEyeCam::GetAutoBrightMaxGain()
{
    long result;
    GetProperty(0x9F, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoBrightMaxGain(long nMaxGain)
{
    SetProperty(0x9F, VT_I4, nMaxGain);
}

double CuEyeCam::GetAutoBrightMaxExp()
{
    double result;
    GetProperty(0xA0, VT_R8, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoBrightMaxExp(double dblMaxBrightness)
{
    SetProperty(0xA0, VT_R8, dblMaxBrightness);
}

long CuEyeCam::GetAutoWBROffset()	
{
    long result;
    GetProperty(0xA1, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoWBROffset(long nOffset)
{
    SetProperty(0xA1, VT_I4, nOffset);
}

long CuEyeCam::GetAutoWBBOffset()	
{
    long result;
    GetProperty(0xA2, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoWBBOffset(long nOffset)
{
    SetProperty(0xA2, VT_I4, nOffset);
}

long CuEyeCam::GetAutoWBMinGain()	
{
    long result;
    GetProperty(0xA3, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoWBMinGain(long nMinGain)
{
    SetProperty(0xA3, VT_I4, nMinGain);
}

long CuEyeCam::GetAutoWBMaxGain()	
{
    long result;
    GetProperty(0xA4, VT_I4, (void*)&result);
    return result;
}

void CuEyeCam::SetAutoWBMaxGain(long nMaxGain)
{
    SetProperty(0xA4, VT_I4, nMaxGain);
}

long CuEyeCam::GetSaturationU()
{
	long result;
	InvokeHelper(0xA5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::GetSaturationV()
{
	long result;
	InvokeHelper(0xA6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CuEyeCam::SetSaturation(long nChromU, long nChromV)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xA7, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nChromU, nChromV);
	return result;
}

long CuEyeCam::AddHDRKneepoint(double dblX, double dblY)
{
    long result;
    static BYTE parms[] =
		VTS_R8 VTS_R8;
	InvokeHelper(0x12F, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        dblX, dblY);
	return result;
}

long CuEyeCam::SetAtHDRKneepoint(int nPos, double dblX, double dblY)
{
    long result;
    static BYTE parms[] =
		VTS_I4 VTS_R8 VTS_R8;
	InvokeHelper(0x130, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nPos, dblX, dblY);
	return result;
}
long CuEyeCam::DeleteHDRKneepoint(int nPos)
{
    long result;
    static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x131, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
        nPos);
	return result;
}

long CuEyeCam::SetHDRKneepointsDefault()
{
    long result;
	InvokeHelper(0x132, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::IsSupportedCPUIdleStatesBattery()
{
	long result;
	InvokeHelper(0x13C, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::IsSupportedCPUIdleStatesMains()
{
	long result;
	InvokeHelper(0x13B, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::IsSupportedOpenMP()
{
	long result;
	InvokeHelper(0x13A, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::IsSupportedInitialParameterset()
{
	long result;
	InvokeHelper(0x139, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetEnableOpenMPDefault()
{
	long result;
	InvokeHelper(0x138, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetTriggerStatus()
{
	long result;
	InvokeHelper(0x13E, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetAutoSkipframe()
{
	long result;
    GetProperty(0x13F, VT_I4, (void*)&result);
    return result;
}


void CuEyeCam::SetAutoSkipframe(int newVal)
{
	SetProperty(0x13F, VT_I4, newVal);
}


long CuEyeCam::GetAutoSkipframeMinRange()
{
	long result;
	InvokeHelper(0x140, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetAutoSkipframeMaxRange()
{
	long result;
	InvokeHelper(0x141, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetAutoHysteresis()
{
	long result;
    GetProperty(0x142, VT_I4, (void*)&result);
    return result;
}


void CuEyeCam::SetAutoHysteresis(int newVal)
{    	
	SetProperty(0x142, VT_I4, newVal);
}


long CuEyeCam::GetAutoHysteresisMinRange()
{
	long result;
	InvokeHelper(0x143, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


long CuEyeCam::GetAutoHysteresisMaxRange()
{
	long result;
	InvokeHelper(0x144, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}
