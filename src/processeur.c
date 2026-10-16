#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

char memory[65536];      // Mémoire principale
int r[32];               // Registres généraux
int PC = 0;              // Adresse de la prochaine instruction à exécuter
int Z = 0, C = 0, N = 0; // Registre d'état

typedef struct
{
    int opcode;
    int rd;
    int rn;
    int is_imm; // 1 si le 2ème opérande est une valeur immédiate, 0 si c'est un registre
    int rm;
    int imm_val; // Valeur immédiate signée sur 32 bits
} Instruction;

int hexCharToInt(char c)
// Convertit un nombre hexadécimal à 2 chiffres en entier
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return 0;
}

void addToMemory(char *line, int *load_address)
// Fonction qui prend une ligne de hexa.txt et la stocke dans la mémoire (ATTENTION si la mémoire est pleine, la ligne n'est pas recopiée)
{
    for (int i = 0; i < 4; i++)
    {
        // On récupère les 2 caractères
        char char_high = line[2 * i];
        char char_low = line[2 * i + 1];

        if (*load_address >= 65536) // On vérifie qu'on ne dépasse pas la mémoire
        {
            return;
        }

        int byte_val = (hexCharToInt(char_high) * 16) + hexCharToInt(char_low); // conversion de la chaîne
        memory[*load_address] = (char)byte_val;

        // On incrémente l'adresse d'écriture
        (*load_address)++;
    }
}

unsigned int getInstruction32Bits(int *pc)
{
    int addr = *pc;
    if (addr < 0 || addr > 65532) // On vérifie qu'on ne sort pas de la mémoire
        return 0;

    // On lit les 4 octets (unsigned char) pour éviter que certains nombres deviennent négatifs lors du décalage.
    unsigned int b1 = (unsigned char)memory[addr];
    unsigned int b2 = (unsigned char)memory[addr + 1];
    unsigned int b3 = (unsigned char)memory[addr + 2];
    unsigned int b4 = (unsigned char)memory[addr + 3];

    unsigned int instruction = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4; // On recolle
    return instruction;
}

Instruction decode(unsigned int raw_inst)
{
    Instruction instr;
    instr.opcode = (raw_inst >> 27) & 0x1F; // 0x1F vaut 11111 en binaire (pour ne garder que les 5 bits de poids faible avce le ET)
    instr.rd = (raw_inst >> 22) & 0x1F;
    instr.rn = (raw_inst >> 17) & 0x1F;
    instr.is_imm = (raw_inst >> 16) & 0x1;

    if (instr.is_imm)
    {
        short temp = (short)(raw_inst & 0xFFFF); // On met en short pour forcer l'extension de signe (pour gérer les nombres négatifs)
        instr.imm_val = (int)temp;
        instr.rm = 0;
    }
    else // Le numéro du registre est dans les 5 bits de poids faible
    {
        instr.rm = raw_inst & 0x1F;
        instr.imm_val = raw_inst & 0x1F;
        ;
    }

    return instr;
}

void Z_update(int val)
{
    if (val == 0)
        Z = 1;
    else
        Z = 0;
}

void N_update(int val)
{
    int shift_amount = sizeof(val) * 8 - 1;
    N = ((unsigned int)val >> shift_amount) & 1;
}

void NZ_update(int val)
{
    Z_update(val);
    N_update(val);
}

int get_random_int(int min, int max)
{
    if (min > max)
        return max;
    if (max - min - 1 == 0)
        return min;
    return min + rand() % (max - min + 1);
}

int logical_shift(int value, int k)
{
    if (k > 0)
    {
        if (k <= 32)
            C = (value >> (k - 1)) & 1;
        else
            C = (k == 32) ? (value >> 31) & 1 : 0;
        return (unsigned int)value >> k;
    }
    else if (k < 0)
    {
        int shift_amt = -k;
        if (shift_amt <= 32)
            C = (value >> (32 - shift_amt)) & 1;
        else
            C = (shift_amt == 32) ? (value & 1) : 0;
        return value << (-k);
    }
    return value;
}

void execute_ligne(Instruction *instr, int *running)
{
    long result_long;
    int tmp;
    switch (instr->opcode)
    {
    case 0: // or
        if (instr->is_imm == 0)
            r[instr->rd] = r[instr->rn] | r[instr->imm_val];
        else
            r[instr->rd] = r[instr->rn] | instr->imm_val;

        NZ_update(r[instr->rd]);
        C = 0;
        PC += 4;
        break;

    case 1: // and
        if (instr->is_imm == 0)
            r[instr->rd] = r[instr->rn] & r[instr->imm_val];
        else
            r[instr->rd] = r[instr->rn] & instr->imm_val;

        NZ_update(r[instr->rd]);
        C = 0;
        PC += 4;
        break;

    case 2: // xor
        if (instr->is_imm == 0)
            r[instr->rd] = r[instr->rn] ^ r[instr->imm_val];
        else
            r[instr->rd] = r[instr->rn] ^ instr->imm_val;

        NZ_update(r[instr->rd]);
        C = 0;
        PC += 4;
        break;
    case 3: // add; overflow que "en haut"?
        if (instr->is_imm == 0)
        {
            r[instr->rd] = r[instr->rn] + r[instr->imm_val];
            result_long = (long)r[instr->rn] + r[instr->imm_val];
        }
        else
        {
            r[instr->rd] = r[instr->rn] + instr->imm_val;
            result_long = (long)r[instr->rn] + instr->imm_val;
        }
        if (result_long > INT_MAX || result_long < INT_MIN)
            C = 1;
        else
            C = 0;
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 4: // sub

        if (instr->is_imm == 0)
        {
            r[instr->rd] = r[instr->rn] - r[instr->imm_val];
            result_long = (long)r[instr->rn] - r[instr->imm_val];
        }
        else
        {
            r[instr->rd] = r[instr->rn] - instr->imm_val;
            result_long = (long)r[instr->rn] - instr->imm_val;
        }

        if (result_long > INT_MAX || result_long < INT_MIN)
            C = 1;
        else
            C = 0;
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 5: // mul

        if (instr->is_imm == 0)
        {
            r[instr->rd] = r[instr->rn] * r[instr->imm_val];
            result_long = (long)r[instr->rn] * r[instr->imm_val];
        }
        else
        {
            r[instr->rd] = r[instr->rn] * instr->imm_val;
            result_long = (long)r[instr->rn] * instr->imm_val;
        }
        if (result_long > INT_MAX || result_long < INT_MIN)
            C = 1;
        else
            C = 0;
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 6: // div
        if (instr->is_imm == 0)
        {
            if (r[instr->rm] == 0)
            {
                r[instr->rd] = 0; // Division par zéro
                C = 1;            // On peut mettre C à 1 pour signaler l'erreur ?
            }
            else
            {
                r[instr->rd] = r[instr->rn] / r[instr->rm];
                C = 0;
            }
        }
        else
        {
            if (instr->imm_val == 0)
            {
                r[instr->rd] = 0; // Division par zéro
                C = 1;            //
            }
            else
            {
                r[instr->rd] = r[instr->rn] / instr->imm_val;
                C = 0;
            }
        }

        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 7: // shr; rm: C se change dans logical_shift()
        if (instr->is_imm == 0)
        {
            if (r[instr->imm_val] != 0)
                r[instr->rd] = logical_shift(r[instr->rn], r[instr->imm_val]);
            else
                C = 0;
        }
        else
        {
            if (instr->imm_val != 0)
                r[instr->rd] = logical_shift(r[instr->rn], instr->imm_val);
            else
                C = 0;
        }
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 12: // ldb
        if (instr->is_imm == 0)
            r[instr->rd] = memory[r[instr->rn] + r[instr->imm_val]];
        else
            r[instr->rd] = memory[r[instr->rn] + instr->imm_val];
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 13: // ldh
        if (instr->is_imm == 0)
        {
            unsigned char high = (unsigned char)memory[r[instr->rn] + r[instr->imm_val]];
            unsigned char low = (unsigned char)memory[r[instr->rn] + r[instr->imm_val] + 1];
            r[instr->rd] = (short)(high << 8) | low; // Pas toucher au (short) !!! (ca étend le signe comme on est pas avec ldw)
        }
        else
        {
            unsigned char high = (unsigned char)memory[r[instr->rn] + instr->imm_val];
            unsigned char low = (unsigned char)memory[r[instr->rn] + instr->imm_val + 1];
            r[instr->rd] = (short)(high << 8) | low;
        }
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 14: // ldw
        if (instr->is_imm == 0)
        {
            unsigned char b0 = (unsigned char)memory[r[instr->rn] + r[instr->imm_val]]; // Octet de poids fort
            unsigned char b1 = (unsigned char)memory[r[instr->rn] + r[instr->imm_val] + 1];
            unsigned char b2 = (unsigned char)memory[r[instr->rn] + r[instr->imm_val] + 2];
            unsigned char b3 = (unsigned char)memory[r[instr->rn] + r[instr->imm_val] + 3]; // Octet de poids faible
            r[instr->rd] = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
        }
        else
        {
            unsigned char b0 = (unsigned char)memory[r[instr->rn] + instr->imm_val];
            unsigned char b1 = (unsigned char)memory[r[instr->rn] + instr->imm_val + 1];
            unsigned char b2 = (unsigned char)memory[r[instr->rn] + instr->imm_val + 2];
            unsigned char b3 = (unsigned char)memory[r[instr->rn] + instr->imm_val + 3];
            r[instr->rd] = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
        }

        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 15: // stb
        if (instr->is_imm == 0)
            memory[r[instr->rd] + r[instr->imm_val]] = (char)r[instr->rn];
        else
            memory[r[instr->rd] + instr->imm_val] = (char)r[instr->rn];
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 16: // sth
        if (instr->is_imm == 0)
        {
            memory[r[instr->rd] + r[instr->imm_val]] = (char)(r[instr->rn] >> 8);
            memory[r[instr->rd] + r[instr->imm_val] + 1] = (char)r[instr->rn];
        }
        else
        {
            memory[r[instr->rd] + instr->imm_val] = (char)(r[instr->rn] >> 8);
            memory[r[instr->rd] + instr->imm_val + 1] = (char)r[instr->rn];
        }
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 17: // stw
        if (instr->is_imm == 0)
        {
            memory[r[instr->rd] + r[instr->imm_val]] = (char)(r[instr->rn] >> 24);
            memory[r[instr->rd] + r[instr->imm_val] + 1] = (char)(r[instr->rn] >> 16);
            memory[r[instr->rd] + r[instr->imm_val] + 2] = (char)(r[instr->rn] >> 8);
            memory[r[instr->rd] + r[instr->imm_val] + 3] = (char)(r[instr->rn]);
        }
        else
        {
            memory[r[instr->rd] + instr->imm_val] = (char)(r[instr->rn] >> 24);
            memory[r[instr->rd] + instr->imm_val + 1] = (char)(r[instr->rn] >> 16);
            memory[r[instr->rd] + instr->imm_val + 2] = (char)(r[instr->rn] >> 8);
            memory[r[instr->rd] + instr->imm_val + 3] = (char)(r[instr->rn]);
        }
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 21: // jmp
        if (instr->is_imm == 0)
            PC = r[instr->imm_val];
        else
            PC = instr->imm_val;
        break;
    case 22: // jzs
        if (instr->is_imm == 0)
        {
            if (Z == 1)
                PC = r[instr->imm_val];
            else
                PC += 4;
        }
        else
        {
            if (Z == 1)
                PC = instr->imm_val;
            else
                PC += 4;
        }
        break;
    case 23: // jzc
        if (instr->is_imm == 0)
        {
            if (Z == 0)
                PC = r[instr->imm_val];
            else
                PC += 4;
        }
        else
        {
            if (Z == 0)
                PC = instr->imm_val;
            else
                PC += 4;
        }
        break;
    case 24: // jcs
        if (instr->is_imm == 0)
        {
            if (C == 1)
                PC = r[instr->imm_val];
            else
                PC += 4;
        }
        else
        {
            if (C == 1)
                PC = instr->imm_val;
            else
                PC += 4;
        }
        break;
    case 25: // jcc
        if (instr->is_imm == 0)
        {
            if (C == 0)
                PC = r[instr->imm_val];
            else
                PC += 4;
        }
        else
        {
            if (C == 0)
                PC = instr->imm_val;
            else
                PC += 4;
        }
        break;
    case 26: // jns
        if (instr->is_imm == 0)
        {
            if (N == 1)
                PC = r[instr->imm_val];
            else
                PC += 4;
        }
        else
        {
            if (N == 1)
                PC = instr->imm_val;
            else
                PC += 4;
        }
        break;
    case 27: // jnc
        if (instr->is_imm == 0)
        {
            if (N == 0)
                PC = r[instr->imm_val];
            else
                PC += 4;
        }
        else
        {
            if (N == 0)
                PC = instr->imm_val;
            else
                PC += 4;
        }
        break;
    case 28: // in; probleme: on peut theoriquement passer un nombre tres grand, qui va tomber sur la plage de int
        scanf("%ld", &result_long);
        tmp = (int)result_long;
        r[instr->rd] = tmp;
        if (result_long > INT_MAX || result_long < INT_MIN)
            C = 1;
        else
            C = 0;
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 29: // out
        printf("%d\n", r[instr->rd]);
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 30: // rnd
        r[instr->rd] = get_random_int(r[instr->rn], (instr->imm_val) - 1);
        NZ_update(r[instr->rd]);
        PC += 4;
        break;
    case 31: // hlt
        r[0] = 0;
        *running = 0;
        return;
    }

    r[0] = 0;
}

int executeProgram(void)
{
    srand(time(NULL));

    for (int i = 0; i < 32; i++)
    {
        r[i] = 0; // r0 devra toujours valoir 0 !
    }

    // ====================
    //  LECTURE DU FICHIER
    // ====================
    FILE *file = fopen("hexa.txt", "r");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir hexa.txt\n");
        return 1;
    }

    char line[64];
    int load_address = 0; // Adresse temporaire pour remplir la mémoire

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strlen(line) < 8)
            continue; // Ligne vide ou trop courte (normalement ca n'arrivera pas si la vérification s'est bien passée)

        addToMemory(line, &load_address);
    }
    fclose(file);

    // ============================
    //  EXECUTION DES INSTRUCTIONS
    // ============================
    PC = 0;
    int running = 1; // Variable pour arrêter la boucle quand on croise "hlt"

    while (running && PC < load_address)
    {
        unsigned int raw_inst = getInstruction32Bits(&PC);
        Instruction instr = decode(raw_inst);
        execute_ligne(&instr, &running);
    }

    return 0;
}