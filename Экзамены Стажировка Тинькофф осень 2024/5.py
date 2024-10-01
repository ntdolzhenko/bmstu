
def timeDiffseconds(t1, t2): #[hh, mm, ss]
    T1 = t1[0] * 3600 + t1[1] * 60 + t1[2] # время в секундах
    T2 = t2[0] * 3600 + t2[1] * 60 + t2[2]
    if T2 >= T1:
        return (T2 - T1) // 60
    return (24*3600 + T2 - T1) // 60

def main():
    
    Tstart = list(map(int,input().split(':'))) #[hh, mm, ss]

    n = int(input())
    
    failedZaproses = {} # (name, serverName) : countFailedZaproses
    result = {} # name : [ countAccessed: int, fine: int]
    
    for _ in range(n):
        
        name, time, serverName, status = input().split()
        
        time = list(map(int,time.split(':'))) #[hh, mm, ss]
        
        #если встретили команду впервые
        if name not in result:
            result[name] = [0, 0]
            
        if status == "ACCESSED":
            result[name][0] += 1
            
            if (name, serverName) in failedZaproses:
                result[name][1] += failedZaproses[(name, serverName)] * 20
                
            result[name][1] += timeDiffseconds(Tstart, time)
                
        elif status == 'DENIED' or status == 'FORBIDEN':
            
            if (name, serverName) not in failedZaproses:
                failedZaproses[(name, serverName)] = 0
            
            failedZaproses[(name, serverName)] += 1
            
    #формируем финальную таблицу
            
    report = []
    
    for teamName in result:
        #чтобы sort не перепутал по кол-во взломанных серверов и штраф между собой умножаем кол-во серверов на -1
        # так sort в начало всегда будет ставить кол-во взломанных серверов
        report.append( [ (-result[teamName][0], result[teamName][1]), teamName ] )
        
    # print(report)
        
    report = sorted(report)
    
    # print(report)
    
    place = 1
    for i in range(len(report)):
        
        #если это не первая команда и либо не равно кол-во серверов либо разный штраф
        if i != 0 and (report[i-1][0][0] != report[i][0][0] or report[i-1][0][1] != report[i][0][1]):
            place = i+1
            
        print(place, report[i][1], -report[i][0][0], report[i][0][1])

if __name__ == "__main__":
    main()