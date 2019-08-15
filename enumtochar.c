/* ----- enumtochar.c ----- */
/* Functions that put in char the enum */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "enumtochar.h"

char* typetochar(Type t) {
  /*returns the Type element t in char* */
  char *s;
  switch(t){
  case NOISY:
    s= "Noisy";
    break;
  case LAZY:
    s= "Lazy";
    break;
  case MOTIVATED:
    s= "Motivated";
    break;
  case TEACHER:
    s= "Teacher";
    break;
  default:
    printf("erreur");
    break;
  }
  return s;
}

char* varietytochar(Variety v){
  /*returns the Variety element t in char* */
  char *s;
  switch(v){
  case PARLFOR:
    s= "Parlfor";
    break;
  case ISMAR:
    s= "Ismar";
    break;
  case RIGOLAMOR:
    s= "Rigolamor";
    break;
  case PROCRASTINO:
    s= "Procrastino";
    break;
  case COUCHTAR:
    s= "Couchtar";
    break;
  case NUIDEBOU:
    s= "Nuidebou";
    break;
  case BUCHAFON:
    s= "Buchafon";
    break;
  case BELMENTION:
    s= "Belmention";
    break;
  case PROMOMAJOR:
    s= "Promomajor";
    break;
  case EDRESSEUR:
    s= "Enseignant-dresseur";
    break;
  default:
    printf("erreur");
    break;
  }
  return s;
}

Variety charToVariety(char name[MAXCHAR]){
	/* returns variety of name */
	char *names[] = {"PARLFOR","ISMAR","RIGOLAMOR","PROCRASTINO","COUCHTAR","NUIDEBOU","BUCHAFON","BELMENTION","PROMOMAJOR","EDRESSEUR"};
	int i;
	for(i=0;i<NBVAR;i++){
		if (!strcmp(names[i],name)){
			return i;
		}
	}
	printf("error");
	return -1;
}

char* attacktochar(AttName a){
  /*returns a name of an attack in char* */
  char *s;
  switch(a){
  case BAVARDAGE:
    s= "Bavardage";
    break;
  case GROBOUCAN:
    s= "Groboucan";
    break;
  case MEGAV:
    s= "Mégaphone-vocal";
    break;
  case BAILLEMENT:
    s= "Baîllement";
    break;
  case PTISOM:
    s= "Pti'Somme";
    break;
  case SDODO:
    s= "Superdodo";
    break;
  case OBOULO:
    s= "Oboulo";
    break;
  case EXOMAI:
    s= "Exo-Maison";
    break;
  case MAXREVIZ:
    s= "Max-Reviz";
    break;
  case TITQ:
    s= "Tit'Question";
    break;
  case POSERCO:
    s= "Poser-colle";
    break;
  case FATALINT:
    s= "Fatal-interro";
    break;
  default:
    printf("error");
    break;
  }
  return s;
}
