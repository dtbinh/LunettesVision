#include "MyTimer.h"

MyTimer::MyTimer(void)
{
	int index = 0;
	char filename[15];
	do {
		index++;
		sprintf(filename, "log/log%i.txt", index);
	} while(fileExists(filename));
	myfile.open (filename);
	reset();
}

MyTimer::~MyTimer(void)
{
	myfile.close();
}

void MyTimer::start() {
	currentTime = clock();
	currentState = OTHER;
}

void MyTimer::changeState(State s) {
	clock_t tmp = clock();
	time[currentState] += (tmp - currentTime);
	currentTime = tmp;
	currentState = s;
}

void MyTimer::stop() {
	clock_t tmp = clock();
	time.at(currentState) += (tmp - currentTime);
}

void MyTimer::reset() {
	for(std::map<State,double>::iterator it=time.begin() ; it!=time.end() ; it++)
	{
		time[it->first] = 0;
	}
}

void MyTimer::print() {
	for(std::map<State,double>::iterator it=time.begin() ; it!=time.end() ; it++)
	{
		switch(it->first) {
		case GET_CAM_FRAME: cout << "Recuperation de l'image depuis le flux de la camera : "; break;
		case REMAP_RESIZE: cout << "Temps de traitement de l'image (Remap ou Resize) : "; break;
		case OTHER: cout << "Autres temps de traitement : "; break;
		case MEMORY_TRANSIT: cout << "Echanges de donnees CPU/GPU : "; break;
		case DISPLAY: cout << "Affichage a l'ecran : "; break;
		case USER_INPUT: cout << "User input: "; break;
		case OSD: cout << "Superposition d'elements : "; break;
		case IMAGE_COPY: cout << "Copie de l'image : "; break;
		}
		cout << (it->second/CLOCKS_PER_SEC) << endl;
	}
}

void MyTimer::printInFile(int resX, int resY) {
	myfile << resX << "\t" << resY;
	for(std::map<State,double>::iterator it=time.begin() ; it!=time.end() ; it++)
	{
		myfile << "\t" << (it->second/CLOCKS_PER_SEC);
	}
	myfile << endl;
}

bool MyTimer::fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}