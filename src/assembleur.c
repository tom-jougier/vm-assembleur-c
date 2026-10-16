#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "assembleur.h"

void add_tag(char *name, int current_pc, Tag *tag_table, int *tag_count)
{
    // Ajout de la nouvelle étiquette
    strcpy(tag_table[*tag_count].name, name);
    tag_table[*tag_count].address = current_pc;
    (*tag_count)++;
}

int get_tag_address(char *name, Tag *tag_table, int *tag_count)
{
    for (int i = 0; i < *tag_count; i++)
    {
        if (strcmp(tag_table[i].name, name) == 0)
            return tag_table[i].address;
    }
    return -1; // Pas trouvé
}

int check_and_remove_comma(char *token)
// Enlève la virgule à la fin de la chaîne et renvoie 1. Cela renvoie 0 s'il n'y en a pas
{
    int len = strlen(token);
    if (len > 0 && token[len - 1] == ',')
    {
        token[len - 1] = '\0'; // On enlève la virgule
        return 1;
    }
    return 0;
}

void ft_putchar(char c, int *size, char *dest)
{
    // printf("%c", c);
    dest[*size] = c;
    (*size)++;
    dest[*size] = '\0';
}

int ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

int ft_check_base(char *base)
{
    int i;
    int j;

    // If the base is less than 2, returns 0
    if (ft_strlen(base) < 2)
        return (0);
    i = 0;
    while (base[i])
    {
        j = i + 1;

        // If it's found signs of + or -
        // or non-printable characters, returns 0
        if (base[i] == '+' || base[i] == '-' || base[i] < 32 || base[i] > 126)
            return (0);

        // check if the base has any repeated character
        // If there is any, the return is 0
        while (base[j])
        {
            if (base[i] == base[j])
                return (0);
            j++;
        }
        i++;
    }
    // If there is no problem, the return is 1
    return (1);
}

// This function will convert a decimal number to decimal or other bases and print it
void ft_putnbr_base(int nbr, char *base, char *result)
{
    char box[32];
    int length = 0;
    long n;
    int size;
    int i;

    // Call the function to check the past base
    // If the return on the function is 0, the program finish
    if (!ft_check_base(base))
        return;

    // counts the size of the base
    size = ft_strlen(base);
    if (nbr == 0)
    {
        ft_putchar(base[0], &length, result);
        return;
    }
    // long n avoid the issues with the int min
    n = nbr;
    if (n < 0)
    {
        ft_putchar('-', &length, result);
        n *= -1;
    }
    i = 0;

    while (n > 0)
    {
        // Array Box receives the converted values of 'n' to the base
        // This formula of 'n' module size will find the correct number at the base
        box[i] = base[n % size];
        n = n / size;
        i++;
    }

    // The values stored in box were reversed
    // This repetition will print what is in the box
    // starting from last position to the first
    while (--i >= 0)
        ft_putchar(box[i], &length, result);
}

int fta_check_base(char *str)
{
    int i;
    int j;

    i = 0;
    // If the beginning of the base is null, returns 0
    if (str[0] == '\0' || str[1] == '\0')
        return (0);
    while (str[i])
    {
        // If there are non-printable characters or signs of - and +, returns 0
        if (str[i] <= 32 || str[i] == 127 || str[i] == '-' || str[i] == '+')
            return (0);
        j = i + 1;

        // Check if there are repeated character
        while (str[j])
        {
            if (str[i] == str[j])
                return (0);
            j++;
        }
        i++;
    }

    // Returns the size of the base
    return (i);
}

int fta_base_match(char c, char *base)
{
    int i;

    i = 0;
    while (base[i])
    {
        // If the past Char is the same as the base, returns the 'i'
        if (c == base[i])
            return (i);
        i++;
    }
    // If the past char is the null of the string, the return will be -1
    return (-1);
}

int fta_pre_digits(char *str, int *ptr_sign)
{
    int sign;
    int i;

    sign = 1;
    i = 0;
    // Check the spaces and increased 'i'
    while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
        i++;

    // checks the signs of + and - and increases the 'i'
    while (str[i] && (str[i] == '+' || str[i] == '-'))
    {
        if (str[i] == '-')
            sign *= -1;
        i++;
    }
    // Change the value of sign in ft_atoi_base through the pointer ptr_sign
    *ptr_sign = sign;

    // In the end, returns the lenght from spaces and signs
    return (i);
}

// converts any base to decimal
int ft_atoi_base(char *str, char *base)
{
    int i;
    int sign;
    int sum;
    int current;
    int base_size;

    sum = 0;
    sign = 1;

    // receives the base lenght or receives 0
    base_size = fta_check_base(base);

    // If the base length is less than 2
    // or the base check returns zero, the return will be zero
    // Otherwise the conversions are made
    if (base_size >= 2)
    {
        // 'i' receives lenght from pre digits
        i = fta_pre_digits(str, &sign);

        // receives the first correspondence of the base
        current = fta_base_match(str[i], base);

        // as long as there is correspondence
        while (current != -1)
        {
            // This formula converts the bases to decimal
            sum = (sum * base_size) + current;
            i++;

            // Current receives ft_base_match from the next Index
            current = fta_base_match(str[i], base);
        }
        return (sign * sum);
    }
    return (0);
}

void clean(char *ligne)
// Fonction qui prend en argument l'adresse d'une ligne et qui la nettoie pour la traduction en hexadécimal
{
    int i = 0; // Indice pour parcourir la ligne
    int j = 0; // Indice pour écrire sur la ligne

    while (ligne[i] == ' ' || ligne[i] == '\t') // Ignore les espaces/tabulations du début
        i++;

    // On compacte le reste
    while (ligne[i] != '\0' && ligne[i] != '\n' && ligne[i] != '\r')
    {
        if (ligne[i] != ' ' && ligne[i] != '\t') // Si ce n'est pas un espace/tabulation, on le recopie
            ligne[j++] = ligne[i];
        else
        {
            if (j > 0 && ligne[j - 1] != ' ' && ligne[j - 1] != '\t') // On ajoute un espace seulement si le précédent n'est pas un espace/tabulation
                ligne[j++] = ' ';
        }
        i++;
    }

    if (j > 0 && ligne[j - 1] == ' ') // On enlève l'espace à la fin (s'il y en avait plusieurs, l'étape d'avant n'en a recopié qu'un seul)
        j--;

    ligne[j] = '\0';
}

int doublepoint_counter(char *line)
// Renvoie -1 si la ligne contient plus qu'une étiquette, 0 s'il n'y en a pas, et l'indice de ':' s'il y en a une seule
{
    int counter = 0;
    int index = 0;
    for (int i = 0; line[i] != '\0'; i++)
        if (line[i] == ':')
        {
            counter++;
            index = i;
        }

    if (counter > 1)
        return -1;
    if (counter == 0)
        return counter;
    return index;
}

int is_tag_ok(char *line, int *pc, int is_it_first_read, Tag *tag_table, int *tag_count)
// Indépendant de la structure Tag
// is_it_first_read vaut 1 si on est dans la boucle qui recense les étiquettes déclarées, 0 sinon
// Renvoie 0 s'il n'y a pas d'étiquette, 1 s'il y en a une, -1 si le format est incorrect. S'il y a une étiquette, elle est ajoutée à la table des étiquettes
{
    int counter = doublepoint_counter(line);
    if (counter == 0)
        return 0;
    if (counter == -1)
        return -1;

    for (int j = 0; j < counter; j++)
    {
        if (!isalnum(line[j]) && line[j] != '_') // On vérifie si les caractères sont autorisés
            return -1;
    }

    // Enregistrement de l'étiquette dans la table
    if (is_it_first_read)
    {
        char tag_name[64];
        if (counter >= 64) // Vérifier si ça doit être 63 (laisser la place pour '\0')
            return -1;     // L'étiquette est trop grande
        strncpy(tag_name, line, counter);
        tag_name[counter] = '\0';
        if (get_tag_address(tag_name, tag_table, tag_count) != -1)
            return -1; // L'étiquette existe déjà

        add_tag(tag_name, *pc, tag_table, tag_count);
    }

    return 1;
}

int get_opcode(char *instruction)
// Fonction qui prend en argument une instruction et qui renvoie son code opératoire
{
    if (strcmp(instruction, "or") == 0)
        return 0;
    if (strcmp(instruction, "and") == 0)
        return 1;
    if (strcmp(instruction, "xor") == 0)
        return 2;
    if (strcmp(instruction, "add") == 0)
        return 3;
    if (strcmp(instruction, "sub") == 0)
        return 4;
    if (strcmp(instruction, "mul") == 0)
        return 5;
    if (strcmp(instruction, "div") == 0)
        return 6;
    if (strcmp(instruction, "shr") == 0)
        return 7;
    if (strcmp(instruction, "ldb") == 0)
        return 12;
    if (strcmp(instruction, "ldh") == 0)
        return 13;
    if (strcmp(instruction, "ldw") == 0)
        return 14;
    if (strcmp(instruction, "stb") == 0)
        return 15;
    if (strcmp(instruction, "sth") == 0)
        return 16;
    if (strcmp(instruction, "stw") == 0)
        return 17;
    if (strcmp(instruction, "jmp") == 0)
        return 21;
    if (strcmp(instruction, "jzs") == 0)
        return 22;
    if (strcmp(instruction, "jzc") == 0)
        return 23;
    if (strcmp(instruction, "jcs") == 0)
        return 24;
    if (strcmp(instruction, "jcc") == 0)
        return 25;
    if (strcmp(instruction, "jns") == 0)
        return 26;
    if (strcmp(instruction, "jnc") == 0)
        return 27;
    if (strcmp(instruction, "in") == 0)
        return 28;
    if (strcmp(instruction, "out") == 0)
        return 29;
    if (strcmp(instruction, "rnd") == 0)
        return 30;
    if (strcmp(instruction, "hlt") == 0)
        return 31;

    return -1; // Si commande inconnue du compilateur
}

int is_ImmValue_ok(char *value, int is_positive)
// Renvoie 1 si c'est un hexadécimal, 0 si c'est un décimal, -1 s'il y a une erreur
// is_positive vaut 1 si la valeur doit être entre 0 et 65535 (pour indiquer une adresse mémorie par exemple)
// Il vaut 0 si c'est entre -32K et 32K
{
    if (value == NULL || value[0] == '\0')
        return -1;

    long converted_value;
    char *endptr;

    // Valeur hexadécimale
    if (value[0] == 'h')
    {
        if (value[1] == '\0')
            return -1; // Rien après le h

        // Le reste doit être composé de chiffres hexa
        for (int i = 1; value[i] != '\0'; i++)
        {
            if (!isxdigit(value[i]))
                return -1;
        }
        converted_value = strtol(value + 1, &endptr, 16); // Conversion base 16 (on saute le 'h')

        // Vérification de la plage 16 bits (0 à 65535)
        if (converted_value < 0 || converted_value > 65535)
            return -1;
        return 1;
    }

    // Valeur décimale (même costruction que le bloc d'avant)
    else
    {
        int start_index = 0;
        if (value[0] == '-')
        {
            start_index = 1;
            if (value[1] == '\0')
                return -1;
        }

        // On vérifie que le reste n'est composé que de chiffres
        for (int i = start_index; value[i] != '\0'; i++)
        {
            if (!isdigit(value[i]))
                return -1;
        }

        converted_value = strtol(value, &endptr, 10);

        // Vérification des bornes selon le contexte
        if (is_positive)
        {
            if (converted_value < 0 || converted_value > 65535)
                return -1;
        }
        else
        {
            if (converted_value < -32768 || converted_value > 32767)
                return -1;
        }

        return 0;
    }
}

int is_registry(char *value)
// Renvoie 1 si c'est de la forme rX (avec X entre 0 et 31), 0 sinon
{
    if (value[0] != 'r' || !isdigit(value[1]))
        return 0;

    int len = strlen(value);
    if (len < 2 || len > 3) // Soit il n'y a pas assez de chiffres, soit il y en a trop
        return 0;

    for (int i = 1; i < len; i++)
    {
        if (!isdigit(value[i]))
            return 0;
    }

    int reg_num = atoi(value + 1); // On vérifie que le nombre est entre 0 et 31
    if (reg_num >= 0 && reg_num <= 31)
        return 1;

    return 0;
}

int read_all_tags(FILE *file, Tag *tag_table, int *tag_count)
{
    char line[64];
    int pc = 0;
    *tag_count = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        clean(line);
        int line_num = 1;
        int index_dp = doublepoint_counter(line);
        if (index_dp > 0) // Il y a une étiquette
        {
            char tag_name[64];

            if (index_dp < 64) // On vérifie que la taille est correcte
            {
                strncpy(tag_name, line, index_dp);
                tag_name[index_dp] = '\0';
                if (is_registry(tag_name))
                {
                    printf("Erreur : Etiquette ligne %d ne peut pas avoir le format d'un registre\n", line_num);
                    return 0;
                }

                else if (get_tag_address(tag_name, tag_table, tag_count) == -1) // On vérifie qu'elle n'existe pas déjà pour éviter les doublons
                    add_tag(tag_name, pc, tag_table, tag_count);
                else
                {
                    printf("Erreur : Etiquette redeclaree ligne %d\n", line_num);
                    return 0;
                }
            }
        }

        // Incrémentation du PC (s'il y a une instruction sur la ligne, alors ce n'est pas une ligne qui déclare juste une étiquette)
        char line_copy[64];
        strcpy(line_copy, line);
        char *token = strtok(line_copy, " :\t\n\r");

        if (token != NULL)
        {
            if (index_dp > 0)
                token = strtok(NULL, " :\t\n\r"); // On regarde s'il y a quelque chose après

            if (token != NULL) // Il y a une instruction
                pc += 4;
        }
        line_num++;
    }
    return 1;
}

int is_line_ok(char *line, int *pc, Tag *tag_table, int *tag_count)
// Fonction qui renvoie 0 si la ligne contient une erreur, 1 sinon
{
    int index = is_tag_ok(line, pc, 0, tag_table, tag_count);
    if (index == -1) // Problème avec l'étiquette
        return 0;

    for (int i = 0; line[i] != '\0'; i++)
        line[i] = tolower(line[i]); // On met la chaîne en minuscule

    char line_copy[64];
    strcpy(line_copy, line);
    char *token = strtok(line_copy, " \t\n\r"); // Pour récupérer chaque paramètre

    // Vérification de l'instruction
    if (index > 0)
        token = strtok(NULL, " :\t\n\r"); // S'il y a une étiquette, on se place après le ": "

    if (!token)
        return 1; // ligne vide ou juste une étiquette. On n'incrémente pas PC

    int opcode = get_opcode(token);
    if (opcode == -1) // On vérifie que cette instruction existe
        return 0;

    (*pc) += 4; // Si on arrive là c'est qu'il y a une instruction ==> laisser 4 octets de mémoire pour la stocker

    // hlt (ne possède pas d'argument)
    if (opcode == 31)
    {
        token = strtok(NULL, " \t\n\r");
        if (token != NULL)
            return 0;
        return 1;
    }

    token = strtok(NULL, " \t\n\r"); // On lit arg1
    if (!token)
        return 0; // Rien après l'instruction

    //---------------------------------
    // Vérification pour stb, sth, stw
    //---------------------------------
    if (opcode >= 15 && opcode <= 17)
    {
        if (!check_and_remove_comma(token))
            return 0; // Il manque une virgule

        if (token[0] != '(') // arg1 doit commencer par une parenthèse
            return 0;
        char *closing_paren = strchr(token, ')');
        if (!closing_paren) // Pas de parenthèse fermante
            return 0;

        *closing_paren = '\0'; // On coupe la chaîne isoler rd
        char *rd = token + 1;  // On saute la parenthèse fermante
        if (!is_registry(rd))
            return 0;

        char *S = closing_paren + 1; // Ce qui est juste après ')'
        if (*S == '\0')              // S doit être spécifié
            return 0;

        // S est soit un registre, soit une valeur immédiate
        int s_is_reg = is_registry(S);
        int s_is_imm = 0;

        if (!s_is_reg)
        {
            if (S[0] == '#')
            {
                if (is_ImmValue_ok(S + 1, 0) != -1) // On saute '#' et on regarde la valeur derrière
                    s_is_imm = 1;
            }
        }

        if (!s_is_reg && !s_is_imm) // S n'est ni un registre, ni une valeur immédiate
            return 0;

        // Vérification de rn
        token = strtok(NULL, " \t\n\r");
        if (!token || check_and_remove_comma(token) || !is_registry(token))
            return 0; // Est vide / n'est pas un registre / a une virgule à la fin

        // Vérifier qu'il n'y a rien après
        token = strtok(NULL, " \t\n\r");
        if (token != NULL)
            return 0;

        return 1;
    }

    // --------------------------------
    // Vérification pour ldb, ldh, ldw
    // --------------------------------
    else if (opcode >= 12 && opcode <= 14)
    {
        if (!check_and_remove_comma(token) || !is_registry(token)) // rd doit être un registre
            return 0;

        token = strtok(NULL, " \t\n\r"); // arg2 = (rn)S
        if (!token || check_and_remove_comma(token))
            return 0;
        if (token[0] != '(')
            return 0;
        char *closing_paren = strchr(token, ')');
        if (!closing_paren)
            return 0;

        *closing_paren = '\0';
        char *rn = token + 1;
        if (!is_registry(rn))
            return 0; // rn doit être un registre

        char *S = closing_paren + 1;
        if (*S == '\0') // S n'est pas spécifié
            return 0;

        // On vérifie que S est un registre ou une valeur immédiate
        int s_is_reg = is_registry(S);
        int s_is_imm = 0;
        if (!s_is_reg)
        {
            if (S[0] == '#')
            {
                if (is_ImmValue_ok(S + 1, 0) != -1)
                    s_is_imm = 1;
            }
        }

        if (!s_is_reg && !s_is_imm) // S n'est ni un registre ni une valeur immédiate
            return 0;

        // Vérifier qu'il n'y a rien après
        token = strtok(NULL, " \t\n\r");
        if (token != NULL)
            return 0;

        return 1;
    }

    // -------------------------------------------
    // Vérification pour les instructions de saut
    // -------------------------------------------
    else if (opcode >= 21 && opcode <= 27)
    {
        if (check_and_remove_comma(token))
            return 0;

        int s_is_reg = is_registry(token);
        int s_is_imm = 0;
        int s_is_tag = 0;

        if (!s_is_reg)
        {
            if (token[0] == '#')
            {
                if (is_ImmValue_ok(token + 1, 1) != -1)
                {
                    // On vérifie que l'adresse indiquée est bien multiple de 4 (car les instructions sont codées sur 4 octets et la mémoire est simulée par des char)
                    long val;
                    if (token[1] == 'h')
                        val = strtol(token + 2, NULL, 16);
                    else
                        val = atoi(token + 1);

                    if (val % 4 == 0)
                        s_is_imm = 1;
                }
            }
            // Vérification si c'est une étiquette
            if (get_tag_address(token, tag_table, tag_count) >= 0)
                s_is_tag = 1;
        }

        if (!s_is_reg && !s_is_imm && !s_is_tag) // S n'est ni un registre ni une valeur immédiate ALIGNÉE ni une étiquette
            return 0;

        // Vérifier qu'il n'y a rien après
        token = strtok(NULL, " \t\n\r");
        if (token != NULL)
            return 0;

        return 1;
    }

    // ---------------------------------------------------------
    // Vérification pour in, out
    // ---------------------------------------------------------
    else if (opcode == 28 || opcode == 29)
    {
        if (!is_registry(token) || check_and_remove_comma(token))
            return 0;

        // Vérifier qu'il n'y a rien après
        token = strtok(NULL, " \t\n\r");
        if (token != NULL)
            return 0;

        return 1;
    }

    // ---------------------------------------------------------
    // Vérification pour les instructions arithmétiques et rnd
    // ---------------------------------------------------------
    else if ((opcode >= 0 && opcode <= 7) || opcode == 30)
    {
        if (!check_and_remove_comma(token) || !is_registry(token))
            return 0; // Il manque une virgule ou ce n'est pas un registre

        token = strtok(NULL, " \t\n\r");
        if (!token || !check_and_remove_comma(token) || !is_registry(token)) // rn doit être un registre, et sans virgule
            return 0;

        // Vérification pour S
        token = strtok(NULL, " \t\n\r");
        if (!token || check_and_remove_comma(token))
            return 0;
        int s_is_reg = is_registry(token);
        int s_is_imm = 0;
        if (!s_is_reg)
        {
            if (token[0] == '#')
            {
                if (is_ImmValue_ok(token + 1, 0) != -1)
                    s_is_imm = 1;
            }
        }

        if (!s_is_reg && !s_is_imm) // S n'est ni un registre ni une valeur immédiate
            return 0;

        // Vérifier qu'il n'y a rien après
        token = strtok(NULL, " \t\n\r");
        if (token != NULL)
            return 0;

        return 1;
    }

    return 0;
}

int gestion_erreurs(FILE *file, int *line_index, Tag *tag_table, int *tag_count)
// Fonction qui renvoie 1 si le fichier n'a pas d'erreur (i.e. peut être traduit en hexadécimal), 0 sinon
{
    char line[64];
    int pc = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        (*line_index)++; // On note qu'on passe à la ligne suivante (en cas d'erreur, on saura que c'est cette ligne)
        clean(line);

        if (is_line_ok(line, &pc, tag_table, tag_count) == 0)
        {
            return 0; // Si on détecte une erreur, on renvoie 0
        }
    }
    return 1;
}

// ============================
// ----------------------------
//  TRANSLATE_LINE.c
// ----------------------------
// ============================

void binary_to_hex(const char *str_binary, char *str_hex)
{
    const char *hex_digits = "0123456789ABCDEF";
    int len = strlen(str_binary);
    int hex_index = 0;

    int remainder = len % 4;

    if (remainder > 0)
    {
        int val = 0;
        for (int i = 0; i < remainder; i++)
        {
            val = (val << 1) | (str_binary[i] - '0');
        }
        str_hex[hex_index++] = hex_digits[val];
    }

    for (int i = remainder; i < len; i += 4)
    {
        int val = 0;
        for (int j = 0; j < 4; j++)
        {
            val = (val << 1) | (str_binary[i + j] - '0');
        }
        str_hex[hex_index++] = hex_digits[val];
    }

    str_hex[hex_index] = '\0';
}

int rtoi(char *r, int if_minus)
{
    char tmp[20];
    int i = 1;
    if (if_minus == 0)
        i = 1;
    else
        i = 2;
    int j = i;
    while (r[i] != '\0')
    {
        tmp[i - j] = r[i];
        i++;
    }
    tmp[i - j] = '\0';
    return atoi(tmp);
}

int hextoi(char *hex_line, int if_diez)
{

    char tmp[32];
    int i;
    int j;
    if (if_diez == 0)
        i = 1;
    else
        i = 2;
    j = i;
    while (hex_line[i] != '\0')
    {
        tmp[i - j] = hex_line[i];
        i++;
    }
    tmp[i - j] = '\0';
    return ft_atoi_base(tmp, "0123456789ABCDEF");
}

void put_next_word(char *line, char *buf, int *start_index)
{
    int j = 0;
    while (line[*start_index] != ' ' && line[*start_index] != '\t' &&
           line[*start_index] != '\0' && line[*start_index] != '\n' &&
           line[*start_index] != '\r' && line[*start_index] != ',')
    {
        buf[j] = line[*start_index];
        j++;
        (*start_index)++;
    }
    buf[j] = '\0';
}

void resultcat(char *line, int val, int len)
{
    char buf[len + 1];
    ft_putnbr_base(val, "01", buf);
    int len_tmp = strlen(buf);
    while (len_tmp < len)
    {
        for (int i = len_tmp; i > 0; i--)
        {
            buf[i] = buf[i - 1];
        }
        buf[0] = '0';
        len_tmp++;
    }
    buf[len_tmp] = '\0';
    strcat(line, buf);
}

void add_S(char *buf, char *result_in_binary, char *line)
{
    if (buf[0] == 'r')
    {
        strcat(result_in_binary, "0");
        resultcat(result_in_binary, rtoi(buf, 0), 16);
    }
    else if (buf[0] == '#')
    {
        strcat(result_in_binary, "1");
        if (buf[1] == 'h')
        {
            resultcat(result_in_binary, hextoi(buf, 1), 16);
        }
        else if (buf[1] == '-')
        {
            resultcat(result_in_binary, 65536 - rtoi(buf, 1), 16);
        }
        else
        {
            resultcat(result_in_binary, rtoi(buf, 0), 16);
        }
    }
}

void add_rnS_transfert(char *buf, char *result_in_binary, char *line)
{
    char buf2[32];
    int i = 0;
    while (buf[i] != ')')
    {
        i++;
    }
    buf[i] = '\0';
    resultcat(result_in_binary, rtoi(buf, 1), 5); // Traite le registre rn
    i++;
    int j = 0;
    while (buf[i] != '\0')
    {
        buf2[j] = buf[i];
        j++;
        i++;
    }
    buf2[j] = '\0';

    if (buf2[0] == '#')
    {
        strcat(result_in_binary, "1");
        if (buf2[1] == 'h')
            resultcat(result_in_binary, hextoi(buf2, 1), 16);
        else if (buf2[1] == '-')
            resultcat(result_in_binary, 65536 - rtoi(buf2, 1), 16);
        else
            resultcat(result_in_binary, rtoi(buf2, 0), 16);
    }
    else if (buf2[0] == 'r')
    {
        strcat(result_in_binary, "0");
        resultcat(result_in_binary, rtoi(buf2, 0), 16);
    }
    else if (buf2[0] == 'h')
    {
        strcat(result_in_binary, "1");
        resultcat(result_in_binary, hextoi(buf2, 0), 16);
    }
    else if (buf2[0] == '-')
    {
        strcat(result_in_binary, "1");
        resultcat(result_in_binary, 65536 - rtoi(buf2, 0), 16);
    }
    else if (buf2[0] >= '0' && buf2[0] <= '9')
    {
        strcat(result_in_binary, "1");
        resultcat(result_in_binary, atoi(buf2), 16);
    }
    else
    {
        strcat(result_in_binary, "00000000000000000");
    }
}

void add_rdS_transfert(char *buf, char *result_in_binary, char *line, char *bufS)
{
    char buf2[32];
    int i = 0;
    while (buf[i] != ')')
    {
        i++;
    }
    buf[i] = '\0';
    resultcat(result_in_binary, rtoi(buf, 1), 5); // Traite le registre rd
    i++;
    int j = 0;
    while (buf[i] != '\0')
    {
        buf2[j] = buf[i];
        j++;
        i++;
    }
    buf2[j] = '\0';

    if (buf2[0] == '#')
    {
        strcat(bufS, "1");
        if (buf2[1] == 'h')
            resultcat(bufS, hextoi(buf2, 1), 16);
        else if (buf2[1] == '-')
            resultcat(bufS, 65536 - rtoi(buf2, 1), 16);
        else
            resultcat(bufS, rtoi(buf2, 0), 16);
    }
    else if (buf2[0] == 'r')
    {
        strcat(bufS, "0");
        resultcat(bufS, rtoi(buf2, 0), 16);
    }
    else if (buf2[0] == 'h')
    {
        strcat(bufS, "1");
        resultcat(bufS, hextoi(buf2, 0), 16);
    }
    else if (buf2[0] == '-')
    {
        strcat(bufS, "1");
        resultcat(bufS, 65536 - rtoi(buf2, 0), 16);
    }
    else if (buf2[0] >= '0' && buf2[0] <= '9')
    {
        strcat(bufS, "1");
        resultcat(bufS, atoi(buf2), 16);
    }
    else
    {
        strcat(bufS, "00000000000000000");
    }
}

void add_S_saut(char *buf, char *result_in_binary, char *line, Tag *tag_table, int *tag_count)
{
    if (is_registry(buf))
    {
        strcat(result_in_binary, "0");
        resultcat(result_in_binary, rtoi(buf, 0), 16);
    }
    else if (buf[0] == '#' && buf[1] == 'h')
    {
        strcat(result_in_binary, "1");
        resultcat(result_in_binary, hextoi(buf, 0), 16);
    }
    else if (isdigit(buf[0]) || buf[0] == '-' || (buf[0] == '#' && isdigit(buf[1])))
    {
        strcat(result_in_binary, "1");
        int val = (buf[0] == '#') ? atoi(buf + 1) : atoi(buf);
        resultcat(result_in_binary, val, 16);
    }
    else
    {
        int addr = get_tag_address(buf, tag_table, tag_count);
        if (addr != -1)
        {
            strcat(result_in_binary, "1");
            resultcat(result_in_binary, addr, 16);
        }
        else
        {
            printf("Erreur : Etiquette '%s' inconnue lors de la traduction.\n", buf);
        }
    }
}

char *atohex_line(char *line, Tag *tag_table, int *tag_count)
{
    char buf[32];
    int cursor = 0;
    int j = 0;
    int switch_code;
    int code;

    clean(line);

    int len = strlen(line);
    if (len <= 1)
        return NULL;

    // Ne pas déplacer ce bout de code plus haut, sinon le if ci-dessus ne fera pas de free()
    char *result_in_binary = malloc(sizeof(char) * 33);
    char *result_in_hexadecimal = malloc(sizeof(char) * 9);

    int index_dp = doublepoint_counter(line);

    if (index_dp > 0)
    {
        // S'il y a une étiquette, on place le curseur juste après le ':'
        cursor = index_dp + 1;

        if (line[cursor] == ' ')
            cursor++;
    }

    // On lit le mot à partir du curseur jusqu'au prochain espace
    while (line[cursor] != ' ' && line[cursor] != '\0' && line[cursor] != '\n' &&
           line[cursor] != ',' && line[cursor] != '\t' && line[cursor] != '\r')
    {
        buf[j] = line[cursor];
        cursor++;
        j++;
    }
    buf[j] = '\0';

    result_in_binary[0] = '\0'; // On vide la chaîne pour être sûr
    resultcat(result_in_binary, get_opcode(buf), 5);

    code = get_opcode(buf);
    // 1 - arithmetiques et logiques 2 - transfert load
    // 3 - transfert store 4 - sauts 5 - entree-sortie 6 - diverses
    if (code >= 0 && code <= 7)
    {
        switch_code = 1;
    }
    else if (code >= 12 && code <= 14)
    {
        switch_code = 2;
    }
    else if (code >= 15 && code <= 17)
    {
        switch_code = 3;
    }
    else if (code >= 21 && code <= 27)
    {
        switch_code = 4;
    }
    else if (code >= 28 && code <= 29)
    {
        switch_code = 5;
    }
    else if (code == 30)
    {
        switch_code = 6;
    }
    else if (code == 31)
    {
        switch_code = 7;
    }
    while (line[cursor] == ' ' || line[cursor] == '\t' || line[cursor] == ',')
        cursor++;
    switch (switch_code)
    {
    case 1:
        // rd
        put_next_word(line, buf, &cursor);
        resultcat(result_in_binary, rtoi(buf, 0), 5);
        while (line[cursor] == ' ' || line[cursor] == ',' || line[cursor] == '\t')
            cursor++;
        // rn
        put_next_word(line, buf, &cursor);
        resultcat(result_in_binary, rtoi(buf, 0), 5);
        while (line[cursor] == ' ' || line[cursor] == ',' || line[cursor] == '\t')
            cursor++;

        // S
        put_next_word(line, buf, &cursor);
        add_S(buf, result_in_binary, line);
        break;
    case 2:
        // rd
        put_next_word(line, buf, &cursor);
        resultcat(result_in_binary, rtoi(buf, 0), 5);
        while (line[cursor] == ' ' || line[cursor] == ',' || line[cursor] == '\t')
            cursor++;

        //(rn)S
        put_next_word(line, buf, &cursor);
        add_rnS_transfert(buf, result_in_binary, line);
        break;
    case 3:
    {
        // rd + S->bufS
        char bufS[32];
        bufS[0] = '\0';
        put_next_word(line, buf, &cursor);
        add_rdS_transfert(buf, result_in_binary, line, bufS);
        while (line[cursor] == ' ' || line[cursor] == ',' || line[cursor] == '\t')
            cursor++;

        // rn
        put_next_word(line, buf, &cursor);
        resultcat(result_in_binary, rtoi(buf, 0), 5);
        while (line[cursor] == ' ' || line[cursor] == ',' || line[cursor] == '\t')
            cursor++;

        // S
        strcat(result_in_binary, bufS);

        break;
    }
    case 4:
        // S
        put_next_word(line, buf, &cursor);
        strcat(result_in_binary, "0000000000");
        add_S_saut(buf, result_in_binary, line, tag_table, tag_count);
        break;
    case 5:
        // rd
        put_next_word(line, buf, &cursor);
        resultcat(result_in_binary, rtoi(buf, 0), 5);
        strcat(result_in_binary, "0000000000000000000000");
        break;
    case 6:
        // rd
        put_next_word(line, buf, &cursor);
        resultcat(result_in_binary, rtoi(buf, 0), 5);
        while (line[cursor] == ' ' || line[cursor] == ',' || line[cursor] == '\t')
            cursor++;

        // rn
        put_next_word(line, buf, &cursor);
        resultcat(result_in_binary, rtoi(buf, 0), 5);
        while (line[cursor] == ' ' || line[cursor] == ',' || line[cursor] == '\t')
            cursor++;

        // strcat(result_in_binary, "1");
        // S
        put_next_word(line, buf, &cursor);
        add_S(buf, result_in_binary, line);
        break;
    case 7:
        strcat(result_in_binary, "000000000000000000000000000");
        break;
    default:
        printf("Error in switch\n");
        return NULL;
    }
    // printf("%s\n", result_in_binary);

    binary_to_hex(result_in_binary, result_in_hexadecimal);
    free(result_in_binary);
    return result_in_hexadecimal;
}

int translateCode(char fileName[], Tag *tag_table, int *tag_count)
{
    FILE *file = fopen(fileName, "r");

    if (!read_all_tags(file, tag_table, tag_count))
    {
        return 1;
    }

    rewind(file); // On repart au début du fichier

    int line_index = 0; // On commence à 0 car la boucle dans gestion_erreurs() incrémente line_index au début
    int is_file_ok = gestion_erreurs(file, &line_index, tag_table, tag_count);

    if (is_file_ok)
    {
        rewind(file); // On rembobine pour relire le fichier du début
        FILE *output = fopen("hexa.txt", "w");

        char line[64];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            char *hex_string = atohex_line(line, tag_table, tag_count);

            if (hex_string != NULL)
            {
                fprintf(output, "%s\n", hex_string);
                free(hex_string);
            }
        }

        fclose(output);
    }

    else
    {
        printf("Erreur ligne %d\n", line_index);
        return 1;
    }

    fclose(file);

    return 0;
}