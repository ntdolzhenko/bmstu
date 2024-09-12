import networkx as nx
import matplotlib.pyplot as plt

n = 10
a1, b2, c3, d4, e5, f6, g7, h8, i9, j0 = range(n)
N = [
    {b2, c3, e5}, #a
    {a1, c3, d4}, #b
    {a1, b2, d4, e5, f6}, #c
    {b2, c3, e5, g7}, #d
    {a1, c3, d4, f6, g7, h8, i9}, #e
    {c3, e5, i9}, #f
    {d4, e5, h8, j0}, #g
    {e5, g7, i9, j0}, #h
    {e5, f6, h8, j0}, #i
    {g7, h8, i9}, #j
]

'''
#1-2 алгоритмы
MaxList = [0]
for i in range(1, n):
    for j in range(1, i+1):
        if j >= len(MaxList):
            MaxList.append(i)
        if len(N[i]) > len(N[MaxList[j-1]]):
            MaxList.insert(j-1, i)
            break
#print(MaxList)

M = []
M.append([MaxList[0]+1])
for i in range(1, n):
    flag = 0
    for j in range(len(M)):
        count = 0
        for q in range(len(M[j])):
            if MaxList[i] not in N[M[j][q]-1]:
                count += 1
        if count == len(M[j]):
            M[j].append(MaxList[i]+1)
            flag = 1
        if flag == 1:
            break
    if flag == 0:
        M.append([MaxList[i]+1])
for i in range(len(M)):
    print(i+1, 'группа:', M[i])
'''

a1, b2, c3, d4, e5, f6, g7, h8, i9, j0 = range(n)
N = [
    {b2:1, c3:4, e5:4}, #a
    {c3:4}, #b
    {e5:2, f6:3}, #c
    {b2:1, c3:2, e5:7}, #d
    {f6:2, g7:2, i9:3}, #e
    {i9:1}, #f
    {d4:6, h8:4, j0:4}, #g
    {e5:3, i9:1, j0:4}, #h
    {j0:2}, #i
    {}, #j
]

'''#3 алгоритм
Ai, Aj, Aq, count = [], [], [0], 0
for i in range(n):
    for j in range(n):
        if j in N[i]:
            Ai.append(j)
            Aj.append(i)
            count += 1
    Aq.append(count)
print(Ai, Aj, Aq, sep = '\n')
'''
#4 алгоритм
#?

#5 алгоритм
'''G_5al = nx.Graph()
used = [0]
not_used = []
for i in range(1, n):
    not_used.append(i)
count = 0
c = len(used)

for i in range(1, n):
    for j in used:
        temp = []
        for q in not_used:
            if q in N[j]:
                temp.append(q)
                G_5al.add_edge(q + 1, j + 1)
        for w in range(len(temp)):
            used.append(temp[w])
            not_used.remove(temp[w])
        if c != len(used):
            count+=1
        c = len(used)

nx.draw(G_5al, with_labels = True)
plt.savefig("graph_5al.png")
print('Ширина графа:', count)'''

#алгоритм 6
class Graph:
    def __init__(self, graph):
        self.graph = graph
        self.ROW = len(graph)

    def BFS(self, s, t, parent):
        visited = [False] * (self.ROW)
        queue = []
        queue.append(s)
        visited[s] = True
        while queue:
            u = queue.pop(0)
            for ind, val in enumerate(self.graph[u]):
                if visited[ind] == False and val > 0:
                    queue.append(ind)
                    visited[ind] = True
                    parent[ind] = u
        return True if visited[t] else False

    def FordFulkerson(self, source, sink):
        parent = [-1] * (self.ROW)
        max_flow = 0
        while self.BFS(source, sink, parent):
            path_flow = float("Inf")
            s = sink
            while s != source:
                path_flow = min(path_flow, self.graph[parent[s]][s])
                s = parent[s]
            max_flow += path_flow
            v = sink
            while v != source:
                u = parent[v]
                self.graph[u][v] -= path_flow
                self.graph[v][u] += path_flow
                v = parent[v]
        return max_flow

graph = [[0, 1, 9, 0, 4, 0, 0, 0, 0, 0],
         [0, 0, 4, 0, 0, 0, 0, 0, 0, 0],
         [0, 0, 0, 0, 2, 3, 0, 0, 0, 0],
         [0, 1, 2, 0, 7, 0, 0, 0, 0, 0],
         [0, 0, 0, 0, 0, 2, 7, 0, 0, 0],
         [0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
         [0, 0, 0, 2, 0, 0, 0, 4, 0, 4],
         [0, 0, 0, 0, 1, 0, 0, 0, 1, 4],
         [0, 0, 0, 0, 0, 0, 0, 0, 0, 2],
         [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]

g = Graph(graph)

source = 0
sink = 5

print("Максимальный поток: %d " % g.FordFulkerson(source, sink))