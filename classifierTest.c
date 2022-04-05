#include "classifier.c"

int main()
{
    Network* network = malloc(sizeof(network));//Create Network(Heap)

    //Do network test
    if(isForwardWorks(network))
    {
        printf("Forward Prop: ");
        printf("OK\n");
    }else
    {
        printf("Forward Prop: ");
        printf("ERROR\n");
    }
    freeNetwork(network);//Free Network including all attached variables on the heap

    network = malloc(sizeof(network));//re create Network(Heap)
}