# RÉCAPITULATIF DES TESTS

## Test 1 - Opérations bit à bit

**Description:** Teste les opérations OR, AND, XOR sur tous les bits
**Entrée utilisateur:** Aucune
**Résultat attendu:**

- 15 OR 10 = 15
- 15 AND 10 = 10
- 15 XOR 10 = 5
  **Sortie:** 15, 10, 5

---

## Test 2 - Multiplication 16 bits

**Description:** Vérifie que seuls les 16 bits de poids faible sont utilisés
**Entrée utilisateur:** Aucune
**Résultat attendu:**

- 255 \* 255 = 65025
- 256 \* 256 = 65536
  **Sortie:** 65025, 65536

---

## Test 3 - Décalages gauche/droite

**Description:** Teste les décalages à droite (positif) et à gauche (négatif)
**Entrée utilisateur:** Aucune
**Résultat attendu:**

- 8 >> 2 = 2
- 8 << 2 = 32
  **Sortie:** 2, 32

---

## Test 4 - Décalage extrême

**Description:** Teste un décalage de plus de 32 bits
**Entrée utilisateur:** Aucune
**Résultat attendu:** 255 >> 39 = 1
**Sortie:** 1

---

## Test 5 - Instruction IN valide

**Description:** Teste l'entrée d'un nombre valide (positif ou négatif)
**Entrée utilisateur:** 42
**Résultat attendu:** Le nombre entré
**Sortie:** 42

---

## Test 6 - Sauts conditionnels

**Description:** Teste les sauts basés sur Z, N, C
**Entrée utilisateur:** Aucune
**Résultat attendu:** Tous les sauts sont pris correctement
**Sortie:** 100, 200, 300

---

## Test 7 - r0 toujours zéro

**Description:** Vérifie que r0 reste à 0 même après écriture
**Entrée utilisateur:** Aucune
**Résultat attendu:** r0 = 0 toujours
**Sortie:** 0, 0, 0

---

## Test 8 - Accès mémoire

**Description:** Teste ldb/ldh/ldw et stb/sth/stw
**Entrée utilisateur:** Aucune
**Résultat attendu:** Lecture = Écriture
**Sortie:** 5, 5, 5

---

## Test 9 - Extension de signe

**Description:** Teste l'extension de signe pour ldb et ldh
**Entrée utilisateur:** Aucune
**Résultat attendu:** -1 sur 1 ou 2 octets devient -1 sur 32 bits
**Sortie:** -1, -1

---

## Test 10 - Nombres aléatoires

**Description:** Teste l'instruction rnd
**Entrée utilisateur:** Aucune
**Résultat attendu:**

- Un nombre entre 10 et 19
- 0 (car range = [0, 0])
  **Sortie:** [10-19], 0

---

## Test 11 - Valeurs immédiates négatives

**Description:** Teste les valeurs immédiates négatives
**Entrée utilisateur:** Aucune
**Résultat attendu:** -50, puis -50+100=50
**Sortie:** -50, 50

---

## Test 12 - Boucle (exemple des consignes)

**Description:** Programme qui inverse le signe jusqu'à recevoir 0
**Entrée utilisateur:** 5, -3, 0
**Résultat attendu:**

- Entrée 5 → Sortie -5
- Entrée -3 → Sortie 3
- Entrée 0 → Fin
  **Sortie:** -5, 3

---

## Test 13 - Saut avec registre

**Description:** Teste un saut indirect (adresse dans registre)
**Entrée utilisateur:** Aucune
**Résultat attendu:** Saut à l'adresse 20
**Sortie:** 999

---

## Test 14 - Programme sans HLT

**Description:** Programme qui se termine sans hlt
**Entrée utilisateur:** Aucune
**Résultat attendu:** Exécution jusqu'à la fin du code chargé
**Sortie:** 42

---

## Test 15 - Offset mémoire

**Description:** Teste les offsets immédiats et registres
**Entrée utilisateur:** Aucune
**Résultat attendu:** Lecture = Écriture avec différents offsets
**Sortie:** 77, 88

---

## Test 16 - Tous les flags

**Description:** Vérifie Z=1, N=1, Z=0, N=0
**Entrée utilisateur:** Aucune
**Résultat attendu:** Tous les tests passent
**Sortie:** 1, 1, 1, 1

---

## Test 17 - Immédiat vs Registre

**Description:** Compare opérations avec valeurs immédiates vs registres
**Entrée utilisateur:** Aucune
**Résultat attendu:** Mêmes résultats
**Sortie:** 15, 15, 50, 50

---

## Test 18 - Décalage et carry

**Description:** Vérifie le bit C lors des décalages
**Entrée utilisateur:** Aucune
**Résultat attendu:**

- 5 >> 1 met C=1 (bit perdu = 1)
- 4 >> 1 met C=0 (bit perdu = 0)
  **Sortie:** 100, 200

---

## Test 19 - Programme vide

**Description:** Juste un hlt
**Entrée utilisateur:** Aucune
**Résultat attendu:** Arrêt immédiat
**Sortie:** (aucune)

---

## Test 20 - Compteur

**Description:** Compte de 1 à 10
**Entrée utilisateur:** Aucune
**Résultat attendu:** Affiche les nombres de 1 à 10
**Sortie:** 1, 2, 3, 4, 5, 6, 7, 8, 9, 10

---

## Test 21 - Factorielle

**Description:** Calcule la factorielle d'un nombre
**Entrée utilisateur:** 5
**Résultat attendu:** 5! = 120
**Sortie:** 120

---

## Test 22 - Minimum

**Description:** Trouve le minimum de deux nombres
**Entrée utilisateur:** 15, 8
**Résultat attendu:** Le plus petit des deux
**Sortie:** 8

---

## Test 23 - Sauts sans modification des flags

**Description:** Vérifie que jmp ne modifie pas les flags
**Entrée utilisateur:** Aucune
**Résultat attendu:** Z reste à 1 après jmp
**Sortie:** 1

---

## Test 24 - Accès mémoire hors limites

**Description:** Essaye de lire à une adresse > 65535
**Entrée utilisateur:** Aucune
**Résultat attendu:** Erreur avec message explicite, arrêt propre du programme

---

## Test 25 - Écriture sur le code

**Description:** Tente d'écrire à l'adresse 0 (zone du programme)
**Entrée utilisateur:** Aucune
**Résultat attendu:** Ne doit pas interférer car même si on écrit sur r0, il est remis à 0
**Sortie:** 999

---

## Test 26 - Programme complexe

**Description:** Test de robustesse avec plusieurs opérations : (10+20)\*3-5
**Entrée utilisateur:** Aucune
**Résultat attendu:** 85, puis stockage et relecture
**Sortie:** 85, 85

---

## Test 27 - Fibonacci

**Description:** Calcule les 10 premiers termes de Fibonacci
**Entrée utilisateur:** Aucune
**Résultat attendu:** Suite de Fibonacci
**Sortie:** 1, 1, 2, 3, 5, 8, 13, 21, 34, 55

---

## Test 28 - PGCD

**Description:** Calcule le PGCD de deux nombres (algorithme d'Euclide)
**Entrée utilisateur:** 48, 18
**Résultat attendu:** PGCD(48, 18) = 6
**Sortie:** 6

---

## Test 29 - Somme des N premiers entiers

**Description:** Calcule 1+2+3+...+N
**Entrée utilisateur:** 10
**Résultat attendu:** 1+2+...+10 = 55
**Sortie:** 55

---

## Test 30 - Puissance

**Description:** Calcule a^b
**Entrée utilisateur:** 2, 8
**Résultat attendu:** 2^8 = 256
**Sortie:** 256

---

## Test 31 - Parité

**Description:** Vérifie si un nombre est pair (0) ou impair (1)
**Entrée utilisateur:** 7
**Résultat attendu:** 1 (impair)
**Sortie:** 1

---

## Test 32 - Valeur absolue

**Description:** Calcule la valeur absolue d'un nombre
**Entrée utilisateur:** -42
**Résultat attendu:** 42
**Sortie:** 42

---

## Test 33 - Comptage de bits

**Description:** Compte le nombre de bits à 1 dans un nombre
**Entrée utilisateur:** 7 (0b111)
**Résultat attendu:** 3 bits à 1
**Sortie:** 3

---

## Test 34 - Tableau en mémoire

**Description:** Stocke et relit un tableau de 5 éléments
**Entrée utilisateur:** Aucune
**Résultat attendu:** [10, 20, 30, 40, 50]
**Sortie:** 10, 20, 30, 40, 50

---

## Cas non pris en charge mais à considérer:

- Boucle infinie (Ctrl+C pour arrêter)
- Fichier hexa.txt corrompu
- Programme trop grand (> 64 Ko)
