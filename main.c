#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 997

typedef struct word{
    char document[30];
    char word[30];
}word;

int readFromFile(char*, word*, int*);
int hashSearch (word*, char*, int);
int insertTable(word*,int, char*, char*, int*);

void writeWords(word* , int, const char * );
int readWords(word hashTable[], const char * fileName);

int hornersMethod(char*);
int h_1(int, int);
int h_2(int, int);
int hash(int, int, int);

int copyString(char*, char[], unsigned int);
int stringLength(char*);
int addDocument(word **, char*, int);


int main() {
    word hashTable[M] = {NULL};
    int totalWordsAdded = 0;
    char stringTmp[30];
    int operation;
    int programLoop = 1;
    int position;
    readWords(hashTable, "2.txt");

    printf("Welcome to the Hash Dictionary. Please Select an operation\n");

   while (programLoop == 1) {
    printf("<1> Search\t<2> Add a Word\t<3> Read from file");
    scanf("%d", &operation);

        if (operation == 1) {
            printf("\nPlease enter the word you are looking for:");
            scanf("%s", stringTmp);
            strlwr(stringTmp);
            position = hashSearch(hashTable, stringTmp, M);
            if (position == -1) {
                printf("nope\n");
            } else {
                printf("located at: %d\n", position);
            }
        } else if (operation == 2){
            printf ("Word you wanna add:");
            scanf("%s", stringTmp);
            strlwr(stringTmp);
            position = hashSearch(hashTable, stringTmp, M);
            if (position == -1) {
                insertTable(hashTable, M, stringTmp, "user", &totalWordsAdded);
            } else {
                printf("The word you are looking for is already exists in the dictionary and located at: %d\n", position);
            }
        }else{
            printf ("Please enter the file path");
            scanf ("%s", stringTmp);
            readFromFile(stringTmp, hashTable, &totalWordsAdded);
        }

        writeWords(hashTable, M, "2.txt");

        printf ("there are %d words in this dictionary\n%0.3f\n", totalWordsAdded, (float)totalWordsAdded/(float)M);
        printf ("do you wanna continue");
        scanf ("%d", &programLoop);
    }

    return 0;
}

int hornersMethod(char *word){
    int key = 0;
    int i;
    unsigned int L = stringLength(word);
    for (i = 0; i < L ; i++){
        key =( (31 * key) + (*(word + i)) ) % M;
    }
    key = (key & 0x7fffffff) % M;                   //clearing sign bit
    return key;
}

int h_1(int key, int tableSize){
    key = key % tableSize;
    return key;
}

int h_2(int key, int tableSize){
    key = key % (tableSize * tableSize);
    return key;
}

int hash(int key, int i, int tableSize){
    int h = 0;
    h = h_1(key,tableSize) + (i * h_2(key, tableSize));
    h = h % tableSize;
    return h;
}

int copyString(char *tableElement, char tmp[], unsigned int size){
    int i = 0;
    for (i=0 ; i<size ; i++){
        *(tableElement + i) = tmp[i];
    }
    *(tableElement + i) = '\0';
    return 1;
}

int insertTable(word hashTable[], int tableSize, char wordPara[], char source[], int *wordCount){
    unsigned int wordLength = strlen(wordPara);
    int i = 0;
    int key = hornersMethod(wordPara);
    int h = hash(key, i, tableSize);
    int lastHash = h;
    while (i < tableSize && strcmp(hashTable[h].word, "*") != 0){
        if(strcmp(hashTable[h].word, wordPara) == 0){
            return -1;
        }
        lastHash = h;
        h = hash(key, i, tableSize);
        i++;
    }

    h = lastHash;
    if (i != tableSize){
        copyString(hashTable[lastHash].word, wordPara, wordLength);

        wordLength = strlen(source);
        copyString(hashTable[lastHash].document, source, wordLength);
        *wordCount = *wordCount + 1;
        return h;
    }
    else{
        printf ("Error?");
        exit(0);
    }
}

int stringLength(char* word){
    int i = 0;
    while (*(word + i) != NULL){
        i++;
    }
    return i;
}

int hashSearch(word hashTable[], char wordPara[], int tableSize ){
    int i = 0;
    int key = hornersMethod(wordPara);
    int h = hash(key, i, tableSize);


    while (i < tableSize && hashTable[h].word != NULL){
            if(strcmp(hashTable[h].word, wordPara) == 0){
                return h;
            }
            h = hash(key, i, tableSize);
            i++;
    }

    if (i >= tableSize || i == 0){
        return -1;
    }
}

int readFromFile (char filename[], word hashTable[], int *wordCount){
    FILE *fp;
    char stringTmp[30];
    int position;
    unsigned int lastElement;

    fp = fopen(filename, "r");

    while (fgets(stringTmp, 30, fp) != NULL){
        lastElement = strlen(stringTmp);
        stringTmp[lastElement - 1] = '\0';
        position = hashSearch(hashTable, stringTmp, M);
        if (position == -1) {
         insertTable(hashTable, M, stringTmp, filename, wordCount);
        }
        else printf ("the word %s already exists ", stringTmp);
    }
}

void writeWords(word hashTable[], int items, const char * fileName)
{
    FILE* ptr = fopen(fileName,"wb");
    if(! ptr) return; // TODO: better error handling
    fwrite( hashTable, sizeof(word), items, ptr);
    fclose(ptr);
}


int readWords(word hashTable[], const char * fileName)
{
    FILE* ptr = fopen(fileName,"rb");
    if( !ptr ) return 0;
    int n = 0;
    for (n=0; !feof(ptr); ++n)
    {
        if ( fread(&hashTable[n],sizeof(word),1,ptr) != 1) break;
    }

    fclose(ptr);
    return n;
}


