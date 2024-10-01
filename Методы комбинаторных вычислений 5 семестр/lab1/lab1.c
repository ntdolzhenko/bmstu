#include <stdio.h>

#define N 7  // Общее количество шагов (4 H и 3 V)
#define K 3  // Количество шагов по вертикали V

// печать текущей траектории
void print_path(int v_position[], int n){
    for (int i = 0; i < n; i++){
        if (v_position[i] == 1) printf("H");
        else printf("V");
    }
    printf("\n");
}

// генерация следующей траектории
int next_path(int v_position[], int n, int k){
    
    int cur_nul_pos = k - 1;
    
    // проверяем, сколько нулей в конце - наша цель - переместить в конец все нули
    while(cur_nul_pos >= 0 && v_position[cur_nul_pos] == n - k + cur_nul_pos) {  
        cur_nul_pos--;
    }
    // если i < 0 - в конце 000, все пути перебрали, 
    // если i >= 0, то i показывает с каким из 0 в v_position мы работаем
    if (cur_nul_pos < 0) return 0;  
    
    // транспозиция - двигаем самый правый 0, который еще не в конце (меняем 01 на 10)
    v_position[cur_nul_pos]++;  
    
    // если мы работаем не с самым правым 0, 
    // то делаем сдвиг - смещаем все нули правее текущего на текущую позицию+1
    for (int j = cur_nul_pos + 1; j < k; j++) v_position[j] = v_position[j - 1] + 1;
    return 1;
}

int main(){
    // позиции V в пути
    int v_position[K];
    
    // определяем первый путь - vvvhhhh - позиции V: 0,1,2
    for (int i = 0; i < K; i++) v_position[i] = i;

    // массив для хранения текущего пути (V = 0, H = 1, старт vvvhhhh)
    int path[N];

    do{
        // заполняем (обновляем) путь: сначала заполняем все 1 (H), потом ставим 0 (V) на позиции из v_position 
        for (int i = 0; i < N; i++) path[i] = 1;
        for (int i = 0; i < K; i++) path[v_position[i]] = 0;
        
        // печать текущего пути
        print_path(path, N);
        
    } while(next_path(v_position, N, K));

    return 0;
}