#include "classifier.c"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    /*Network* network1 = malloc(sizeof(Network));//Create Network(Heap)
    int model[] = {2,2,1,-1};
    init(network1,model);
    //Do network test
    if(isForwardWorks(network1))
    {
        printf("Forward Prop: ");
        printf("OK\n");
    }else
    {
        printf("Forward Prop: ");
        printf("ERROR\n");
    }*/

    //freeNetwork(network); Free Network including all attached variables on the heap

    //Network* network = malloc(sizeof(Network));//re create Network(Heap)
    Network network;

    int model[] = {2,2,1,-1};
    double inputs[4][2] = {{0,0},{1,0},{0,1},{1,1}};
    double desired[4][1] = {{0},{1},{1},{0}};
    init(&network,model);
    dumpNetwork(&network);
    printf("\n\n\n\n");
    trainNetwork(&network,inputs,desired,40000,4);
    dumpNetwork(&network);
    return 0;
}