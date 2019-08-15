/* --- request.c --- */
/* handle the request part */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "enumtochar.h"
#include "team.h"
#include "pokecafet.h"
#include "fightMain.h"
#include "map.h"
#include "request.h"

char *reqChar[NBREQ] = {"k","wild","rival","nurse","show","switch","move-table","drop","pick","release","catch","xp","z","d","q","s"};

int canFight(Team *team){
	/* return 1 if a team can fight */
	int i;
	if (team->pos == 0) return 0;
	for(i=0;i<team->pos;i++){
		if(team->pe[i]->HP>0){
			return 1;
		}
	}
	return 0;
}

int moveMap(Map *map,Team* myTeam, Pokecafeteriat* pokecafet,Attack tabAtt[NBTYPATT]){
	/* treat the consequences of movment in the map, if it returns 0,
	the game has to stop */
	int val=atoi(map->tab[map->pos[0]][map->pos[1]]),nb,wildOrNot,answer,c,i;

	if (val<=LVLMAX && val >= 1){
		/* if the cell is a wild zone :*/
		wildOrNot=rand()%100;
		if (wildOrNot < 20){
			displayMap(map);
			if(canFight(myTeam)){
				printf("A wild poketudiant came out !");
				wild(myTeam,pokecafet,val,val,tabAtt);
				if (!canFight(myTeam)){
					return 0;
				}
			} else {
				printf("You met a wild poketudiant but you don't have any poketudiant available in your team ! You can't fight !\n");
			}
		}
	} else {
		if(map->tab[map->pos[0]][map->pos[1]][0] == 'c'){
			/* if the cell is a member of the "conseil des diplomes" :*/
			nb=atoi(&map->tab[map->pos[0]][map->pos[1]][1])-1;
			displayMap(map);

			/* if the conseil is already ko*/
			if (!canFight(map->conseil[nb])){
				printf("You put %s KO !!\n",map->conseil[nb]->name);
			} else {
				do {
					display_team(map->conseil[nb]);
					printf("Do you want to fight against %s ? (yes:1 no:0) \n",map->conseil[nb]->name);
					scanf("%d",&answer);
					while((c=getchar()) != EOF && c != '\n');
				} while (answer != 1 && answer != 0);

				/* if you want to fight the conseil */
				if (answer == 1){
					if (!canFight(myTeam)){
						printf("Your team is empty or all your pe are KO, you can't fight !\n");
					} else {
						/* if you can fight: the fight begins: */
						rival(myTeam,map->conseil[nb],0,0,tabAtt);
						if (!canFight(myTeam)){
							return 0;
						}

						/* if all the member of the conseil are KO,the game ends:*/
						for(i=0;i<MAXCONSEIL;i++){
							if(canFight(map->conseil[i])){
								return 1;
							}
						}
						printf("\nTHE ALL CONSEIL DES DIPLOMES IS KO !YOU WIN THE GAME !\n\n");
						return 0;
					}
				}
			}
		} else {

			/* if you are on the nurse cell: */
			if(!strcmp(map->tab[map->pos[0]][map->pos[1]],"n")){
				/* if the cell is the nurse :*/
				nurse(myTeam,pokecafet);
			}
		}
	}
	return 1;
}

void display_this_poke(Pokecafeteriat *cafet, Team *myTeam,int id){
  /* returns the poketudiant with identifiant id in the team or	pokecafeteriat */
  Poketudiant *pe1;
  pe1=in_cafet(cafet,id);
  if (pe1 == NULL){
    pe1 = in_team(myTeam,id);
  }
  if (pe1 == NULL){
    printf("The poketudiant with this id doesn't exist !\n");
  } else {
    full_display(pe1);
  }
}

void nurse(Team *team,Pokecafeteriat *cafet){
  /* all your poketudiants's HP are put at there max */
  if(team->pos == 0){
    printf("The team is empty !\n");
  } else {
    nurse_team(team);
    printf("The poketudiants in the team have been healed !\n");
  }
  if (empty_cafet(cafet)){
    printf("The pokecafeteriat is empty !\n");
  } else {
    nurse_cafet(cafet);
    printf("The poketudiants in the pokecafeteriat have been healed!\n");
  }
}

void debog_XP(Pokecafeteriat *pokecafet,Team *team,int id,int xp){
  /* give xp to id */
  Poketudiant *pe1;
  if (xp > 0){
    pe1 = in_team(team,id);
    if (pe1 == NULL){
      pe1 = in_cafet(pokecafet,id);
    }
    if (pe1 != NULL){
      printf("\nThis poketudiant is going to earn %d xp:\n",xp);
      full_display(pe1);
      earnXP_debog(pe1,xp);
      printf("\nLet's see how its new caracteritics look like:\n");
      full_display(pe1);
    } else {
      printf("Unknown id !\n");
    }
  } else {
    printf("Number of xp invalid !\n");
  }
}

void request(Team *myTeam,Pokecafeteriat *pokecafet,Attack tabAtt[NBTYPATT],Map *map){
  /* handles the requests typed in the console */

  Request requTab[NBREQ]= {QUIT,WILDR,RIVALR,NURSE,SHOW,SWITCH,MOVTAB,DROP,PICK,RELEASE,CATCH,XP,MOVUP,MOVR,MOVL,MOVDOWN};

  Poketudiant *pe;
  char buffer[SIZEBUF], *request;
  int i,fin = 0;
  long ii,jj;
  char *charac;

  /* we extract the first word of the command */
  while (!fin){

		displayMap(map);
    printf("\n###Enter a command: ");
    fgets(buffer,SIZEBUF,stdin);
    request = strtok(buffer,"\n ");
    if (request == NULL){
      printf("Please,type a request.\n");
    } else {
      i =0;
      while( i < NBREQ && strcmp(reqChar[i],request)) i++; /*look for the corresponding request*/

      switch(requTab[i]){
      case WILDR:

      case RIVALR:

	/* we read the i of "rival i j" */
	request = strtok(NULL,"\n ");
	if (request == NULL){
	  printf("Typing error !\n");
	  break;
	}
	ii = strtol(request,&charac, 10);
	if(charac == NULL || *charac !=0){
	  printf("Typing error !\n");
	  break;
	}

	/* we read the j of "rival i j" */
	request = strtok(NULL,"\n ");
	if (request == NULL){
	  printf("Typing error !\n");
	  break;
	}
	jj = strtol(request,&charac, 10);
	if (charac == NULL || *charac !=0){
	  printf("Typing error !\n");
	  break;
	}

	/* we launch the wild or rival fight */
	if (ii >= 1 && ii <= LVLMAX && jj>=ii && jj<=LVLMAX){
	  if (requTab[i] == WILDR){
			if (canFight(myTeam)){
	   		printf("Launch combat against a wild poketudiant with level between %ld and %ld \n",ii,jj);
	   		wild(myTeam,pokecafet,ii,jj,tabAtt);
				if (!canFight(myTeam)){
					fin=1;
				}
			} else {
				printf("You don't have any poketudiant ok in your team, you can't fight !\n");
			}
	  } else{
			if (canFight(myTeam)){
	    	printf("Launch combat against a rival (lvl %ld - %ld)\n",ii,jj);
	    	rival(myTeam,NULL,ii,jj,tabAtt);
				if (!canFight(myTeam)){
					fin=1;
				}
			} else {
				printf("You don't have any poketudiant ok in your team, you can't fight !\n");
			}
	  }
	} else {
	  printf("Typing error !\n");
	}
	break;

      case NURSE:
	nurse(myTeam,pokecafet);
	break;

      case SHOW:
	ii=-1;
	request = strtok(NULL,"\n ");
	if (request != NULL){
	  if(!strcmp("team",request)){
	    display_team(myTeam);
	    break;
	  }
	  if(!strcmp("pokecafeteriat",request)){
	    printf("\nThe pokecafeteriat:\n");
	    show_pokecafet(pokecafet);
	    break;
	  }
	  if(!strcmp("revision-table",request)){
	    request = strtok(NULL,"\n ");
	    if (request == NULL){
	      printf("Typing error !\n");
	      break;
	    }
	    ii = strtol(request,&charac, 10);
	    if (charac != NULL && *charac ==0){
	      show_table(pokecafet,ii);
	      break;
	    }
	  }
	  ii = strtol(request,&charac, 10);
	  if (charac != NULL && *charac ==0 && ii>=0){
	    display_this_poke(pokecafet,myTeam,ii);
	    break;
	  }
	}
	printf("Typing error !\n");
	break;

      case SWITCH:

      case MOVTAB:
	/* we read the i of "switch i j" or "move-table i j" */
	request = strtok(NULL,"\n ");
	if (request == NULL){
	  printf("Typing error !\n");
	  break;
	}
	ii = strtol(request,&charac, 10);
	if(charac == NULL || *charac !=0){
	  printf("Typing error !\n");
	  break;
	}

	/* we read the j of "switch i j" or "move-table i j" */
	request = strtok(NULL,"\n ");
	if (request == NULL){
	  printf("Typing error !\n");
	  break;
	}
	jj = strtol(request,&charac, 10);
	if(charac == NULL || *charac !=0){
	  printf("Typing error !\n");
	  break;
	}

	/* we inverse the positions if it's switch */
	if(requTab[i] == SWITCH && ii>=0 && jj>=0 && jj != ii){
	  switch_req(pokecafet,myTeam,jj,ii);
	  break;
	}
	/* we put pe at table jj if it's move-table */
	if(requTab[i] == MOVTAB && ii>=0 && jj>=0){
	  move_table_req(pokecafet,myTeam,ii,jj);
	} else {
	  printf("Impossible command !\n");
	}
	break;

      case DROP:

      case PICK:

      case RELEASE:
	/* we read the i of drop, pick or release command */
	request = strtok(NULL,"\n ");
	if (request == NULL){
	  printf("Typing error !\n");
	  break;
	}
	ii = strtol(request,&charac, 10);
	if(charac == NULL || *charac !=0){
	  printf("Typing error !\n");
	  break;
	}
	if(requTab[i] == DROP) drop(pokecafet,myTeam,ii);
	if(requTab[i] == PICK) pick(pokecafet,myTeam,ii);
	if(requTab[i] == RELEASE) release(pokecafet,ii);
	break;

      case CATCH:
	request = strtok(NULL,"\n ");
	if (request == NULL){
	  printf("Typing error !\n");
	  break;
	}
	ii = strtol(request,&charac, 10);
	if(charac == NULL || *charac !=0){
	  printf("Typing error !\n");
	  break;
	}
	i=0;
	while(i <ii){
	  pe = randomPE(1+rand()%LVLMAX,WILD,tabAtt);
	  success_capture(myTeam,pokecafet,pe);
	  i++;
	}
	break;

	/* if you want to move to the right in the map */
	case 	MOVR:
		if( map->pos[1]+1 < map->nbCol){
			map->pos[1]++;
			if(!moveMap(map,myTeam,pokecafet,tabAtt)){
				fin=1;
			}
		}
		break;

	/* if you want to move to the left in the map */
	case MOVL:
		if( map->pos[1]-1 >= 0){
			map->pos[1]--;
			if(!moveMap(map,myTeam,pokecafet,tabAtt)){
				fin=1;
			}
		}
		break;

	/* if you want to move down in the map */
	case MOVDOWN:
		if( map->pos[0]+1 < map->nbLines ){
			map->pos[0]++;
			if(!moveMap(map,myTeam,pokecafet,tabAtt)){
				fin=1;
			}
		}
		break;

	/* if you want to move up in the map */
	case MOVUP:
		if( map->pos[0]-1 >= 0){
			map->pos[0]--;
			if(!moveMap(map,myTeam,pokecafet,tabAtt)){
				fin=1;
			}
		}
		break;

	case XP:
	request = strtok(NULL,"\n ");
	/* test if there is something after the word xp */
	if (request == NULL){
	  printf("Typing error !\n");
	  break;
	}
	/* test if the 2n arg is a number */
	ii = strtol(request,&charac, 10);
	if(charac == NULL || *charac !=0){
	  printf("Typing error !\n");
	  break;
	}

	request = strtok(NULL,"\n ");
	/* test if there is a 3rd arg */
	if (request == NULL){
	  printf("Typing error !\n");
	  break;
	}
	/* test if the 3rd arg is a number */
	jj = strtol(request,&charac, 10);
	if(charac == NULL || *charac !=0){
	  printf("Typing error !\n");
	  break;
	}
	debog_XP(pokecafet,myTeam,ii,jj);
	break;

	case QUIT:
  	printf("You left the game!\n");	
		fin = 1;
		break;
	default:
		printf("Sorry, unknown command !\n");
		break;
      }
    }
  }
}
