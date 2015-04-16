/*!
 * \file    CDuEyeCam.h
 * \date    19.07.2011
 * \author  IDS - Imaging Development Systems GmbH
 */
class CuEyeCam : public CWnd
{
protected:
	DECLARE_DYNCREATE(CuEyeCam)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xa88f30fe, 0x4c03, 0x4e37, { 0xb6, 0xe4, 0xed, 0x93, 0x41, 0x9, 0x37, 0x80 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attribute
public:
	BOOL GetLiveAtStartup();
	void SetLiveAtStartup(BOOL);
	BOOL GetActivate1stCamera();
	void SetActivate1stCamera(BOOL);
	BOOL GetAllowPopupMenu();
	void SetAllowPopupMenu(BOOL);
	BOOL GetEnableErrorReport();
	void SetEnableErrorReport(BOOL);
	BOOL GetEnableBadPixelCorrection();
	void SetEnableBadPixelCorrection(BOOL);
	long GetInitBrightness();
	void SetInitBrightness(long);
	long GetInitContrast();
	void SetInitContrast(long);
	long GetInitGamma();
	void SetInitGamma(long);
	BOOL GetInitFitToWindow();
	void SetInitFitToWindow(BOOL);
	BOOL GetTransferFailedSound();
	void SetTransferFailedSound(BOOL);
	BOOL GetEnableEvents();
	void SetEnableEvents(BOOL);
	BOOL GetEnableColorCorrection();
	void SetEnableColorCorrection(BOOL);
	long GetSequenceLength();
	void SetSequenceLength(long);
	BOOL GetDisplayEnable();
	void SetDisplayEnable(BOOL);
	BOOL GetEnableDirectDraw();
	void SetEnableDirectDraw(BOOL);
    BOOL GetEnableDirect3D();
    void SetEnableDirect3D(BOOL);
	unsigned long GetDDKeyColor();
	void SetDDKeyColor(unsigned long);
	BOOL GetEnableMaxExposure();
	void SetEnableMaxExposure(BOOL);
	BOOL GetEnableVerticalFlipping();
	void SetEnableVerticalFlipping(BOOL);
	BOOL GetEnableHorizontalFlipping();
	void SetEnableHorizontalFlipping(BOOL);
	OLE_COLOR GetBackColor();
	void SetBackColor(OLE_COLOR);

// Operationen
public:
	void AboutBox();
	long InitCamera(long hb);
	void ExitCamera();
	long FreezeImage(long timeout);
	long StartLiveVideo(long nMode);
	long StartLiveVideoWait(long nTimeout);
	long StopLiveVideo(long nWait);
	long GetRenderMode();
	long SetRenderMode(long nMode);
	long GetColorMode();
	long SetColorMode(long nMode);
	long GetBrightness();
	long SetBrightness(long nBrightness);
	long GetContrast();
	long SetContrast(long nContrast);
	long GetGamma();
	long SetGamma(long nGamma);
	long GetMasterGain();
	long GetRedGain();
	long GetGreenGain();
	long GetBlueGain();
	long SetHardwareGain(long nMaster, long nRed, long nGreen, long nBlue);
	long GetImagePosition(long* xPos, long* yPos);
	long SetImagePosition(long xPos, long yPos);
	long GetImageSize(long* nWidth, long* nHeight);
	long SetImageSize(long nWidth, long nHeight);
	long SetImageAOI(long lPosX, long lPosY, long lWidth, long lHeight);
	long GetRopEffect();
	long SetRopEffect(long nEffect, long nParam);
	long GetExternalTrigger();
	long SetExternalTrigger(long nMode);
	long GetWhiteBalance();
	long SetWhiteBalance(long nMode);
	long GetWhiteBalanceMultipliers(double* nRed, double* nGreen, double* nBlue);
	long SetWhiteBalanceMultipliers(double nRed, double nGreen, double nBlue);
	CString ReadEEProm();
	long WriteEEProm(LPCTSTR pcString);
	long SaveImage(LPCTSTR strFileName);
	long GetError();
	CString GetErrorText();
	long IsErrorReportEnabled();
	long SetErrorReport(long nMode);
	long GetPixelClock();
	long SetPixelClock(long nMhz);
	double GetFrameRate();
	double SetFrameRate(double dFps);
	double GetExposureTime();
	double SetExposureTime(double dExp);
	long GetEdgeEnhancement();
	long SetEdgeEnhancement(long nEnable);
	long GetColorCorrection();
	long SetColorCorrection(long nMode);
	long IsColorCorrectionEnabled();
	long GetBlOffset();
	long SetBlCompensation(long nEnable, long nOffset);
	long IsBlCompensationEnabled();
	long GetBadPixelCorrection();
	long GetBadPixelThreshold();
	long SetBadPixelCorrection(long nEnable, long nTreshold);
	long IsHardwareBPCEnabled();
	long IsSoftwareBPCEnabled();
	long LoadBPCTable(LPCTSTR FILE);
	long GetDC();
	long ReleaseDC(long hDC);
	VARIANT GetImageMem();
	long InquireImageMem(long* nWidth, long* nHeight, long* nBits, long* nPitch);
	long PropertyDialog();
	long HasVideoStarted();
	long IsVideoFinish();
	long GetDLLVersion();
	long GetControlVersion();
	long GetOSVersion();
	long GetCameraType();
	long GetSensorType();
	CString GetSerialNumber();
	CString GetCameraVersion();
	long GetCameraStatus(long nInfo);
	long GetCameraID();
    long GetDeviceID();
	double GetFramesPerSecond();
	long EnableEvent(long nWhich, long bEnable);
	long SetFlashStrobe(long nMode);
	long SetSubSampling(long nMode);
	long GetSubSampling();
	long SetMemoryMode(long nCount, long nDelay);
	long GetLastMemorySequence();
	long GetNumberOfMemoryImages(long nSeqID);
	long TransferImage(long nMemID, long nSeqID, long nImgNr);
	long TransferMemorySequence(long nSeqID, long nStartNr, long nCount, long nSeqPos);
	BOOL IsMemoryBoardConnected();
	long ForceTrigger();
	long GetFlashStrobe();
	long MemoryFreezeImage(long timeout);
	void UpdateDisplay();
	long GetNumberOfCameras();
	long GetImageWidth();
	long GetImageHeight();
	CString GetSensorName();
	long SetCameraId(long lid);
	long SetFlashDelay(unsigned long ulDelay);
	unsigned long GetFlashDelay();
	long SetFlashDuration(unsigned long ulDuration);
	unsigned long GetFlashDuration();
	long SetGlobalFlashDelays();
	BOOL ReadTriggerPort();
	long GetBinning();
	long SetBinning(long lMode);
	long StealVideo(long lWait);
	long LoadParameters(LPCTSTR strFileName);
	long SaveParameters(LPCTSTR strFileName);	
    long CopyImage(ULONGLONG pBuffer, long lSize);
	long SetAllocatedImageMem(ULONGLONG pBuffer, long lSize);
	long SetViewWindow (OLE_HANDLE WindowHandle);
	long DirectRenderer (long nMode, void *pParam, long SizeOfParam);
    long GetDuration (long nMode, long *pnTime);

	long SaveImageEx (LPCSTR strFileName, long fileFormat, long Param);
	long GetUsedBandwidth (void);
	long ClearDirect3DOverlay (void);
	long ReadI2C (long nDeviceAddr, long nRegisterAddr, BYTE* pdDATA, long nLen);
	long WriteI2C (long nDeviceAddr, long nRegisterAddr, BYTE* pbData, long nLen);
	long GetCameraLUT (unsigned long Mode, unsigned long NumberOfEntries, double* pRed_Grey, double* pGreen, double* pBlue);
 	long GetColorConverter (long ColorMode, long* pCurrentConvertMode, long* pDefaultConvertMode, long* pSupportedConvertModes);
	long GetSupportedTestImages (long* SupportedTestImages);
	long EnableHdr (long Enable);
	long GetComportNumber (unsigned long* pComportNumber);
	long GetHdrMode (long* Mode);
	VARIANT GetImageHistogram (long ColorMode);
	long ResetCaptureErrorInfo (void);
	long SetAOI (long type, long* pXPos, long* pYPos, long* pWidth, long* pHeight);
	long SetCameraLUT (unsigned long Mode, unsigned long NumberOfEntries, double* pRed_Grey, double* pGreen, double* pBlue);
	long SetColorConverter (long ColorMode, long ConvertMode);
	long SetGlobalShutter (long mode);
	long SetLED (long nValue);
	long SetOptimalCameraTiming (long Mode, long Timeout_, long* pMaxPxlClk, double* pMaxFrameRate);
	long SetSensorTestImage (long TestImage, long Param);
	long SetTimeout (unsigned long nMode, unsigned long Timeout);
	long GetTestImageValueRange (long TestImage, long* TestImageValueMin, long* TestImageValueMax);
	long GetTimeout (unsigned long nMode, unsigned long* pTimeout);
	long GetColorDepth (long* pnCol, long* pnColMode);
	long SetIO(long nIO);
	long GetIO(void);
	long SetIOMask(long nIOMask);
	long GetIOMask(void);
	long ResetToDefault(void);
	long GetBusSpeed(void);
    VARIANT GetHdrKneepoints(void);
    VARIANT GetHdrKneepointInfo(long* NumberOfSupportedKneepoints, double* MinValueX, double* MaxValueX,
							 double* MinValueY, double* MaxValueY);

	VARIANT GetCaptureErrorInfo(unsigned long* dwCapErrCnt_Total);

    long GetExposureTimeRange(double* minExp, double* maxExp, double* intervall);
	long GetPixelClockRange (long* nMin, long* nMax, long* nDefault);
	long GetFrameTimeRange( double* minFps, double* maxFps, double* intervall);


    // New functions for the uEye XS

    // Focus
    BOOL GetEnableAutoFocus();
    void SetEnableAutoFocus(BOOL propVal);
    long GetManualFocus();
    void SetManualFocus(long propVal);
    long GetAutoFocusRange();
    void SetAutoFocusRange(long propVal);
    long GetFocusDistance();
    long GetManualFocusRange(long *pMin, long *pMax, long *pInc);

    // Face detection
    BOOL GetEnableFaceDetection();
    void SetEnableFaceDetection(BOOL propVal);
    BOOL GetSuspendFaceDetection();
    void SetSuspendFaceDetection(BOOL propVal);
    VARIANT GetFDTFaceList();
    long GetFDTMaxNumOvl();
    void SetFDTMaxNumOvl(long propVal);
    long GetFDTMaxNumFaces();
    long GetFDTNumberFaces();
    BOOL GetEnableFDTSearchAngle();
    void SetEnableFDTSearchAngle(BOOL propVal);
    long GetFDTSearchAngle();
    void SetFDTSearchAngle(long propVal);
    long GetFDTLineWidthOvl();
    void SetFDTLineWidthOvl(long propVal);
    long GetFDTResolution(unsigned long * pulHorizRes, unsigned long *pulVertRes);

    // Image format
    long GetIFFormat();
    void SetIFFormat(long propVal);
    long GetIFNumberFormats();
    VARIANT GetIFFormatList();

    // Image stabilization
    BOOL GetEnableImageStabilization();
    void SetEnableImageStabilization(BOOL propVal);

    BOOL IsIFArbitraryAOISupported();

    // Sensor auto features
    BOOL GetEnableAutoSensorWB();
    void SetEnableAutoSensorWB(BOOL propVal);
    BOOL GetEnableAutoSensorShutter();
    void SetEnableAutoSensorShutter(BOOL propVal);
    BOOL GetEnableAutoSensorGain();
    void SetEnableAutoSensorGain(BOOL propVal);
    long GetAutoSensorShutterMode();
    void SetAutoSensorShutterMode(long propVal);
    long GetAutoSensorShutterDefault();
    long GetAutoSensorShutterCapabilities();
    long GetAutoSensorGainMode();
    void SetAutoSensorGainMode(long propVal);
    long GetAutoSensorGainDefault();
    long GetAutoSensorGainCapabilities();
    long GetAutoSensorWBMode();
    void SetAutoSensorWBMode(long propVal);
    long GetAutoSensorWBDefault();
    long GetAutoSensorWBCapabilities();

    // Anti flicker
    BOOL IsAntiFlickerSupported();
    long GetAntiFlickerMode();
    void SetAntiFlickerMode(long propVal);
    long GetAntiFlickerModeDefault();
    long GetAntiFlickerSupportedModes();
    
    // Auto backlight compensation
    BOOL IsAutoBacklightCompSupported();
    BOOL GetEnableAutoBacklightComp();
    void SetEnableAutoBacklightComp(BOOL propVal);
	
    // Auto contrast correction
    BOOL IsAutoContrastCorrSupported();
    double GetAutoContrastCorrectionValue();
    void SetAutoContrastCorrectionValue(double propVal);
    double GetAutoContrastCorrectionValueDefault();
    long GetAutoContrastCorrectionRange  (double* pdMin, double* pdMax, double* pdInc);
    
    // Enable use of FDT AOI for AES/aGC
    BOOL GetEnableUseFDTAOIforAESAGC();
    void SetEnableUseFDTAOIforAESAGC(BOOL propVal);
    
    // Scene Selection
    BOOL IsScenePresetSupported(); 
    void SetScenePreset(long propVal);
    long GetScenePreset();
    long GetScenePresetDefault();
    long GetSceneSupportedPresets();
    
    // Digital Zoom
    BOOL IsDigitalZoomSupported();
    VARIANT GetDigitalZoomSupportedFactors();
    void SetDigitalZoomFactor(double dZoomFactor);
    double GetDigitalZoomFactor();
    
    // Sharpness correction
    BOOL IsSharpnessSupported();
    void SetSharpnessValue(long nSharpness);
    long GetSharpnessValue();
    long GetSharpnessValueDefault();
    long GetSharpnessRange(long* pnMin, long* pnMax, long* pnInc);
    
    // Saturation
    BOOL IsSaturationSupported();
    void SetSaturationValue(long nSaturation);
    long GetSaturationValue();
    long GetSaturationValueDefault();
    long GetSaturationRange(long* pnMin, long* pnMax, long* pnInc);

    // RGB color model
    void SetRGBColorModel(long nColorModel);
    long GetRGBColorModel();
    long GetRGBColorModelDefault();
    long GetRGBColorModelSupportedModels();

    // color temperature
    void SetColorTemperature(long nTemperature);
    long GetColorTemperature();
    long GetColorTemperatureDefault();
    long GetColorTemperatureRange(long* pnMin, long* pnMax, long* pnInc);

    // trigger debouncing
    void SetTriggerDebounceMode(long nMode);
    long GetTriggerDebounceMode();
    long GetTriggerDebounceModeDefault();
    long GetTriggerDebounceSupportedMode();
    void SetTriggerDebounceDelay(long nDelay);
    long GetTriggerDebounceDelay();
    long GetTriggerDebounceDelayDefault();
    long GetTriggerDebounceDelayRange(long* pnMin, long* pnMax, long* pnInc);

	long GetCameraHandle();

    BOOL IsDirectRendererSupported();
	long DRSetHWND(OLE_HANDLE hWnd);
	long DRClearOverlay();
	long DRLoadOverlayFromFile(LPCTSTR strFileName);
	long DRGetSyncUserRange(long * pnMin, long * pnMax);
	long DRSetSyncUser(long nSyncPos);
	long DREnableSyncAuto(BOOL bNewVal);
	long DREnableSemiTransparentOverlay(BOOL bNewVal);
	long DREnableImageScaling();
	long DREnableScaling(BOOL bNewVal);
	long DRShowOverlay(BOOL bNewVal);
	long DRSetOverlayKeyColor(long nRed, long nGreen, long nBlue);
	long DRGetOverlayKeyColor(long * pnRed, long * pnGreen, long * pnBlue);
	long DRSetOverlayPos(long nX, long nY);
	long DRSetOverlaySize(long nWidth, long nHeight);
	long DRGetMaxOverlaySize(long * pnWidth, long * pnHeight);
	long DRReleaseOverlayDC();
	long DRGetOverlayDC(long* hDC);
    void DRSetStealFormat(long nColorFormat);
    long DRGetStealFormat();
    
    long GetTriggerDelay();
	
    void SetTriggerDelay(long nDelay);
    /* auto feature functions [19/07/2010] - jma */
    BOOL GetEnableAutoExposure();	
    void SetEnableAutoExposure(BOOL bNewVal);
    BOOL GetEnableAutoGain();
    void SetEnableAutoGain(BOOL bNewVal);
    BOOL GetEnableAutoFramerate();	
    void SetEnableAutoFramerate(BOOL bNewVal);
    long GetAutoBrightSpeed();
    void SetAutoBrightSpeed(long nSpeed);
    long GetAutoWBSpeed();
    void SetAutoWBSpeed(long nSpeed);
    long GetAutoBrightReference();
    void SetAutoBrightReference(long nReference);
    long GetAutoBrightMaxGain();
    void SetAutoBrightMaxGain(long nMaxGain);
    double GetAutoBrightMaxExp();
    void SetAutoBrightMaxExp(double dblMaxBrightness);
    long GetAutoWBROffset();	
    void SetAutoWBROffset(long nOffset);
    long GetAutoWBBOffset();	
    void SetAutoWBBOffset(long nOffset);
    long GetAutoWBMinGain();	
    void SetAutoWBMinGain(long nMinGain);
    long GetAutoWBMaxGain();	
    void SetAutoWBMaxGain(long nMaxGain);

    long GetSaturationU();
    long GetSaturationV();
    long SetSaturation(long nChromU, long nChromV);

    long AddHDRKneepoint(double dblX, double dblY);
    long SetAtHDRKneepoint(int nPos, double dblX, double dblY);
    long DeleteHDRKneepoint(int nPos);
    long SetHDRKneepointsDefault();

	long IsSupportedCPUIdleStatesBattery();
	long IsSupportedCPUIdleStatesMains();

	long IsSupportedOpenMP();
	long GetEnableOpenMPDefault();
	
	long IsSupportedInitialParameterset();

    long GetTriggerStatus();
		
	long GetAutoSkipframe();
	long GetAutoSkipframeMinRange();
	long GetAutoSkipframeMaxRange();
	void SetAutoSkipframe(int newVal);
	long GetAutoHysteresis();
	long GetAutoHysteresisMinRange();
	long GetAutoHysteresisMaxRange();
	void SetAutoHysteresis(int newVal);
};
