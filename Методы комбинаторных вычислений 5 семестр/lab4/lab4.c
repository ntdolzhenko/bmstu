#include <stdio.h>
#include <stdlib.h>

// Поиск максимального слагаемого
int maxsum(int* p, int n) {
	int max = 0;
	for (int i=0; i<n; i++)
		if(p[i] != 0 && max < i) max = i;
	return max+1;
}

// Печать разбиения в формате суммы
void print(int* p, int n) {
	for (int i=n-1; i>-1; i--)
		if (p[i] != 0)
			for (int j=0; j<p[i]; j++)
				printf("%d+", i+1);
	printf("\b \n");
}

// Алгоритм Эрлиха
int* iter(int* p, int n) {
	int m, left, j;
	for (m=0; m<n-1; m++)
		if (p[m] != 0) {
			left = 0;
			if (p[m] == 1) {
				for (j=m+1; !p[j] && j<n; j++);
				left += (m+1)*p[m] + (j+1)*p[j];
				p[m] = 0;
				p[j] = 0;
				m = j;
			} else {
				p[m] -= 2;
				left += (m+1)*2;
			}
			p[m+1] += 1;
			left -= m+2;
			p[0] += left;
			return p;
		}
	return 0;
}


int main(int argc, char *argv[]) {
    int n = 7, l = 8, u = 9, temp;
    int* p = (int*) malloc(n * sizeof(int));

    if (argc > 1) {
        temp = atoi(argv[1]);
        if (temp > 0) {
            n = temp;
        } else {
            printf("|n| должно быть целым числом больше нуля\n");
            free(p);
            return 1;
        }
    }

    if (argc > 2) {
        temp = atoi(argv[2]);
        if (abs(temp) <= n) {
            l = temp;
        } else {
            printf("|l| должно быть целым числом и l <= n\n");
            free(p);
            return 1;
        }
    }
    if (argc > 3) {
        temp = atoi(argv[3]);
        if (abs(temp) <= n) {
            u = temp;
        } else {
            printf("|u| должно быть целым числом и l <= u <= n\n");
            free(p);
            return 1;
        }
    }

    for (int i=0; i<n; i++)
        p[i] = 0;
    p[0] = n;

    do if (maxsum(p, n) < l || maxsum(p, n) > u) print(p, n);
    while ((iter(p, n)));

    free(p);
    return 0;
}