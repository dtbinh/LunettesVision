#include "Matrix.h"

/*
	La classe fonction sert simplement a étendre une fonction progressive
	(par exemple, x² sur l'intervalle [0,1]), sur un intervalle plus grand
	et renseigné à la fonction.
	Sert a faire varier les coeficients de maniere progressive.
*/

class myFunction : public Function {
	float getX(float x) {
		int signe = (x < 0 ? -1 : 1);
		return pow(x/width,2) * height;
		return x;
	}

	float getY(float y) {
		return y;
	}
};

Matrix::Matrix() {
	initialized = false;
}

Matrix::Matrix(cv::Size input, cv::Size output)
{
	inputSize = input;
	outputSize = output;
	func = new myFunction();
	mX.create(outputSize, CV_32FC1);
	mY.create(outputSize, CV_32FC1);

	center = new cv::Point(outputSize.width/2, outputSize.height/2);
	width = outputSize.width/2;
	height = outputSize.height/2;
	// IMPLEMENTATION DU ZOOM DIFFEREND POUR LES AXES X et Y :
	// Il suffit d'avoir deux valeures de zoom
	// et de les utiliser pour le calcul du coef de l'axe X et de l'axe Y
	zoomCenter = 0.7;

	func->setSize(output.width, output.height);
}

Matrix::~Matrix(void)
{
	mX.release();
	mY.release();
}

void Matrix::invalidate() {
	// recalculate the matrix from the function !
	std::cout << "Regenerate Matrix "<< std::endl;
	if(func == NULL)  {
		std::cout << "La fonction generatrice est NULL"<< std::endl;
		func = new Function();
	}

	// parcours de l'image de destination
	for( int j = 0; j < outputSize.height; j++ ) {
		for( int i = 0; i < outputSize.width; i++ ) {

			// De base, coefs égaux à un, aucune transformation
			float coefX = 1, coefY = 1;

			// Ces coefs représentent le rapport entre la distance pixel - axe (X et Y)
			// de l'image de base par rapport à l'image transformée.
			// Par exemple, un coefX de 0.5 signifie que la distance entre l'axe des
			// ordonnées (au centre de l'image) et le pixel, est multipliée par 2 lors du passage
			// de l'image de base à l'image transformée
			// Un coef de 0.5 représente donc dans notre cas un zoom x2


			// Si on est dans la zone centrale (axe des ordonnées)
			if(abs(center->x - i) < width/2) {
				// Le coef est égal au zoom
				coefX = zoomCenter;
			} else {
				// Sinon l'objectif est de faire varier ce coef progressivement
				// pour qu'il soit egal a 1 au niveau des bordures de l'image
				// (Pour ne pas avoir de pertes d'informations sur les cotés de l'image)

				float x, y;

				y = (1-zoomCenter);
				// y représente la valeur du coef manquante pour obtenir 1

				if(i>center->x) {
					// si on est dans la partie droite de l'image
					x = outputSize.width - (center->x + width/2);
					// x représente la largeur de la zone latérale déformée

					func->setSize(x,y);
					// La fonction doit donc varier entre 0 et y, sur l'intervalle [0,x]

					coefX = zoomCenter + func->getX(i-(center->x+width/2));
					// quand le pixel est sur le bord, alors x vaut toute la largeur de la bande
					// La fonction renvoie donc y (1- zoomCenter), vu précédement
					// finalement, coefX = zoomCenter + (1- zoomCenter) et donc coefX est a 1.
				} else {
					// Sinon, dans la partie gauche
					// Meme chose, simplement une affaire de signe.
					x = (center->x - width/2);
					func->setSize(x,y);
					coefX = zoomCenter + func->getX((center->x-width/2) - i);
				}
			}

			// Meme chose pour l'axe Y pour la transformation
			if(abs(center->y - j) < height/2) {
				coefY = zoomCenter;
			} else {
				float x, y;
				y = (1-zoomCenter);

				if(j>center->y) {
					x = outputSize.height - (center->y + height/2);
					func->setSize(x,y);
					coefY = zoomCenter + func->getX(j-(center->y+height/2));
				} else {
					x = (center->y - height/2);
					func->setSize(x,y);
					coefY = zoomCenter + func->getX((center->y-height/2) - j);
				}
			}

			mX.at<float>(j,i) = (center->x + (i - center->x) * coefX) * (float)inputSize.width / outputSize.width;
			mY.at<float>(j,i) = (center->y + (j - center->y) * coefY) * (float)inputSize.height / outputSize.height;
		}
	}
	mXgpu.upload(mX);
	mYgpu.upload(mY);
}

float Matrix::getCoef(int i) {
	return i;
}

void Matrix::resize(cv::Size output) {
	outputSize = output;
	mX.create(outputSize,CV_32FC1);
	mY.create(outputSize,CV_32FC1);
}

cv::Mat Matrix::getXmat() {
	return mX;
}

cv::Mat Matrix::getYmat() {
	return mY;
}

bool Matrix::isInitialized() {
	return initialized;
}

cv::Rect Matrix::getCenterRect() {
	return cv::Rect(center->x - width/2, center->y - height/2, width, height);
}

void Matrix::moveCenterX(int px) {
	center->x += px;
}

void Matrix::moveCenterY(int px) {
	center->y += px;
}

void Matrix::changeWidth(int px) {
	width += px;
}

void Matrix::changeHeight(int px) {
	height += px;
}

void Matrix::changeZoom(float px) {
	zoomCenter += px;
}

cv::Size Matrix::getOutputSize() {
	return outputSize;
}

cv::cuda::GpuMat Matrix::getGpuXmat() {
	return mXgpu;
}

cv::cuda::GpuMat Matrix::getGpuYmat() {
	return mYgpu;
}

void Matrix::setRemapZone(cv::Rect r, double zoom) {
	center = new cv::Point(r.x + r.width/2, r.y + r.height/2);
	width = r.width;
	height = r.height;
	zoomCenter = zoom;
}

void Matrix::setInputSize(cv::Size s) {
	inputSize = s;
}
