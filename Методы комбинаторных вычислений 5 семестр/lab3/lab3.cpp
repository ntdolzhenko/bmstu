#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

#define OSN 8 

using namespace std;

void printPermutation(const vector<int> &perm) {
    for(auto pos: perm)
        if (pos < 10) cout << char(pos + '0'); 
        else cout << char(pos + 'A' - 10); 
    cout << endl;
}

void getPermutation(vector<int> & perm, int m, int i, int j, int index) {
    int s; 

    // ограничение на значений младших разрядов
    if (index > m - i - 1) s = j;
    else s = 0;    

    for(int ii = s; ii < OSN; ii++){
        // проверка на попарное различие
        if (index > 0){
            auto pos = find(perm.begin(), perm.begin()+index, ii); 
            if ( pos != perm.begin()+index ) continue; 
        }

        perm[index] = ii; 

        if (index < perm.size()-1) getPermutation(perm, m, i, j, index+1); 
        else printPermutation(perm);
    }
}

int main(int argc, char *argv[]) {

    int m, i, j;
    if (argc < 4){
        m = 4;
        i = 3;
        j = 5;
    }
    else{
        for(int x = 1; x < argc; x++){
            for(int y = 0; argv[x][y]; y++){
                if ( !(argv[x][y] >= '0' && argv[x][y] <= '9') ){
                    cout << "Error. Input must integer" << endl;
                    return 0;
                }
            }
        }

        m = atoi(argv[1]);
        if( !(m > 0 && m <= OSN) ){
            cout << "Error, must be: (1st number) m > 0 and m <= " << OSN << endl;
            return 0;
        }
            
        i = atoi(argv[2]);
        if(!(i > 0 && i <= OSN)) {
            cout << "Error, must be: (2nd number) i > 0 and i <= " << OSN << endl;
            return 0;
        }
            
        j = atoi(argv[3]);
        if( !(j > 0) ){
            cout << "Error, must be: (3rd number) j > 0" << endl;
            return 0;
        }
                
        if(!(i <= m)){
            cout << "Error, must be: (1st number) i <= (2nd number) m" << endl;
            return 0;
        }

        if (!(OSN-j >= i)){
            cout << "Error, must be: " << OSN << " - (2nd number) j >= (1st number) i" << endl;
            return 0;
        }

        
    }
    vector<int> perm(m); 
    getPermutation(perm, m, i, j, 0); 
    return 0;
}