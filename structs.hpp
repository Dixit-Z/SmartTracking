#ifndef STRUCT_H
#define STRUCT_H

typedef struct
{
	float Xcoord;
	float Ycoord;
	float Zcoord;
} ObjCoord;

typedef struct ListeObjCoord
{
	ObjCoord noeud;
	int const nbNoeud=15;
	ListeObjCoord *suivant;
}ListeObjCoord;

/*typedef struct
{
	bool lastFound;
	float size; //Est ce qu'on le met en int ? (Si récupéré en pixel)
	float densite;
}Object;
*/
#endif
