/*******
TODO
OPTIMISER LES MALLOCS ET LES CREATIONS DE TABLEAU AFIN QUE LA TAILLE LIMITE SOIT LA RAM
GERER LES CAS STUPIDES
FAIRE LA DOC POUR LES FONCTIONS DONT JE NE LAI PAS ENCORE FAIT
*******/

#include "projet.h"
#define TAILLE_MINI 100

Liste* initListe(){
	/*
	Initialise la liste chainée qui contiendra l'arbre généalogique
	*/
	Liste* l = (Liste*) malloc(sizeof(Liste));
	l->premier = NULL;
	l->nbIndividu = 0;
	return l;
}

char* getMere(Individu* i){
	/*
	Fonction renvoyant le prénom de la mère de l'individu passé en paramètre
	*/
	if(!i){
		return NULL;
	}
	if (i->mere){
		return i->mere->prenom;
	}
	return NULL;
}

char* getPere(Individu* i){
	/*
	Fonction renvoyant le prénom du père de l'individu passé en paramètre
	*/
	if(!i){
		return NULL;
	}
	if (i->pere){
		return i->pere->prenom;
	}
	return NULL;
}

char EnMaj(char c){
	/*
	Fonction renvoyant le caractère passé en paramètre en majuscule
	*/
	return (c>='a' && c<='z'?c-'a'+'A':c);
}

int estEgaleS(char* ch1, char* ch2){
	/*
	Fonction d'égalité entre 2 chaines ch1 et ch2.
	Cette comparaison ignore la casse (cad Lucien et LUCIEN sont égaux).
	*/
	int i = 0;
	if (ch1==NULL || ch2 == NULL){
		return 0;
	}
	while (ch1[i]!='\0' && ch2[i] !='\0'){
		if (EnMaj(ch1[i])!=EnMaj(ch2[i])){
			return 0;
		}
		i++;
	}
	if (ch1[i]!='\0' || ch2[i] != '\0'){
		return 0;
	}
	return 1;
}

int estEgaleI(Individu* i1, Individu* i2){
	/*
	Fonction d'égalité entre 2 entités Individu i1 et i2.
	Cette comparaison ignore la casse (cad Lucien et LUCIEN sont égaux).
	*/
	return estEgaleS(i1->prenom,i2->prenom);
}

int estEgaleI2(Individu* i, char* ch){
	/*
	Fonction d'égalité entre une entité Individu i et une chaine ch.
	Cette comparaison ignore la casse (cad Lucien et LUCIEN sont égaux).
	*/
	return estEgaleS(i->prenom,ch);
}

int estSexeValide(char sexe){
	/*
	Fonction permettant de tester si le sexe donné est valide
	1 si oui 0 si non
	*/
	return (sexe == 'f' || sexe == 'm' || sexe == 0);
}

Liste* load(char* nomFichier){
	/*
	On lit une ligne du fichier par personne, et on l'ajoute à une liste.
	*/
	FILE* fichier = fopen(nomFichier,"r");
	Liste* new=initListe();
	if(fichier!=NULL){
		char current=fgetc(fichier);
		while(current!=EOF){
			char *prenom=malloc(sizeof(char)*TAILLE_MINI);
			char *pere=malloc(sizeof(char)*TAILLE_MINI);
			char *mere=malloc(sizeof(char)*TAILLE_MINI);
			char sexe=0;
			int i = 0;
			while(current!=':'){
				prenom[i++]=current;
				current=fgetc(fichier);
			}
			prenom[i]='\0';
			current=fgetc(fichier);

			//Lecture du sexe
			while(current!=','){
				sexe=current;
				current=fgetc(fichier);
			}
			current=fgetc(fichier);

			//Lecture du père
			i=0;
			while(current!=','){
				pere[i++]=current;
				current=fgetc(fichier);
			}
			pere[i]='\0';
			current=fgetc(fichier);

			//Lecture de la mère
			i=0;
			while(current!='\n'){
				mere[i++]=current;
				current=fgetc(fichier);
			}
			mere[i]='\0';
			//Free
			if(pere[0]=='\0'){
				free(pere);
				pere=NULL;
			}
			if(mere[0]=='\0'){
				free(mere);
				mere=NULL;
			}
			ajouter(new, prenom, sexe, pere, mere);
			current = fgetc(fichier);
		}
	}
	fclose(fichier);

	return new;
}

int ajouter(Liste* l, char* prenom, char sexe, char* pere, char* mere){
	/*
	Fonction permettant d'ajouter un nouveau membre de la famille

	Elle consiste en 4 questions dans une boucle qui fouille la liste chainée :
		1) est ce que la personne actuel est le pere (l'element en cours de la liste chainée)
		2) si non est ce qu'un membre de sa famille est le pere
		3) est ce que la personne actuel est la mere
		4) si non est ce qu'un membre de sa famille est la mere
	On gere un cas séparé pour une personne de la liste chainée et pour une personne de l'arbre
	car si on ajoute un enfant d'une personne de la liste chainée alors celui ci prend sa place dans la liste
	(supprime l'element en trop si le pere ET la mere sont dans la liste chainée)

	Renvoie 0 si tout s'est bien passé.
	CODE D'ERREUR DE LA FONCTION :
	1 si le sexe donné n'est pas valide
	2 si la personne correspondant au nom donné n'est pas du sexe attendu (ex : mere qui serait de sexe m)
	3 si il ya un probleme dans la suppresion (l'element n'existe pas) (/!\ ne devrait jamais arriver)
	*/
	if (!estSexeValide(sexe)){
		return 1;
	}
	Individu* i;
	int passe = 1;
	if (!(i=cherche(l,prenom))){
		i = (Individu*) malloc(sizeof(Individu));
		i->prenom = prenom;
		i->sexe = sexe;
		i->pere = NULL;
		i->mere = NULL;
		passe = 0;
	}
	Individu* iPere = NULL;
	Individu* iMere = NULL;
	Element* rac = l->premier;
	Element* aSupprimer = NULL;
	Element* prec = NULL;
	Element* remplacer = NULL;
	int trouve=0;
	int trouvePere = 0;
	int trouveMere = 0;
	while (rac!=NULL && trouve!=2){
		if (estEgaleS(rac->personne->prenom,pere)){
				if (rac->personne->sexe=='m'|| rac->personne->sexe ==0){
					rac->personne->sexe = 'm';
					i->pere = rac->personne;
					if (!trouve){
						remplacer = rac;
					} else {
						aSupprimer = rac;
					}
					trouve++;
					trouvePere++;
				} else {
					//free(i);
					return 2;
				}
		} else if (iPere = estDansFamille(rac->personne,pere)) {
			if (iPere->sexe == 'm'|| iPere->sexe == 0){
				iPere->sexe = 'm';
				i->pere = iPere;
				trouvePere++;
			} else {
				//free(i);
				return 2;
			}
		}
		if (estEgaleS(rac->personne->prenom,mere)){
				if (rac->personne->sexe=='f'|| rac->personne->sexe ==0){
					rac->personne->sexe = 'f';
					i->mere = rac->personne;
					if (!trouve){
						remplacer = rac;
					} else {
						aSupprimer = rac;
					}
					trouve++;
					trouveMere++;
				} else {
					//free(i);
					return 2;
				}
		} else if (iMere = estDansFamille(rac->personne,mere)) {
			if (iMere->sexe == 'f'|| iMere->sexe == 0){
				iMere->sexe = 'f';
				i->mere = iMere;
				trouveMere++;
			} else {
				//free(i);
				return 2;
			}
		}
		prec = rac;
		rac = rac->suivant;
	}
	if(!trouvePere && pere!=NULL){
		Individu* nouveau = (Individu*) malloc(sizeof(Individu));
		nouveau->prenom = pere;
		nouveau->sexe = 'm';
		nouveau->pere = NULL;
		nouveau->mere = NULL;
		i->pere = nouveau;
		l->nbIndividu++;
	}
	if (!trouveMere && mere!=NULL){
		Individu* nouveau = (Individu*) malloc(sizeof(Individu));
		nouveau->prenom = mere;
		nouveau->sexe = 'f';
		nouveau->pere = NULL;
		nouveau->mere = NULL;
		i->mere = nouveau;
		l->nbIndividu++;
	}
	if (!trouve){
		if (!passe){
			Element* nouv = (Element*) malloc(sizeof(Element));
			nouv->personne = i;
			nouv->suivant = NULL;
			if (prec){
				prec->suivant = nouv;
			} else {
				l->premier = nouv;
			}
			l->nbIndividu++;
		}
	} else {
		if (aSupprimer){
			if (supprimer(l,aSupprimer)){
				return 3;
			}
		}
		l->nbIndividu++;
		remplacer->personne = i;
	}
	return 0;
}

Individu* estDansFamille(Individu* racine, char* recherche){
	/*
	Fonction recursive qui renvoie si la personne recherché (renseigné par la chaine de caractère) est dans la famille de la personne racine
	renvoie l'adresse pointant la personne si trouvé
	NULL sinon
	*/
	if (racine->mere == NULL && racine->pere == NULL){
		return NULL;
	}
	Individu* en_cours=NULL;
	if (racine->mere != NULL){
		if (estEgaleS(racine->mere->prenom,recherche)){
			return racine->mere;
		} else {
			if (en_cours = estDansFamille(racine->mere,recherche)){
				return en_cours;
			}
		}
	}
	if (racine->pere != NULL){
		if (estEgaleS(racine->pere->prenom,recherche)){
			return racine->pere;
		} else {
			if (en_cours = estDansFamille(racine->pere,recherche)){
				return en_cours;
			}
		}
	}
	return NULL;
}

int supprimer(Liste* l, Element* e){
	/*
	Fonction qui supprimer l'element e dans la liste l passé en paramètre
	*/
	if (l->premier == e){
		l->premier = e->suivant;
		free(e);
	}
	Element* rac = l->premier;
	Element* prec = NULL;
	while (rac!=NULL){
		if (rac==e){
			prec->suivant = e->suivant;
			free(e);
			return 0;
		}
		prec = rac;
		rac = rac->suivant;
	}
	return 1;
}

void afficheListeChainee(Liste* l){
	/*
	Fonction d'affichage entière de la liste chainée passé en paramètre
	L'affichage est de la forme :
	(Debut) - (ELEMENT) - (ELEMENT) - (NULL)
	*/
	Element* rac = l->premier;
	printf("\n(Debut)");
	while (rac!=NULL){
		printf(" - (");
		afficheArbre(rac->personne);
		printf(") ");
		rac = rac->suivant;
	}
	printf("- (NULL)\n");
}

void afficheArbre(Individu* i){
	/*
	Fonction d'affichage permettant d'affichée l'arbre de la famille de l'individu passé en paramètre
	L'affichage sera sous la forme PRENOM [SEXE] pere : (AFFICHAGE ARBRE PERE) mere : (AFFICHAGE ARBRE MERE)
	*/
	if (i==NULL){
		printf("NULL");
	}else {
		printf("%s [%c] ",i->prenom,i->sexe);
		printf("pere : (");
		afficheArbre(i->pere);
		printf(") mere : (");
		afficheArbre(i->mere);
		printf(")");
	}
}

int inChar(char** dejaFait, char* prenom,int* taille){
	for (int i=0;i<*taille;i++){
		if (estEgaleS(prenom,dejaFait[i])){
			return 1;
		}
	}
	return 0;
}

int inIndividu(Individu** tab, char* prenom,int taille){
	for (int i=0;i<taille;i++){
		if (estEgaleS(prenom,tab[i]->prenom)){
			return 1;
		}
	}
	return 0;
}
void saveFamille(FILE* fichier, Individu* i,char** dejaFait,int* taille){
	if (i){
		savePersonne(fichier,i,dejaFait,taille);
		saveFamille(fichier,i->pere,dejaFait,taille);
		saveFamille(fichier,i->mere,dejaFait,taille);
	}
}

void savePersonne(FILE* fichier,Individu* i,char** dejaFait,int* taille){
	if (!inChar(dejaFait,i->prenom,taille)){
		if (!i->sexe){
			fprintf(fichier,"%s:,%s,%s\n",i->prenom,(!i->pere?"":getPere(i)),(!i->mere?"":getMere(i)));
		}else {
			fprintf(fichier,"%s:%c,%s,%s\n",i->prenom,i->sexe,(!i->pere?"":getPere(i)),(!i->mere?"":getMere(i)));
		}
		dejaFait[(*taille)++] = i->prenom;
	}
}

void save(Liste* l, char* nomFichier){
	FILE* fichier = fopen(nomFichier,"w");
	Element* rac = l->premier;
	char** dejaFait = (char**) malloc(sizeof(char*)*l->nbIndividu);
	int taille=0;
	int* ptaille = &taille;
	while (rac!=NULL){
		saveFamille(fichier,rac->personne,dejaFait,ptaille);
		rac=rac->suivant;
	}
	fclose(fichier);
}

void viewPersonne(Individu* i,char** dejaFait,int* taille){
	if (!inChar(dejaFait,i->prenom,taille)){
		if (!i->sexe){
			printf("%s:,%s,%s\n",i->prenom,(!i->pere?"":getPere(i)),(!i->mere?"":getMere(i)));
		}else {
			printf("%s:%c,%s,%s\n",i->prenom,i->sexe,(!i->pere?"":getPere(i)),(!i->mere?"":getMere(i)));
		}
		dejaFait[(*taille)++] = i->prenom;
	}
}

void viewFamille(Individu* i,char** dejaFait,int* taille){
	if (i){
		viewPersonne(i,dejaFait,taille);
		viewFamille(i->pere,dejaFait,taille);
		viewFamille(i->mere,dejaFait,taille);
	}
}

void view(Liste* l){
    Element* rac = l->premier;
	char** dejaFait = (char**) malloc(sizeof(char*) * l->nbIndividu);
	int taille=0;
	int* ptaille = &taille;
	while (rac!=NULL){
		viewFamille(rac->personne,dejaFait,ptaille);
		rac=rac->suivant;
	}
}

void test(Liste* l, char* nom, char sexe, char* pere, char* mere){
	/*
	Fonction de test qui récupère les erreurs et les affiche dans la console
	*/
	int codeExec = ajouter(l,nom,sexe,pere,mere);
	switch (codeExec){
		case 0:
			printf("Tout s'est bien passe !\n");
			break;
		case 1:
			printf("Probleme avec le sexe !");
			break;
		case 2:
			printf("Probleme avec le sexe d'un parent ! ");
			break;
		default:
			printf("WTF");
	}
}

Individu* cherche(Liste* l,char* prenom){
	/*
	Fonction qui à partir d'une chaine passé en paramètre va renvoyer la Structure Individu correspondante dans la liste passé en paramètre
	NULL si non trouvé
	*/
	Element* rac = l->premier;
	Individu* i = NULL;
	while (rac!=NULL){
		if(estEgaleS(rac->personne->prenom,prenom)){
			return rac->personne;
		} else if (i=estDansFamille(rac->personne,prenom)){
			return i;
		}
		rac=rac->suivant;
	}
	return NULL;
}

void afficheInfoS(Liste* l, char* prenom){
	/*
	Affiche les infos de la personne possèdant la chaine passé en paramètre comme prénom
	*/
	Individu* i = cherche(l,prenom);
	afficheInfo(i);
}
void afficheInfo(Individu* i){
	/*
	Affiche les infos d'un Individu
	Affichage sous la forme :
	Information sur PRENOM:
	Sexe : SEXE
	Père : PRENOMPERE
	Mère : PRENOMMERE
	*/
	printf("Information sur %s :\n",i->prenom);
	printf("Sexe : ");
	if (i->sexe ==0){
		printf("%s\n","Non défini");
	}else {
		printf("%c\n",i->sexe);
	}
	printf("Père : %s\n",(i->pere==NULL?"Non disponible":i->pere->prenom));
	printf("Mère : %s\n",(i->mere==NULL?"Non disponible":i->mere->prenom));
}

void retrouveMere(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie la mère de la personne dont le prénom est celui passé en paramètre
	*/
	Individu* i = cherche(l,prenom);
	printf("Sa mère est : %s\n",(getMere(i)==NULL?"Non Disponible":getMere(i)));
}

void retrouvePere(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie le père de la personne dont le prénom est celui passé en paramètre
	*/
	Individu* i = cherche(l,prenom);
	printf("Son père est : %s\n",(getPere(i)==NULL?"Non Disponible":getPere(i)));
}

void retrouveParent(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie la mère et le père de la personne dont le prénom est celui passé en paramètre
	*/
	retrouvePere(l,prenom);
	retrouveMere(l,prenom);
}

void retrouveGMere(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie la grand mère de la personne dont le prénom est celui passé en paramètre
	*/
	Individu* i = cherche(l,prenom);
	char* gm1 = getMere(cherche(l,getMere(i)));
	char* gm2 = getMere(cherche(l,getPere(i)));
	printf("Ses grands mères sont : %s et %s\n",(gm1==NULL?"Non Disponible":gm1),(gm2==NULL?"Non Disponible":gm2));
}

void retrouveGPere(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie le grand père de la personne dont le prénom est celui passé en paramètre
	*/
	Individu* i = cherche(l,prenom);
	char* gp1 = getPere(cherche(l,getMere(i)));
	char* gp2 = getPere(cherche(l,getPere(i)));
	printf("Son grand père est : %s et %s\n",(gp1==NULL?"Non Disponible":gp1),(gp2==NULL?"Non Disponible":gp2));
}

void retrouveGParent(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie la grand mère et le grand père de la personne dont le prénom est celui passé en paramètre
	*/
	retrouveGPere(l,prenom);
	retrouveGMere(l,prenom);
}

void afficheAscendant(Individu* i){
	if (!(i==NULL)){
		printf("%s\n",i->prenom);
		afficheAscendant(i->pere);
		afficheAscendant(i->mere);
	}
}

void ascendants(Liste* l, char* prenom){
		Individu* i = cherche(l,prenom);
		printf("Ces ascendants sont :\n");
		afficheAscendant(i->pere);
		afficheAscendant(i->mere);
}


int creeTEnfant(Individu* i,char* prenom,Individu** tab,int indice){
	if (!(i==NULL)){
		if (estEgaleS(((i->mere==NULL)?NULL:i->mere->prenom),prenom)){
			if (!inIndividu(tab,i->prenom,indice)){
				tab[indice++] = i;
			}
			return indice;
		}
		if (estEgaleS(((i->pere==NULL)?NULL:i->pere->prenom),prenom)){
			if (!inIndividu(tab,i->prenom,indice)){
				tab[indice++] = i;
			}
			return indice;
		}
		indice = creeTEnfant(i->mere,prenom,tab,indice);
		indice = creeTEnfant(i->pere,prenom,tab,indice);
		return indice;
	}
	return indice;
}

void afficheTab(Individu** tab,int ind){
	for (int i=0;i<ind;i++){
		printf("%s\n",tab[i]->prenom);
	}
}

void enfants(Liste* l, char* prenom, int option1){
	Element* rac = l->premier;
	Individu* tab[TAILLE_MINI];
	if (option1){
		printf("Enfant de %s :\n",prenom);
	}
	int indice = 0;
	while(rac!=NULL){
		indice = creeTEnfant(rac->personne,prenom,tab,indice);
		rac = rac->suivant;
	}
	if (!indice){
		printf("Pas d'enfants !\n");
	}else {
		afficheTab(tab,indice);
	}
}



void petitsEnfants(Liste* l, char* prenom){
	Element* rac = l->premier;
	printf("Petit(s) enfant(s) de %s:\n",prenom);
	Individu** tab = (Individu**) malloc(sizeof(Individu*)*TAILLE_MINI);
	int indice = 0;
	while(rac!=NULL){
		indice = creeTEnfant(rac->personne,prenom,tab,indice);
		rac = rac->suivant;
	}
	Individu** tab2 = (Individu**) malloc(sizeof(Individu*)*TAILLE_MINI);
	int indice2=0;
	for (int i =0;i<indice;i++){
        Element* rac = l->premier;
        while(rac!=NULL){
            indice2 = creeTEnfant(rac->personne,tab[i]->prenom,tab2,indice2);
            rac=rac->suivant;
        }
    }
	if (!indice2){
		printf("Pas de petits enfants !\n");
	}else {
		afficheTab(tab2,indice2);
	}
}
int creeTDescendant(Individu* i, char* prenom, Individu** tab, int indice){
    if (!(i==NULL)){
		if (estDansFamille(i,prenom)){
			if (!inIndividu(tab,i->prenom,indice)){
				tab[indice++] = i;
			}
		}
		indice = creeTDescendant(i->mere,prenom,tab,indice);
		indice = creeTDescendant(i->pere,prenom,tab,indice);
		return indice;
	}
	return indice;
}

void descendants(Liste* l, char* prenom){
	Element* rac = l->premier;
	Individu* tab[TAILLE_MINI];
	int indice = 0;
	while(rac!=NULL){
		indice = creeTDescendant(rac->personne,prenom,tab,indice);
		rac = rac->suivant;
	}
	printf("Descendants de %s\n",prenom);
	if (indice){
        afficheTab(tab,indice);
	} else {
        printf("Pas de descendants !\n");
    }
}


void partenaires(Liste* l, char* prenom){
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donnée n'existe pas !\n");
    } else {
        Element* rac = l->premier;
        Individu* tab[TAILLE_MINI];
        int indice = 0;
        char sexe = test->sexe;
        while(rac!=NULL){
            indice = creeTEnfant(rac->personne,prenom,tab,indice);
            rac = rac->suivant;
        }
        printf("Partenaire(s) de %s :\n",prenom);
        Individu* dejaFait[TAILLE_MINI];
        int indDejaFait = 0;
        if (indice){
            if (sexe=='m'){
                for (int i = 0;i<indice;i++){
                    if (!(inIndividu(dejaFait,tab[i]->mere->prenom,indDejaFait))){
                        printf("%s\n",tab[i]->mere->prenom);
                        dejaFait[indDejaFait++]=tab[i]->mere;
                    }
                }
            } else {
                for (int i = 0;i<indice;i++){
                    if (!(inIndividu(dejaFait,tab[i]->pere->prenom,indDejaFait))){
                        printf("%s\n",tab[i]->pere->prenom);
                        dejaFait[indDejaFait++]=tab[i]->pere;
                    }
                }
            }
        } else {
            printf("Pas de partenaires connus !\n");
        }
    }
}
int supprimeTab(Individu** tab,int indiceASupp,int taille){
    tab[indiceASupp] = tab[taille-1];
    tab[taille-1] = NULL;
    return taille-1;
}

int creeTAffilie(Liste* l, char* prenom, Individu** tab, int indice,char sexe,int demi){
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donnée n'existe pas !\n");
    } else {
        Individu* i;
        char* prenomMDemi;
        if (demi){
            Individu* iM;
            iM = test->mere;
            if (iM){
                prenomMDemi = iM->prenom;
            }
            i = test->pere;
        } else {
            i = test->mere;
        }
        if (i){
            Element* rac = l->premier;
            int indice = 0;
            char* prenomM = i->prenom;
            while(rac!=NULL){
                indice = creeTEnfant(rac->personne,prenomM,tab,indice);
                rac = rac->suivant;
            }
            for (int i=0;i<indice;i++){
                if (tab[i]->sexe != sexe || estEgaleS(prenom,tab[i]->prenom) || (estEgaleS(prenomMDemi,tab[i]->mere->prenom)&&demi)){
                    indice=supprimeTab(tab,i,indice);
                    i--;
                }
            }
            return indice;
        } else {
            return 0;
        }
    }
}

void affilie(Liste* l, char* prenom,char sexe,int demi){
    if (demi){
        if (sexe=='m'){
            printf("Demi-frere(s) ");
        } else {
            printf("Demi-soeur(s) ");
        }
    } else {
        if (sexe=='m'){
            printf("Frere(s) ");
        } else {
            printf("Soeur(s) ");
        }
    }
    printf("de %s : \n",prenom);
    Individu* tab[TAILLE_MINI];
    int cptG=0;
    cptG = creeTAffilie(l,prenom,tab,cptG,sexe,demi);
    if (!cptG){
        if (demi){
            if (sexe=='m'){
                printf("Pas de demi-freres connus !\n");
            } else {
                printf("Pas de demi-soeurs connus !\n");
            }
        } else {
            if (sexe=='m'){
                printf("Pas de freres connus !\n");
            } else {
                printf("Pas de soeurs connus !\n");
            }
        }
    } else {
        afficheTab(tab,cptG);
    }
}
int creeTAffilieParent(Liste* l,Individu* test,Individu** tab,int indice,char sexe){
    Individu* iP = test->mere;
    Individu* iM = test->pere;
    if (iP){
        indice = creeTAffilie(l,iP->prenom,tab,indice,sexe,0);
    }
    if (iM){
        indice = creeTAffilie(l,iM->prenom,tab,indice,sexe,0);
    }
    return indice;
}

void affilieParent(Liste* l, char* prenom,char sexe){
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donnée n'existe pas !\n");
    } else {
        if (sexe=='m'){
            printf("Oncle(s) de %s : \n",prenom);
        } else {
            printf("Tante(s) de %s : \n",prenom);
        }
        int indice = 0;
        Individu* tab[TAILLE_MINI];
        indice = creeTAffilieParent(l,test,tab,indice,sexe);
        if (!indice){
            if (sexe=='m'){
                printf("Pas d'oncles !\n");
            } else {
                printf("Pas de tantes !\n");
            }
        } else {
            afficheTab(tab,indice);
        }
    }
}

void cousins(Liste* l, char* prenom){
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donnée n'existe pas !\n");
    } else {
        printf("Cousins : \n");
        Individu* affilieP[TAILLE_MINI];
        int indiceP = 0;
        indiceP = creeTAffilieParent(l,test,affilieP,indiceP,'m');
        indiceP = creeTAffilieParent(l,test,affilieP,indiceP,'f');
        Individu* cousin[TAILLE_MINI];
        int indiceC = 0;
        for (int i =0;i<indiceP;i++){
            Element* rac = l->premier;
            while(rac!=NULL){
                indiceC = creeTEnfant(rac->personne,affilieP[i]->prenom,cousin,indiceC);
                rac=rac->suivant;
            }
        }
        if (indiceC){
            afficheTab(cousin,indiceC);
        }else{
            printf("Pas de cousins !\n");
        }

    }
}

void viderBuffer(){
  int c;
  while((c=getchar()) != EOF && c != '\n');
}

int lanceCommande(Liste** l,char* fonction, char* parametre){
    if (estEgaleS(fonction,"load")){
        if(!strlen(parametre)) {
            printf("La fonction load necessite un paramètre qui correspond au fichier dans lequel se trouve la famille.\n");
        } else {
            *l = load(parametre);
        }
    } else if (estEgaleS(fonction,"save")){
        if(!strlen(parametre)) {
            printf("La fonction save necessite un paramètre qui correspond au fichier dans lequel la famille sera enregistré.\n");
        } else {
            save(*l,parametre);
        }
    } else if (estEgaleS(fonction,"view")){
        view(*l);
    } else if (estEgaleS(fonction,"exit")){
        return 1;
    } else if (estEgaleS(fonction,"new")){
        if(!strlen(parametre)) {
            printf("La fonction new necessite au moins un paramètre prénom et au plus 4 paramètre qui sont prenom, sexe , pere , mere dans cette ordre.\n");
        } else {

            char* prenom = (char*) malloc(sizeof(char)*100);
            char sexe;
            char* pere;
            char* mere;
            int taillemax = strlen(parametre);
            int indCha = 0;
            int ind = 0;
            while (ind<taillemax && parametre[ind] != ','){
                prenom[indCha++]=parametre[ind++];
            }
            prenom[indCha]='\0';
            if (ind>=taillemax){
                test(*l,prenom,0,NULL,NULL);
                return 0;
            }
            ind++;
            if (!(parametre[ind]==',')){
                sexe = parametre[ind++];
                ind++;
            } else {
                sexe = 0;
                ind++;
            }
            indCha = 0;
            if (!(parametre[ind]==',')){
                pere = (char*) malloc(sizeof(char)*100);
                while (ind<taillemax && parametre[ind] != ','){
                    pere[indCha++]=parametre[ind++];
                }
                pere[indCha]='\0';
            } else {
                pere = NULL;
            }
            ind++;
            indCha = 0;
            if (ind == taillemax||parametre[ind]==','){
                mere = NULL;
            } else {
                mere = (char*) malloc(sizeof(char)*100);
                while (ind<taillemax && parametre[ind] != ','){
                    mere[indCha++]=parametre[ind++];
                }
                mere[indCha]='\0';
            }
            test(*l,prenom,sexe,pere,mere);
        }
    } else if (estEgaleS(fonction,"info")){
        if(!strlen(parametre)) {
            printf("La fonction info necessite un paramètre qui correspond la personne pour lequel on recherche les infos.\n");
        } else {
            afficheInfoS(*l,parametre);
        }
    } else if (estEgaleS(fonction,"mere")){
        if(!strlen(parametre)) {
            printf("La fonction mere necessite un paramètre qui correspond la personne pour lequel on recherche la mere.\n");
        } else {
            retrouveMere(*l,parametre);
        }
    } else if (estEgaleS(fonction,"pere")){
        if(!strlen(parametre)) {
            printf("La fonction pere necessite un paramètre qui correspond la personne pour lequel on recherche le pere.\n");
        } else {
            retrouvePere(*l,parametre);
        }
    } else if (estEgaleS(fonction,"parents")){
        if(!strlen(parametre)) {
            printf("La fonction parents necessite un paramètre qui correspond la personne pour lequel on recherche les parents.\n");
        } else {
            retrouveParent(*l,parametre);
        }
    } else if (estEgaleS(fonction,"gdmeres")){
        if(!strlen(parametre)) {
            printf("La fonction gdmeres necessite un paramètre qui correspond la personne pour lequel on recherche les grands meres.\n");
        } else {
            retrouveGMere(*l,parametre);
        }
    } else if (estEgaleS(fonction,"gdperes")){
        if(!strlen(parametre)) {
            printf("La fonction gdperes necessite un paramètre qui correspond la personne pour lequel on recherche les grands peres.\n");
        } else {
            retrouveGPere(*l,parametre);
        }
    } else if (estEgaleS(fonction,"gdparents")){
        if(!strlen(parametre)) {
            printf("La fonction gdparents necessite un paramètre qui correspond la personne pour lequel on recherche les grands parents.\n");
        } else {
            retrouveGParent(*l,parametre);
        }
    } else if (estEgaleS(fonction,"ascendants")){
        if(!strlen(parametre)) {
            printf("La fonction ascendants necessite un paramètre qui correspond la personne pour lequel on recherche les ascendants.\n");
        } else {
            ascendants(*l,parametre);
        }
    } else if (estEgaleS(fonction,"enfants")){
        if(!strlen(parametre)) {
            printf("La fonction enfants necessite un paramètre qui correspond la personne pour lequel on recherche les enfants.\n");
        } else {
            enfants(*l,parametre,1);
        }
    } else if (estEgaleS(fonction,"petitsenfants")){
        if(!strlen(parametre)) {
            printf("La fonction petitsenfants necessite un paramètre qui correspond la personne pour lequel on recherche les petits enfants.\n");
        } else {
            petitsEnfants(*l,parametre);
        }
    } else if (estEgaleS(fonction,"descendants")){
        if(!strlen(parametre)) {
            printf("La fonction descendants necessite un paramètre qui correspond la personne pour lequel on recherche les descendants.\n");
        } else {
            descendants(*l,parametre);
        }
    } else if (estEgaleS(fonction,"partenaires")){
        if(!strlen(parametre)) {
            printf("La fonction partenaires necessite un paramètre qui correspond la personne pour lequel on recherche les partenaires.\n");
        } else {
            partenaires(*l,parametre);
        }
    } else if (estEgaleS(fonction,"freres")){
        if(!strlen(parametre)) {
            printf("La fonction freres necessite un paramètre qui correspond la personne pour lequel on recherche les freres.\n");
        } else {
            affilie(*l,parametre,'m',0);
        }
    } else if (estEgaleS(fonction,"soeurs")){
        if(!strlen(parametre)) {
            printf("La fonction soeurs necessite un paramètre qui correspond la personne pour lequel on recherche les soeurs.\n");
        } else {
            affilie(*l,parametre,'f',0);
        }
    } else if (estEgaleS(fonction,"demifreres")){
        if(!strlen(parametre)) {
            printf("La fonction demifreres necessite un paramètre qui correspond la personne pour lequel on recherche les demi freres.\n");
        } else {
            affilie(*l,parametre,'m',1);
        }
    } else if (estEgaleS(fonction,"demisoeurs")){
        if(!strlen(parametre)) {
            printf("La fonction demisoeurs necessite un paramètre qui correspond la personne pour lequel on recherche les demisoeurs.\n");
        } else {
            affilie(*l,parametre,'f',1);
        }
    } else if (estEgaleS(fonction,"oncles")){
        if(!strlen(parametre)) {
            printf("La fonction oncles necessite un paramètre qui correspond la personne pour lequel on recherche les oncles.\n");
        } else {
            affilieParent(*l,parametre,'m');
        }
    } else if (estEgaleS(fonction,"tantes")){
        if(!strlen(parametre)) {
            printf("La fonction tantes necessite un paramètre qui correspond la personne pour lequel on recherche les tantes.\n");
        } else {
            affilieParent(*l,parametre,'f');
        }
    } else if (estEgaleS(fonction,"cousins")){
        if(!strlen(parametre)) {
            printf("La fonction cousins necessite un paramètre qui correspond la personne pour lequel on recherche les cousins.\n");
        } else {
            cousins(*l,parametre);
        }
    } else {
        printf("La commande n'existe pas !");
    }
    return 0;
}

int parseCommande(Liste** l){
    printf("> ");
    char buffer[100];
    char fonction[14];
    char parametre[100];
    scanf("%s",buffer);
    viderBuffer();
    int tailleEntree = strlen(buffer);
    int indBuffer=0;
    int indCha = 0;
    while (indBuffer<tailleEntree&&buffer[indBuffer]!='('){
        if (indCha<13){
            fonction[indCha++]=buffer[indBuffer];
        }
        indBuffer++;
    }
    fonction[indCha]='\0';
    indBuffer++;
    indCha = 0;
    while(indBuffer<tailleEntree-1 && buffer[indBuffer]!=')'){
        parametre[indCha]=buffer[indBuffer];
        indBuffer++;
        indCha++;
    }
    parametre[indCha]='\0';
    printf("Fonction %s (%s)\n",fonction,parametre);
    return lanceCommande(l,fonction,parametre);
}

int main(int argc, char* argv[]){
	/*
	Fonction principal du module
	*/
	Liste* l = initListe();
	int fini = 0;
	while (!fini){
        fini = parseCommande(&l);
        printf("\n");
	}
}

