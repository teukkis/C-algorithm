#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct Node {
    
    char word[64];          
    struct Node * next;  //pointer to next node
} node;

//hashtable variables for both files
node *hashTable[1000] = {0};
node *hashTable2[1000] = {0};
int hashTableSize = 600;

const int amountOfPrintedWords = 50;
const int notInHashTable = -1;

//"state machine" for the control of file reading
enum file {FILE1, FILE2};
enum file state = FILE1;

//function prototypes
int hashValue(char word[]);
void addWord();
int searchWord(char nword[]);
void textFileHandling(char *fileName);

//calculates hashvalue using ASCII table values for characters.
//sum of characters's values
int hashValue(char word[]) {
    
    int wordASCIIsum = 0;
    for(int i = 0;i < strlen(word); i++){
        wordASCIIsum += word[i];
    }
    return wordASCIIsum;
}

//adds a word to hashtable. Depends on the state of state machine
void addWord(char* sana){
    
    int wordASCIIsum, key;
    node *nPtr;
    
    //allocates memory for a next node
    nPtr = (node *)malloc(sizeof(node));
    
    strcpy(nPtr->word, sana);
    
    nPtr->next = NULL;
    
    //calls hashValue() and calculates a modulus for the right key value
    wordASCIIsum = hashValue(nPtr->word);
    key = wordASCIIsum % hashTableSize;
    
    
    switch(state){
        //executes this "case" when state machine is FILE1
        case FILE1:
            if(hashTable[key] == NULL) {
                hashTable[key] = nPtr;
            }
            
            else {
                //if array is not empty -->searches the tail
                node* prevptr = hashTable[key];
                while (1) {
                    
                    if (prevptr->next == NULL) {
                        prevptr->next = nPtr;
                        break;
                    }
                    
                    prevptr = prevptr->next;
                }
            }
            
            break;
            
        case FILE2:
            //executes this "case" when state machine is FILE2
            if(hashTable2[key] == NULL) {
                hashTable2[key] = nPtr;
            }
            
            else {
                //if array is not empty -->searches the tail
                node* prevptr = hashTable2[key];
                while (1) {
                 
                    if (prevptr->next == NULL) {
                        prevptr->next = nPtr;
                        break;
                    }

                    prevptr = prevptr->next;
                }
            }
            
            break;
    }
}

//searches the specific word from hashtable
int searchWord(char nword[]){
    
    int wordASCIIsum = hashValue(nword);
    int key = wordASCIIsum % hashTableSize;
    node *nPtr;
    
    switch(state){
        
        case FILE1:
            //gets the right array using key value and searches the word
            //we are looking for comparing two words one by one
            for(nPtr=hashTable[key]; nPtr != NULL; nPtr=nPtr->next){
                if(strcmp(nPtr->word, nword) == 0){
                    return key;
                }        
            }
            break;
        
        case FILE2:
            //does the same than upper "case" but for second hashtable
            for(nPtr=hashTable2[key]; nPtr != NULL; nPtr=nPtr->next){
                if(strcmp(nPtr->word, nword) == 0){
                    return key;
                }
            }
            break;
    }
    return notInHashTable;
}

//reads both text files, parse them, add words(same word only once) to hashtable
// and before adding words of second file to hashtable, 
//function checks those 50 words that is not in first file 
void textFileHandling(char *fileName){
    
    //usefull variables
    int key;
    int i = 0;
    int wCounter = 0;
    int counter = 0;
    FILE *f;
    char c;
    const char parser[32] = " []#$%&()1234567890-,;_!?\".\n:/*";
    char *token;
    
    f=fopen(fileName,"rt");
    
    if(!f){
        printf("Cannot open file!");
        exit(1);
    }
    
    //go through the file and count words for the right sized array
    while((fgetc(f))!=EOF){
        counter++;
    }
    
    fclose(f);
    
    f=fopen(fileName,"rt");
    
    //initializing an array
    char *text = malloc(sizeof (int) * counter);
    for (int x=0; x<counter; x++)
        {
          text[x] = '\0';
        }
    
    counter = 0;
    //go through the file and change lowercase letter to uppercase
    while((c=fgetc(f))!=EOF){
        
        if(isalpha(c)){
            c = toupper(c);
        }
        
        text[i] = c;
        counter++;
        i++;     
    }
    
    token = strtok(text, parser);
    
    //text file parsing
    counter = 0;
    while( token != '\0') {
       
        key = searchWord(token);
        if(key == notInHashTable){
            
            switch(state){
                
                case FILE1:
                    addWord(token);
                    break;
                    
                case FILE2:
                    key = searchWord(token);
                    if(key == notInHashTable){
                        state = FILE1;
                        key = searchWord(token);
                        
                        if(key == notInHashTable && wCounter < amountOfPrintedWords){
                            printf("%s\n", token);
                            wCounter++;
                            
                            if(wCounter == 0){
                                printf("No printable words");
                            }
                        }
                    }
                    
                    state = FILE2;
                    addWord(token);
                    break;
            }
        }
        counter++;
        token = strtok(NULL, parser);
   }
   
    fclose(f);
}

int main() {
    
    clock_t t; 
    char firstFile[32];
    char secondFile[32];
    
    printf("First file: \n");
    scanf("%s", &firstFile);
    
    printf("Second file: \n");
    scanf("%s", &secondFile);
    
    t = clock();
    
    textFileHandling(secondFile);
    state = FILE2;
    
    textFileHandling(firstFile);
    
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
    printf("\nFUNCTION EXECUTION TIME: %f \n", time_taken); 
    
    return 0;
}






