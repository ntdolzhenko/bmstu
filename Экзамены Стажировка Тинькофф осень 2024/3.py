#3

from collections import defaultdict

def main():
    stroka = input()
    requirements = input()
    maxlen = int(input())

    symbols = {}
    for ch in requirements: symbols[ch] = 1

    strings = []
    last = ""

    for symbol in stroka:
        if symbol not in symbols:
            if len(last) != 0: strings.append(last)
            last = ""
        else:
            last += symbol

    if len(last) != 0:
        strings.append(last)


    for i in range(len(strings)-1, -1, -1): #in strings
        
        stroka = strings[i]
        length = len(stroka)
        
        symbolsInStroka = defaultdict(int)
        
        for j in range(length - 1, -1, -1): # in stroka
            
            symbolsInStroka[ stroka[j] ] += 1
            
            if j + maxlen <= length - 1:
                
                symbolsInStroka[ stroka[j + maxlen] ] -= 1
                
                if symbolsInStroka[ stroka[j + maxlen] ] == 0:
                    
                    del symbolsInStroka[ stroka[j + maxlen] ]
                    
            if len(symbolsInStroka) == len(symbols):
                
                print(stroka[j:min(j + maxlen, length)])
                
                return

    print(-1)

if __name__ == "__main__":
    main()