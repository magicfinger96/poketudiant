#ifndef POKECAFET_H
#define POKECAFET_H

#define NBTABLES 3 /* number of tables available in the pokecafeteriat */
#define NBCHAIRS 4 /* number of chairs available by table in the pookecafet */

typedef struct Pokecafeteriat Pokecafeteriat; 
struct Pokecafeteriat {
  int nbTables; /* number of tables */
  int nbChairs; /* number of chair by table */
  int *pos; /* table of the number of pe by table at the pokecafet */
  Poketudiant*** seat; /* matrix which represent the pokecafet */
};

Pokecafeteriat *initPokecafet(int nbTables, int nbChairs);

int put_pe_in_cafet(Pokecafeteriat *pokecafet, Poketudiant *pe);
void put_pe_table(Pokecafeteriat *pokecafet, Poketudiant *pe, int nTable);
void drop(Pokecafeteriat *cafet, Team *team, int id);

void move_table_req(Pokecafeteriat *cafet, Team *team, int id, int nTable);
void switch_req(Pokecafeteriat *cafet, Team *team, int id1, int id2);

void nurse_cafet(Pokecafeteriat *pokecafet);

void supp_pe_cafet(Pokecafeteriat *pokecafet, Poketudiant *pe);
void supp_pe_table(Pokecafeteriat *pokecafet, int nTable, int ind);
void pick (Pokecafeteriat *pokecafet, Team *team, int id);
void release(Pokecafeteriat *cafet, int id);

int empty_cafet(Pokecafeteriat *cafet);

int success_capture(Team *myTeam, Pokecafeteriat* pokecafet, Poketudiant *wildPe);

int not_full_table(Pokecafeteriat *pokecafet, int nTable);

Poketudiant* in_cafet(Pokecafeteriat *pokecafet, int id);
int ind_pe_cafet(Poketudiant **table, int nbPe, int id);
void show_pokecafet(Pokecafeteriat *pokecafet);
void show_table(Pokecafeteriat *pokecafet, int ntable);

void free_pokecafet(Pokecafeteriat *pokecafet);

#endif
