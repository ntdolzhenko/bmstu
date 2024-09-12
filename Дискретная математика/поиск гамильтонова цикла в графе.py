# from copy import deepcopy

graph = [
        [0,1,9,0,4,0,0,0,0,0],
        [1,0,4,1,0,0,0,0,0,0],
        [9,4,0,2,2,5,0,0,0,0],
        [0,1,2,0,1,0,5,0,0,0],
        [4,0,2,1,0,2,7,3,3,0],
        [0,0,5,0,2,0,0,0,1,0],
        [0,0,0,5,7,0,0,1,0,4],
        [0,0,0,0,3,0,1,0,1,4],
        [0,0,0,0,3,1,0,1,0,1],
        [0,0,0,0,0,0,4,4,1,0]
    ]
    
    
def greedyGamilAlgo(graph, start, summ = 0, visited = [], gamCycle = []):
    
    if len(visited) == len(graph) or len(visited) > 1 and gamCycle[0] == gamCycle[-1]:
        gamCycle.append(start)
        print(summ)
        return gamCycle
    
    visited.append(start)
    gamCycle.append(start)
    
    minn = 10000000
    minIndex = 0

    for i in range(len(graph[start])):
        
        if graph[start][i] != 0 and graph[start][i] < minn and i not in visited:
            minn = graph[start][i]
            minIndex = i

    result = greedyGamilAlgo(graph, minIndex, summ + graph[start][minIndex], visited, gamCycle)
    return result
    
    
c =  greedyGamilAlgo(graph, 0)
print(c)