
#1
def main():
    inputString = input().split(',')

    for stroka in inputString:
        
        if '-' in stroka:
            arr = list(map(int,stroka.split('-')))
            for i in range(arr[0], arr[1]+1): print(i, end=' ')
            
        else: print(int(stroka), end=' ')

if __name__ == '__main__':
    main()