#include <iostream>
#include <string.h>
#include "Matrix.hpp"

int main()
{
    SparseMatrix c(10,29);
    c.set(0, 2, 2); //check "set"
    c.set(1, 2, 5);
    c.set(1, 3, 6);
    std::cout << c.get(1, 2) << std::endl; //check "get"
    std::cout << c.get(1, 5) << std::endl; //check "get"
    SparseMatrix u(3,4);
    u.set(0, 0, 10);
    std::cout <<"Hello "<< u[0][0]<<std::endl; //check indexing
    size_t u_row = u.num_rows();
    std::cout << u_row << std::endl; // check "num_rows"
    std::cout << u.num_columns() << std::endl; // check "num_cols"
    SparseMatrix d(29,90);
    d.set(2, 0, 10);
    std::cout<<d[2][0]<<std::endl;
    SparseMatrix e = c * d;
    std::cout << e; //check mult
    std::cout << e[1][0] << '\n';
    std::cout << (e != c)<< std::endl; //check "!="
    std::cout << (e == u)<< std::endl; //check "=="
    std::cout <<std::endl;
    std::cout << e; //check mult
    std::cout<<  " " ;
    std::cout<< **e <<  " " ;
    std::cout<< *(*(e + 2));
    *(*(e + 2) + 5) = 4;
    std::cout <<  *(*(e + 2)) << std::endl;
    std::cout <<  *(*(e + 2) + 5) << std::endl;
    std::cout <<  *(*(e + 2) + 4) << std::endl;
    std::cout <<  *(*(e + 10) + 10) << std::endl;
    std::cout <<  *(*e + 1) << std::endl;
    std::cout <<  "lololo" << std::endl;
    return 0;
}
