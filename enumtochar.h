#ifndef ENUMTOCHAR_H
#define ENUMTOCHAR_H
#define MAXCHAR

char* typetochar(Type t);
char* varietytochar(Variety v);
Variety charToVariety(char name[MAXCHAR]);
char* attacktochar(AttName a);

#endif
