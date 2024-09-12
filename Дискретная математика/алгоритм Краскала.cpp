#include <iostream>
#include <vector>
#include <algorithm> 

#include "edge.h"

using namespace std;

#define N 5

int main(){
    
    int graph[N][N] = {
        {0, 9, 75, 0, 0},
        {9, 0, 95, 19, 42},
        {75, 95, 0, 51, 66},
        {0, 19, 51, 0, 31},
        {0, 42, 66, 31, 0}
    };
    
    vector<Edge> listOfEdges;
    
    // строим список ребер
    for(int i = 0; i < N; i++){
        for(int j = i+1; j < N; j++){
            if(graph[i][j] != 0){
                listOfEdges.push_back(Edge(i,j,graph[i][j]));
            }
        }
    }
    
    //отсортировали ребра по весам
    sort(begin(listOfEdges), end(listOfEdges), comparator);
    
    int selected[N];
    for(int i = 0; i < N; i++) selected[i] = 0;
    
    
    vector<Edge> ostav;
    ostav.push_back(listOfEdges[0]);    //добавили минимальное ребро в остав
    
    selected[listOfEdges[0].vertex1] = 1;  //вершины первого добавленного ребра уже выбраны
    selected[listOfEdges[0].vertex2] = 1;  
    
    Edge tempEdge;
    
    for(int i = 1; i < listOfEdges.size(); i++){
        tempEdge = listOfEdges[i];
        
        // если еще не выбраны вершины
        if(selected[tempEdge.vertex1] == 0 && selected[tempEdge.vertex2] == 0
                || selected[tempEdge.vertex1] == 0 && selected[tempEdge.vertex2] != 0
                || selected[tempEdge.vertex1] != 0 && selected[tempEdge.vertex2] == 0){
                    
                    ostav.push_back(listOfEdges[i]);
                    selected[listOfEdges[i].vertex1] = 1; 
                    selected[listOfEdges[i].vertex2] = 1;  
                    
        }
    }
    
    cout << "Ребра остава:" << endl;
    for(int i = 0; i < ostav.size(); i++) cout << ostav[i].vertex1 << " - " << ostav[i].vertex2 << " : " << ostav[i].weight << endl;

    return 0;
}
