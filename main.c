#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <conio.h>

typedef enum Valuta{
    EUR=001,USD=002
}Valuta;
typedef struct Operatiune{
    char codBanca[6];
    Valuta valuta;
    char codClient[11];
    double sumaOperatiune;
    struct Operatiune *next;
}Operatiune;
typedef struct CursValutar{
    char codBanca[6];
    Valuta valuta;
    double curs;
    struct CursValutar *next;
}CursValutar;

void copyUntilSeparator(const char *source, char *destination, int destinationSize, char separator){
    char *pozitieSeparator = strchr(source, separator);

    if (pozitieSeparator != NULL) {
        // Găsim separatorul în șir
        int indexSeparator = pozitieSeparator - source;
        if(indexSeparator<destinationSize){
            memmove(destination, source,  indexSeparator);
            destination[indexSeparator]='\0';
        }
        else{
            /*Dacă dimensiunea destinației e mai mica decât șirul până la separator
             * nu copiem nimic pentru a nu cauza deformarea în memorie a șirului inițial
             */
        }
    }
    else {
        // Dacă separatorul nu este găsit, șirul rămâne neschimbat
    }
}
void removeUntilSeparator(char *sir, char separator) {
    char *pozitieSeparator = strchr(sir, separator);
    if (pozitieSeparator != NULL) {
        // Găsim separatorul în șir
        int indexSeparator = pozitieSeparator - sir;
        memmove(sir, sir + indexSeparator + 1, strlen(sir) - indexSeparator);
    }
    else {
        // Dacă separatorul nu este găsit, șirul rămâne neschimbat
    }
}
void createOperation(const char codBanca[6], const char codClient[11], const Valuta valuta,  const double sumaOperatiune, Operatiune **head) {
    Operatiune *node = malloc(sizeof(Operatiune));
    if (node == NULL) {
        fputs("Error: Out of memory\n", stderr);
        exit(1);
    } else {
        strcpy(node->codBanca, codBanca);
        node->valuta=valuta;
        strcpy(node->codClient, codClient);
        node->sumaOperatiune = sumaOperatiune;
        if (*head == NULL) {
            node->next = *head;
            *head = node;
        } else {
            Operatiune *iterr = *(head);
            while (iterr->next != NULL)
                iterr = iterr->next;
            iterr->next = node;
            node->next = NULL;


        }
    }
}
void createCursValutar(const char codBanca[6], const Valuta valuta,  const double curs, CursValutar **head) {
    CursValutar *node = malloc(sizeof(CursValutar));
    if (node == NULL) {
        fputs("Error: Out of memory\n", stderr);
        exit(1);
    } else {
        strcpy(node->codBanca, codBanca);
        node->valuta=valuta;
        node->curs = curs;
        if (*head == NULL) {
            node->next = *head;
            *head = node;
        } else {
            CursValutar *iterr = *(head);
            while (iterr->next != NULL)
                iterr = iterr->next;
            iterr->next = node;
            node->next = NULL;


        }
    }
}
int parseFileOperations(Operatiune **head,char *fileName){
    FILE *fptr;
    fptr = fopen(fileName,"r");
    char operation[38];
    if(fptr == NULL)
    {
        printf("Error opening the file: '%s' !",fileName);
        getch();
        exit(1);
    }
    char codBanca[6];
    char codValuta[4];
    char codClient[11];
    double sumaOperatiune;
    int valuta;
    //Citim toate operațiunile din fișier
    while (fgets(operation, sizeof(operation), fptr)){
        if (operation[0] == '\r' || operation[0]=='\n') {
            continue; // Ignoră caracterul '\r' si '\n'
        }
        //Citim fiecare operațiune rând cu rând  în variabila operation apoi extragem din ea datele
        copyUntilSeparator(operation,codBanca,6,';');
        removeUntilSeparator(operation,';');
        copyUntilSeparator(operation,codClient,11,';');
        removeUntilSeparator(operation,';');
        copyUntilSeparator(operation,codValuta,4,';');
        removeUntilSeparator(operation,';');

        //Facem replace la ',' în '.' pentru a putea parsa suma operațiunii
        char *pozitieSeparator = strchr(operation, ',');
        if (pozitieSeparator != NULL) {
            *pozitieSeparator='.';
        }
        valuta = (int) strtod(codValuta,'\0');
        sumaOperatiune = strtod(operation, '\0');
        createOperation(codBanca,codClient,valuta,sumaOperatiune,head);

    }
    fclose(fptr);
    return 0;

}
int parseFileCursValutar(CursValutar **head,char *fileName){
    FILE *fptr;
    fptr = fopen(fileName,"r");
    char operation[27];
    if(fptr == NULL)
    {
        printf("Error opening the file: '%s' !",fileName);
        getch();
        exit(1);
    }
    char codBanca[6];
    char codValuta[4];
    double curs;
    int valuta;
    //Citim toate operațiunile din fișier
    while (fgets(operation, sizeof(operation), fptr)){
        if (operation[0] == '\r' || operation[0]=='\n') {
            continue; // Ignoră caracterul '\r' si '\n'
        }
        //Citim fiecare operațiune rând cu rând  în variabila operation apoi extragem din ea datele
        copyUntilSeparator(operation,codBanca,6,';');
        removeUntilSeparator(operation,';');
        copyUntilSeparator(operation,codValuta,4,';');
        removeUntilSeparator(operation,';');

        //Facem replace la ',' în '.' pentru a putea parsa suma operațiunii
        char *pozitieSeparator = strchr(operation, ',');
        if (pozitieSeparator != NULL) {
            *pozitieSeparator='.';
        }
        valuta = (int) strtod(codValuta,'\0');
        curs = strtod(operation, '\0');
        printf("\n curs : %lf",curs);
        createCursValutar(codBanca,valuta,curs,head);

    }
    fclose(fptr);
    return 0;

}
int writeOutputFile(Operatiune *operatiuneHead,CursValutar *cursHead, char *fileName){
    FILE *fptr;
    fptr = fopen(fileName,"w");
    if(fptr == NULL)
    {
        printf("Error opening the file: '%s' !",fileName);
        getch();
        exit(1);
    }
    double sumaOperatiuniiInLei;
    while (operatiuneHead!=NULL){
        CursValutar *head=cursHead;
        while (head!=NULL){
            if(strcmp(operatiuneHead->codBanca,head->codBanca)==0 && operatiuneHead->valuta==head->valuta){
                sumaOperatiuniiInLei=operatiuneHead->sumaOperatiune*head->curs;
                break;
            }
            else sumaOperatiuniiInLei=0;
            head=head->next;
        }
        fprintf(fptr,"%s;%s;%03d;%012.3f\n",operatiuneHead->codBanca,operatiuneHead->codClient,operatiuneHead->valuta,sumaOperatiuniiInLei);

        operatiuneHead=operatiuneHead->next;
    }
    fclose(fptr);
    return 0;
}
int main() {
    Operatiune *operatiuneHead=NULL;
    CursValutar *cursHead=NULL;
    parseFileOperations(&operatiuneHead, "operations.txt");
    parseFileCursValutar(&cursHead,"cursValutar.txt");
    writeOutputFile(operatiuneHead,cursHead,"output.txt");
    getch();
    return 0;
}
