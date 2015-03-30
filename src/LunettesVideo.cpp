#include "LunettesVideo.h"
#include "OSD.h"


mutex frameMutex;
MyTimer * myTimer;
FPS *fps;

void remapThread(Area* r);

//////////////////////////////////////////////////////////////////////////////
/// CONSTRUCTEUR - DESTRUCTEUR
//////////////////////////////////////////////////////////////////////////////

LunettesVideo::LunettesVideo(void)
{
	initialized = false;
	cout << "Initialisation du programme - creation de l'objet LunettesVideo" << endl;

	loadProfiles();
	if(profiles.size() == 0) { cin.get(); return; }
	myTimer = new MyTimer();
	osd = new OSD(this);
	fps = new FPS("Affichage");
	showEffects = true;
	currentAreaRectangle = 0;
	showSelectArea = false;
	needClose = false;
	needHdr = false;
	changeExposure = false;
	currentProfile = NULL;

	// Initialisation
	///////////////////////////

	currentProfileIndex = 0;
	cameraCount = 1;
	resX = WIDTH;
	resY = HEIGHT;
	
	INT nNumCam;
	if( is_GetNumberOfCameras( &nNumCam ) == IS_SUCCESS) {
		if( nNumCam >= 1 ) {
			cout << "Nombre de cameras = " << nNumCam << endl;
			// Initialize list with suitable size
			pucl = (UEYE_CAMERA_LIST*) new BYTE [sizeof (DWORD) + nNumCam * sizeof (UEYE_CAMERA_INFO)];
			pucl->dwCount = nNumCam;
		}
	}
	initProfilCameras();
	initialized = true;
}

LunettesVideo::~LunettesVideo(void)
{
	cv::destroyAllWindows();
}

//////////////////////////////////////////////////////////////////////////////
/// INIT FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

///// INIT CAMERAS /////

vector<int> camIds;

bool LunettesVideo::initProfilCameras()
{
	bool errorLoadingCameras = false;
	std::vector<int> camIds;
	int camIndex;
	cout <<"taille liste de profil = "<<profiles.size()<<endl;
	///// Parcours des profiles /////       //A quoi �a sert si on ne demande de base qu'un seul profil ???
	/*for(int p=0; p<profiles.size() ;p++)  
	{
		///// Parcours des Aires /////
		std::vector<Area*> areas = profiles.at(p)->listArea;
		cout <<"passage dans le profil "<<p<<endl;
		for(int a=0; a<areas.size() ;a++)
		{
			int camIndex = areas.at(a)->camIndex;
			if(camIndex != -1) {
				if(camList.find(areas.at(a)->camIndex) == camList.end()) {
					///// On a trouv� une cam�ra non initialis�e /////
					cout << "Camera " << camIndex << " : ";
					camIds.push_back(camIndex);

					camList[camIndex] = new Camera(camIndex,getCamRes(camIndex));

					if(camList.at(camIndex)->active == true){					 
						cout << "OK" << endl;
					}
					else 
						cout << "ERROR" << endl;
				}

				if(!camList.at(camIndex)->active)
					errorLoadingCameras = true;
				areas.at(a)->camera = camList.at(areas.at(a)->camIndex); //attribution de la camera	� une area
			}
		}
	}*/
	//Version Aur�lien : tout l'initialisation de la cam�ra dans une seule fonction
	///// Parcours des Aires du Profil/////
	std::vector<Area*> areas = profiles.at(currentProfileIndex)->listArea;
	for(int a=0; a<areas.size() ;a++)
	{
		int camIndex = areas.at(a)->camIndex;
		if(camIndex != -1) {
			if(camList.find(camIndex) == camList.end()) {
				///// On a trouv� une cam�ra non initialis�e /////
				cout << "Camera " << camIndex << " : ";
				camIds.push_back(camIndex);

				camList[camIndex] = new Camera(camIndex);

				if(camList.at(camIndex)->active == true){					 
					cout << "OK" << endl;
				}
				else 
					cout << "ERROR" << endl;
			}

			if(!camList.at(camIndex)->active)
				errorLoadingCameras = true;
				
			// Connexion to IDS camera
			// Sample from IDS documentation
			// http://www.prolinx.co.jp/supplier/IDS/uEye_Manual_En/index.html?is_getcameralist.html
			///////////////////////////////////////////////////////////////////////////////////////////	
			if (is_GetCameraList(pucl) == IS_SUCCESS) {
				
					camList[camIndex]->cameraID = pucl->uci[camIndex].dwCameraID; 
					int ret = is_ParameterSet(camList[camIndex]->hCam, IS_PARAMETERSET_CMD_LOAD_EEPROM, NULL, 0);
					cout << "Camera no " << camList[camIndex]->cameraID<< " - loading code (0 = OK) : " << ret << endl;
					cout <<"HIDS de la camera = "<<camList[camIndex]->hCam<<endl;
				
			}
			areas.at(a)->camera = camList.at(camIndex); //attribution de la camera	� une area
		}
	}
	return !errorLoadingCameras;
}

bool LunettesVideo::setWindowsParams(void)
{
	cv::namedWindow("camera",cv::WINDOW_NORMAL);
	cv::setWindowProperty("camera", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

	//cv::namedWindow("camera", CV_WINDOW_AUTOSIZE);
	//cv::moveWindow("camera",WINDOWCOORDSX,0);
	return true;
}

//////////////////////////////////////////////////////////////////////////////
/// THREADS CONTROL
//////////////////////////////////////////////////////////////////////////////

void LunettesVideo::startRemapThreads()
{
	for(int i = 0;i<currentProfile->listArea.size();i++) {
		currentProfile->listArea.at(i)->startThread();
	}
}

void LunettesVideo::stopRemapThreads()
{
	for(int i = 0;i<currentProfile->listArea.size();i++) {
		currentProfile->listArea.at(i)->stopThread();
	}
}

//////////////////////////////////////////////////////////////////////////////
/// MAIN LOOP
//////////////////////////////////////////////////////////////////////////////

void LunettesVideo::run() {
	///// DISPLAY THREAD /////
	
	if(!initialized) return;
	cout << "------------ Video running ! ------------" << endl;

	///// INIT /////
	
	int frame = 0;
	int key = 0;
	double pParam;
	long pIntr, pActIntr;
	Mat finalFrame(resY, resX,CV_8UC3,Scalar(0,0,0));
	setWindowsParams();

	if(!switchProfile(currentProfileIndex)) {
		cout << "Fin du Programme" <<endl;
		return;
	}
	/// START LOOP ///
	//////////////////////////////////////////////////////////

	myTimer->start();
	cout<<"Debut Boucle Acquisition"<<endl;
	//cout <<" currentProfile->listArea.size() = " << currentProfile->listArea.size() << endl;
	Sleep(30000); ////////////////////artifice, � changer ////////////////
	do {
		// Benchmark stuff
		if(frame>FRAMES_COUNT && TEST_MODE) break;
		frame ++;
		
		// Blend Areas
		///////////////////////////////////////////
		
		currentProfileMutex.lock();
		for(int i = 0;i<currentProfile->listArea.size();i++) {
			Area* r = currentProfile->listArea.at(i);
			if(!r->hidden) {
				// Need the read access

				// Wait for a frame (at the beginning)
				while(r->currentFrame.empty() && r->type == Area::CAMERA) {
					cout<<"useless"<<endl;
					Sleep(100);
				}				
				if (changeExposure){
					if (!needHdr) {
						pParam = 10;					
						INT nMask = 0;
						is_AOI(r->camera->hCam, IS_AOI_SEQUENCE_SET_ENABLE, (void*)&nMask, sizeof(nMask)); //Stop Sequence HDR
						is_Exposure(r->camera->hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*) &pParam, sizeof (pParam));
						int nMode = IS_DEVICE_FEATURE_CAP_SHUTTER_MODE_ROLLING;
						is_DeviceFeature(r->camera->hCam, IS_DEVICE_FEATURE_CMD_GET_SHUTTER_MODE, (void*) &nMode, sizeof(nMode));
					}
					else{	
						initSequenceAOI(r);
					}	
				}
				r->frameMutex.lock();
				// Blend !
				if(r->type == Area::CAMERA) {
					//Copy in the DisplayRect on finale frame
					//cout<<"jusqu'ici"<<endl;
					
					r->currentFrame.download(finalFrame(r->getDisplayRect()));
				} else {
					cv::rectangle(finalFrame,r->displayZone,r->color,cv::FILLED);
				}
				
				// Unlock the mutex
				r->frameMutex.unlock();
			}
			
		}
		changeExposure = false;
		// On Screen Display features
		///////////////////////////////////////////
		osd->blendOSD(finalFrame);
		if(osd->isDisplay()) 
			blendAreasZones(finalFrame);
		std::ostringstream oss117;
		oss117<<frame;
		//indicateur HDR
		if(needHdr){
				cv::putText(finalFrame,"HDR On", cv::Point(resX-150,50),1,2,cv::Scalar(0,255,0),2);
				//imwrite("results/image"+oss117.str()+".png", finalFrame);
		}else{
				cv::putText(finalFrame,"HDR Off", cv::Point(resX-150,50),1,2,cv::Scalar(0,0,255),2);
		}
		currentProfileMutex.unlock();
		imshow("camera",finalFrame);
		// Refresh screen !
		//finalFrame.zeros(resY, resX,CV_8UC3);
		myTimer->changeState(MyTimer::USER_INPUT);
		key = waitKey(1);
		myTimer->changeState(MyTimer::OTHER);
		osd->addInput(key);
	} while (key != 'q' && key != 'Q' && !needClose);

	//////////////////////////////////////////////////////////

	///// EXIT THREADS /////

	stopRemapThreads();
	cout << "------------ End of the Video ------------" << endl << "Frames count : " << frame << endl;

	//Benchmark stuff
	myTimer->stop();
	//myTimer->print();
	//	myTimer->printInFile(resX, resY);
	myTimer->reset();
}

//////////////////////////////////////////////////////////////////////////////
/// OTHER FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

void LunettesVideo::blendAreasZones(Mat &img)
{
	for(int i = 0;i<currentProfile->listArea.size();i++)
	{
		Area* r = currentProfile->listArea.at(i);
		Scalar s1(0,0,255), s2(255,0,0);

		if(i == (int)(currentAreaRectangle/2))
		{
			if(currentAreaRectangle%2 == 0)
				s1 = Scalar(0,255,255);
			else
				s2 = Scalar(0,255,255);

			rectangle(img,r->getCentralZoneRect(), s1,2);
			rectangle(img,r->getCameraCropRect(), s2,2);
		}
	}
}

void LunettesVideo::initSequenceAOI(Area *r){
	
	INT nMask = 0;
	
	//Parameters Initialization
	AOI_SEQUENCE_PARAMS Param;
	
	// Set parameters of AOI 1
	Param.s32AOIIndex = IS_AOI_SEQUENCE_INDEX_AOI_1;
	Param.s32NumberOfCycleRepetitions = 1;
	Param.s32X = 0;
	Param.s32Y = 0;
	Param.dblExposure = 8;
	Param.s32DetachImageParameters = 1; //changes of Params does not affect others AOI
	is_AOI(r->camera->hCam, IS_AOI_SEQUENCE_SET_PARAMS, (void*)&Param, sizeof(Param));
	
	
	Param.s32AOIIndex = IS_AOI_SEQUENCE_INDEX_AOI_2;
	Param.s32NumberOfCycleRepetitions = 1;
	Param.dblExposure = 12;
	Param.s32DetachImageParameters = 1; //changes of Params does not affect others AOI
	is_AOI(r->camera->hCam, IS_AOI_SEQUENCE_SET_PARAMS, (void*)&Param, sizeof(Param));
	
	nMask = IS_AOI_SEQUENCE_INDEX_AOI_1 |
                IS_AOI_SEQUENCE_INDEX_AOI_2;

    // enable sequence mode
    is_AOI(r->camera->hCam, IS_AOI_SEQUENCE_SET_ENABLE, (void*)&nMask, sizeof(nMask));
	
}

void LunettesVideo::selectNextArea()
{
	if(currentAreaRectangle+1 < currentProfile->listArea.size() * 2)
		currentAreaRectangle++;
	else currentAreaRectangle = 0;
}

void LunettesVideo::selectPrevArea()
{
	if(currentAreaRectangle > 0)
		currentAreaRectangle--;
}

void LunettesVideo::resetSelectArea()
{
	currentAreaRectangle = 0;
}

void LunettesVideo::switchCrop() {
	for(int i = 0;i<currentProfile->listArea.size();i++) {
		Area* r = currentProfile->listArea.at(i);
		r->switchCrop();
	}
}

void LunettesVideo::switchHdr() {
	for(int i = 0;i<currentProfile->listArea.size();i++) {
		Area* r = currentProfile->listArea.at(i);
		r->switchHdr();
	}
	needHdr = !needHdr;
	changeExposure = true;
}

void LunettesVideo::switchRemap() {
	for(int i = 0;i<currentProfile->listArea.size();i++) {
		Area* r = currentProfile->listArea.at(i);
		r->switchRemap();
	}
}

void LunettesVideo::hideCurrentArea() {
	getCurrentArea()->HideAndShow();
}

void LunettesVideo::swapProfileZones()
{
	for(int i = 0;i<currentProfile->listArea.size();i++)
	{
		Area *area = currentProfile->listArea[i];
		area->displayZone.x = (WIDTH -area->displayZone.x) - area->displayZone.width;
	}
}

void LunettesVideo::exit()
{
	needClose = true;
}

//////////////////////////////////////////////////////////////////////////////
/// PROFILE SWITCH
//////////////////////////////////////////////////////////////////////////////

bool LunettesVideo::switchProfile(int i) {
	cout<<"indexProfil = "<<i<<endl;
	if(currentProfileIndex + i >= 0 && currentProfileIndex + i < profiles.size())
	{
		if(!isCameraAviable(profiles[currentProfileIndex+i])) {
			cout << "La camera "<<currentProfileIndex+i<<" utilisee dans ce profil n'est pas disponible" << endl;
			return false;
		}

		std::cout << "####### SWITCH PROFILE #######" << endl;

		currentProfileMutex.lock();
		if(currentProfile != NULL){
			 stopRemapThreads();
		}
		currentProfileIndex += i;
		currentProfile = profiles[currentProfileIndex];
		startRemapThreads();          
		currentProfileMutex.unlock();
		return true;
	}
	return false;
}

bool LunettesVideo::isCameraAviable(Profile* p)
{
	std::vector<Area*> areas = p->listArea;
	
	for(int a=0; a<areas.size() ;a++)
	{
		int camIndex = areas.at(a)->camIndex;
		if(camList[camIndex]->active) 
			
		if(camIndex == -1) continue;
		if(camList.find(camIndex) == camList.end()){
			cout << "fin de liste" <<endl;
			return false;
		}
		if(!camList[camIndex]->active) {
			cout<<"camera non active"<<endl;
			return false;
		}
	}
	return true;
}

// save the currents params into the current profile XML file
void LunettesVideo::saveCurrentProfile()
{
	std::cout << "#######  SAVE PROFILE  #######" << endl;

	tinyxml2::XMLDocument doc;

	tinyxml2::XMLElement *root = doc.NewElement("profile");
	doc.InsertEndChild(root);

	tinyxml2::XMLElement *name = doc.NewElement("name");
	name->InsertEndChild(doc.NewText(currentProfile->name.c_str()));
	root->InsertEndChild(name);

	tinyxml2::XMLElement *areaList = doc.NewElement("display");
	root->InsertEndChild(areaList);

	// Parcours des aires du profil
	for(int i = 0;i<currentProfile->listArea.size();i++)
	{
		Area* r = currentProfile->listArea.at(i);
		tinyxml2::XMLElement *area = doc.NewElement("area");
		areaList->InsertEndChild(area);

		if(r->type == r->CAMERA) {
			area->SetAttribute("Type","camera");
			area->SetAttribute("Id",r->camIndex);
		} else if(r->type == r->COLOR)
			area->SetAttribute("Type","color");

		tinyxml2::XMLElement *cameraRoi = doc.NewElement("cameraROI");
		area->InsertEndChild(cameraRoi);
		cameraRoi->SetAttribute("Enable",r->needCrop);

		if(r->needCrop)
		{
			cameraRoi->SetAttribute("X",r->cameraROI.x);
			cameraRoi->SetAttribute("Y",r->cameraROI.y);
			cameraRoi->SetAttribute("Width",r->cameraROI.width);
			cameraRoi->SetAttribute("Height",r->cameraROI.height);
		}

		tinyxml2::XMLElement *remap = doc.NewElement("remap");
		area->InsertEndChild(remap);
		remap->SetAttribute("Enable",r->needRemap);

		if(r->needRemap)
		{
			remap->SetAttribute("X",r->centralZone.x);
			remap->SetAttribute("Y",r->centralZone.y);
			remap->SetAttribute("Width",r->centralZone.width);
			remap->SetAttribute("Height",r->centralZone.height);
			remap->SetAttribute("Zoom",r->zoom);
		}

		tinyxml2::XMLElement *displayZone = doc.NewElement("displayZone");
		area->InsertEndChild(displayZone);

		displayZone->SetAttribute("X",r->displayZone.x);
		displayZone->SetAttribute("Y",r->displayZone.y);
		displayZone->SetAttribute("Width",r->displayZone.width);
		displayZone->SetAttribute("Height",r->displayZone.height);
	}

	char filename[20];
	sprintf(filename, "res/profile%i.xml", currentProfileIndex);
	doc.SaveFile( filename );
}

//////////////////////////////////////////////////////////////////////////////
/// GET
//////////////////////////////////////////////////////////////////////////////

Area* LunettesVideo::getCurrentArea() {
	return currentProfile->listArea[currentAreaRectangle/2];
}

int LunettesVideo::getCurrentRectWidth()
{
	switch(currentAreaRectangle%2) {
	case 0:
		return getCurrentArea()->getWidth(Area::REMAP_ZONE);
	case 1:
	default:
		return getCurrentArea()->getWidth(Area::CROP_ZONE);
	}
}

int LunettesVideo::getCurrentRectHeight()
{
	switch(currentAreaRectangle%2) {
	case 0:
		return getCurrentArea()->getHeight(Area::REMAP_ZONE);
	case 1:
	default:
		return getCurrentArea()->getHeight(Area::CROP_ZONE);
	}
}

int LunettesVideo::getCurrentRectX()
{
	switch(currentAreaRectangle%2) {
	case 0:
		return getCurrentArea()->getX(Area::REMAP_ZONE);
	case 1:
	default:
		return getCurrentArea()->getX(Area::CROP_ZONE);
	}
}

int LunettesVideo::getCurrentRectY()
{
	switch(currentAreaRectangle%2) {
	case 0:
		return getCurrentArea()->getY(Area::REMAP_ZONE);
	case 1:
	default:
		return getCurrentArea()->getY(Area::CROP_ZONE);
	}
}

//////////////////////////////////////////////////////////////////////////////
/// SET
//////////////////////////////////////////////////////////////////////////////

void LunettesVideo::setCurrentRectWidth(int i)
{
	switch(currentAreaRectangle%2) {
	case 0:
		getCurrentArea()->setWidth(Area::REMAP_ZONE,i);
		break;
	case 1:
		getCurrentArea()->setWidth(Area::CROP_ZONE,i);
		break;
	}
}

void LunettesVideo::setCurrentRectHeight(int i)
{
	switch(currentAreaRectangle%2) {
	case 0:
		getCurrentArea()->setHeight(Area::REMAP_ZONE,i);
		break;
	case 1:
		getCurrentArea()->setHeight(Area::CROP_ZONE,i);
		break;
	}
}

void LunettesVideo::setCurrentRectX(int i)
{
	switch(currentAreaRectangle%2) {
	case 0:
		getCurrentArea()->setX(Area::REMAP_ZONE,i);
		break;
	case 1:
		getCurrentArea()->setX(Area::CROP_ZONE,i);
		break;
	}
}

void LunettesVideo::setCurrentRectY(int i)
{
	switch(currentAreaRectangle%2) {
	case 0:
		getCurrentArea()->setY(Area::REMAP_ZONE,i);
		break;
	case 1:
		getCurrentArea()->setY(Area::CROP_ZONE,i);
		break;
	}
}

bool LunettesVideo::isInitialized() {
	return initialized;
}

void LunettesVideo::zoomInCurrentZone(double d)
{
	Area* r = getCurrentArea();
	r->zoom -= d;
	r->invalidate();
}

void LunettesVideo::zoomOutCurrentZone(double d){
	Area* r = getCurrentArea();
	r->zoom += d;
	r->invalidate();
}

//////////////////////////////////////////////////////////////////////////////
/// XML LOADING
//////////////////////////////////////////////////////////////////////////////

///// EXCEPTION /////

class myexception: public exception
{
	virtual const char* what() const throw()
	{
		return "NullPointerException";
	}
} npex;

///// LOADING /////

void LunettesVideo::loadXMLParams()
{
}

Size LunettesVideo::getCamRes(int idcam)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("res/params.xml");

	//On a pas trouv� le ficher de configuration
	if(doc.ErrorID() != 0) return DEFAULT_SIZE;

	for (tinyxml2::XMLElement* camNode = doc.FirstChildElement("Camera"); camNode != NULL; camNode = camNode->NextSiblingElement("Camera"))
	{
		if(camNode->IntAttribute("id") == idcam) {
			// On a trouv� la cam�ra correspondante
			try {
				return cv::Size(camNode->IntAttribute("resX"),camNode->IntAttribute("resY"));
			} catch (...) {
				return DEFAULT_SIZE;
			}
		}
	}

	//On a pas trouv� de cam�ra avec le bon ID
	return DEFAULT_SIZE;
}

//////////////////////////////////////////////////////////////////////////////
/// XML LOADING - PROFILES
//////////////////////////////////////////////////////////////////////////////

void LunettesVideo::loadProfiles() {
	int profileIndex = 0;
	tinyxml2::XMLDocument doc;
	char filename[20];

	///// LOOP /////

	do {
		// Open xml file
		sprintf(filename, "res/profile%i.xml", profileIndex);
		doc.LoadFile(filename);
		cout << "Trying to open profile "<< profileIndex << " (" << filename << ")" << endl;

		if(doc.ErrorID() != 0) break;

		// Load xml file
		////////////////////////////////////////////////////////////////////////////
		Profile* profile = new Profile();
		try{
			tinyxml2::XMLElement* root = doc.FirstChildElement();
			if(root == NULL) throw npex;

			tinyxml2::XMLElement* nameNode = root->FirstChildElement("name");
			if(nameNode == NULL) throw npex;
			profile->name = nameNode->GetText();

			tinyxml2::XMLElement* displayNode = root->FirstChildElement("display");
			if(displayNode == NULL) throw npex;

			Area* area;

			// Load areas
			//////////////////////////////////////////////////

			for (tinyxml2::XMLElement* areaNode = displayNode->FirstChildElement("area"); areaNode != NULL; areaNode = areaNode->NextSiblingElement("area"))
			{
				try {
					area = new Area();
					string type = areaNode->Attribute("Type");

					if(type == "color")
					{
						// COLOR
						//////////////////////////////////////////////////////////

						area->type = Area::COLOR;

						tinyxml2::XMLElement* colorNode = areaNode->FirstChildElement("color");
						if(colorNode == NULL) throw npex;

						int r,v,b;
						colorNode->QueryIntAttribute("R",&r);
						colorNode->QueryIntAttribute("G",&v);
						colorNode->QueryIntAttribute("B",&b);
						area->color = cv::Scalar(b,v,r);

						tinyxml2::XMLElement* displayZone = areaNode->FirstChildElement("displayZone");
						if(displayZone != NULL) {
							displayZone->QueryIntAttribute("Width",&area->displayZone.width);
							displayZone->QueryIntAttribute("Height",&area->displayZone.height);
							displayZone->QueryIntAttribute("X",&area->displayZone.x);
							displayZone->QueryIntAttribute("Y",&area->displayZone.y);

						} else throw npex;

					} else if (type == "camera")
					{
						// CAMERA
						//////////////////////////////////////////////////////////

						area->type = Area::CAMERA;
						areaNode->QueryIntAttribute("Id",&area->camIndex);

						tinyxml2::XMLElement* cameraROI = areaNode->FirstChildElement("cameraROI");
						if(cameraROI != NULL) {
							cameraROI->QueryBoolAttribute("Enable",&area->baseCrop);
							if(area->baseCrop) {
								cameraROI->QueryIntAttribute("Width",&area->cameraROI.width);
								cameraROI->QueryIntAttribute("Height",&area->cameraROI.height);
								cameraROI->QueryIntAttribute("X",&area->cameraROI.x);
								cameraROI->QueryIntAttribute("Y",&area->cameraROI.y);
								area->needCrop = area->baseCrop;
							}
						}

						tinyxml2::XMLElement* centralZone = areaNode->FirstChildElement("remap");
						if(centralZone != NULL) {
							centralZone->QueryBoolAttribute("Enable",&area->baseRemap);
							if(area->baseRemap) {
								centralZone->QueryIntAttribute("Width",&area->centralZone.width);
								centralZone->QueryIntAttribute("Height",&area->centralZone.height);
								centralZone->QueryIntAttribute("X",&area->centralZone.x);
								centralZone->QueryIntAttribute("Y",&area->centralZone.y);
								centralZone->QueryDoubleAttribute("Zoom",&area->zoom);
								area->needRemap = area->baseRemap;
							}
						}

						tinyxml2::XMLElement* displayZone = areaNode->FirstChildElement("displayZone");
						if(displayZone != NULL) {
							displayZone->QueryIntAttribute("Width",&area->displayZone.width);
							displayZone->QueryIntAttribute("Height",&area->displayZone.height);
							displayZone->QueryIntAttribute("X",&area->displayZone.x);
							displayZone->QueryIntAttribute("Y",&area->displayZone.y);

						} else throw npex;
					}

					profile->listArea.push_back(area);

				} catch (exception& e) {
					delete area;
					cout << "ERROR WHEN LOADING PROFILE" << e.what() << endl;
				}
			}

			profiles.push_back(profile);
		} catch(...) {
			cout << "Cannot load view profile " << profileIndex << endl;
		}
		profileIndex++;
	} while(doc.ErrorID() == 0);
	cout << "Successfully loaded  " << profiles.size() << " profiles" << endl;
}
