#include <stdio.h> 
#include <stdlib.h>

void main(){
    FILE* outputFile;
    FILE* outputFile2;
    int i, key, r;
    int arr[1000000];
    char c = 'I';
    char fd[2];
    char q = 'Q';
    fd[0] = 'F';
    fd[1] = 'D';
    
    outputFile = fopen("input-large-keys.txt", "w");
    outputFile2 = fopen("input-large.txt", "wa");
    
    if (outputFile == NULL) {
        printf("error\n");
    }
    
    for(i = 0; i < 1000000 ; i++){
        key = rand() % 1000000;
        fprintf(outputFile, "%d\n", key);
        fprintf(outputFile2, "%c %d %d\n", c, key, rand() % 1000000);
    }

    for(i = 0; i < 1000000 ; i++){
        r = rand() % 2; 
        fprintf(outputFile2, "%c %d\n", fd[r], rand() % 1000000);
    }
    
    fprintf(outputFile2, "%c", q);
    fclose(outputFile);
}
