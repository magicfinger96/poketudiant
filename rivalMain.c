/* --- rivalMain.c --- */
/* the main function of the rival fight */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "poketudiant.h"
#include "enumtochar.h"
#include "team.h"
#include "pokecafet.h"
#include "fightMain.h"

void rival(Team* myTeam,Team* rivalTeam,int min,int max,Attack tabAtt[NBTYPATT]){
  /* handles the fight against a rival team with poke of min <= level <= max */
  int fin=0,rivalpass,nAtt,choice,i,choicePE,tmp,nbFighters=0,saveVariety,c,conseil;

	/*if there isn't rival team, we make it */
	if (rivalTeam == NULL){
  	rivalTeam = randomTeam(min,max,tabAtt);
		conseil=0;
	} else{
		conseil=1;
	}

  Poketudiant *myFighter = myTeam->pe[0];
  Poketudiant *rivalsFighter = rivalTeam ->pe[0];
  int didFight[myTeam->pos];
  int mynKO[myTeam->pos],mynKOInd=0;
  int rivalnKO[myTeam->pos],rivalnKOInd=0;

  for(i=0;i<myTeam->pos;i++){
    didFight[i] = 0;
  }

  /* fill the table with the indice of all my pe not KO 
	nKO[0] always represents the indice of myFighter */

  for (i=0;i<myTeam->pos;i++){
    if(myTeam->pe[i]->HP >= 1){
      mynKO[mynKOInd++]=i;
    }
  }

  for (i=0;i<rivalTeam->pos;i++){
    if(rivalTeam->pe[i]->HP >= 1){
      rivalnKO[rivalnKOInd++]=i;
    }
  }

  do {
    rivalpass=0; /* used if we don't want rival pe to attacks */
    printf("\n##################################\n");
    display_team(myTeam);
    display_team(rivalTeam);
    printf("\nMy fighter: ");
    displayPE(myFighter);
    printf("The rival's: ");
    displayPE(rivalsFighter);
    printf("\n");
    rivalDisplay(myFighter);
    printf("##################################\n\n");
    do{
      choice = 0;
      printf("### Your choice ? ");
      scanf("%d",&choice);
      while((c=getchar()) != '\n' && c!=EOF);
    } while(choice != 1 && choice != 2 && choice != 3);

    switch(choice){
    case 1:
    case 2:
      /* you choose to fight: */
      if (!didFight[mynKO[0]]){
	didFight[mynKO[0]] = 1;
	nbFighters++;
      }

      printf("\nMy %s n°%d used its attack: %s...\n",
	     varietytochar(myFighter->variety),myFighter->id,
	     attacktochar(myFighter->attacks[choice-1].name));
      printf("The rival's Poketudiant lost %d Health points.\n",
	     attack(myFighter,rivalsFighter,choice-1));

      /* if the rival's PE is KO */
      if (rivalsFighter->HP < 1){
	printf("\nThe rival's %s n°%d is KO !\n\n",
	       varietytochar(rivalsFighter->variety),rivalsFighter->id);

	/* display the list of PE earning XP & increase XP */
	printf("My poketudiant(s) not KO who fought:\n");
	for (i=0;i<mynKOInd;i++){
	  if (didFight[mynKO[i]]){

	    /* in order to see if it triggers an evolution */
	    saveVariety = myTeam->pe[mynKO[i]]->variety;

	    printf("	- %s n°%d ",
		   varietytochar(myTeam->pe[mynKO[i]]->variety),
		   myTeam->pe[mynKO[i]]->id);
	    printf("earned %d XP",earnXP(myTeam->pe[mynKO[i]],
					 rivalsFighter,nbFighters));

	    /* shows the eventual evolution */
	    if(saveVariety != myTeam->pe[mynKO[i]]->variety){
	      printf(", It evolves to %s lvl %d !",
		     varietytochar(myTeam->pe[mynKO[i]]->variety),
		     myTeam->pe[mynKO[i]]->lvl);
	    }
	    printf("\n");
	  }
	}
	printf("\n");


	/* rivalnKOInd is the number of pe not KO, as one of them
					has just been KO: there are rivalnKOInd-1 KO */
	if(rivalnKOInd < 2){
	  printf("ALL THE RIVAL'S POKETUDIANT ARE KO ! YOU WON !!");
	  fin = 1;
	} else {

	  /* we update the table of all the indice of
						the not KO rival PE */
	  rivalnKOInd = 0;
	  for (i=0;i<rivalTeam->pos;i++){
	    if(rivalTeam->pe[i]->HP >= 1){
	      rivalnKO[rivalnKOInd++]=i;
	    }
	  }
	  rivalsFighter = rivalTeam->pe[rivalnKO[rand()%rivalnKOInd]] ;
	  printf("The new rival's fighter is: %s n°%d\n",
		 varietytochar(rivalsFighter->variety),
		 rivalsFighter->id);

	  /* we reinit the table of the fight of all my pe at 0,
						as they didn't fight the new rival's pe yet */
	  for(i=0;i<myTeam->pos;i++){
	    didFight[i] = 0;
	  }
	  nbFighters = 0;
	}
      }
      break;

    case 3:
      /* if you choosed to switch your fighter */
      if (mynKOInd == 1 ){
	printf("You don't have any other Poketudiant available!\n");
	rivalpass = 1;
      } else {

	/* show all the pe available for the switch */
	printf("\nExchange with:\n\n");
	for(i=1;i<mynKOInd;i++){
	  if (myTeam->pe[mynKO[i]] != myFighter){
	    printf("Choice n°%d -> ",i);
	    full_display(myTeam->pe[mynKO[i]]);
	  }
	}

	/* you make your choice */
	do{
	  choicePE = -1;
	  printf("### Your choice ? (0 to go back) ");
	  scanf("%d",&choicePE);
	  while((c=getchar()) != '\n' && c!=EOF);
	} while( choicePE >= mynKOInd || choicePE < 0);

	if ( !choicePE){
	  rivalpass = 1;
	} else {

	  /* we put the new indice of myFighter in nk0[0] 
						and put the ex myFighter's indice in nk0[choicePE] */
	  tmp = mynKO[0];
	  mynKO[0] = mynKO[choicePE];
	  mynKO[choicePE]=tmp;

	  myFighter = myTeam->pe[mynKO[0]];

	  printf("I've switched with");
	  displayPE(myFighter);
	}
      }
      break;
    default:
      break;
    }

    if (!fin && !rivalpass){
      /* the wild pe makes its move */
      nAtt = rand()%2;
      printf("\nThe rival's Poketudiant used its attack: %s...\n",
	     attacktochar(rivalsFighter->attacks[nAtt].name));
      printf("My %s n°%d lost %d Health points.\n",
	     varietytochar(myFighter->variety),myFighter->id,
	     attack(myFighter,myFighter,nAtt));

      /* if my fighter is ko: */
      if (myFighter->HP < 1){
	printf("My %s n°%d is KO !\n",varietytochar(myFighter->variety), myFighter->id);

	/* if all my pe are KO */
	if (mynKOInd < 2){
	  printf("You lost the battle !!\n");
	  fin = 1;
	} else {

	  if (didFight[mynKO[0]] == 1 ) nbFighters --;

	  /* display all the pe not KO */
	  printf("\nPick your new fighter:\n\n");
	  for(i=1;i<mynKOInd;i++){
	    if (myTeam->pe[mynKO[i]] != myFighter){
	      printf("Choice n°%d -> ",i);
	      full_display(myTeam->pe[mynKO[i]]);
	    }
	  }

	  /* I choose my new fighter */
	  do{
	    choicePE = 0;
	    printf("### Your choice ? ");
	    scanf("%d",&choicePE);
	    while((c=getchar()) != '\n' && c!=EOF);
	  } while( choicePE >= mynKOInd || choicePE < 1);
	  myFighter = myTeam->pe[mynKO[choicePE]];

	  /* we remake the table nKO */
	  mynKO[0]= mynKO[choicePE];
	  mynKOInd = 1;
	  for (i=0;i<myTeam->pos;i++){
	    if(myTeam->pe[i]->HP >= 1 && i!= mynKO[0]){
	      mynKO[mynKOInd++]=i;
	    }
	  }
	}
      }
    }
  } while (!fin);
	if(!conseil){
  	free_team(rivalTeam);
  	free(rivalTeam);
	}
  printf("THE FIGHT IS OVER !!\n");
}
