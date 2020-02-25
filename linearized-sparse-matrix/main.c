#include <stdio.h>
#include <math.h>

#define MAX_N 100

// ------------------ INPUT / OUTPUT UTILS

void printMenu() {
    printf("Options:\n"
           "\t1 - load matrix\n"
           "\t2 - get element\n"
           "\t3 - set element\n"
           "\t4 - print matrix\n"
           "\t5 - calculate saved memory space\n"
           "\t6 - delete matrix\n"
           "\t0 - exit\n\n"
           "Choice: ");
}

int getMaxDimension(){
    return (int)(sqrt(1+8*MAX_N)-1)/2;
}


// ------------------ MATRIX PRESENCE INDICATION

int isLoaded = 0;

int checkIsLoaded() {
    if (!isLoaded) printf("\nERROR: matrix not initialized.\n\n");
    return isLoaded;
}

int checkIsNotLoaded() {
    if (isLoaded) printf("\nERROR: matrix already initialized.\n\n");
    return !isLoaded;
}


// ------------------ MATRIX ACCESS

#define max(a,b) (a >= b ? a : b)
int n, mat[MAX_N];

int* element(int i, int j) {
    int a = max(i, j), b = i + j - a;
    return &mat[a * (a + 1) / 2 + b];
}


// ------------------ HIGH-LEVEL FUNCTIONS

void loadMatrix (){
    if (!checkIsNotLoaded()) return;

    printf("\nEnter matrix dimension (max %d): ", getMaxDimension());
    scanf("%d", &n);
    if (n < 0 || n > getMaxDimension()) {
        printf("\nERROR: invalid matrix dimension.\n\n");
        return;
    }

    int i = -1, j;
    while (++i < n) {
        j = -1;
        while (++j <= i) {
            if (i == j) printf("Enter value of element [%d,%d]: ",i,j);
            else printf("Enter value of elements [%d,%d] i [%d,%d]: ",i,j,j,i);
            scanf("%d", element(i, j));
        }
    }

    isLoaded = 1;
    printf("Matrix loaded.\n\n");

}

void getElementValue() {
    if (!checkIsLoaded()) return;

    int i, j;
    printf("\nEnter element indices: ");
    scanf("%d%d",&i,&j);

    if (i < 0 || i >= n || j < 0 || j >= n)
        printf("\nERROR: invalid indices.\n\n");
    else {
        int val = *element(i, j);
        printf("Value of element [%d, %d]: %d", i, j, val);
    }
}

void setElement() {
    if (!checkIsLoaded()) return;

    int i, j;
    printf("\nEnter element indices: ");
    scanf("%d%d", &i, &j);
    printf("\nEnter element value: ");
    scanf("%d", element(i, j));
}

void printMatrix () {
    if (!checkIsLoaded()) return;

    printf("\nMatrix output:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            printf("%d%c", *element(i, j), j == n-1 ? '\n' : ' ');

    putchar('\n');
}

void calculateSavings(){
    if (!checkIsLoaded()) return;

    int savedSpace = (pow(n,2)-n*(n+1)/2) * sizeof(int);
    printf("\nSaved %dB by storing linearized matrix.\n\n",savedSpace);

}

void deleteMatrix() {
    if (!checkIsLoaded()) return;

    isLoaded = 0;
    printf("\nMatrix is deleted.\n\n");
}

void (*pFunction[])() = {
        loadMatrix,
        getElementValue,
        setElement,
        printMatrix,
        calculateSavings,
        deleteMatrix};

int main() {
    int c;
    while (1) {
        printMenu();
        scanf("%d",&c);

        if (c == 0) return 0;
        else if (c >= 1 && c <= 6) pFunction[c - 1]();
        else printf("\nERROR: invalid option chosen.\n\n");
    }
}