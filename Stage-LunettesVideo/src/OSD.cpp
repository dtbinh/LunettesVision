#include "OSD.h"
#include "LunettesVideo.h"

#define LEFT case 2162688: case 2424832:
#define RIGHT case 2228224: case 2555904:
#define UP case 58: case 13:

#define STEP 10
using namespace std;
using namespace cv;

OSD::OSD(LunettesVideo* lunettes)
{
	///// Inits /////

	displayProfile = false;
	displayOsd = false;
	parent = lunettes;
	currentIndex = 0;

	///// Load the background /////
	
	backgroud = imread("res/osd_background.png", 1); //1 = CV_LOAD_IMAGE_COLOR (temporaire)
	if(!backgroud.data) {
		std::cout << "ERROR - could not find OSD background" << std::endl;
		return;
	}
	cv::resize(backgroud,backgroud,cv::Size((WIDTH/2)-100,100),0,0,cv::INTER_LINEAR);

	///// Init the OSD nodes /////

	root = new OsdNode("");

	root->addChild(new OsdNode("Inverser side-by-side",OsdNode::SWAP));
	root->addChild(new OsdNode("Selection de la zone",OsdNode::CHANGE_ZONE));
	OsdNode *zone = root->addChild(new OsdNode("Position de la zone"));
	OsdNode *remap = root->addChild(new OsdNode("Options de remap"));
	OsdNode *crop = root->addChild(new OsdNode("Options de rognage"));
	OsdNode *hdr = root->addChild(new OsdNode("Mode HDR"));
	root->addChild(new OsdNode("Cacher la zone active",OsdNode::HIDE_ZONE));
	root->addChild(new OsdNode("Sauvegarder le profil",OsdNode::SAVE));
	root->addChild(new OsdNode("Quitter le programme",OsdNode::QUIT));

	remap->addChild(new OsdNode("On/Off Remap",OsdNode::SWAP_REMAP));
	remap->addChild(new OsdNode("Zoom zone centrale",OsdNode::ZOOM_REMAP));
	remap->addChild(new OsdNode("Dezoom zone centrale",OsdNode::DEZOOM_REMAP));
	remap->addChild(new OsdNode("Retour",OsdNode::BACK));

	zone->addChild(new OsdNode("Augmenter hauteur zone",OsdNode::ZONE_HEIGHT_INC));
	zone->addChild(new OsdNode("Diminuer hauteur zone",OsdNode::ZONE_HEIGHT_DEC));
	zone->addChild(new OsdNode("Augmenter largeur zone",OsdNode::ZONE_WIDTH_INC));
	zone->addChild(new OsdNode("Diminuer largeur zone",OsdNode::ZONE_WIDTH_DEC));
	zone->addChild(new OsdNode("Decaler zone haut",OsdNode::ZONE_Y_DEC));
	zone->addChild(new OsdNode("Decaler zone bas",OsdNode::ZONE_Y_INC));
	zone->addChild(new OsdNode("Decaler zone gauche",OsdNode::ZONE_X_DEC));
	zone->addChild(new OsdNode("Decaler zone droit",OsdNode::ZONE_X_INC));
	zone->addChild(new OsdNode("Retour",OsdNode::BACK));

	crop->addChild(new OsdNode("On/Off Crop",OsdNode::SWAP_CROP));
	crop->addChild(new OsdNode("Retour",OsdNode::BACK));

	hdr->addChild(new OsdNode("On/Off HDR",OsdNode::SWAP_HDR));
	OsdNode *hdrExpo = hdr->addChild(new OsdNode("Regler exposition"));
	hdr->addChild(new OsdNode("Retour",OsdNode::BACK));

	hdrExpo->addChild(new OsdNode("Retour",OsdNode::BACK));

	current = root;
}

OSD::~OSD(void)
{
}

void OSD::addInput(int key)
{
	if(key == -1) return;

	//cout << "New key ! " << key << endl;
	//les valeurs numériques sont utilisé pour linux, les noms des touches (LEFT, RIGHT, ENTER) sont pour windows
	switch(key) {
	case 2424832: { //LEFT // 1113937 for linux
			if(displayOsd) {
				if(currentIndex>0) currentIndex--;
				showOSD();
			} else {
				// Change profile
				parent->switchProfile(-1);
			}
		}
		break;
	case 2555904: { //RIGHT  // 1113939 for linux
			if(displayOsd) {
				if(currentIndex<current->children.size()-1) currentIndex++;
					showOSD();
			} else {
				// Change profile
				parent->switchProfile(1);
			}
		}
		break;
		case 13 : { //ENTER // 1048586 for linux
			if(displayOsd) {
				applyEffect();
			}
			else showOSD();
				if(displayOsd) {
					showOSD();
			}
		}
		break;
	}
}

void OSD::showOSD()
{
	time(&startTime);
	displayOsd = true;
	displayProfile = false;
}

bool OSD::isDisplay()
{
	time(&now);
	if(difftime(now,startTime) > ON_SCREEN_TIME) exitOSD();
	return displayOsd;
}

void OSD::exitOSD()
{
	displayProfile = false;
	displayOsd = false;
	current = root;
	currentIndex = 0;
}

void OSD::blendOSD(Mat& m)
{
	if(isDisplay())
	{
		// display the main OSD menu
		Mat m_roi = m(cv::Rect(50,HEIGHT-150,(WIDTH/2)-100,100));
		backgroud.copyTo(m_roi);
		cv::putText(m,current->children[currentIndex]->text,cv::Point(70,HEIGHT-60),FONT_HERSHEY_PLAIN,2.5,cv::Scalar(255,255,255),2);
		//cout << current->children[currentIndex]->text << endl;
		
	}
	else if(displayProfile)
	{
		// display only the current profile name
		putText(m,parent->currentProfile->name,cv::Point(20,60),FONT_HERSHEY_PLAIN,3,cv::Scalar(0,0,255),2);
	}
}

void OSD::applyEffect()
{
	switch(current->children[currentIndex]->effect) {
	case OsdNode::NONE:
		current = current->children[currentIndex];
		currentIndex = 0;
		break;
	case OsdNode::BACK:
		if(current == root) exitOSD();
		else {
			currentIndex = 0;
			current = current->parent;
		}
		break;
	case OsdNode::SWAP:
		parent->swapProfileZones();
		break;
	case OsdNode::SWAP_REMAP:
		parent->switchRemap();
		break;
	case OsdNode::SWAP_CROP:
		parent->switchCrop();
		break;
	case OsdNode::SWAP_HDR:
		parent->switchHdr();
		break;
	case OsdNode::QUIT:
		parent->exit();
		break;
	case OsdNode::HIDE_ZONE:
		parent->hideCurrentArea();
		break;
	case OsdNode::CHANGE_ZONE:
		parent->selectNextArea();
		break;

	case OsdNode::ZONE_HEIGHT_INC:
		parent->setCurrentRectHeight(parent->getCurrentRectHeight() + STEP);
		break;
	case OsdNode::ZONE_HEIGHT_DEC:
		parent->setCurrentRectHeight(parent->getCurrentRectHeight() - STEP);
		break;
	case OsdNode::ZONE_WIDTH_INC:
		parent->setCurrentRectWidth(parent->getCurrentRectWidth() + STEP);
		break;
	case OsdNode::ZONE_WIDTH_DEC:
		parent->setCurrentRectWidth(parent->getCurrentRectWidth() - STEP);
		break;
	case OsdNode::ZONE_X_INC:
		parent->setCurrentRectX(parent->getCurrentRectX() + STEP);
		break;
	case OsdNode::ZONE_X_DEC:
		parent->setCurrentRectX(parent->getCurrentRectX() - STEP);
		break;
	case OsdNode::ZONE_Y_INC:
		parent->setCurrentRectY(parent->getCurrentRectY() + STEP);
		break;
	case OsdNode::ZONE_Y_DEC:
		parent->setCurrentRectY(parent->getCurrentRectY() - STEP);
		break;
	case OsdNode::ZOOM_REMAP:
		parent->zoomInCurrentZone(0.1);
		break;
	case OsdNode::DEZOOM_REMAP:
		parent->zoomOutCurrentZone(0.1);
		break;
	case OsdNode::SAVE:
		parent->saveCurrentProfile();
		break;
	}
}
