//
//  main.c
//  Lab1
//
//  Created by Brooke Ly on 1/4/17.
//  Copyright © 2017 Brooke Ly. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_ARRAY_SIZE 100

struct Dungeon {
    int start;
    char description[100];
    int n;
    int s;
    int e;
    int w;
};

struct Dungeon d[MAX_ARRAY_SIZE];
char cmd[MAX_ARRAY_SIZE];
int currentRmNumber = -1;
char *userCMD;
char *fileName;
bool loaded = false;

int i, j;
int n;
char *token;
char buffer[MAX_ARRAY_SIZE];
char descrip[MAX_ARRAY_SIZE];

char delimiters[6] = " $";
char *del;
char *ptr;

void populateStruct(FILE * file){
    int roomNumber;
    char * item;
    char line[100];
    while (fgets (line, 100, file) != NULL) {
        strcpy(buffer, line);
        del =  &delimiters[0];
        token = strtok(buffer, del);
        roomNumber = atoi(token);
        d[roomNumber].start = roomNumber;
        if (currentRmNumber == -1){
            currentRmNumber = roomNumber;
        }
        n = strlen(token);
        token = &token[n+1];
        token = &token[1];
        n = strlen(token);
        j = 0;
        ptr = descrip;
        for ( i = 0; i < n; i++ )
        {
            if (token[i] != '$' )
            {
                ptr[i] = token[i];
            }
            else
            {
                ptr[i] = '\0';
                break;
            }
        }
        strcpy(d[roomNumber].description, descrip);
        token = &token[i+1];
        token = strtok(token, " ");
        d[roomNumber].n = atoi(token);
        token = strtok(NULL, " ");
        d[roomNumber].s = atoi(token);
        token = strtok(NULL, " ");
        d[roomNumber].e = atoi(token);
        token = strtok(NULL, " ");
        d[roomNumber].w = atoi(token);
    }
    loaded = true;
}

void printDesc(void){
    printf("%s\n", d[currentRmNumber].description);
}

void north(){
    if (d[currentRmNumber].n == -1){
        printf("You can't go there\n");
    }
    else {
        currentRmNumber = d[currentRmNumber].n;
        printf("%s\n",d[currentRmNumber].description);
    }
}

void south(){
    if (d[currentRmNumber].s == -1){
        printf("You can't go there\n");
    }
    else{
        currentRmNumber = d[currentRmNumber].s;
        printf("%s\n",d[currentRmNumber].description);
    }
}

void east(){
    if (d[currentRmNumber].e == -1) {
        printf("You can't go there\n");
    }
    else {
        currentRmNumber = d[currentRmNumber].e;
        printf("%s\n",d[currentRmNumber].description);
    }
}

void west(){
    if (d[currentRmNumber].w == -1) {
        printf("You can't go there\n");
    }
    else {
        currentRmNumber = d[currentRmNumber].w;
        printf("%s\n",d[currentRmNumber].description);
    }
}

void reinitialize(){
    currentRmNumber = -1;
    loaded = false;
    
}

void quit(){
    exit(0);
}

int main(int argc, const char * argv[]) {
    
    while (1){
        printf("$ ");
        scanf ("%[^\n]%*c", cmd);
        userCMD = strtok(cmd, " ");
        if (strcmp(userCMD, "loaddungeon") == 0){
            fileName = strtok(NULL, " ");
            if (loaded){
                reinitialize();
            }
            FILE * file;
            file = fopen(fileName, "r");
            if (file == NULL){
                printf("Error opening file\n");
            }
            else {
                populateStruct(file);
                printDesc();
            }
        }
        
        else if (strcmp(cmd, "quit") == 0){
            quit();
        }
        
        else {
            if (loaded){
                if (strcmp(cmd, "north") == 0){
                    north();
                }
                
                else if (strcmp(cmd, "south") == 0){
                    south();
                }
                else if (strcmp(cmd, "east") == 0){
                    east();
                }
                else if (strcmp(cmd, "west") == 0){
                    west();
                }
            }
            else {
                printf("Dungeon file has not be loaded\n");
            }
        }
    }
    return 0;
}

// loaddungeon /Users/brookely/Desktop/dfile2.txt
