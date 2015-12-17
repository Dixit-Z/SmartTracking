#include "cible.hpp"

#define TERR 5

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

ObjCoord cible::getRealPos()
{
	/*printf("PosOPENCV :\nX:%4f\nY:%4f\nZ:%4f\n", this->currentCVPos.Xcoord, this->currentCVPos.Ycoord, this->currentCVPos.Zcoord);
	printf("PosTLDTLD :\nX:%4f\nY:%4f\nZ:%4f\n", this->currentTLDPos.Xcoord, this->currentTLDPos.Ycoord, this->currentTLDPos.Zcoord);*/
	return this->currentRealPos;
}

void cible::setRealPos()
{
	//listeObjCoord tmpList = 
	float diffX = this->currentCVPos.Xcoord - this->currentTLDPos.Xcoord;
	float diffY = this->currentCVPos.Ycoord - this->currentTLDPos.Ycoord;
	float diffZ = this->currentCVPos.Zcoord - this->currentTLDPos.Zcoord;

	if(this->foundCV && !this->foundTLD)
	{
		this->currentRealPos=currentCVPos;
		//Rajouté comparaison au précédent
	}
	else if(this->foundCV && this->foundTLD)
	{
		if(diffX<TERR && diffX>-TERR && diffY<TERR && diffY>-TERR && diffZ<TERR && diffZ>-TERR)
		{
			this->currentRealPos=this->currentCVPos;
		}
		else
		{
			this->currentRealPos=this->currentCVPos;
			//Position couleur plus précise, comparaison au précédent plus tard
		}
	}
	else if(this->foundTLD && !this->foundCV)
	{
		this->currentRealPos=this->currentTLDPos;
		//Rajouté comparaison au précédent
	}
	else
	{
		this->currentRealPos.Xcoord=0;
		this->currentRealPos.Ycoord=0;
		this->currentRealPos.Zcoord=0;
	}
}
