#include <stdio.h>
#include <stdlib.h>
#define N 5
#define M 120


// подсчет кол-ва инверсий
int countInversions(int *perm){
    int count = 0;
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (perm[i] > perm[j]) {
                count++;
            }
        }
    }
    return count;
}

int getNextPermutation(int *perm, int **listPermsWithInversions, int *index){
    
    // ищем max k: perm[k] < perm[k + 1]
    int k = -1;
    for (int i = N - 2; i >= 0; i--) {
        if (perm[i] < perm[i + 1]) {
            k = i;
            break;
        }
    }
    
    if (k == -1) return 0; // перебрали все перестановки, конец
    
    // ищем max l: perm[k] < perm[l]
    int l = -1;
    for (int i = N - 1; i > k; i--) {
        if (perm[k] < perm[i]) {
            l = i;
            break;
        }
    }
    
    // меняем местами perm[k] и perm[l]
    int temp = perm[k];
    perm[k] = perm[l];
    perm[l] = temp;
    
    // разворачиваем последовательность от k + 1 до конца
    for (int i = k + 1, j = N - 1; i < j; i++, j--) {
        temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }
    
    // записываем перестановку и кол-во ее инверсий для будущей сортировки
    for(int i = 0; i < N; i++) listPermsWithInversions[*index][i] = perm[i];
    listPermsWithInversions[*index][N] = countInversions(perm);
    (*index) ++;
    
    return 1;
    
}

// печать текущей перестановки
void printPermutation(int *perm) {
    for (int i = 0; i < N; i++)  printf("%d ", perm[i]);
    printf("\n");
    // printf("%d\n", countInversions(perm));
}

int comparator(const void *x1, const void *x2){

    const int *perm1 = *(const int**)x1; // указатель на строку с перестановкой 1
    const int *perm2 = *(const int**)x2; // Указатель на строку с перестановкой 2

    return perm1[N] - perm2[N];
}

int main()
{
    int perm[N];
    
    for (int i = 0; i < N; i++) perm[i] = i + 1;
    
    
    int **listPermsWithInversions = (int**)malloc(M * sizeof(int*));
    for (int i = 0; i < M; i++) listPermsWithInversions[i] = (int*)malloc((N+1) * sizeof(int));

    int index = 0;
    
    for(int i = 0; i < N; i++) listPermsWithInversions[index][i] = perm[i];
    listPermsWithInversions[index][N] = countInversions(perm);
    index++;
    
    // printPermutation(perm);
    
    while(getNextPermutation(perm, listPermsWithInversions, &index));
    
    // printf("\n\n\n");
    // for(int i = 0; i<M; i++){
    //     for(int j = 0; j < N+1; j++)  printf("%d ", listPermsWithInversions[i][j]);
    //     printf("\n");
    // }

    qsort(listPermsWithInversions, M, sizeof(int*), comparator);
    
    // printf("\n\n\n");
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++)printf("%d ", listPermsWithInversions[i][j]);
        printf("\t%d", listPermsWithInversions[i][N]);
        printf("\n");
    }

    return 0;
}
