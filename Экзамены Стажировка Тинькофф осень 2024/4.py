#4

from math import floor

def findPrimeNumbers(N):

    arr = [i for i in range(N + 1)]
    
    i = 2
    while i <= N:
        
        if arr[i] != -1:

            j = 2*i
            while j <= N:
                arr[j] = -1
                j += i
                
        i += 1
    
    # -1 - значит число составное
    res = [i for i in arr if i != -1][2:]
    return res
    
def findNumberOfDivisors(number):
    count = 0
    for i in range(1, floor(number**0.5) + 1):
        if number % i == 0:
            count += 1
            if i*i != number:
                count += 1
    return count
    
    
def main():
    l, r = map(int, input().split())
    
    primeNumbers = findPrimeNumbers(r)
    
    resCount = 0
    
    for num in range(l, r+1):
        if num not in primeNumbers and findNumberOfDivisors(num) in primeNumbers: resCount += 1
        
    print(resCount)

if __name__ == '__main__':
    main()