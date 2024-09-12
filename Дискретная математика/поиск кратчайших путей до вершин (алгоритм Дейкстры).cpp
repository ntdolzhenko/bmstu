#include <iostream>
#include <vector>
#define MAXINT 10000000
#define N 6

using namespace std;

int main(){
    
    // int N; //количество вершин
    // cout << "Введите количество вершин графа: " << endl;
    // cin >> N;
    
    // int data[N][N];
    int graph[N][N] = {
        {0,7,9,0,0,14},
        {7,0,10,15,0,0},
        {9,10,0,11,0,2},
        {0,15,11,0,6,0},
        {0,0,0,6,0,9},
        {14,0,2,0,9,0}
    };
    
    // // Инициализация матрицы смежности
    // for (int i = 0; i < N; i++){
    //     graph[i][i] = 0;
    //     for (int j = i + 1; j < N; j++){
    //       printf("Введите расстояние %d - %d: ", i + 1, j + 1);
    //       int x;
    //       cin >> x;
    //       graph[i][j] = x;
    //       graph[j][i] = x;
    //     }
    // }
      
    // Вывод матрицы смежности
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) printf("%5d ", graph[i][j]);
        printf("\n");
    }    
    
    int res[N];         // минимальные расстояния от искомой вершины
    int checked[N];     // массив с отмеченными посещенными вершинами
    vector<int> paths[N]; 
    
    for(int i = 0; i < N; i++) {
        res[i] = MAXINT;
        checked[i] = 0;
    }
    
    int start;
    cout << "Введите номер начальной вершины: " << endl;
    cin >> start;
    
    res[start - 1] = 0;
    
    int temp;
    int minPath = MAXINT, indexMinPath = MAXINT;
    
    while(1){
        
        minPath = MAXINT;           //  минимальный путь
        indexMinPath = MAXINT;      // идекс вершины, на которой закончился этот минимальный путь 
        
        //из непосещенных вершин ищем ту, путь к которой минимален
        for(int i  = 0; i < N; i++){
            if(checked[i] == 0 && res[i] < minPath){
                minPath = res[i];
                indexMinPath = i;
            }
        }
        
        if(minPath == MAXINT) break; // если не нашли непосещенных вершин, то выходим из цикла
        
        for(int i = 0; i < N; i++){
            //если есть ребро между вершинами
            if(graph[indexMinPath][i] > 0) res[i] = min(res[i], minPath + graph[indexMinPath][i]);
        }
        
        //прошли вершину, отмечаем ее как пройденную
        checked[indexMinPath] = 1;
    }

    // Вывод кратчайших расстояний до вершин
    printf("\nКратчайшие расстояния до вершин: \n");
    for (int i = 0; i<N; i++) printf("От %d до %d: %d\n", start, i+1, res[i]);
    
    
    // Восстановление путей
    for(int end = 0; end < N; end++){

        int path[N];                                    // путь
        int tempEnd = end;                              // индекс конечной вершины
        
        path[0] = tempEnd + 1;                          // начальный элемент - конечная вершина
        int weight = res[tempEnd];                      // вес конечной вершины
        int k = 1;                                      // индекс предыдущей вершины
    
        while (tempEnd != start-1){                     // пока не дошли до начальной вершины
            for (int i = 0; i < N; i++){                // просматриваем все вершины
            
                if (graph[i][tempEnd] != 0){             // если связь есть
                
                    temp = weight - graph[i][tempEnd];   // определяем вес пути из предыдущей вершины
                    if (temp == res[i]){                // если вес совпал с рассчитанным
                                                        // значит из этой вершины и был переход
                        weight = temp;                  // сохраняем новый вес
                        tempEnd = i;                    // сохраняем предыдущую вершину
                        path[k] = i + 1;                // и записываем ее в массив
                        k++;
                    }
                }
            }
        }
        //начальная вершина оказалась в конце массива из k элементов
        for (int i = k - 1; i >= 0; i--) paths[end].push_back(path[i]);
    }
    
    // Вывод кратчайших путей до вершин
    printf("\nКратчайшие пути до вершин: \n");
    for (int i = 0; i<N; i++){
        printf("От %d до %d: ", start, i+1);
        for(int j = 0; j < paths[i].size(); j++){
            cout << paths[i][j] << " ";
        }
        cout << endl;
    } 
    

   
    

    return 0;
}

