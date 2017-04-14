//Brooke Ly 33256705 
//Sunaina Kumar 27947698

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int searchArray[10];
int intToFind,returned, statval, exstat, child_status;
int counter = 0, status = 0,length = 0;;
pid_t right,left,wpid,waiter;
char user_input[100], direct[500];
char * filename;

int check_ifempty (char * name) {
    FILE * file = fopen(name, "r");
    int count = 0;
    char linez[500];
    char * itemz;
    int cond = 1;
    while (cond == 1){
        if ( fgets(linez, 500, file)!=NULL){
            itemz = strtok(linez, " \n\r\t");
            if (itemz != NULL){
                count++;
            }
        }
        else{
            cond = 0;
        }
    }
    
    if (count < 1){
        printf("ERROR: The file seems only contains spaces!\n");
        printf("-1\n");
        return -1;
    }
    else{
        return 0;
    }
}

int open_file(char * name){
    char line [500];
    char *item;
    FILE * file = fopen(name, "r");
    if (file == NULL){
        printf("ERROR: Problem with opening the file! \n");
        printf("-1\n");
        return -1;
    }
    else {
        fgets(line, 500, file);
        item = strtok(line, " \n");
        while (item != NULL){
            int i;
            for (i = 0; i <strlen(item); i++){
                if (!isdigit(item[i])){
                    printf("ERROR: This file  does not contain only integers!\n");
                    printf ("-1\n");
                    return -1;
                }
            }
            searchArray[counter++] = atoi(item);
            item = strtok(NULL, " \n");
            length++;
        }
        
        if (length > 10){
            printf("ERROR: The length of the file is more than 10!\n");
            printf("-1\n");
            return -1;
        }
    }
    fclose(file);
    return 0;
}


void forkSearch(int a[], int index_number, int length)
{
    int leftindex = (double)(int)(length/2) -1;
    int rightindex = (double)(int)(length/2);
    int leftlen = (double)(int)(length/2.0);
    int rightlen = (double)(int)((length/2.0) + 0.5);
    left = fork();
    if (left < 0){
        perror("fork() error\n");
        exit(-1);
    }
    
    if (left == 0){
        if (leftlen > 1){
            forkSearch(a, index_number, leftlen);
        }
        else{
            if (a[leftindex] == intToFind){
                printf("%i\n", index_number);
                exit(0);
            }
            else{
                exit(1);
            }
        }
    }
    
    else{
        index_number = index_number + rightindex;
        right = fork();
        
        if (right == 0){
            if (rightlen > 1){
                forkSearch(&a[rightindex], index_number, rightlen);
            }
            else{
                if (a[rightindex] == intToFind ){
                    printf("%i\n", index_number);
                    exit(0);
                }
                else{
                    exit(1);
                }
            }
        }
    }
    int status;
    int status2;
    int ec;
    int sec;
    waitpid(right, &status2, 0);
    ec = WEXITSTATUS(status2);
    waitpid(left, &status, 0);


    sec = WEXITSTATUS(status);
    if (sec == 0 || ec ==0)
        exit(0);
    else
        exit(1);
}


int main(int args, char * arg []){
    int x = -1;
    int y = -1;
    if (args < 3){
        printf (" ERROR: Cannot proceed. Insufficient amount of arguments passed to main!\n");
        return -1;
    }
    
    if (args == 3){
        filename = arg[1];
        FILE * file1;
        file1 = fopen(filename, "r");
        if (file1 == NULL){
            printf("ERROR: Cannot open file because it does not exist!\n");
            printf("-1\n");
            return 1;
        }
        intToFind = atoi(arg[2]);
        y = check_ifempty (filename);
        x = open_file(filename);
    }
    
    if (x == 0 && y == 0){
        pid_t  main_parent= fork();
        if (main_parent ==0){
            forkSearch(searchArray, 0, length);
            exit(0);
        }
        int status;
        int ec;
        waitpid(main_parent, &status, 0);
        ec= WEXITSTATUS(status);
        if (ec ==1)
        {
            printf ("-1\n");
        }    
    }
}
