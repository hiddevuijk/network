#ifndef GAURD_TRIANGULAR_NETWORK_H
#define GAURD_TRIANGULAR_NETWORK_H

#include "network.h"

Network create_triangular_network(int N_row_col, double L)
{
    Network network;


    // add nodes
    int label = 0;
    for(int row=0;row<N_row_col; ++row) for(int col=0;col<N_row_col; ++col) {
        std::vector<char> b(0);
        std::vector<Node*> c(0);

        if( (row%2) == 0 ) {
            network.add_node(col, row,label++, c, b);
        }else {
            network.add_node(col+0.5, row,label++, c, b);
        }
    }


    // add connections 
    for(int i=0;i<network.get_N(); ++i) {

        int col = i%N_row_col;
        int row = int(i/N_row_col);
        // not the last column, connect to right node
        if(col < (N_row_col - 1) ) {
            network.connect(i,i+1,'B');
        }
        // not the last row and not the last column
        // connect to upper right node
        if(col < (N_row_col-1) && row < (N_row_col-1) ) {
            network.connect(i,i+N_row_col,'B');
        }
        // not the first column and not the last row
        // connect to upper left node
        if(col > 0 && row < (N_row_col-1) ) {
            network.connect(i,i+N_row_col-1,'B');
        }

         

    }           


    return network; 
}



#endif
