#2

def main():
    n = int(input())
    data = list(map(int, input().split()))
    
    res = data
    
    if (data[0] == -1): data[0] = 1

    for i in range(1,n):
        
        if data[i] == -1:
            data[i] = data[i-1] + 1
        elif data[i] <= data[i-1] or data[i] > 10**9: 
            print('NO')
            return
    
    print("YES", data[0], end=' ', sep='\n')
    for i in range(1,n):
        print(data[i] - data[i-1], end=' ')
        

if __name__ == '__main__':
    main()