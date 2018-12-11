#include "projet.h"
#define TAILLE_MINI 500
//SUPPRIMER LES ELEMENTS DE ORDRENAISS ETC QUAND ON LES SUPPRIME DE LA LISTE

Liste* initListe(){
	/*
	Initialise la liste chain�e qui contiendra l'arbre g�n�alogique
	*/
	Liste* l = (Liste*) malloc(sizeof(Liste));
	l->premier = NULL;
	l->nbIndividu = 0;
	return l;
}

Liste2* initListe2(){
	/*
	Initialise la liste chain�e qui contiendra l'arbre g�n�alogique
	*/
	Liste2* l = (Liste2*) malloc(sizeof(Liste2));
	l->premier = NULL;
	l->dernier = NULL;
	l->nbIndividu = 0;
	return l;
}

char* getMere(Individu* i){
	/*
	Fonction renvoyant le pr�nom de la m�re de l'individu pass� en param�tre
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
	Fonction renvoyant le pr�nom du p�re de l'individu pass� en param�tre
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
	Fonction renvoyant le caract�re pass� en param�tre en majuscule
	*/
	return (c>='a' && c<='z'?c-'a'+'A':c);
}

char EnMin(char c){
	/*
	Fonction renvoyant le caract�re pass� en param�tre en minuscule
	*/
	return (c>='A'&&c<='Z'?c-'A'+'a':c);
}

int estEgaleS(char* ch1, char* ch2){
	/*
	Fonction d'�galit� entre 2 chaines ch1 et ch2.
	Cette comparaison ignore la casse (cad Lucien et LUCIEN sont �gaux).
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
	Fonction d'�galit� entre 2 entit�s Individu i1 et i2.
	Cette comparaison ignore la casse (cad Lucien et LUCIEN sont �gaux).
	*/
	return estEgaleS(i1->prenom,i2->prenom);
}

int estEgaleI2(Individu* i, char* ch){
	/*
	Fonction d'�galit� entre une entit� Individu i et une chaine ch.
	Cette comparaison ignore la casse (cad Lucien et LUCIEN sont �gaux).
	*/
	return estEgaleS(i->prenom,ch);
}

int estSexeValide(char sexe){
	/*
	Fonction permettant de tester si le sexe donn� est valide
	1 si oui 0 si non
	*/
	return (sexe == 'f' || sexe == 'm' || sexe == 0);
}

void freeElement(Element* e){
    if (e->personne){
        freeIndividu(e->personne);
    }
    e->suivant=NULL;
    free(e);
}

void freeIndividu(Individu* i){
    i->mere=NULL;
    i->pere=NULL;
    free(i->prenom);
    if (i->naissance){
        free(i->naissance);
    }
    free(i);
}

Liste* load(char* nomFichier){
	/*
	On lit une ligne du fichier par personne, et on l'ajoute � une liste.
	*/
	FILE* fichier = fopen(nomFichier,"r");
	Liste* new=initListe();
	if(fichier!=NULL){
		char current=fgetc(fichier);
		while(current!=EOF){
			char *prenom=malloc(sizeof(char)*TAILLE_MINI);
			char *pere=malloc(sizeof(char)*TAILLE_MINI);
			char *mere=malloc(sizeof(char)*TAILLE_MINI);
			char *dateN=malloc(sizeof(char)*TAILLE_MINI);
			char *dateM=malloc(sizeof(char)*TAILLE_MINI);
			char *naissance=malloc(sizeof(char)*TAILLE_MINI);
			char sexe=0;
			long date = 0;
			long dateMort = 0;
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

			//Lecture du p�re
			i=0;
			while(current!=','){
				pere[i++]=current;
				current=fgetc(fichier);
			}
			pere[i]='\0';
			current=fgetc(fichier);

			//Lecture de la m�re
			i=0;
			while(current!=','){
				mere[i++]=current;
				current=fgetc(fichier);
			}
			mere[i]='\0';
			current=fgetc(fichier);
            i=0;
            while(current!=','){
                dateN[i++]=current;
                current=fgetc(fichier);
            }
            dateN[i]='\0';
            if (dateN[0]!='\0'){
                char* fin;
                date = strtol(dateN,&fin,10);
            }
            free(dateN);

            current=fgetc(fichier);
            i=0;
            while(current!=','){
                dateM[i++]=current;
                current=fgetc(fichier);
            }
            dateM[i]='\0';
            if (dateM[0]!='\0'){
                char* fin;
                dateMort = strtol(dateM,&fin,10);
            }
            free(dateM);
            current=fgetc(fichier);
            i=0;
			while(current!='\n'){
				naissance[i++]=current;
				current=fgetc(fichier);
			}
			naissance[i]='\0';
			//Free
			if(pere[0]=='\0'){
				free(pere);
				pere=NULL;
			}
			if(mere[0]=='\0'){
				free(mere);
				mere=NULL;
			}
			if(naissance[0]=='\0'){
                free(naissance);
                naissance=NULL;
			}
			ajouter(new, prenom, sexe, pere, mere,date,dateMort,naissance);
			current = fgetc(fichier);
		}
		fclose(fichier);
		return new;
	} else {
        printf("Fichier inexistant ou probl�me dans son ouverture !");
        return NULL;
	}
}

int ajouter(Liste* l, char* prenom, char sexe, char* pere, char* mere,long date,long dateMort,char* naissance){
	/*
	Fonction permettant d'ajouter un nouveau membre de la famille

	Elle consiste en 4 questions dans une boucle qui fouille la liste chain�e :
		1) est ce que la personne actuel est le pere (l'element en cours de la liste chain�e)
		2) si non est ce qu'un membre de sa famille est le pere
		3) est ce que la personne actuel est la mere
		4) si non est ce qu'un membre de sa famille est la mere
	On gere un cas s�par� pour une personne de la liste chain�e et pour une personne de l'arbre
	car si on ajoute un enfant d'une personne de la liste chain�e alors celui ci prend sa place dans la liste
	(supprime l'element en trop si le pere ET la mere sont dans la liste chain�e)

	Renvoie 0 si tout s'est bien pass�.
	CODE D'ERREUR DE LA FONCTION :
	1 si le sexe donn� n'est pas valide
	2 si la personne correspondant au nom donn� n'est pas du sexe attendu (ex : mere qui serait de sexe m)
	3 si il ya un probleme dans la suppresion (l'element n'existe pas) (/!\ ne devrait jamais arriver)
	*/
	if (estEgaleS(prenom,mere)|| estEgaleS(prenom,pere) || estEgaleS(pere,mere)){
		return 5;
	}
	if (!estSexeValide(sexe)){
		return 1;
	}
	Individu* i;
	int passe = 1;
	i=cherche(l,prenom);
	if (!(i)){
		i = (Individu*) malloc(sizeof(Individu));
		transformeNom(prenom);
		i->prenom = prenom;
		i->sexe = sexe;
		i->pere = NULL;
		i->mere = NULL;
		i->naissance = NULL;
		passe = 0;
	} else {
	    free(prenom);
		Individu** probleme = (Individu**) malloc(sizeof(Individu*)*TAILLE_MINI);
		Element* rac = l->premier;
		int indice = 0;
		while (rac!=NULL){
			indice = creeTDescendant(rac->personne,i->prenom,probleme,indice);
			rac = rac->suivant;
		}
		for (int i=0;i<indice;i++){
			if (estEgaleS(probleme[i]->prenom,mere) || estEgaleS(probleme[i]->prenom,pere)){
				return 4;
			}
		}
		free(probleme);
		if (!estEgaleS((!i->mere?NULL:i->mere->prenom),mere) && i->mere != NULL){
			return 6;
		}
		if (!estEgaleS(pere,(!i->pere?NULL:i->pere->prenom)) && i->pere != NULL){
			return 6;
		}
		if (i->sexe==0 && sexe !=0){
			i->sexe = sexe;
		} else if (i->sexe!=0 && sexe!=0 && sexe!=i->sexe){
			return 7;
		}

	}
	i->date = date;
	i->dateMort = dateMort;
	if(!i->naissance){
        i->naissance = naissance;
	}
	tri_age2(ordreAge);
	tri_date(ordreNaiss);
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
					freeIndividu(i);
					return 2;
				}
		} else if ((iPere = estDansFamille(rac->personne,pere))) {
			if (iPere->sexe == 'm'|| iPere->sexe == 0){
				iPere->sexe = 'm';
				i->pere = iPere;
				trouvePere++;
			} else {
				freeIndividu(i);
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
					freeIndividu(i);
					return 2;
				}
		} else if ((iMere = estDansFamille(rac->personne,mere))) {
			if (iMere->sexe == 'f'|| iMere->sexe == 0){
				iMere->sexe = 'f';
				i->mere = iMere;
				trouveMere++;
			} else {
				freeIndividu(i);
				return 2;
			}
		}
		prec = rac;
		rac = rac->suivant;
	}
	if(!trouvePere && pere!=NULL){
		Individu* nouveau = (Individu*) malloc(sizeof(Individu));
		transformeNom(pere);
		nouveau->prenom = pere;
		nouveau->sexe = 'm';
		nouveau->pere = NULL;
		nouveau->mere = NULL;
		nouveau->date = 0;
		nouveau->dateMort = 0;
		nouveau->naissance = NULL;
		i->pere = nouveau;
		ajouterOrdre(nouveau);
		l->nbIndividu++;
	}else if (trouvePere){
        free(pere);
	}
	if (!trouveMere && mere!=NULL){
		Individu* nouveau = (Individu*) malloc(sizeof(Individu));
		transformeNom(mere);
		nouveau->prenom = mere;
		nouveau->sexe = 'f';
		nouveau->pere = NULL;
		nouveau->mere = NULL;
		nouveau->date = 0;
		nouveau->dateMort = 0;
		nouveau->naissance = NULL;
		i->mere = nouveau;
		ajouterOrdre(nouveau);
		l->nbIndividu++;
	} else if (trouveMere){
        free(mere);
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
			ajouterOrdre(i);
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

void transformeNom(char* prenom){
	/*
	Fonction transformant le prenom en ne lui mettant que la premi�re lettre en majuscule
	*/
	prenom[0] = EnMaj(prenom[0]);
	for (int i =1; prenom[i] != '\0';i++){
		prenom[i] = EnMin(prenom[i]);
	}
}

Individu* estDansFamille(Individu* racine, char* recherche){
	/*
	Fonction recursive qui renvoie si la personne recherch� (renseign� par la chaine de caract�re) est dans la famille de la personne racine
	renvoie l'adresse pointant la personne si trouv�
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
			if ((en_cours = estDansFamille(racine->mere,recherche))){
				return en_cours;
			}
		}
	}
	if (racine->pere != NULL){
		if (estEgaleS(racine->pere->prenom,recherche)){
			return racine->pere;
		} else {
			if ((en_cours = estDansFamille(racine->pere,recherche))){
				return en_cours;
			}
		}
	}
	return NULL;
}

int supprimer(Liste* l, Element* e){
	/*
	Fonction qui supprimer l'element e dans la liste l pass� en param�tre
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

int supprimerSansFree(Liste* l, Element* e){
	/*
	Fonction qui supprimer l'element e dans la liste l pass� en param�tre
	*/
	if (l->premier == e){
		l->premier = e->suivant;
		return 0;
	}
	Element* rac = l->premier;
	Element* prec = NULL;
	while (rac!=NULL){
		if (rac==e){
			prec->suivant = e->suivant;
			return 0;
		}
		prec = rac;
		rac = rac->suivant;
	}
	return 1;
}

void afficheListeChainee(Liste* l){
	/*
	Fonction d'affichage enti�re de la liste chain�e pass� en param�tre
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
	Fonction d'affichage permettant d'affich�e l'arbre de la famille de l'individu pass� en param�tre
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
			fprintf(fichier,"%s:,%s,%s,%ld,%ld,%s\n",i->prenom,(!i->pere?"":getPere(i)),(!i->mere?"":getMere(i)),i->date,i->dateMort,(!i->naissance?"":i->naissance));
		}else {
			fprintf(fichier,"%s:%c,%s,%s,%ld,%ld,%s\n",i->prenom,i->sexe,(!i->pere?"":getPere(i)),(!i->mere?"":getMere(i)),i->date,i->dateMort,(!i->naissance?"":i->naissance));
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
	free(dejaFait);
	fclose(fichier);
}

void viewPersonne(Individu* i,char** dejaFait,int* taille){
	if (!inChar(dejaFait,i->prenom,taille)){
		if (!i->sexe){
			printf("%s:,%s,%s,%ld,%ld,%s\n",i->prenom,(!i->pere?"":getPere(i)),(!i->mere?"":getMere(i)),i->date,i->dateMort,(i->naissance?i->naissance:""));
		}else {
			printf("%s:%c,%s,%s,%ld,%ld,%s\n",i->prenom,i->sexe,(!i->pere?"":getPere(i)),(!i->mere?"":getMere(i)),i->date,i->dateMort,(i->naissance?i->naissance:""));
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
    if(!l->nbIndividu){
        return;
    }
    Element* rac = l->premier;
	char** dejaFait = (char**) malloc(sizeof(char*) * l->nbIndividu);
	int taille=0;
	int* ptaille = &taille;
	while (rac!=NULL){
		viewFamille(rac->personne,dejaFait,ptaille);
		rac=rac->suivant;
	}
	free(dejaFait);
}
void ajouterOrdre(Individu* i){
    ajouterOrdreAge(i);
    ajouterOrdreNaiss(i);
    ajouterOrdreAlpha(i);
}

void ajouterOrdreAge(Individu* i){
    Element2* nouv = (Element2*) malloc(sizeof(Element2));
    nouv->personne = i;
    nouv->prec = NULL;
    nouv->suivant = NULL;
    if (ordreAge->premier == NULL){
        ordreAge->premier = nouv;
        ordreAge->dernier = nouv;
    } else {
        nouv->prec = ordreAge->dernier;
        ordreAge->dernier->suivant = nouv;
        ordreAge->dernier = nouv;
    }
    //tri_age2(ordreAge);
}

void ajouterOrdreNaiss(Individu* i){
    Element2* nouv = (Element2*) malloc(sizeof(Element2));
    nouv->personne = i;
    nouv->prec = NULL;
    nouv->suivant = NULL;
    if (ordreNaiss->premier == NULL){
        ordreNaiss->premier = nouv;
        ordreNaiss->dernier = nouv;
    } else {
        nouv->prec = ordreNaiss->dernier;
        ordreNaiss->dernier->suivant = nouv;
        ordreNaiss->dernier = nouv;
    }
    tri_date(ordreNaiss);
}

void ajouterOrdreAlpha(Individu* i){
    Element2* nouv = (Element2*) malloc(sizeof(Element2));
    nouv->personne = i;
    nouv->prec = NULL;
    nouv->suivant = NULL;
    if (ordreAlpha->premier == NULL){
        ordreAlpha->premier = nouv;
        ordreAlpha->dernier = nouv;
    } else {
        Element2* rac = ordreAlpha->premier;
        if (strcmp(i->prenom,rac->personne->prenom)==-1){
            ordreAlpha->premier->prec = nouv;
            nouv->suivant=ordreAlpha->premier;
            ordreAlpha->premier=nouv;
            return;
        }
        while (rac->suivant!=NULL){
            if (strcmp(i->prenom,rac->suivant->personne->prenom)==-1){
                nouv->suivant = rac->suivant;
                nouv->prec = rac;
                rac->suivant = nouv;
                return;
            }
            rac=rac->suivant;
        }
        nouv->prec = rac;
        rac->suivant = nouv;
        ordreAlpha->dernier = nouv;
    }
}

void test(Liste* l, char* nom, char sexe, char* pere, char* mere,long date,long dateMort,char* naissance){
	/*
	Fonction de test qui r�cup�re les erreurs et les affiche dans la console
	*/
	int codeExec = ajouter(l,nom,sexe,pere,mere,date,dateMort,naissance);
	switch (codeExec){
		case 0:
			printf("Tout s'est bien passe !\n");
			break;
		case 1:
			printf("Probleme avec le sexe !\n");
			break;
		case 2:
			printf("Probleme avec le sexe d'un parent !\n");
			break;
		case 4:
			printf("Probl�me un descendant ne peut pas �tre votre parent\n");
			break;
		case 5:
			printf("Probl�me, vous ajoutez quelqu'un qui est son propre parent ou vous essayez d'ajouter une personne avec la m�me personne en p�re et en m�re\n");
			break;
		case 6:
			printf("Vous ne pouvez pas changer un p�re ou une m�re d�j� fourni\n");
			break;
		case 7:
			printf("Vous ne pouvez pas modifier un sexe d�j� d�fini\n");
			break;
		default:
			printf("WTF");
	}
}

Individu* cherche(Liste* l,char* prenom){
	/*
	Fonction qui � partir d'une chaine pass� en param�tre va renvoyer la Structure Individu correspondante dans la liste pass� en param�tre
	NULL si non trouv�
	*/
	Element* rac = l->premier;
	Individu* i = NULL;
	while (rac!=NULL){
		if(estEgaleS(rac->personne->prenom,prenom)){
			return rac->personne;
		} else if ((i=estDansFamille(rac->personne,prenom))){
			return i;
		}
		rac=rac->suivant;
	}
	return NULL;
}

void afficheInfoS(Liste* l, char* prenom){
	/*
	Affiche les infos de la personne poss�dant la chaine pass� en param�tre comme pr�nom
	*/
	Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        Individu* i = cherche(l,prenom);
        afficheInfo(i);
    }
}
void afficheInfo(Individu* i){
	/*
	Affiche les infos d'un Individu
	Affichage sous la forme :
	Information sur PRENOM:
	Sexe : SEXE
	P�re : PRENOMPERE
	M�re : PRENOMMERE
	Date de naissance : AAAAMMJJ
	*/
	printf("Information sur %s :\n",i->prenom);
	printf("Sexe : ");
	if (i->sexe ==0){
		printf("Non d�fini\n");
	}else {
		printf("%c\n",i->sexe);
	}
	printf("P�re : %s\n",(i->pere==NULL?"Non disponible":i->pere->prenom));
	printf("M�re : %s\n",(i->mere==NULL?"Non disponible":i->mere->prenom));
	printf("Date de naissance : %ld\n",i->date);
	if (i->dateMort){
        printf("Date de d�c�s : %ld\n",i->dateMort);
	} else {
        printf("Date de d�c�s : Encore vivant\n");
	}
	printf("Lieu de naissance : %s\n",(i->naissance==NULL?"Non disponible":i->naissance));
}

void retrouveMere(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie la m�re de la personne dont le pr�nom est celui pass� en param�tre
	*/
	Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        Individu* i = cherche(l,prenom);
        printf("Sa m�re est : %s\n",(getMere(i)==NULL?"Non Disponible":getMere(i)));
    }
}

void retrouvePere(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie le p�re de la personne dont le pr�nom est celui pass� en param�tre
	*/
	Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        Individu* i = cherche(l,prenom);
        printf("Son p�re est : %s\n",(getPere(i)==NULL?"Non Disponible":getPere(i)));
    }
}

void retrouveParent(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie la m�re et le p�re de la personne dont le pr�nom est celui pass� en param�tre
	*/
	Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        retrouvePere(l,prenom);
        retrouveMere(l,prenom);
    }
}

void retrouveGMere(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie la grand m�re de la personne dont le pr�nom est celui pass� en param�tre
	*/
	Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        Individu* i = cherche(l,prenom);
        char* gm1 = getMere(cherche(l,getMere(i)));
        char* gm2 = getMere(cherche(l,getPere(i)));
        printf("Ses grands m�res sont : %s et %s\n",(gm1==NULL?"Non Disponible":gm1),(gm2==NULL?"Non Disponible":gm2));
    }
}

void retrouveGPere(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie le grand p�re de la personne dont le pr�nom est celui pass� en param�tre
	*/
	Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        Individu* i = cherche(l,prenom);
        char* gp1 = getPere(cherche(l,getMere(i)));
        char* gp2 = getPere(cherche(l,getPere(i)));
        printf("Son grand p�re est : %s et %s\n",(gp1==NULL?"Non Disponible":gp1),(gp2==NULL?"Non Disponible":gp2));
    }
}

void retrouveGParent(Liste* l, char* prenom){
	/*
	Fonction d'affichage qui renvoie la grand m�re et le grand p�re de la personne dont le pr�nom est celui pass� en param�tre
	*/
	Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        retrouveGPere(l,prenom);
        retrouveGMere(l,prenom);
    }
}

void afficheAscendant(Individu* i){
    /*
	Fonction d'affichage qui affiche les ascendants de l'individu pass� en parametre
	*/
	if (!(i==NULL)){
		printf("%s\n",i->prenom);
		afficheAscendant(i->pere);
		afficheAscendant(i->mere);
	}
}

void ascendants(Liste* l, char* prenom){
    /*
	Fonction d'affichage qui affiche les ascendants du nom sous forme de chaine de caractere pass� en parametre
	*/
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
		Individu* i = cherche(l,prenom);
		printf("Ces ascendants sont :\n");
		afficheAscendant(i->pere);
		afficheAscendant(i->mere);
    }
}


int creeTEnfant(Individu* i,char* prenom,Individu** tab,int indice){
    /*
	Fonction qui ajoute dans son tableau tab pass� en param�tre les enfants de l'individu prenom
	Pour cela, elle va regarder si les parents de i sont prenom si oui et si ils ne sont pas d�j� � l'interieur, on les ajoute
	*/
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
    /*
    Fonction g�n�rique qui affiche le contenu d'un tableau d'individu
    */
	for (int i=0;i<ind;i++){
		printf("%s\n",tab[i]->prenom);
	}
}

void enfants(Liste* l, char* prenom){
    /*
    Fonction d'affichage qui affiche les enfants de la personne prenom pass� en param�tre
    */
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        Element* rac = l->premier;
        Individu* tab[TAILLE_MINI];
        printf("Enfant de %s :\n",prenom);
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
}



void petitsEnfants(Liste* l, char* prenom){
    /*
    Fonction d'affichage qui affiche les petits enfants de la personne prenom pass� en param�tre
    */
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
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
        free(tab);
        free(tab2);
    }
}
int creeTDescendant(Individu* i, char* prenom, Individu** tab, int indice){
    /*
	Fonction qui ajoute dans son tableau tab pass� en param�tre les descendants de l'individu prenom
	Pour cela, elle va regarder si prenom est dans la famille de l'individu si oui et si il n'est pas d�j� � l'interieur, on les ajoute
	*/
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
    /*
    Fonction d'affichage qui affiche les descendants de la personne prenom pass� en param�tre
    */
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
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
}


void partenaires(Liste* l, char* prenom){
    /*
    Fonction d'affichage qui affiche les partenaires de la personne prenom pass� en param�tre
    */
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        Element* rac = l->premier;
        Individu** tab = malloc(sizeof(Individu*)*TAILLE_MINI);
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
                    Individu* existe = tab[i]->mere;
                    if (tab[i]->mere){
                        if (!(inIndividu(dejaFait,tab[i]->mere->prenom,indDejaFait))){
                            dejaFait[indDejaFait++]=tab[i]->mere;
                        }
                    }
                }
            } else {
                for (int i = 0;i<indice;i++){
                    if (tab[i]->pere){
                        if (!(inIndividu(dejaFait,tab[i]->pere->prenom,indDejaFait))){
                            dejaFait[indDejaFait++]=tab[i]->pere;
                        }
                    }
                }
            }
            if(indDejaFait){
                afficheTab(dejaFait,indDejaFait);
            } else {
                printf("Pas de partenaires connus !\n");
            }
        } else {
            printf("Pas de partenaires connus !\n");
        }
        free(tab);
    }
}
int supprimeTab(Individu** tab,int indiceASupp,int taille){
    /*
    Fonction qui supprime l'element indiceASupp d'un tableau tab de taille taille
    */
    tab[indiceASupp] = tab[taille-1];
    tab[taille-1] = NULL;
    return taille-1;
}

int creeTAffilie(Liste* l, char* prenom, Individu** tab, int indice,char sexe,int demi){
    /*
	Fonction qui ajoute dans son tableau tab pass� en param�tre les personnes de m�me parent de l'individu prenom
	Cette fonction prend 2 param�tres sp�ciaux qui sont demi et sexe
	Sexe est soit m ou f et defini si on cherche un frere ou une soeur
	demi quand � lui sert � d�finir si l'on veut les freres/soeurs ou les demi freres/soeurs, 1 pour demi,0 sans
	*/
	Individu* test = cherche(l,prenom);
	Individu* i;
	char* prenomMDemi=NULL;
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


void affilie(Liste* l, char* prenom,char sexe,int demi){
    /*
    Fonction d'affichage qui affiche les freres/soeurs/demi freres/demi soeurs de la personne prenom pass� en param�tre en fonction des param�tres
    */
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
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
}
int creeTAffilieParent(Liste* l,Individu* test,Individu** tab,int indice,char sexe){
    /*
	Fonction qui ajoute dans son tableau tab pass� en param�tre les oncles/tantes de l'individu prenom
	Cette fonction prend un param�tre sp�cial qui est sexe
	Sexe est soit m ou f et defini si on cherche une tante ou un oncle
	*/
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

int compteGeneration(Individu* i,int gen){
	if (i==NULL){
		return gen;
	} else {
		int genP = compteGeneration(i->pere,gen+1);
		int genM = compteGeneration(i->mere,gen+1);
		return (genP>genM?genP:genM);
	}
}

void ascendance(Liste* l, char* prenom){
	Individu* i = cherche(l,prenom);
	if (!i){
		printf("La personne donn�e n'existe pas !\n");
	} else {
		int genP = compteGeneration(i->mere,0);
		int genM = compteGeneration(i->pere,0);
		int res = (genP>genM?genP:genM);
		printf("%s est issu de %d g�n�rations\n",prenom,res);
	}
}

void tailleArbre(Liste*l){
	Element* rac = l->premier;
	int res = 0;
	while (rac!=NULL){
		int enCours = compteGeneration(rac->personne,0);
		if (enCours > res){
			res = enCours;
		}
		rac = rac->suivant;
	}
	printf("La taille de l'arbre est %d\n",res);
}

void affilieParent(Liste* l, char* prenom,char sexe){
    /*
    Fonction d'affichage qui affiche les oncles/tantes de la personne prenom pass� en param�tre en fonction des param�tres
    */
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
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
    /*
    Fonction d'affichage qui affiche les cousins de la personne prenom pass� en param�tre
    */
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
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
    /*
    Fonction qui vide le buffer.
    Peut �tre inutile mais permet au cas o� de ne pas g�n�rer de bugs.
    */
    int c;
    while((c=getchar()) != EOF && c != '\n');
}

int recense_famille(Individu* i, char sexe,char** dejaFait,int taille){
	if(i==NULL){
		return taille;
	} else {
		if (i->sexe == sexe && !inChar(dejaFait,i->prenom,&taille)){
			printf("%s\n",i->prenom);
			dejaFait[taille++] = i->prenom;
		}
		taille = recense_famille(i->pere,sexe,dejaFait,taille);
		taille = recense_famille(i->mere,sexe,dejaFait,taille);
		return taille;
	}
}

void recense(Liste* l, char c){
	if (!estSexeValide(c)){
		printf("Le sexe donn�e n'est pas valide");
	} else {
		Element* rac = l->premier;
		char** dejaFait = malloc(sizeof(char*)*TAILLE_MINI);
		int taille = 0;
		while (rac!=NULL){
			taille  = recense_famille(rac->personne,c,dejaFait,taille);
			rac = rac->suivant;
		}
		free(dejaFait);
	}
}

int creeTAscendant(Individu* i,Individu** tab,int taille){
	if (i==NULL){
		return taille;
	}
	tab[taille++] = i;
	taille = creeTAscendant(i->pere,tab,taille);
	taille = creeTAscendant(i->mere,tab,taille);
	return taille;
}

int ontLien(Individu** tab1,int taille1,char* prenom1, Individu** tab2,int taille2,char* prenom2){
	for (int i =0;i<taille1;i++){
		for (int j = 0;j<taille2;j++){
			if(estEgaleI(tab1[i],tab2[j])){
				return 1;
			}
		}
	}
	return 0;
}

void lien(Liste* l, char* p1,char* p2){
	Individu* i1 = cherche(l,p1);
	Individu* i2 = cherche(l,p2);
	if (!i1 || !i2){
		printf("Pas de lien de parent� !\n");
	} else {
		int taille1=0;
		int taille2=0;
		Individu** ancetreP1 = (Individu**) malloc(sizeof(Individu));
		taille1 = creeTAscendant(i1,ancetreP1,taille1);
		Individu** ancetreP2 = (Individu**) malloc(sizeof(Individu));
		taille2 = creeTAscendant(i2,ancetreP2,taille2);
		if (ontLien(ancetreP1,taille1,p1,ancetreP2,taille2,p2)){
			printf("Ils sont parent�s\n");
		} else {
			printf("Ils ne sont pas parent�s\n");
		}
		free(ancetreP1);
		free(ancetreP2);
	}
}

void freeNaiss(char* prenom){
    Element2* rac = ordreNaiss->premier;
    if (estEgaleS(prenom,rac->personne->prenom)){
        ordreNaiss->premier = ordreNaiss->premier->suivant;
        if (ordreNaiss->premier){
            ordreNaiss->premier->prec = NULL;
        }
        if (estEgaleS(ordreNaiss->dernier->personne->prenom,prenom)){
            ordreNaiss->dernier = ordreNaiss->premier;
        }
        free(rac);
        return;
    }
    while (rac->suivant!=NULL){
        if (estEgaleS(prenom,rac->suivant->personne->prenom)){
            Element2* aSuppr = rac->suivant;
            rac->suivant = rac->suivant->suivant;
            if(rac->suivant){
                rac->suivant->prec = rac;
            }
            if (estEgaleS(ordreNaiss->dernier->personne->prenom,prenom)){
                ordreNaiss->dernier = rac;
            }
            free(aSuppr);
            return;
        }
        rac=rac->suivant;
    }
}

void freeAge(char* prenom){
    Element2* rac = ordreAge->premier;
    if (estEgaleS(prenom,rac->personne->prenom)){
        ordreAge->premier = ordreAge->premier->suivant;
        if (ordreAge->premier){
            ordreAge->premier->prec = NULL;
        }
        if (estEgaleS(ordreAge->dernier->personne->prenom,prenom)){
            ordreAge->dernier = ordreAge->premier;
        }
        free(rac);
        return;
    }
    while (rac->suivant!=NULL){
        if (estEgaleS(prenom,rac->suivant->personne->prenom)){
            Element2* aSuppr = rac->suivant;
            rac->suivant = rac->suivant->suivant;
            if(rac->suivant){
                rac->suivant->prec = rac;
            }
            if (estEgaleS(ordreAge->dernier->personne->prenom,prenom)){
                ordreAge->dernier = rac;
            }
            free(aSuppr);
            return;
        }
        rac=rac->suivant;
    }
}

void freeAlpha(char* prenom){
    Element2* rac = ordreAlpha->premier;
    if (estEgaleS(prenom,rac->personne->prenom)){
        ordreAlpha->premier = ordreAlpha->premier->suivant;
        if (ordreAlpha->premier){
            ordreAlpha->premier->prec = NULL;
        }
        if (estEgaleS(ordreAlpha->dernier->personne->prenom,prenom)){
            ordreAlpha->dernier = ordreAlpha->premier;
        }
        free(rac);
        return;
    }
    while (rac->suivant!=NULL){
        if (estEgaleS(prenom,rac->suivant->personne->prenom)){
            Element2* aSuppr = rac->suivant;
            rac->suivant = rac->suivant->suivant;
            if(rac->suivant){
                rac->suivant->prec = rac;
            }
            if (estEgaleS(ordreAlpha->dernier->personne->prenom,prenom)){
                ordreAlpha->dernier = rac;
            }
            free(aSuppr);
            return;
        }
        rac=rac->suivant;
    }
}

void del(Liste* l, char* prenom){
	Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
		if (test->pere != NULL || test->mere != NULL){
			printf("Suppression impossible ! Veuillez supprimer les parents d'abord !\n");
			return;
		} else {
			Individu** tab = (Individu**) malloc(sizeof(Individu*)*TAILLE_MINI);
			int indice = 0;
			Element* rac = l->premier;
			if (rac!=NULL){
                if(estEgaleS(rac->personne->prenom,prenom)){
                    l->premier = rac->suivant;
                    freeElement(rac);
                    free(tab);
                    return;
                }
			}
			while (rac!=NULL){
                if (rac->suivant!=NULL){
                    if (estEgaleS(rac->suivant->personne->prenom,prenom)){
                        Element* aSuppr = rac->suivant;
                        rac->suivant = rac->suivant->suivant;
                        freeElement(aSuppr);
                        free(tab);
                        return;
                    }
                }
				indice = creeTEnfant(rac->personne,prenom,tab,indice);
				rac = rac->suivant;
			}
			for (int i =0;i<indice;i++){
				if (test->sexe == 'm'){
					tab[i] -> pere = NULL;
				} else {
					tab[i] -> mere = NULL;
				}
			}
			free(tab);
			freeNaiss(prenom);
			freeAge(prenom);
			freeAlpha(prenom);
			freeIndividu(test);
		}
	}
}

Element* chercheMin(Liste* l){
    Element* rac = l->premier;
    if (rac==NULL){
        return NULL;
    }
    Element* min = l->premier;
    while (rac!=NULL){
        if (min->personne->date > rac->personne->date){
            min=rac;
        }
        rac=rac->suivant;
    }
    return min;
}

Element2* chercheMin2(Liste2* l){
    Element2* rac = l->premier;
    if (rac==NULL){
        return NULL;
    }
    Element2* min = l->premier;
    while (rac!=NULL){
        if (min->personne->date > rac->personne->date){
            min=rac;
        }
        rac=rac->suivant;
    }
    return min;
}

Element2* chercheMin3(Liste2* l){
    Element2* rac = l->premier;
    if (rac==NULL){
        return NULL;
    }
    struct tm* info;
    time_t temps;
    time(&temps);
    info = localtime(&temps);
    long actuel = info->tm_mday + 100* (info->tm_mon+1)+10000*(info->tm_year+1900);
    int ageMin;
    if (rac->personne->dateMort){
        ageMin = rac->personne->dateMort - rac->personne->date;
    } else {
        ageMin = actuel - rac->personne->date;
    }

    Element2* min = l->premier;
    int ageEnCours;
    while (rac!=NULL){
        if (rac->personne->dateMort){
            ageEnCours = rac->personne->dateMort - rac->personne->date;
        } else {
            ageEnCours = actuel - rac->personne->date;
        }
        if (ageEnCours < ageMin){
            min=rac;
            ageMin=ageEnCours;
        }
        rac=rac->suivant;
    }
    return min;
}

void ajouteListeChainee2(Liste2* l, Element2* e){
    e->suivant=NULL;
    if (l->premier == NULL){
        e->prec = NULL;
        l->premier = e;
        l->dernier = e;
    } else {
        e->prec = l->dernier;
        l->dernier->suivant = e;
        l->dernier = e;
    }
}

int supprimerSansFree2(Liste2* l, Element2* e){
	/*
	Fonction qui supprimer l'element e dans la liste l pass� en param�tre
	*/
	if (l->premier == e){
		l->premier = e->suivant;
		return 0;
	}
	Element2* rac = l->premier;
	Element2* prec = NULL;
	while (rac!=NULL){
		if (rac==e){
			prec->suivant = e->suivant;
			return 0;
		}
		prec = rac;
		rac = rac->suivant;
	}
	return 1;
}

void tri_date(Liste2* l){
    Liste2* nouv = (Liste2*) malloc(sizeof(Liste2));
    nouv->premier=NULL;
    Element2* min;
    while(l->premier!=NULL){
        min = chercheMin2(l);
        supprimerSansFree2(l,min);
        ajouteListeChainee2(nouv,min);
    }
    *l=*nouv;
    free(nouv);
}

void tri_age2(Liste2* l){
    Liste2* nouv = (Liste2*) malloc(sizeof(Liste2));
    nouv->premier=NULL;
    Element2* min;
    while(l->premier!=NULL){
        min = chercheMin3(l);
        supprimerSansFree2(l,min);
        ajouteListeChainee2(nouv,min);
    }
    *l=*nouv;
    free(nouv);
}

void ajouteListeChainee(Liste* l, Element* e){
    e->suivant=NULL;
    if (!l->premier){
        l->premier=e;
    } else {
        Element* rac = l->premier;
        while (rac->suivant!=NULL){
            rac=rac->suivant;
        }
        rac->suivant=e;
    }
}

void tri_age(Liste* l){
    Liste* nouv = (Liste*) malloc(sizeof(Liste));
    nouv->nbIndividu = l->nbIndividu;
    nouv->premier=NULL;
    Element* min;
    while(l->premier!=NULL){
        min = chercheMin(l);
        supprimerSansFree(l,min);
        ajouteListeChainee(nouv,min);
    }
    *l=*nouv;
    free(nouv);
}

void deces(Liste* l, char* prenom,long date){
    Individu* test = cherche(l,prenom);
    if (!(test)){
        printf("La personne donn�e n'existe pas !\n");
    } else {
        test->dateMort = date;
        tri_age2(ordreAge);
    }
}

void aines(long age){
    age = age*10000;
    Element2* rac = ordreAge->dernier;
    if (rac==NULL){
        printf("Il n'y a pas de personne plus ag� !");
        return;
    }
    struct tm* info;
    time_t temps;
    time(&temps);
    info = localtime(&temps);
    long actuel = info->tm_mday + 100* (info->tm_mon+1)+10000*(info->tm_year+1900);
    long ageActu;
    if (rac->personne->dateMort){
        ageActu = rac->personne->dateMort - rac->personne->date;
    } else {
        ageActu = actuel - rac->personne->date;
    }
    while (rac!=NULL && ageActu >= age){
        if (rac->personne->date){
            printf("%s\n",rac->personne->prenom);
        }
        rac=rac->prec;
        if (rac!=NULL){
            if (rac->personne->dateMort){
                ageActu = rac->personne->dateMort - rac->personne->date;
            } else {
                ageActu = actuel - rac->personne->date;
            }
        }
    }
}

void benjamins(int age){
    age = age*10000;
    Element2* rac = ordreAge->premier;
    if (rac==NULL){
        printf("Il n'y a pas de personne plus ag� !");
        return;
    }
    struct tm* info;
    time_t temps;
    time(&temps);
    info = localtime(&temps);
    long actuel = info->tm_mday + 100* (info->tm_mon+1)+10000*(info->tm_year+1900);
    long ageActu;
    if (rac->personne->dateMort){
        ageActu = rac->personne->dateMort - rac->personne->date;
    } else {
        ageActu = actuel - rac->personne->date;
    }
    printf("AGE ACTUEL : %ld Age max : %d\n",ageActu,age);
    while (rac!=NULL && ageActu < age){
        if (rac->personne->date){
            printf("%s\n",rac->personne->prenom);
        }
        rac=rac->suivant;
        if (rac!=NULL){
            if (rac->personne->dateMort){
                ageActu = rac->personne->dateMort - rac->personne->date;
            } else {
                ageActu = actuel - rac->personne->date;
            }
        }
    }
}

void chronologie(){
    Element2* rac = ordreNaiss->premier;
    while(rac!=NULL){
        if (rac->personne->date){
            printf("%s\n",rac->personne->prenom);
        }
        rac=rac->suivant;
    }
}

void remonteTemps(){
    Element2* rac = ordreNaiss->dernier;
    while(rac!=NULL){
        if (rac->personne->date){
            printf("%s\n",rac->personne->prenom);
        }
        rac=rac->prec;
    }
}

void annuaire(){
    Element2* rac = ordreAlpha->premier;
    while(rac!=NULL){
        printf("%s\n",rac->personne->prenom);
        rac=rac->suivant;
    }
}

int lanceCommande(Liste** l,char* fonction, char* parametre){
    /*
	Fonction qui � partir du nom de fonction et des param�tres, lance les fonctions correspondantes
	Cette fonction s'occupe de d�couper les param�tres si besoin (exemple fonction new)
	*/
    if (estEgaleS(fonction,"load")){
        if(!strlen(parametre)) {
            printf("La fonction load necessite un param�tre qui correspond au fichier dans lequel se trouve la famille.\n");
        } else {
            Liste* l2 = load(parametre);
            if (l2){
                **l = *l2;
                free(l2);
            }
        }
    } else if (estEgaleS(fonction,"save")){
        if(!strlen(parametre)) {
            printf("La fonction save necessite un param�tre qui correspond au fichier dans lequel la famille sera enregistr�.\n");
        } else {
            save(*l,parametre);
        }
    } else if (estEgaleS(fonction,"view")){
        view(*l);
    } else if (estEgaleS(fonction,"chronologie")){
        chronologie();
    } else if (estEgaleS(fonction,"remonteTemps")){
        remonteTemps();
    } else if (estEgaleS(fonction,"annuaire")){
        annuaire();
    } else if (estEgaleS(fonction,"exit")){
        return 1;
    } else if (estEgaleS(fonction,"new")){
        if(!strlen(parametre)) {
            printf("La fonction new necessite au moins un param�tre pr�nom et au plus 6 param�tres qui sont prenom, sexe , pere , mere, date naissance et date mort dans cette ordre.\n");
        } else {

            char* prenom = (char*) malloc(sizeof(char)*200);
            char sexe;
            char* pere;
            char* mere;
            char* date;
            char* dateMort;
            char* naissance;
            int taillemax = strlen(parametre);
            int indCha = 0;
            int ind = 0;
            while (ind<taillemax && parametre[ind] != ','){
                prenom[indCha++]=parametre[ind++];
            }
            prenom[indCha]='\0';
            if (ind>=taillemax){
                test(*l,prenom,0,NULL,NULL,0,0,NULL);
                return 0;
            }
            ind++;
            if (!(parametre[ind]==',' || ind >= taillemax)){
                sexe = parametre[ind++];
                ind++;
            } else {
                sexe = 0;
                ind++;
            }
            indCha = 0;
            if (!(parametre[ind]==','|| ind >= taillemax)){
                pere = (char*) malloc(sizeof(char)*300);
                while (ind<taillemax && parametre[ind] != ','){
                    pere[indCha++]=parametre[ind++];
                }
                pere[indCha]='\0';
            } else {
                pere = NULL;
            }
            ind++;
            indCha = 0;
            if (ind >= taillemax||parametre[ind]==','){
                mere = NULL;
            } else {
                mere = (char*) malloc(sizeof(char)*400);
                while (ind<taillemax && parametre[ind] != ','){
                    mere[indCha++]=parametre[ind++];
                }
                mere[indCha]='\0';
            }

            ind++;
            indCha = 0;
            if (ind >= taillemax||parametre[ind]==','){
                date = "0";
            } else {
                date = (char*) malloc(sizeof(char)*250);
                while (ind<taillemax && parametre[ind] != ','){
                    date[indCha++]=parametre[ind++];
                }
                date[indCha]='\0';
            }
            char* fin;
            long dateL = strtol(date,&fin,10);
            if (dateL){
                free(date);
            }
            indCha=0;
            ind++;
            if (ind >= taillemax||parametre[ind]==','){
                dateMort = "0";
            } else {
                dateMort = (char*) malloc(sizeof(char)*350);
                while (ind<taillemax && parametre[ind] != ','){
                    dateMort[indCha++]=parametre[ind++];
                }
                dateMort[indCha]='\0';
            }
            long dateMortL = strtol(dateMort,&fin,10);
            if (dateMortL){
                free(dateMort);
            }
            ind++;
            indCha = 0;
            if (ind >= taillemax||parametre[ind]==','){
                naissance = NULL;
            } else {
                naissance = (char*) malloc(sizeof(char)*450);
                while (ind<taillemax && parametre[ind] != ','){
                    naissance[indCha++]=parametre[ind++];
                }
                naissance[indCha]='\0';
            }
            test(*l,prenom,sexe,pere,mere,dateL,dateMortL,naissance);
            tri_age(*l);
        }
    } else if (estEgaleS(fonction,"info")){
        if(!strlen(parametre)) {
            printf("La fonction info necessite un param�tre qui correspond la personne pour lequel on recherche les infos.\n");
        } else {
            afficheInfoS(*l,parametre);
        }
    } else if (estEgaleS(fonction,"mere")){
        if(!strlen(parametre)) {
            printf("La fonction mere necessite un param�tre qui correspond la personne pour lequel on recherche la mere.\n");
        } else {
            retrouveMere(*l,parametre);
        }
    }else if (estEgaleS(fonction,"aines")){
        if(!strlen(parametre)) {
            printf("La fonction aines necessite un param�tre qui corresponde � l'�ge minimum � atteindre\n");
        } else {
            char* fin;
            long age = strtol(parametre,&fin,10);
            aines(age);
        }
    }else if (estEgaleS(fonction,"benjamins")){
        if(!strlen(parametre)) {
            printf("La fonction benjamins necessite un param�tre qui corresponde � l'�ge maximum � atteindre\n");
        } else {
            char* fin;
            long age = strtol(parametre,&fin,10);
            benjamins(age);
        }
    } else if (estEgaleS(fonction,"pere")){
        if(!strlen(parametre)) {
            printf("La fonction pere necessite un param�tre qui correspond la personne pour lequel on recherche le pere.\n");
        } else {
            retrouvePere(*l,parametre);
        }
    } else if (estEgaleS(fonction,"parents")){
        if(!strlen(parametre)) {
            printf("La fonction parents necessite un param�tre qui correspond la personne pour lequel on recherche les parents.\n");
        } else {
            retrouveParent(*l,parametre);
        }
    } else if (estEgaleS(fonction,"gdmeres")){
        if(!strlen(parametre)) {
            printf("La fonction gdmeres necessite un param�tre qui correspond la personne pour lequel on recherche les grands meres.\n");
        } else {
            retrouveGMere(*l,parametre);
        }
    } else if (estEgaleS(fonction,"gdperes")){
        if(!strlen(parametre)) {
            printf("La fonction gdperes necessite un param�tre qui correspond la personne pour lequel on recherche les grands peres.\n");
        } else {
            retrouveGPere(*l,parametre);
        }
    }else if (estEgaleS(fonction,"ascendance")){
        if(!strlen(parametre)) {
            printf("La fonction ascendance necessite un param�tre qui correspond la personne pour lequel on recherche le nombre d'ascendance.\n");
        } else {
            ascendance(*l,parametre);
        }
    } else if (estEgaleS(fonction,"gdparents")){
        if(!strlen(parametre)) {
            printf("La fonction gdparents necessite un param�tre qui correspond la personne pour lequel on recherche les grands parents.\n");
        } else {
            retrouveGParent(*l,parametre);
        }
    } else if (estEgaleS(fonction,"ascendants")){
        if(!strlen(parametre)) {
            printf("La fonction ascendants necessite un param�tre qui correspond la personne pour lequel on recherche les ascendants.\n");
        } else {
            ascendants(*l,parametre);
        }
    }else if (estEgaleS(fonction,"del")){
        if(!strlen(parametre)) {
            printf("La fonction del necessite un param�tre qui correspond la personne � supprimer.\n");
        } else {
            del(*l,parametre);
        }
    }else if (estEgaleS(fonction,"taille_arbre")){
        tailleArbre(*l);
    }else if (estEgaleS(fonction,"tri_age")){
        tri_age(*l);
    } else if (estEgaleS(fonction,"enfants")){
        if(!strlen(parametre)) {
            printf("La fonction enfants necessite un param�tre qui correspond la personne pour lequel on recherche les enfants.\n");
        } else {
            enfants(*l,parametre);
        }
    }else if (estEgaleS(fonction,"lien")){
        if(!strlen(parametre)) {
            printf("La fonction lien necessite deux param�tres qui corresponde aux deux personnes du potentiel lien\n");
        } else {
            char* prenom1 = (char*) malloc(sizeof(char)*TAILLE_MINI);
            char* prenom2 = (char*) malloc(sizeof(char)*TAILLE_MINI);
            int ind = 0;
            int indP1 = 0;
            int indP2 = 0;
            while (parametre[ind]!=','){
				prenom1[indP1] = parametre[ind];
				ind++;
				indP1++;
			}
			prenom1[indP1] = '\0';
			ind++;
			while(parametre[ind] != '\0'){
				prenom2[indP2] = parametre[ind];
				ind++;
				indP2++;
			}
			prenom2[indP2] = '\0';
			lien(*l,prenom1,prenom2);
			free(prenom1);
			free(prenom2);
        }
    }else if (estEgaleS(fonction,"deces")){
        if(!strlen(parametre)) {
            printf("La fonction deces necessite deux param�tres prenom et nouvelleDate qui correspondent � la personne qe l'on souhaite modifier et � la nouvelle date � assigner\n");
        } else {
            char* prenom = (char*) malloc(sizeof(char)*TAILLE_MINI);
            char* nouvelleDate = (char*) malloc(sizeof(char)*TAILLE_MINI);
            int ind = 0;
            int indP1 = 0;
            int indP2 = 0;
            while (parametre[ind]!=','){
				prenom[indP1] = parametre[ind];
				ind++;
				indP1++;
			}
			prenom[indP1] = '\0';
			ind++;
			while(parametre[ind] != '\0'){
				nouvelleDate[indP2] = parametre[ind];
				ind++;
				indP2++;
			}
			nouvelleDate[indP2] = '\0';
			char* fin;
			long nouvDate = strtol(nouvelleDate,&fin,10);
			deces(*l,prenom,nouvDate);
			free(prenom);
			free(nouvelleDate);
        }
    } else if (estEgaleS(fonction,"petitsenfants")){
        if(!strlen(parametre)) {
            printf("La fonction petitsenfants necessite un param�tre qui correspond la personne pour lequel on recherche les petits enfants.\n");
        } else {
            petitsEnfants(*l,parametre);
        }
    }else if (estEgaleS(fonction,"recense_genre")){
        if(!strlen(parametre)) {
            printf("La fonction recense necessite un param�tre qui correspond au sexe � recenser\n");
        } else {
            recense(*l,parametre[0]);
        }
    } else if (estEgaleS(fonction,"descendants")){
        if(!strlen(parametre)) {
            printf("La fonction descendants necessite un param�tre qui correspond la personne pour lequel on recherche les descendants.\n");
        } else {
            descendants(*l,parametre);
        }
    } else if (estEgaleS(fonction,"partenaires")){
        if(!strlen(parametre)) {
            printf("La fonction partenaires necessite un param�tre qui correspond la personne pour lequel on recherche les partenaires.\n");
        } else {
            partenaires(*l,parametre);
        }
    } else if (estEgaleS(fonction,"freres")){
        if(!strlen(parametre)) {
            printf("La fonction freres necessite un param�tre qui correspond la personne pour lequel on recherche les freres.\n");
        } else {
            affilie(*l,parametre,'m',0);
        }
    } else if (estEgaleS(fonction,"soeurs")){
        if(!strlen(parametre)) {
            printf("La fonction soeurs necessite un param�tre qui correspond la personne pour lequel on recherche les soeurs.\n");
        } else {
            affilie(*l,parametre,'f',0);
        }
    } else if (estEgaleS(fonction,"demifreres")){
        if(!strlen(parametre)) {
            printf("La fonction demifreres necessite un param�tre qui correspond la personne pour lequel on recherche les demi freres.\n");
        } else {
            affilie(*l,parametre,'m',1);
        }
    } else if (estEgaleS(fonction,"demisoeurs")){
        if(!strlen(parametre)) {
            printf("La fonction demisoeurs necessite un param�tre qui correspond la personne pour lequel on recherche les demisoeurs.\n");
        } else {
            affilie(*l,parametre,'f',1);
        }
    } else if (estEgaleS(fonction,"oncles")){
        if(!strlen(parametre)) {
            printf("La fonction oncles necessite un param�tre qui correspond la personne pour lequel on recherche les oncles.\n");
        } else {
            affilieParent(*l,parametre,'m');
        }
    } else if (estEgaleS(fonction,"tantes")){
        if(!strlen(parametre)) {
            printf("La fonction tantes necessite un param�tre qui correspond la personne pour lequel on recherche les tantes.\n");
        } else {
            affilieParent(*l,parametre,'f');
        }
    } else if (estEgaleS(fonction,"cousins")){
        if(!strlen(parametre)) {
            printf("La fonction cousins necessite un param�tre qui correspond la personne pour lequel on recherche les cousins.\n");
        } else {
            cousins(*l,parametre);
        }
    } else {
        printf("La commande n'existe pas !");
    }
    return 0;
}

int parseCommande(Liste** l){
    /*
	Fonction de parsing des lignes de commandes
	Cette fonction se charge de d�couper la commande en 2 sous chaines fonction et parametre
	*/
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
    while(indBuffer<tailleEntree && buffer[indBuffer]!=')'){
        parametre[indCha]=buffer[indBuffer];
        indBuffer++;
        indCha++;
    }
    parametre[indCha]='\0';
    printf("Fonction %s(%s)\n",fonction,parametre);
    return lanceCommande(l,fonction,parametre);
}

void afficheListe2(Liste2* l){
    Element2* rac = l->premier;
    printf("DEBUT-");
    while (rac!= NULL){
        printf("[%s]-",rac->personne->prenom);
        rac=rac->suivant;
    }
    printf("NULL\n");
}

void freeFamille(Liste* l, Individu* i){
    if (i->pere){
        freeFamille(l,i->pere);
    }
    if (i->mere){
        freeFamille(l,i->mere);
    }
    Individu** tab = (Individu**) malloc(sizeof(Individu*)*TAILLE_MINI);
    int indice = 0;
    Element* rac = l->premier;
    while (rac!=NULL){
        indice = creeTEnfant(rac->personne,i->prenom,tab,indice);
        rac = rac->suivant;
    }
    for (int j =0;j<indice;j++){
        if (i->sexe == 'm'){
            tab[j] -> pere = NULL;
        } else {
            tab[j] -> mere = NULL;
        }
    }
    free(tab);
    freeIndividu(i);
}

void freeListe(Liste* l){
    Element* aSuppr;
    while (l->premier!=NULL){
        aSuppr = l->premier;
        l->premier = l->premier->suivant;
        freeFamille(l,aSuppr->personne);
        free(aSuppr);
    }
    free(l);
}

void freeListe2(Liste2* l){
    Element2* aSuppr;
    while(l->premier!=NULL){
        aSuppr = l->premier;
        l->premier = l->premier->suivant;
        free(aSuppr);
    }
    free(l);
}

void cleanAll(Liste* l){
    freeListe(l);
    freeListe2(ordreAge);
    freeListe2(ordreAlpha);
    freeListe2(ordreNaiss);
}

int main(int argc, char* argv[]){
	/*
	Fonction principal du module
	*/
	Liste* l = initListe();
	ordreAge = initListe2();
	ordreAlpha = initListe2();
	ordreNaiss = initListe2();
	int fini = 0;
	while (!fini){
        fini = parseCommande(&l);
        printf("\n");
	}
	cleanAll(l);
}

