def deepFirstSearch(graph, start, visited=None):
    
    if visited is None:
        visited = set()
    
    visited.add(start)

    print(start)
    
    for node in graph[start]:
        if node not in  visited:
            deepFirstSearch(graph, node, visited)
    return visited

import collections

def breadthFirstSearch(graph, start): 
    
    visited = set()
    queue = list([start])
    
    visited.add(start)
    print(start)
    
    while len(visited) != len(graph): 
        node = queue.pop(0)
        
        for neighbourNode in graph[node]: 
            if neighbourNode not in visited: 
                visited.add(neighbourNode) 
                
                print(neighbourNode)
                
                queue.append(neighbourNode) 

graph = [{1,2,3},
    {0,2},
    {0,1,4},
    {0},
    {2}]

deepFirstSearch(graph, 0)

print('\n')
breadthFirstSearch(graph, 0)
