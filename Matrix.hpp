#ifndef Matrix_hpp
#define Matrix_hpp

#include <iostream>
#include <string.h>

class CopySparse;
class RowSparse;
void wide_array(size_t*& arr, size_t index, size_t value, size_t size);
void wide_array(double*& arr, size_t index, double value, size_t size);


class SparseMatrix {

    size_t count_rows = 4;
    size_t count_cols = 0;
    double epsilon = 0.001;
    size_t* rows;
    int size = 0;
    size_t* cols;
    double* data;
    double zero = 0;
    
    
public:
    class Row
    {
        friend class SparseMatrix;
        SparseMatrix* parent = NULL;
        const SparseMatrix* parent1 = NULL;
        int index;
    public:
        double& operator[](int c);
        const double& operator[](int c) const;
        Row(SparseMatrix* m, int idx) : parent(m), index(idx){}
        Row(const SparseMatrix* m, int idx) : parent1(m), index(idx){}
    };
    SparseMatrix(size_t tmp_row = 0, size_t tmp_column = 0);
    SparseMatrix(const SparseMatrix& matr);
    ~SparseMatrix();
    
    
    double get(size_t row, size_t col) const;
    void set(size_t row, size_t col, double value);
    void set_epsilon(double new_eps);
    size_t num_rows() const;
    size_t num_columns() const;
    friend bool operator== (const SparseMatrix& matr_1, const SparseMatrix& matr_2);
    friend bool operator!= (const SparseMatrix& matr_1, const SparseMatrix& matr_2);
    friend std::ostream& operator<<(std::ostream&, const SparseMatrix& matr);
    SparseMatrix operator+ (const SparseMatrix& matr);
    SparseMatrix operator* (const SparseMatrix& matr);
    CopySparse operator+ (int);
    Row operator[](int r);
    const Row operator[](int r) const;
    
    RowSparse operator*();
    friend class CopySparse;
    friend class RowSparse;
};


class CopySparse
{
    int need_row;
    //int need_col;
    size_t*& rows;
    size_t*& cols;
    double*& data;
    int& size;
    size_t count_rows = 4;
    size_t count_cols = 0;
    
public:
    
    CopySparse(SparseMatrix& matr, int row) : need_row(row), rows(matr.rows), cols(matr.cols), data(matr.data), size(matr.size), count_rows(matr.count_rows), count_cols(matr.count_cols)
    {}
    
    RowSparse operator*();
    friend class RowSparse;
};

class RowSparse
{
    size_t count_rows = 4;
    size_t count_cols = 0;
    int need_row = 0;
    int need_col = 0;
    size_t*& rows;
    size_t*& cols;
    double*& data;
    int& size;
public:
    
    RowSparse(SparseMatrix& matr, int i) : count_rows(matr.count_rows), count_cols(matr.count_cols), need_row(i), rows(matr.rows), cols(matr.cols), data(matr.data), size(matr.size)
    {}
    
    RowSparse(CopySparse& matr, int i): count_rows(matr.count_rows), count_cols(matr.count_cols), need_row(i),rows(matr.rows), cols(matr.cols), data(matr.data), size(matr.size){}
    
    RowSparse(RowSparse& matr, int i): count_rows(matr.count_rows), count_cols(matr.count_cols), need_row(matr.need_row), need_col(i), rows(matr.rows), cols(matr.cols), data(matr.data), size(matr.size){}
    
    RowSparse operator+ (int);
    double& operator*();
    
};


#endif /* Matrix_hpp */
