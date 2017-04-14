#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 256
#define HEADERSIZE 2

char *heap;
char delimiters[6] = " \n";
int alreadyAlloc = 0;
int blockCount; 
char * blockPtr[MAXSIZE];
int availableBuffer = 128;

void allocate(int numOfBytes){
    char * prevBlockAddress;
    char * nextBlockPtr;
    int prevBlockSize;
    char header1, header2;
    if (availableBuffer >= (numOfBytes+HEADERSIZE) ) {
        if (blockCount == 0){
            blockCount++;
            printf("%d\n",blockCount);
            heap[0] = blockCount;
            blockPtr[blockCount-1] = &heap[0];
            *(heap+1) = (numOfBytes << 1) | 1;
            availableBuffer -= (numOfBytes + HEADERSIZE);
        }
        else {
            prevBlockAddress = blockPtr[blockCount-1];
            prevBlockSize = (prevBlockAddress[1] >> 1) + HEADERSIZE;
            nextBlockPtr = prevBlockAddress + prevBlockSize;
            blockCount++;
            blockPtr[blockCount-1] = nextBlockPtr;
            header1 = blockCount;
            header2 = ((numOfBytes << 1) | 1);
            nextBlockPtr[0] = header1;
            nextBlockPtr[1] = header2;
            printf("%d\n", blockCount);
            availableBuffer -= (numOfBytes + HEADERSIZE);
        }
    }
    else {
        printf("Remaining memory is not enough to fit request of %d\n", numOfBytes);
    }
}

void freeBlock(int blockNum){
    int blockPtrID = blockNum-1;
    if (blockNum <= 0 || blockNum > blockCount){
        printf("Invalid Block ID of %d\n", blockNum);
    }
    else {
        blockPtr[blockPtrID][1] = (blockPtr[blockPtrID][1] >> 1) << 1;
    }
}

void blocklist(char * h){
    printf("Size\tAllocated\tStart\t\tEnd\n");
    int lsb, i, blockSize, endAddress, startAddress;
    for (i = 0; i < blockCount; i++){
        startAddress = blockPtr[i];
        blockSize = (blockPtr[i][1] >> 1) + HEADERSIZE;
        lsb = (blockPtr[i][1] & 1);
        endAddress = startAddress + blockSize - 1;
        printf("%d\t\t%s\t\t\t0x%x\t 0x%x\n", blockSize, ((lsb) ? "yes" : "no"),startAddress, endAddress);
    }
}

void writeHeap(int blocknum, char letter, int copies){
    int blockPtrID = blocknum -1;
    int startAddress, endAddress, blockSize, payloadSize, i;
    int x = 2;
    blockSize = (blockPtr[blockPtrID][1] >> 1) + HEADERSIZE;
    payloadSize = (blockPtr[blockPtrID][1] >> 1);
    startAddress = blockPtr[blockPtrID];
    endAddress = startAddress +blockSize -1;
    if (copies > payloadSize){
        printf("Write too big\n");
    }
    else {
        for (i = 0; i < copies; i++){
            blockPtr[blockPtrID][x] = letter;
            x++;
        }
    }
}

void printHeap (int blocknum, int numBytes){
    if (blocknum <= 0 || blocknum > blockCount){
        printf("Invalid Block ID of %d\n", blocknum);
    }
    else {
        int blockPtrID = blocknum -1;
        int i;
        for (i = 0; i < numBytes; i++){
            printf("%c", blockPtr[blockPtrID][i+HEADERSIZE]);
        }
        printf("\n");
    }
}

void printHeader (int blockNum) {
    if (blockNum <= 0 || blockNum > blockCount){
        printf("Invalid Block ID of %d\n", blockNum);
    }
    else {
        unsigned int headerInfo;
        headerInfo = blockPtr[blockNum-1][0] << 8;
        headerInfo = headerInfo | blockPtr[blockNum-1][1];
        printf("0x%04x\n", headerInfo);
    }
}

int main(int argc, const char * argv[]) {
    heap = (char *) malloc (128 * sizeof(char));
    memset(heap, 0, 128);
    int i = 1;
    blockCount = 0;
    while (i) {
        char *cmd;
        char * token;
        char command[100];
        printf("> ");
        fgets(command, 100, stdin);
        cmd = strtok(command, delimiters);
        if (strcmp(cmd, "allocate") == 0){
            token = strtok(NULL, delimiters);
            int numOfBytes = atoi(token);
            allocate(numOfBytes);
        }
        else if (strcmp(cmd, "free") == 0){
            token = strtok(NULL, delimiters);
            int blockid = atoi(token);
            freeBlock(blockid);
        }
        else if (strcmp(cmd, "blocklist") == 0){
            blocklist(heap);
        }
        else if (strcmp(cmd, "writeheap") == 0){
            token = strtok(NULL, delimiters);
            int blockID = atoi(token);
            token = strtok(NULL, delimiters);
            char letter = *((char*)token);
            token = strtok(NULL, delimiters);
            int times = atoi(token);
            writeHeap(blockID, letter, times);
        }
        else if (strcmp(cmd, "printheap") == 0){
            token = strtok(NULL, delimiters);
            int blockID = atoi(token);
            token = strtok(NULL, delimiters);
            int numOfBytes = atoi(token);
            printHeap(blockID, numOfBytes);
        }
        else if (strcmp(cmd, "printheader") == 0){
            token = strtok(NULL, delimiters);
            int blockNumOfHeader = atoi(token);
            printHeader(blockNumOfHeader);
        }
        else if (strcmp(cmd, "quit") == 0){
            free(heap);
            exit(0);
        }
    }
    free(heap);
    return 0;
}
