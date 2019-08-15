/* --- wildMain.c --- */
/* the main function of the wild fight */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "enumtochar.h"
#include "team.h"
#include "pokecafet.h"
#include "fightMain.h"

void emptyBuffer(void){
  /* to avoid buffer issue with the scanf */
  int c;
  while((c=getchar()) != EOF && c != '\n');
}

void wild(Team* myTeam, Pokecafeteriat *pokecafet,int min,int max,Attack tabAtt[NBTYPATT]){
  /* handles the fight against a wild poketudiant of min <= level <= max */

  int lvl = rand()%(max-min+1)+min,fin=0,wildpass,nAtt,choice,i,choicePE,c,
    saveVariety,tmp,nbFighters=0,kept=0;
  Poketudiant *wildPe = randomPE(lvl,WILD,tabAtt),
    *myFighter = myTeam->pe[0];
  int didFight[myTeam->pos], nKO[myTeam->pos],nKOInd=0;
  for(i=0;i<myTeam->pos;i++){
    didFight[i] = 0;
  }

  /* fill the table with the indice of all my pe not KO 
	nKO[0] always represents the indice of myFighter */

  for (i=0;i<myTeam->pos;i++){
    if(myTeam->pe[i]->HP >= 1){
      nKO[nKOInd++]=i;
    }
  }
  do {
    wildpass=0; /* used if we don't want wild pe to attacks */
    printf("\n##################################\n");
    printf("\nMy fighter: ");
    displayPE(myFighter);
    printf("The wild one: ");
    displayPE(wildPe);
    printf("\n");
    wildDisplay(myFighter);
    printf("##################################\n\n");
    do{
      choice = 0;
      printf("### Your choice ?");
      scanf("%d",&choice);
      while((c=getchar()) != '\n' && c!=EOF);
    } while(choice != 1 && choice != 2 && choice != 3 && choice !=4 && choice != 5);
    switch(choice){
    case 1:
    case 2:
      /* you choose to fight: */
      if (!didFight[nKO[0]]){
				didFight[nKO[0]] = 1;
				nbFighters++;
      }

      printf("\nMy %s n°%d used its attack: %s...\n",
	   	varietytochar(myFighter->variety),myFighter->id,
	  	attacktochar(myFighter->attacks[choice-1].name));
      printf("The wild Poketudiant lost %d Health points.\n",
	  	attack(myFighter,wildPe,choice-1));

      /* if the wild PE is KO */
      if (wildPe->HP < 1){
				printf("\nThe wild %s n°%d is KO ! YOU WON !!!\n\n",
	     	varietytochar(wildPe->variety),wildPe->id);

				printf("fighters: %d\n",nbFighters);

				/* display the list of PE earning XP & increase XP */
				printf("My poketudiant(s) not KO who fought:\n");
				for (i=0;i<nKOInd;i++){
					if (didFight[nKO[i]]){

						/* in order to see if it triggers an evolution */
						saveVariety = myTeam->pe[nKO[i]]->variety;

						printf("	- %s n°%d ",
					 	varietytochar(myTeam->pe[nKO[i]]->variety),
					 	myTeam->pe[nKO[i]]->id);
						printf("earned %d XP",earnXP(myTeam->pe[nKO[i]],
						wildPe,nbFighters));

						/* shows the eventual evolution */
						if(saveVariety != myTeam->pe[nKO[i]]->variety){
							printf(", It evolves to %s lvl %d !",
							varietytochar(myTeam->pe[nKO[i]]->variety),
						 	myTeam->pe[nKO[i]]->lvl);
						}
						printf("\n");
					}
				}
				printf("\n");
				fin = 1;
      }
      break;
      
    case 3:
      /* if you choosed to switch your fighter */
      if (nKOInd == 1 ){
				printf("You don't have any other Poketudiant available!\n");
				wildpass = 1;
      } else {

				/* show all the pe available for the switch */
				printf("\nExchange with:\n\n");
				for(i=1;i<nKOInd;i++){
					if (myTeam->pe[nKO[i]] != myFighter){
						printf("Choice n°%d -> ",i);
						full_display(myTeam->pe[nKO[i]]);
					}
				}

				/* you make your choice */
				do{
					choicePE = -1;
					printf("### Your choice ? (0 to go back) ");
					scanf("%d",&choicePE);
					while((c=getchar()) != '\n' && c!=EOF);
				} while( choicePE >= nKOInd || choicePE < 0);

				if ( !choicePE){
					wildpass = 1;
				} else {

					/* we put the new indice of myFighter in nk0[0] 
									and put the ex myFighter's indice in nk0[choicePE] */
					tmp = nKO[0];
					nKO[0] = nKO[choicePE];
					nKO[choicePE]=tmp;

					myFighter = myTeam->pe[nKO[0]];

					printf("I've switched with");
					displayPE(myFighter);
				}
      }
      break;
      
    case 4:
			/* you try to capture */
      if (capture(myTeam,wildPe)){
				if(success_capture(myTeam,pokecafet,wildPe)){
					kept = 1;
				}
				fin = 1;
      } else {
				printf("Ooops...You failed !\n");
      }
      break;
    case 5:
			/* you try to run out */
      if(fled(myFighter,wildPe)){
				printf("My %s n°%d managed to flee !\n",
				varietytochar(myFighter->variety),myFighter->id);
				fin = 1;
      } else {
				printf("My %s n°%d failed its escape !\n",
	    	varietytochar(myFighter->variety),myFighter->id);
      }
      break;
    default:
      break;
    }

    if (!fin && !wildpass){
      /* the wild pe makes its move */
      nAtt = rand()%2;
      printf("\nThe wild Poketudiant used its attack: %s...\n",
	     attacktochar(wildPe->attacks[nAtt].name));
      printf("My %s n°%d lost %d Health points.\n",
	     varietytochar(myFighter->variety),myFighter->id,
	     attack(myFighter,myFighter,nAtt));

      /* if my fighter is ko: */
      if (myFighter->HP < 1){
				printf("My %s n°%d is KO !\n",varietytochar(myFighter->variety),
	    	myFighter->id);

				/* if all my pe are KO */
				if (nKOInd-1 < 1){
					printf("You lost the battle !!\n");
					fin = 1;
				} else {

					if (didFight[nKO[0]] == 1 ) nbFighters --;

					/* display all the pe not KO */
					printf("\nPick your new fighter:\n\n");
					for(i=1;i<nKOInd;i++){
						if (myTeam->pe[nKO[i]] != myFighter){
							printf("Choice n°%d -> ",i);
							full_display(myTeam->pe[nKO[i]]);
						}
					}

					/* I choose my new fighter */
					do{
						choicePE = 0;
						printf("### Your choice ? ");
						scanf("%d",&choicePE);
						while((c=getchar()) != '\n' && c!=EOF);
					} while( choicePE >= nKOInd || choicePE < 1);
					myFighter = myTeam->pe[nKO[choicePE]];

					/* we remake the table nKO */
					nKO[0]= nKO[choicePE];
					nKOInd = 1;
					for (i=0;i<myTeam->pos;i++){
						if(myTeam->pe[i]->HP >= 1 && i!= nKO[0]){
							nKO[nKOInd++]=i;
						}
					}
				}
      }
    }
  } while (!fin);
  if (kept == 0){
    free(wildPe);
  }
  printf("THE FIGHT IS OVER !!\n");
}
