#include <stdio.h>
#include <stdlib.h>

typedef struct individu Individu;
typedef struct element Element;
typedef struct liste Liste;

struct individu {
	char* prenom;
	char sexe;
	Individu* pere;
	Individu* mere;
};

struct element
{
    	Individu* personne;
    	Element *suivant;
};

struct liste
{
    	Element *premier;
    	int nbIndividu;
};

Liste* initListe();

char* getMere(Individu* i);

char* getPere(Individu* i);

char EnMaj(char c);

int estEgaleS(char* ch1, char* ch2);

int estEgaleI(Individu* i1, Individu* i2);

int estEgaleI2(Individu* i, char* ch);

int estSexeValide(char sexe);

int ajouter(Liste* l, char* prenom, char sexe, char* pere, char* mere);

Individu* estDansFamille(Individu* racine, char* recherche);

int supprimer(Liste* l, Element* e);

void afficheListeChainee(Liste* l);

void afficheArbre(Individu* i);

void afficheInfo(Individu* i);

void afficheInfoS(Liste* l, char* prenom);

Individu* cherche(Liste* l,char* prenom);

void retrouveMere(Liste* l, char* prenom);

void retrouvePere(Liste* l, char* prenom);

void retrouveParent(Liste* l, char* prenom);

void retrouveGMere(Liste* l, char* prenom);

void retrouveGPere(Liste* l, char* prenom);

void retrouveGParent(Liste* l, char* prenom);

void test(Liste* l, char* nom, char sexe, char* pere, char* mere);

void save(Liste* l, char* nomFichier);

void savePersonne(FILE* fichier,Individu* i,char** dejaFait,int* taille);

void saveFamille(FILE* fichier, Individu* i,char** dejaFait,int* taille);

int in(char** dejaFait, char* prenom,int* taille);