#pragma once
#include <iostream>
#include <iterator>
#include <iomanip>

template <typename T>
class Container{
    private:
    
    T ** values_; //матрица типа который мы зададим
    unsigned nrows_, ncols_; // кол-во строк, столбцов

    public:
        // Конструктор
        Container(unsigned rows, unsigned cols): nrows_(rows), ncols_(cols){
            
            if (rows == 0 || cols == 0) throw std::invalid_argument("You can't create array with size of 0");
            
            values_ = new T*[nrows_];
            
            for(unsigned i = 0; i < nrows_; i++){
                values_[i] = new T[ncols_];
                //for(unsigned j = 0; j < ncols_; j++) _values[i][j] = 0;
            }
         }
         
        //Деструктор
        ~Container(){
            for(unsigned i = 0; i < nrows_; i++) delete [] values_[i];
            delete [] values_;
        }
        
        //Конструктор копирования
        Container(const Container<T> &matrix)
        {
            nrows_ = matrix.nrows_;
            ncols_ = matrix.ncols_;
            
            values_ = new T*[nrows_];
            for(unsigned i = 0; i < nrows_; i++){
                values_[i] = new T[ncols_];
                for(unsigned j = 0; j < ncols_; j++) values_[i][j] = matrix.values_[i][j];
            }
        }
        
        //перегрузка оператора присваивания
        Container<T>& operator=(const Container<T> &matrix){
            if(this == &matrix) return *this;
            
            // если матрицы не совпадают, то очищаем имеющуюся и выделяем место под матрицу с новыми размерами
            if(nrows_ != matrix.nrows_ || ncols_ != matrix.ncols_){ 
                
                for(unsigned i = 0; i < nrows_; i++) delete [] values_[i];
                delete [] values_;
                
                nrows_ = matrix.nrows_;
                ncols_ = matrix.ncols_;
                
                values_ = new T*[nrows_];
                for(unsigned i = 0; i < nrows_; i++) values_[i] = new T[ncols_];
            }
            
            //копируем в старую матрицу новые значения
            for(unsigned i = 0; i < nrows_; i++){
                for(unsigned j = 0; j < ncols_; j++) values_[i][j] = matrix.values_[i][j];
            }
            
            return *this;
        }
        
        //перегрузка оператора получения доступа 
        T* operator[](const int &i){
            
            if (i >= nrows_) throw std::out_of_range("Error: index bigger than array size");
            
            return values_[i];
        }
        
        //перегрузка оператора получения доступа 
        const T* operator[](const int &i)const {
            
            if (i >= nrows_) throw std::out_of_range("Error: index bigger than array size");
            
            return values_[i];
            
        }
        
        // перегрузка оператора побитового сдвига для вывода
        template <typename T1>
        friend std::ostream& operator<<(std::ostream &out, const Container<T1> &matrix){
            
                for (unsigned i = 0; i < matrix.nrows_; ++i){ 
                    out << "[ ";
                    for (unsigned j = 0; j < matrix.ncols_; ++j){
                        out << std::setw(3) << matrix.values_[i][j] << ' ';
                    }
                    out << ']' << '\n';
                }
                return out;
        }
            
        //объявляем итератор как часть классса контейнера
        class Iterator;
        Iterator begin();
        Iterator end();
};


template<typename T>
class Container<T>::Iterator{
    private:
        //указатель на начало контейнера
        T ** begin_;
        
        //Индекс текущей строки в матрице
        int curRow;
        //Индекс текущего столбца в матрице
        int curColumn;
        
        //Размеры матрицы: число строк и столбцов
        unsigned nRow, nCol;

        //Флаг для направления движения итератора (изначально влево)
        int direction;
        
        //Длина змейки
        int len;
        
    public:
    
        using iterator_category = std::random_access_iterator_tag;  // объявили тип итератора 
        using value_type = T;                                       // тип значения которое хранится и возвращается операторами * и ->
        using difference_type = std::ptrdiff_t;                     // тип который описывает растояние между итераторами
        using pointer = value_type*;                                // тип указателя на значение
        using reference = value_type&;                              // тип ссылки на значение
        
        //Конструктор 
        Iterator(T **begin, int x, int y, unsigned nColl, unsigned nRoww, int _len, int dir): \
                    begin_(begin), curColumn(x), curRow(y), nCol(nColl), nRow(nRoww), len(_len), direction(dir){}

        //перегрузка операторов сравнения

        bool operator==(const Iterator &it) const { return len == it.len;}
        bool operator!=(const Iterator &it) const { return len != it.len;}
        bool operator<(const Iterator &it) const { return len < it.len;}
        bool operator>(const Iterator &it) const { return len > it.len;}
        bool operator<=(const Iterator &it) const { return len <= it.len;}
        bool operator>=(const Iterator &it) const { return len >= it.len;}
        
        //перегрузка оператора вызова функции + разыменования
        T& operator *(){
            std:: cout << std:: endl << curRow << ' ' << curColumn << std:: endl;
            return *( *(begin_ + curRow) + curColumn);
        }
        
        //перегрузка оператора вызова функции + разыменования
        const T& operator *() const{
            std:: cout << std:: endl << curRow << ' ' << curColumn << std:: endl;
            return *( *(begin_ + curRow) + curColumn);
        }
        
        //перегрузка оператора префиксного инкремента ++it (перемещение по контейнеру)
        Iterator& operator++(){
            
            if(len == nRow * nCol) return *this; // если мы уже в конце, то не двигаемся
    
            len++;
            curColumn += direction;    // двигаемся на клетку вперёд
            
            if(curColumn < 0){      // проверка на границу слева
            
                curColumn++;        // вышли за границу слева и подвинули клетку обратно (вправо)
                curRow++;           // спустились вниз
                direction *= -1;       // поменяли направление движения (с левого на правое)
                return *this;
            }
            
            if(curColumn > nCol - 1){ // проверка на границу справа
            
                curColumn--;        // вышли за границу справа подвинули клетку в обратно (влево)
                curRow++;           // спустились вниз
                direction *= -1;       // поменяли направление движения (с правого на левое)
                return *this;
            }
            
            return *this;
        }
        
        //перегрузка оператора постфиксного инкремента it++ (перемещение по контейнеру)
        Iterator operator++(int){
            Iterator temp = *this; 
            
            if(len == nRow * nCol) return temp; // если мы уже в конце, то не двигаемся
            
            len++;
            curColumn += direction;            // двигаемся на клетку вперёд
            
            if(curColumn < 0){              // проверка на границу слева
            
                curColumn++;                // вышли за границу слева и подвинули клетку обратно (вправо)
                curRow++;                   // спустились вниз
                direction *= -1;               // поменяли направление (с левого на правое)
                return temp;
            }
            
            if(curColumn > nCol - 1){       // проверка на границу справа
            
                curColumn--;                // вышли за границу справа подвинули клетку в обратно (влево)
                curRow++;                   // спустились вниз
                direction *= -1;               // поменяли направление движения (с правого на левое)
                return temp;
            }
            return temp;
        }
        
        //перегрузка оператора префиксного декремента --it (перемещение по контейнеру)
        Iterator& operator--(){  
            if(len == 0) return *this;
            
            len--;
            
            curColumn += (-1) * direction;     // двигаемся в обратном направлении
            
            if(curColumn < 0){              // проверка на границу слева
                
                curColumn++;                // вышли за границу слева подвинули клетку обратно (вправо)
                curRow--;                   // поднялись вверх
                direction *= -1;               // поменяли направление движения (с левого на правое)
                return *this;
            }
            
            if(curColumn > nCol - 1)        // проверка на границу справа
            {
                curColumn--;                // вышли за границу справа подвинули клетку в обратно
                curRow--;                   // поднялись вверх
                direction *= -1;               // поменяли направление движения (с правого на левое)
                return *this;
            }
            return *this;
        }
        
        //перегрузка оператора постфиксного декремента it-- (перемещение по контейнеру)
        Iterator& operator--(int){
            Iterator& temp = *this;

            if(len == 0) return temp;
            
            len--;
            
            curColumn += (-1) * direction;     // двигаемся в обратном направлении
            
            if(curColumn < 0){              // проверка на границу слева
                
                curColumn++;                // вышли за границу слева подвинули клетку обратно (впрво)
                curRow--;                   // поднялись вверх
                direction *= -1;               // поменяли направление движения (с левого на правое)
                return temp;
            }
            
            if(curColumn > nCol - 1)        // проверка на границу справо
            {
                curColumn--;                // вышли за границу справа подвинули клетку в обратно (влево)
                curRow--;                   // поднялись вверх
                direction *= -1;               // поменяли направление движения (с правого на левое)
                return temp;
            }
            return temp;
        }    
    
   
    // перегрузка оператора сложения (it + 5)
    Iterator operator+(difference_type n) const{
        
        Iterator shiftedIt = *this; // копируем итератор

        for(difference_type i = 0; i < n; i++){

            if(shiftedIt.len == shiftedIt.nRow * shiftedIt.nCol) return shiftedIt;
            
            shiftedIt++;
        }
        return shiftedIt;
    }
    
    //перегрузка оператора += (it += 5)
    Iterator& operator+=(difference_type n){

        for(difference_type i = 0; i < n; i++){
            if(len == nRow * nCol) return *this;
            
            (*this)++;
        }
        return *this;
    }
    
    // перегрузка оператора вычитания (it - 5)
    Iterator operator-(difference_type n) const{
        Iterator shiftedIt = *this; // копируем итератор
        
        for(difference_type i = 0; i < n; i++){
            if(shiftedIt.len == 0) return shiftedIt;
            
            shiftedIt--;
        }
        return shiftedIt;
    }
    
    //перегрузка операции -= (it -= 5)
    Iterator& operator-=(difference_type n){
        for(difference_type i = 0; i < n; i++){
            if(len == 0) return *this;
            
            (*this)--;
        }
        return *this;
    }

    //  перегрузка оператора сложения ( 5 + it)
    friend typename Container<T>::Iterator& operator+(difference_type n, const typename  Container<T>::Iterator &it){
        
        typename Container<T>::Iterator shiftedId = it;
        
        for(difference_type i = 0; i < n; i++){
            if(shiftedId.len == shiftedId.nRow * shiftedId.nCol) return shiftedId;
            shiftedId++;
        }
        
        return shiftedId;
    }

    //перегрузка оператора - для вычитания итераторов
    difference_type operator-(const Iterator& it) const{
        //вычисляем длину между итераторами
        return len - it.len;
    }

    //перегрузка оператора получения доступа (для строки, на которой находится итератор)
    T* operator[](difference_type n){
        if( (unsigned) n < nRow) return *(begin_ + n);
    }
    const T* operator[](difference_type n) const {
        if( (unsigned) n < nrows_) return *(begin_ + n);
    }
};

template <typename T>
typename Container<T>::Iterator Container<T>::begin() {
    return Container<T>::Iterator(values_, ncols_ - 1, 0, ncols_, nrows_, 0, -1);
}

template <typename T>
typename Container<T>::Iterator Container<T>::end(){
    int col = nrows_ % 2 == 0 ? ncols_ - 1 : 0;
    int dir = nrows_ % 2 == 0 ? 1 : -1;
    
    return Container<T>::Iterator(values_, col, nrows_-1, ncols_, nrows_, nrows_ * ncols_, dir);
}