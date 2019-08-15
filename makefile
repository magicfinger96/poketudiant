CC = gcc
CFLAGS = -Wall -pedantic
OBJS = main.o request.o map.o pokecafet.o rivalMain.o wildMain.o fight.o team.o enumtochar.o poketudiant.o

executable : $(OBJS)
	@$(CC) -o exec $(OBJS)
	@echo "It works, write ./exec to continue"

main.o : main.c poketudiant.h enumtochar.h team.h pokecafet.h fightMain.h map.h request.h
	@$(CC) $(CFLAGS) -c main.c

request.o : request.c map.h poketudiant.h enumtochar.h team.h pokecafet.h fightMain.h request.h
	@$(CC) $(CFLAGS) -c request.c

map.o : map.c poketudiant.h enumtochar.h team.h map.h
	@$(CC) $(CFLAGS) -c map.c

pokecafet.o : pokecafet.c poketudiant.h enumtochar.h team.h pokecafet.h
	@$(CC) $(CFLAGS) -c pokecafet.c

rivalMain.o : rivalMain.c poketudiant.h enumtochar.h team.h fightMain.h
	@$(CC) $(CFLAGS) -c rivalMain.c

wildMain.o : wildMain.c poketudiant.h enumtochar.h team.h pokecafet.h fightMain.h
	@$(CC) $(CFLAGS) -c wildMain.c

fight.o : fight.c poketudiant.h enumtochar.h team.h fightMain.h
	@$(CC) $(CFLAGS) -c fight.c

team.o : team.c poketudiant.h team.h
	@$(CC) $(CFLAGS) -c team.c

enumtochar.o : enumtochar.c poketudiant.h enumtochar.h
	@$(CC) $(CFLAGS) -c enumtochar.c

poketudiant.o : poketudiant.c poketudiant.h enumtochar.h
	@$(CC) $(CFLAGS) -c poketudiant.c

clean :
	@rm *.o 2> /dev/null || true
	@echo "Cleaning done"
