#ifndef ASSEMBLEUR_H
#define ASSEMBLEUR_H

typedef struct
{
    char name[64];
    int address;
} Tag;

int translateCode(char fileName[], Tag *tag_table, int *tag_count);

#endif