#include "network.c"

int main()
{
    layer* network = init();
    double inputs[2] = {0,1};
    run(network,inputs);
}