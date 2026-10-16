#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembleur.h"
#include "processeur.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <fichier.txt>\n", argv[0]);
        printf("Exemple: %s programme.txt\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *test_file = fopen(filename, "r");
    if (test_file == NULL)
    {
        printf("Erreur: Impossible d'ouvrir le fichier '%s'\n", filename);
        return 1;
    }
    fclose(test_file);

    Tag tag_table[100];
    int tag_count = 0;
    int assembly_result = translateCode(filename, tag_table, &tag_count);
    if (assembly_result != 0)
        return 1;

    int execution_result = executeProgram();
    if (execution_result != 0)
    {
        printf("\nErreur lors de l'execution.\n");
        return 1;
    }
    return 0;
}
