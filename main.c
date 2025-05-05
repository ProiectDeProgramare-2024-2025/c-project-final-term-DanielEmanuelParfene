#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define parolaUniversala "ana"
#define ROSU "\033[0;31m"
#define VERDE "\033[0;32m"
#define ALBASTRU "\033[0;34m"
#define MOV "\033[0;35m"
#define ALB "\033[0;37m"


// Structura care retine datele despre fiecare cont bancar
typedef struct {
    char numarCont[13], parola[5], nume[13], prenume[13];
    int sold;
} contBancar;


// Declarare functii
// Functii ajutatoare
void esteAdministrator(char* parola);
FILE* deschideFisierul(char* numeFisier, char tipDeschidere);
void printeazaSpatiiAlbe(int numarDeCaractere);
void mutaLaFinalDeLinie(FILE* fisier);
void auxiliarPrintare(contBancar* cont, int index);
void printeaza(contBancar* cont, int pozStart, int numarDeCampuri);
void printeazaHeader(int pozStart, int numarDeCampuri);
int transformaStringInInt(char* numar);
int numarCifre(int numar);
char* baniRamasi(int baniInitiali, int baniRetrasi);
void comandaCorecta(int argc, int numarParametrii);
void afisareOptiuneNeimplementata(char* optiune);
void esteContValid(char* cont);
void esteNumeValid(char* nume, char* prenume);
void esteNumarValid(char* suma, int size);
// Functiile comenzilor
void vizualizareConturi(FILE* fisier);
void vizualizareNumarConturi(FILE* fisier);
void vizualizareClienti(FILE* fisier);
void vizualizareContClient_NumarCont(FILE* fisier, char* numarCont);
void vizualizareContClient_NumeClient(FILE* fisier, char* nume, char* prenume);
void creareCont(FILE* fisier1, FILE* fisier2, char* parola, char* nume, char* prenume);
void stergereCont(FILE* fisier1, char* numarCont);
void vizualizareContPersonal(FILE* fisier, char* numarCont, char* parola);
void adaugareBaniContPersonal(FILE* fisier, char* numarCont, char* parola, char* baniDepusi);
void retragereBaniContPersonal(FILE* fisier, char* numarCont, char* parola, char* baniRetrasi);
void help();
void optiuneNeimplementata(char* optiune);
void optiuneLipsa();


int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "vizualizareConturi") == 0) {
            comandaCorecta(argc, 3);
            esteAdministrator(argv[2]);
            vizualizareConturi(deschideFisierul("DateBancare.txt", 'r'));
        } else if (strcmp(argv[1], "vizualizareNumarConturi") == 0) {
            comandaCorecta(argc, 3);
            esteAdministrator(argv[2]);
            vizualizareNumarConturi(deschideFisierul("DateBancare.txt", 'r'));
        } else if (strcmp(argv[1], "vizualizareClienti") == 0) {
            comandaCorecta(argc, 3);
            esteAdministrator(argv[2]);
            vizualizareClienti(deschideFisierul("DateBancare.txt", 'r'));
        } else if (strcmp(argv[1], "vizualizareContClient_NumarCont") == 0) {
            comandaCorecta(argc, 4);
            esteAdministrator(argv[2]);
            esteContValid(argv[3]);
            vizualizareContClient_NumarCont(deschideFisierul("DateBancare.txt", 'r'), argv[3]);
        } else if (strcmp(argv[1], "vizualizareContClient_NumeClient") == 0) {
            comandaCorecta(argc, 5);
            esteAdministrator(argv[2]);
            esteNumeValid(argv[3], argv[4]);
            vizualizareContClient_NumeClient(deschideFisierul("DateBancare.txt", 'r'), argv[3], argv[4]);
        } else if (strcmp(argv[1], "creareCont") == 0) {
            comandaCorecta(argc, 6);
            esteAdministrator(argv[2]);
            esteNumeValid(argv[4], argv[5]);
            esteNumarValid(argv[3], 4);
            creareCont(deschideFisierul("DateBancare.txt", 'r'), deschideFisierul("DateBancare.txt", 'a'), argv[3], argv[4], argv[5]);
        } else if (strcmp(argv[1], "stergereCont") == 0) {
            comandaCorecta(argc, 4);
            esteAdministrator(argv[2]);
            esteContValid(argv[3]);
            stergereCont(deschideFisierul("DateBancare.txt", 'r'), argv[3]);
        } else if (strcmp(argv[1], "vizualizareContPersonal") == 0) {
            comandaCorecta(argc, 4);
            esteContValid(argv[2]);
            esteNumarValid(argv[3], 4);
            vizualizareContPersonal(deschideFisierul("DateBancare.txt", 'r'), argv[2], argv[3]);
        } else if (strcmp(argv[1], "adaugareBaniContPersonal") == 0) {
            comandaCorecta(argc, 5);
            esteContValid(argv[2]);
            esteNumarValid(argv[4], -1);
            esteNumarValid(argv[3], 4);
            adaugareBaniContPersonal(deschideFisierul("DateBancare.txt", 'u'), argv[2], argv[3], argv[4]);
        } else if (strcmp(argv[1], "retragereBaniContPersonal") == 0) {
            comandaCorecta(argc, 5);
            esteContValid(argv[2]);
            esteNumarValid(argv[4], -1);
            esteNumarValid(argv[3], 4);
            retragereBaniContPersonal(deschideFisierul("DateBancare.txt", 'u'), argv[2], argv[3], argv[4]);
        } else if (strcmp(argv[1], "help") == 0) {
            help();
        }  else {
            optiuneNeimplementata(argv[1]);
        }
    } else {
        help();
    }

    printf(ALB);

    return 0;
}


// Functii ajutatoare
void esteAdministrator(char* parola) {
    if (strcmp(parola, parolaUniversala)) {
        printf("NU esti administrator! Accesul la aceasta functie a sistemului nu iti este permis");
        exit(1);
    }
}

FILE* deschideFisierul(char* numeFisier, char tipDeschidere) {
    FILE* fisier = NULL;
    if (tipDeschidere == 'r') {
        fisier = fopen(numeFisier, "rb");
    } else if (tipDeschidere == 'w') {
        fisier = fopen(numeFisier, "wb");
    } else if (tipDeschidere == 'a') {
        fisier = fopen(numeFisier, "ab");
    } else if (tipDeschidere == 'u') {
        fisier = fopen(numeFisier, "rb+");
    }
    if (fisier == NULL) {
        printf("Nu se pot manipula datele bancii");
        exit(1);
    }
    return fisier;
}

void printeazaSpatiiAlbe(int numarDeCaractere) {
    for (int i = 0; i < numarDeCaractere; i ++) {
        printf(" ");
    }
}

void mutaLaFinalDeLinie(FILE* fisier) {
    char c = 'a';
    while ((c = fgetc(fisier)) != '\n' && c != EOF);
}

void auxiliarPrintare(contBancar* cont, int index) {
    if (index == 0) {
        printf(ROSU);
        printf("%s", cont -> numarCont);
        printeazaSpatiiAlbe(5 + 12 - strlen(cont -> numarCont));
    } else if (index == 1) {
        printf(VERDE);
        printf("%s", cont -> nume);
        printeazaSpatiiAlbe(5 + 12 - strlen(cont -> nume));
    } else if (index == 2) {
        printf(ALBASTRU);
        printf("%s", cont -> prenume);
        printeazaSpatiiAlbe(5 + 12 - strlen(cont -> prenume));
    } else if (index == 3) {
        printf(MOV);
        printf("%d", cont -> sold);
    }
}

void printeaza(contBancar* cont, int pozStart, int numarDeCampuri) {
    // cont, nume, prenume, sold
    for (int i = pozStart; i < pozStart + numarDeCampuri; i ++) {
        auxiliarPrintare(cont, i);
    }
    printf("\n");
}

void printeazaHeader(int pozStart, int numarDeCampuri) {
    char* arr[] = {"Iban", "Nume", "Prenume", "Sold"};
    for (int i = pozStart; i < pozStart + numarDeCampuri; ++ i) {
        printf("%s", arr[i]);
        printeazaSpatiiAlbe(5 + 12 - strlen(arr[i]));
    }
    printf("\n");
    for (int i = 0; i < numarDeCampuri; ++ i) {
        printeazaSpatiiAlbe(5 + 12);
    }
    printf("\n");
}

char* urmatorulNumarCont(char* cont) {
    while (!(*cont >= '0' && *cont <= '9')) {
        cont ++;
    }
    int numar = 0;
    while (*cont != '\0') {
        numar = numar * 10 + (*cont - '0');
        cont ++;
    }
    ++ numar;
    char* numarCont = (char*) malloc(6 * sizeof(char));
    strcpy(numarCont, "00000");
    int index = 4;
    while (numar) {
        numarCont[index --] = (char) ((numar % 10) + '0');
        numar /= 10;
    }
    return numarCont;
}

int transformaStringInInt(char* numar) {
    int noulNumar = 0;
    while (*numar != '\0') {
        noulNumar = noulNumar * 10 + (*numar - '0');
        numar ++;
    }
    return noulNumar;
}

int numarCifre(int numar) {
    int k = 0;
    while(numar) {
        k ++;
        numar /= 10;
    }
    return k;
}

char* baniRamasi(int baniInitiali, int baniRetrasi) {
    int baniRamasi = baniInitiali - baniRetrasi;
    if (baniRamasi < 0) {
        printf("Fonduri insuficiente!");
        exit(1);
    }
    int diferentaCifre = numarCifre(baniInitiali) - numarCifre(baniRamasi);
    char* bani = (char*)malloc(10 * sizeof(char));
    int i = 0;
    if (baniRamasi == 0) {
        *(bani) = '0';
        i ++;
    }
    while (baniRamasi) {
        *(bani + i) = (char) ((baniRamasi % 10) + '0');
        i ++;
        baniRamasi /= 10;
    }
    int j = 0, n = i --;
    while (j < i) {
        char aux = *(bani + j);
        *(bani + j) = *(bani + i);
        *(bani + i) = aux;
        j ++;
        i --;
    }
    while (diferentaCifre > 0) {
        *(bani + n) = ' ';
        n ++;
        diferentaCifre --;
    }
    *(bani + n) = '\0';
    return bani;
}

void comandaCorecta(int argc, int numarParametrii) {
    if (argc != numarParametrii) {
        printf("Sintaza comenzii nu este corecta. Apelati comanda help pentru a vedea sintaxa fiecarei comenzi.\n");
        exit(1);
    }
}

void afisareOptiuneNeimplementata(char* optiune) {
    printf("Optiune %s nu este implementata. Ea este disponibila in versiunea 2 a sistemului", optiune);
    exit(0);
}

void esteContValid(char* cont) {
    if (strstr(cont, "IRoBTRL") != cont || strlen(cont) != 12) {
        printf("Cont invalid");
        exit(1);
    }
    int i = 7;
    while (i < 12) {
        if (!(cont[i] >= '0' && cont[i] <= '9')) {
            printf("Cont invalid");
            exit(1);
        }
        ++ i;
    }
}

void esteNumeValid(char* nume, char* prenume) {
    if (!(nume[0] >= 'A' && nume[0] <= 'Z') || !(prenume[0] >= 'A' && prenume[0] <= 'Z')) {
        printf("Nume invalid");
        exit(0);
    }

    for (int i = 1; nume[i] != '\0'; ++ i) {
        if (!((nume[i] >= 'a' && nume[i] <= 'z') || (nume[i] >= 'A' && nume[i] <= 'Z'))) {
            printf("Nume invalid");
            exit(0);
        }
    }
    for (int i = 1; prenume[i] != '\0'; ++ i) {
        if (!((prenume[i] >= 'a' && prenume[i] <= 'z') || (prenume[i] >= 'A' && prenume[i] <= 'Z'))) {
            printf("Nume invalid");
            exit(0);
        }
    }
}

void esteNumarValid(char* suma, int size) {
    if (size != -1) {
        if (strlen(suma) != size) {
            printf("Numar invalid");
            exit(0);
        }
    }

    for (int i = 0; suma[i] != '\0'; ++ i) {
        if (!(suma[i] >= '0' && suma[i] <= '9')) {
            printf("Numar invalid");
            exit(0);
        }
    }
}


// Functiile comenzilor
void vizualizareConturi(FILE* fisier) {
    contBancar cont;
    printeazaHeader(0, 4);
    while (fscanf(fisier, "%s %s %s %s %d", &cont.numarCont, &cont.parola, &cont.nume, &cont.prenume, &cont.sold) == 5) {
        printeaza(&cont, 0, 4);
    }
    fclose(fisier);
}

void vizualizareNumarConturi(FILE* fisier) {
    contBancar cont;
    printeazaHeader(0, 1);
    while (fscanf(fisier, "%s", &cont.numarCont) == 1) {
        printeaza(&cont, 0, 1);
        mutaLaFinalDeLinie(fisier);
    }
    fclose(fisier);
}

void vizualizareClienti(FILE* fisier) {
    contBancar cont;
    printeazaHeader(1, 2);
    while (fscanf(fisier, "%s %s %s %s", &cont.numarCont, &cont.parola, &cont.nume, &cont.prenume) == 4) {
        printeaza(&cont, 1, 2);
        mutaLaFinalDeLinie(fisier);
    }
    fclose(fisier);
}

void vizualizareContClient_NumarCont(FILE* fisier, char* numarCont) {
    contBancar cont;
    printeazaHeader(0, 4);
    while (fscanf(fisier, "%s", &cont.numarCont) == 1) {
        if (strcmp(cont.numarCont, numarCont) == 0) {
            fscanf(fisier, "%s %s %s %d", &cont.parola, &cont.nume, &cont.prenume, &cont.sold);
            printeaza(&cont, 0, 4);
            break;
        }
        mutaLaFinalDeLinie(fisier);
    }
    fclose(fisier);
}

void vizualizareContClient_NumeClient(FILE* fisier, char* nume, char* prenume) {
    contBancar cont;
    printeazaHeader(0, 4);
    while (fscanf(fisier, "%s %s %s %s %d", &cont.numarCont, &cont.parola, &cont.nume, &cont.prenume, &cont.sold) == 5) {
        if ((strcmp(cont.nume, nume) == 0) && (strcmp(cont.prenume, prenume) == 0)) {
            printeaza(&cont, 0, 4);
        }
    }
    fclose(fisier);
}

void creareCont(FILE* fisier1, FILE* fisier2, char* parola, char* nume, char* prenume) {
    char ultimulNumarCont[13];
    fseek(fisier1, 0, SEEK_END);
    long pozition = ftell(fisier1);
    fseek(fisier1, 0, SEEK_SET);
    while (fscanf(fisier1, "%s", ultimulNumarCont) == 1) {
        mutaLaFinalDeLinie(fisier1);
    }
    char* numar = urmatorulNumarCont(ultimulNumarCont);
    contBancar cont;
    char* numarCont = (char*) malloc(13 * sizeof(13));
    strcpy(numarCont, "IRoBTRL");
    strcat(numarCont, numar);
    strcpy(cont.numarCont, numarCont);
    strcpy(cont.parola, parola);
    strcpy(cont.nume, nume);
    strcpy(cont.prenume, prenume);
    cont.sold = 0;
    if (pozition != 0) {
        fprintf(fisier2, "\n");
    }
    fprintf(fisier2, "%s %s %s %s %d          ", cont.numarCont, cont.parola, cont.nume, cont.prenume, cont.sold);
    fclose(fisier1);
    fclose(fisier2);
}

void stergereCont(FILE* fisier1, char* numarCont) {
    FILE* fisier2 = deschideFisierul("DateBancareTemporare.txt", 'w');
    contBancar cont;
    int primul = 0;
    while (fscanf(fisier1, "%s %s %s %s %d", &cont.numarCont, &cont.parola, &cont.nume, &cont.prenume, &cont.sold) == 5) {
        if (strcmp(cont.numarCont, numarCont) != 0) {
            if (primul) {
                fprintf(fisier2, "\n");
            }
            if (primul == 0) {
                primul = 1;
            }
            fprintf(fisier2, "%s %s %s %s %d          ", cont.numarCont, cont.parola, cont.nume, cont.prenume, cont.sold);
            mutaLaFinalDeLinie(fisier1);
        }
    }
    fclose(fisier1);
    fclose(fisier2);
    remove("DateBancare.txt");
    rename("DateBancareTemporare.txt", "DateBancare.txt");
}

void vizualizareContPersonal(FILE* fisier, char* numarCont, char* parola) {
    contBancar cont;
    printeazaHeader(0, 4);
    while (fscanf(fisier, "%s", &cont.numarCont) == 1) {
        if (strcmp(cont.numarCont, numarCont) == 0) {
            fscanf(fisier, "%s", &cont.parola);
            if (strcmp(cont.parola, parola) == 0) {
                fscanf(fisier, "%s %s %d", &cont.nume, &cont.prenume, &cont.sold);
                printeaza(&cont, 0, 4);
            } else {
                printf("parola este gresita");
            }
            break;
        }
        mutaLaFinalDeLinie(fisier);
    }
    fclose(fisier);
}

void adaugareBaniContPersonal(FILE* fisier, char* numarCont, char* parola, char* baniDepusi) {
    contBancar cont;
    while (fscanf(fisier, "%s", &cont.numarCont) == 1) {
        if (strcmp(cont.numarCont, numarCont) == 0) {
            fscanf(fisier, "%s", &cont.parola);
            if (strcmp(cont.parola, parola) == 0) {
                fscanf(fisier, "%s %s ", &cont.nume, &cont.prenume);
                long pozitie = ftell(fisier);
                fscanf(fisier, "%d ", &cont.sold);
                cont.sold += transformaStringInInt(baniDepusi);
                fseek(fisier, pozitie, SEEK_SET);
                fprintf(fisier, "%d", cont.sold);
            } else {
                printf("parola este gresita");
            }
            break;
        }
        mutaLaFinalDeLinie(fisier);
    }
    fclose(fisier);
}

void retragereBaniContPersonal(FILE* fisier, char* numarCont, char* parola, char* baniRetrasi) {
    contBancar cont;
    while (fscanf(fisier, "%s", &cont.numarCont) == 1) {
        if (strcmp(cont.numarCont, numarCont) == 0) {
            fscanf(fisier, "%s", &cont.parola);
            if (strcmp(cont.parola, parola) == 0) {
                fscanf(fisier, "%s %s ", &cont.nume, &cont.prenume);
                long pozitie = ftell(fisier);
                fscanf(fisier, "%d ", &cont.sold);
                char* bani = baniRamasi(cont.sold, transformaStringInInt(baniRetrasi));
                fseek(fisier, pozitie, SEEK_SET);
                fprintf(fisier, "%s", bani);
            } else {
                printf("parola este gresita");
            }
            break;
        }
        mutaLaFinalDeLinie(fisier);
    }
    fclose(fisier);
}

void help() {
    printf("1. vizualizareConturi:     - prin aceasta comanda se poate vizualiza detaliile despre toate conturile, cu exceptia parolelor, in cazul in care esti administrator\n");
    printf("                           - parola administratorului\n");
    printf("2. vizualizareNumarConturi:     - prin aceasta comanda se poate vizualiza toate numerele de conturi existente in cazul in care esti administrator\n");
    printf("                                - parola administratorului\n");
    printf("3. vizualizareClienti:     - prin aceasta comanda se poate vizualiza toti clientii bancii in cazul in care esti administrator\n");
    printf("                           - parola administratorului\n");
    printf("4. vizualizareContClient_NumarCont:     - prin aceasta comanda se poate vizualiza detaliile despre contul dorit, cu exceptia parolei, in cazul in care esti administrator\n");
    printf("                                        - parola administratorului, numar cont\n");
    printf("5. vizualizareContClient_NumeClient:     - prin aceasta comanda se poate vizualiza detaliile despre contul dorit, cu exceptia parolei, in cazul in care esti administrator\n");
    printf("                                         - parola administratorului, nume complet client\n");
    printf("6. creareCont:     - prin aceasta comanda se poate adauga un cont nou in cazul in care esti administrator\n");
    printf("                   - parola administratorului, parola viitorului cont, numele si prenumele clientului\n");
    printf("7. stergereCont:     - prin aceasta comanda se poate sterge un cont existent in cazul in care esti administrator\n");
    printf("                     - parola administratorului, numar cont\n");
    printf("8. vizualizareContPersonal:     - prin aceasta comanda se poate vizualiza detaliile contului personal\n");
    printf("                                - numar cont, parola contului\n");
    printf("9. adaugareBaniContPersonal:     - prin aceasta comanda se poate adauga bani la contul personal\n");
    printf("                                 - numar cont, parola contului, bani de adaugat\n");
    printf("10. retragereBaniContPersonal:     - prin aceasta comanda se poate retrage bani din contul personal\n");
    printf("                                   - numar cont, parola contului, bani de retras\n");
}

void optiuneNeimplementata(char* optiune) {
    if (strcmp(optiune, "creareContEconomi") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "creareContVirtual") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "recreazaContVirtual") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "transferIntreConturi") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "cererePrietenie") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "acceptareCererePrietenie") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "transfera") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "vizualizareTransferuri") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "tranzactie") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "vizualizareTranzactii") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "plata") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else if (strcmp(optiune, "statistici") == 0) {
        afisareOptiuneNeimplementata(optiune);
    } else {
        optiuneLipsa();
    }
}

void optiuneLipsa() {
    printf("Optiune lipsa! Pentru a vedea o lista cu toate comenzile accesibile folositi optiunea help!");
}

































