#include "cible.hpp"

cible::cible(int id)
{
	this->id = id;
}

void cible::getName()
{
	cout << "Name is :" << this->name << endl;
}

void cible::setName(string name)
{
	this->name = name;
}

void cible::setimgGray(Mat img)
{
	this->imgGray = img;
}

void cible::setDesc(Mat img)
{
	this->desc = img;
}

void cible::setKP(vector<KeyPoint> KP)
{
	this->KP = KP;
}

void cible::setPicture(Mat img)
{
	this->picture=img;
}

void cible::setFoundCV(bool found)
{
	this->foundCV=found;
}

void cible::setFoundTLD(bool found)
{
	this->foundTLD=found;
}

void cible::setCurrentCV(float x, float y, float z)
{
	this->currentCVPos.Xcoord = x;
	this->currentCVPos.Ycoord = y;
	this->currentCVPos.Zcoord = z;
}

void cible::setCurrentTLD(float x, float y, float z)
{
	this->currentTLDPos.Xcoord = x;
	this->currentTLDPos.Ycoord = y;
	this->currentTLDPos.Zcoord = z;
}

Mat cible::getimgGray()
{
	return this->imgGray;
}

Mat cible::getDesc()
{
	return this->desc;
}

vector<KeyPoint> cible::getKP()
{
	return this->KP;
}

Mat cible::getPicture()
{
	return this->picture;
}

bool cible::getFoundCV()
{
	return this->foundCV;
}

bool cible::getFoundTLD()
{
	return this->foundTLD;
}
