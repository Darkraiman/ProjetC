#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void afficheArbre(Individu* i);

void afficheAscendant(Individu* i);

void afficheInfo(Individu* i);

void afficheInfoS(Liste* l, char* prenom);

void afficheListeChainee(Liste* l);

void afficheTab(Individu** tab, int ind);

void affilie(Liste* l, char* prenom,char sexe, int demi);

void affilieParent(Liste* l,char* prenom,char sexe);

int ajouter(Liste* l,char* prenom,char sexe,char* pere,char* mere);

void ascendants(Liste* l,char* prenom);

Individu* cherche(Liste* l, char* prenom);

void cousins(Liste* l, char* prenom);

int creeTAffilie(Liste* l,char* prenom,Individu** tab,int indice,char sexe,int demi);

int creeTAffilieParent(Liste* l,Individu* test,Individu** tab,int indice,char sexe);

int creeTDescendant(Individu* i , char* prenom, Individu** tab,int indice);

int creeTEnfant(Individu* i,char* prenom,Individu** tab,int indice);

void descendants();

void enfants();

char enMaj(char c);

Individu* estDansFamille(Individu* racine,char* recherche);

int estEgaleI(Individu* i1,Individu* i2);

int estEgaleI2(Individu* i,char* ch);

int estEgaleS(char* ch1,char* ch2);

int estSexeValide(char sexe);

char* getMere(Individu* i);

char* getPere(Individu* i);

int inChar(char** dejaFait,char* prenom,int* taille);

int inIndividu(Individu** tab,char* prenom, int taille);

Liste* initListe();

int lanceCommande(Liste** l,char* fonction,char* parametre);

Liste* load(char* nomFichier);

int parseCommande(Liste** l);

void partenaires(Liste* l, char* prenom);

void petitsEnfants(Liste* l, char* prenom);

void retrouveGMere(Liste* l, char* prenom);

void retrouveGParent(Liste* l, char* prenom);

void retrouveGPere(Liste* l, char* prenom);

void retrouveMere(Liste* l, char* prenom);

void retrouveParent(Liste* l, char* prenom);

void retrouvePere(Liste* l, char* prenom);

void save(Liste* l, char* nomFichier);

void saveFamille(FILE* fichier,Individu* i, char** dejaFait, int* taille);

void savePersonne(FILE* fichier,Individu* i, char** dejaFait, int* taille);

int supprimer(Liste* l, Element* e);

int supprimeTab(Individu** tab,int indiceASupp,int taille);

void test(Liste* l, char* nom char sexe, char*pere, char* mere);

void viderBuffer();

void view(Liste* l);

void viewFamille(Individu* i, char** dejaFait, int* taille);

void viewPersonne(Individu* i, char** dejaFait, int* taille);


