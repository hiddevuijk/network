#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "network.h"
#include "triangular_network.h"

using namespace std;



int main()
{
    Network network = create_triangular_network(4,10.);
    network.write_positions("positions.dat");
    return 0;
}
