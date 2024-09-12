from copy import deepcopy

def pruferCode(graph, code = [], deleted = []):

    if len(code) == len(graph) - 2:
        return code
    
    for i in range(len(graph)):
        if len(graph[i]) == 1 and i not in deleted:
            
            code.append(graph[i][0])
            # print(graph[i][0])
            
            graph[graph[i][0]].remove(i)
            
            deleted.append(i)
            break   
                    
    result = pruferCode(graph, code, deleted)
    
    return result

def pruferDecode(code, nodeList, edges = []):
    
    while len(nodeList) > 2:
        for i in nodeList:
            if i not in code:
                a = [i, code[0]]
                edges.append(a)
                code = code[1:]
                nodeList.remove(i)
                break 
    edges.append(nodeList)
    return edges
    
graph = [
    [1,3],
    [0],
    [4],
    [0,4],
    [2,3,5], 
    [4]];

c = pruferCode(deepcopy(graph))
c_ = pruferDecode([0, 3, 4, 4], list(range(len(graph))))
print(graph, c, c_, sep='\n')