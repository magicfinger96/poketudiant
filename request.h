#ifndef REQUEST_H
#define REQUEST_H

#define SIZEBUF 50 /* size of the buffer which receives my command */
#define NBREQ 16  /* number of types of requests (show,nurse,drop etc) */

extern int id;

typedef enum {QUIT,WILDR,RIVALR,NURSE,SHOW,SWITCH,MOVTAB,DROP,PICK,RELEASE,CATCH,XP,MOVUP,MOVR,MOVL,MOVDOWN} Request;

int moveMap(Map *map,Team* myTeam, Pokecafeteriat* pokecafet,Attack tabAtt[NBTYPATT]);
void display_this_poke(Pokecafeteriat *cafet, Team *myTeam, int id);
void nurse(Team *team, Pokecafeteriat *cafet);
void debog_XP(Pokecafeteriat *pokecafet, Team *team, int id, int xp);
void request(Team *myTeam, Pokecafeteriat *pokecafet,Attack tabAtt[NBTYPATT],Map *map);

#endif
