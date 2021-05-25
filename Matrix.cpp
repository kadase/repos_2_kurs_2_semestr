#include "Matrix.hpp"

SparseMatrix::SparseMatrix(size_t tmp_row, size_t tmp_column) : count_rows(tmp_row), count_cols(tmp_column)
{}

SparseMatrix::SparseMatrix(const SparseMatrix &matr) : count_rows(matr.count_rows), count_cols(matr.count_cols), size(matr.size)
{
    rows = new size_t[matr.size];
    cols = new size_t[matr.size];
    data = new double[matr.size];
    for (int i = 0; i < matr.size; i++)
    {
        rows[i] = matr.rows[i];
        cols[i] = matr.cols[i];
        data[i] = matr.data[i];
    }
}

SparseMatrix::~SparseMatrix()
{
    if (size > 0)
    {
        delete [] rows;
        delete [] cols;
        delete [] data;
    }
}

std::ostream& operator<< (std::ostream& out, const SparseMatrix& matr)
{
    int i = 0;
    while (i < matr.size)
    {
        size_t prev = matr.rows[i];
        while (i < matr.size && prev == matr.rows[i])
        {
            out << matr.data[i] << " ";
            i++;
        }
        out << '\n';
    }
    out << '\n';
    return out;
}

void SparseMatrix::set_epsilon(double new_eps)
{
    epsilon = new_eps;
}

SparseMatrix SparseMatrix::operator+ (const SparseMatrix& sparce_matr)   // check size of matrices
{
    SparseMatrix tmp(*this);
    int i = 0;
    double sum;
    if (sparce_matr.size == 0)
    {
        std::cout<<"Error with matr"<< std::endl;
        exit(1);
    }
    if (tmp.size != sparce_matr.size)
    {
        std::cout<<"Different size"<< std::endl;
        exit(1);
    }
    while (i < sparce_matr.size)
    {
        sum = sparce_matr.data[i] + get(sparce_matr.rows[i], sparce_matr.cols[i]);//rows[i], cols[i]
        tmp.set(sparce_matr.rows[i], sparce_matr.cols[i], sum);
        i++;
    }
    return tmp;
}

SparseMatrix SparseMatrix::operator* (const SparseMatrix& matr)
{
    if ((size == 0) || (matr.size == 0))
    {
        std::cout<<"Error with sizes of matr"<< std::endl;
        exit(1);
    }
    int i = 0;
    if (count_rows == 0)
    {
        std::cout<<"Error with sizes of matr"<< std::endl;
        exit(1);
    }
    if (matr.count_rows != count_cols)
    {
        std::cout<<"Error with sizes of matr"<< std::endl;
        exit(1);
    }
    SparseMatrix sorted_cols(matr);
    for (i = 0; i < sorted_cols.size - 1; i++)
    {
        for (int j = 0; j < sorted_cols.size - 1; j++)
        {
            if (sorted_cols.cols[j] > sorted_cols.cols[j + 1])
            {
                size_t k = sorted_cols.cols[j];
                sorted_cols.cols[j] = sorted_cols.cols[j + 1];
                sorted_cols.cols[j + 1] = k;
                
                size_t t = sorted_cols.rows[j];
                sorted_cols.rows[j] = sorted_cols.rows[j + 1];
                sorted_cols.rows[j + 1] = t;
                
                double d = sorted_cols.data[j];
                sorted_cols.data[j] = sorted_cols.data[j + 1];
                sorted_cols.data[j + 1] = d;
            }
        }
    }
    int j = 0;
    i = 0;
    SparseMatrix tmp(count_rows, matr.count_cols);
    if (count_cols == 0)
    {
        std::cout<<"Error with matr"<< std::endl;
        exit(1);
    }
    while (i < size)
    {
        int k = i;
        j = 0;
        size_t prev_row = rows[i];
        while (j < sorted_cols.size)
        {
            size_t prev_col = sorted_cols.cols[j];
            double res = 0;
            while (j < sorted_cols.size && prev_col == sorted_cols.cols[j] &&
                   i < size && prev_row == rows[i])
            {
                if (sorted_cols.rows[j] < cols[i])
                {
                    j++;
                }
                else if (cols[i] < sorted_cols.rows[j])
                {
                    i++;
                }
                else if (cols[i] == sorted_cols.rows[j])
                {
                    res += data[i] * sorted_cols.data[j];
                    i++;
                    j++;
                }
            }
            if (abs(res) > epsilon)
            {
                tmp.set(prev_row, prev_col, res);
            }
            i = k;
            
        }
        
        while (i < size && prev_row == rows[i])
        {
            i++;
        }
        
    }
    return tmp;
}

void wide_array (double*& arr, size_t index, double value, size_t size)
{
    double* tmp = new double [size];
    int j = 0;
    while (j < index)
    {
        tmp[j] = arr[j];
        j++;
    }
    tmp[j] = value;
    j++;
    while (j < size)
    {
        tmp[j] = arr[j - 1];
        j++;
    }
    if (size > 1)
    {
        delete [] arr;
    }
    arr = tmp;
    
}

void wide_array (size_t*& arr, size_t index, size_t value, size_t size)
{
    
    size_t* tmp = new size_t [size];
    int j = 0;
    while (j < index)
    {
        tmp[j] = arr[j];
        j++;
    }
    tmp[j] = value;
    j++;
    while (j < size)
    {
        tmp[j] = arr[j - 1];
        j++;
    }
    if (size > 1)
    {
        delete [] arr;
    }
    arr = tmp;
    
}

void SparseMatrix::set(size_t m_row, size_t m_col, double value)
{
    if ((m_row > count_rows) || (m_col > count_cols))
    {
        std::cout<<"Error with SET (beyond the grant range)"<<std::endl;
        exit(1);
    }
    int i = 0;
    while (i < size && rows[i] < m_row)
    {
        i++;
    }
    while (i < size && rows[i] == m_row && cols[i] < m_col)
    {
        i++;
    }
    if (i < size && rows[i] == m_row && cols[i] == m_col)
    {
        data[i] = value;
        return;
    }
    size++;
    wide_array (rows, i, m_row, size);
    wide_array (cols, i, m_col, size);
    wide_array (data, i, value, size);
    
    return;
}

double SparseMatrix::get(size_t m_row, size_t m_col) const
{
    if ((m_row > count_rows) || (m_col > count_cols))
    {
        std::cout<<"Error with GET(beyond the grant range)"<< std::endl;
        exit(1);
    }
    int  i = 0;
    while (m_row > rows[i])
    {
        i++;
    }
    while (rows[i] == m_row)
    {
        while (m_col > cols[i])
        {
            i++;
        }
        if (cols[i] == m_col)
        {
            return data[i];
        }
        return 0;
    }
    return 0;
}

bool operator== (const SparseMatrix& matr_1, const SparseMatrix& matr_2)
{
    if ((matr_1.size != matr_2.size))
    {
        return 0;
    }
    for (int i = 0; i < matr_1.size; i++)
    {
        if (matr_1.rows[i] != matr_2.rows[i])
        {
            return 0;
        }
        if (matr_1.cols[i] != matr_2.cols[i])
        {
            return 0;
        }
        if (abs(matr_1.data[i] - matr_2.data[i]) > matr_1.epsilon)
        {
            return 0;
        }
    }
    return 1;
}

bool operator!= (const SparseMatrix& matr_1, const SparseMatrix& matr_2)
{
    return !(matr_1 == matr_2);
}

size_t SparseMatrix::num_rows() const
{
    std::cout<<"Amount of rows ";
    return count_rows;
}

size_t SparseMatrix::num_columns() const
{
    std::cout<<"Amount of cols ";
    return count_cols;
}

SparseMatrix::Row SparseMatrix::operator[](int r)
{
    if (r > count_rows)
    {
        std::cout<<"Error (beyond the grant range)"<<std::endl;
        exit(1);
    }
    int i = 0;
    while (i < size && rows[i] < r)
    {
        i++;
    }
    return Row(this, i);
}

const SparseMatrix::Row SparseMatrix::operator[] (int r) const
{
    if (r > count_rows)
    {
        std::cout<<"Error (beyond the grant range)"<<std::endl;
        exit(1);
    }
    int i = 0;
    while (i < size && rows[i] < r)
    {
        i++;
    }
    return Row(this, i);
}

double& SparseMatrix::Row::operator[] (int c)
{
    if (c > parent->count_cols)
    {
        std::cout<<"Error (beyond the grant range)"<<std::endl;
        exit(1);
    }
    int i = index;
    size_t row = parent->rows[index];
    while (i < parent->size && parent->rows[i] == row && parent->cols[i] < c)
    {
        i++;
    }
    if (i < parent->size && parent->rows[i] == row && parent->cols[i] == c)
    {
        return parent->data[i];
    }
    parent->size++;
    wide_array (parent->rows, i, row, parent->size);
    wide_array (parent->cols, i, c, parent->size);
    wide_array (parent->data, i, 0, parent->size);
    return parent->data[i];
}

const double& SparseMatrix::Row::operator[] (int c) const
{
    if (c > parent1->count_cols)
    {
        std::cout<<"Error (beyond the grant range)"<<std::endl;
        exit(1);
    }
    int i = index;
    size_t row = parent1->rows[index];
    while (i < parent1->size && parent1->rows[i] == row && parent1->cols[i] < c)
    {
        i++;
    }
    if (i < parent1->size && parent1->rows[i] == row && parent1->cols[i] == c)
    {
        return parent1->data[i];
    }
    return parent1->zero;
}

CopySparse SparseMatrix::operator+ (int row)   // check size of matrices
{
    CopySparse tmp(*this, row);
    return tmp;
}

RowSparse CopySparse::operator* ()
{
    return RowSparse(*this, need_row);
}

RowSparse SparseMatrix::operator* ()
{
    return RowSparse(*this, 0);
}

RowSparse RowSparse::operator+ (int col_num)
{
    return RowSparse(*this, col_num);
}

double& RowSparse::operator*()
{
    if (need_row > count_rows)
    {
        std::cout<<"Error with an appeal to row"<<std::endl;
        exit(1);
    }
    if (need_col > count_cols)
    {
        std::cout<<"Error with an appeal to column"<<std::endl;
        exit(1);
    }
    int i = 0;
    while (i < size && rows[i] < need_row)
    {
        i++;
    }
    while (i < size && rows[i] == need_row && cols[i] < need_col)
    {
        i++;
    }
    if (i < size && rows[i] == need_row && cols[i] == need_col)
    {
        return data[i] ;
    }
    size++;
    wide_array (rows, i, need_row, size);
    wide_array (cols, i, need_col, size);
    wide_array (data, i, 0, size);
    return data[i];
}
