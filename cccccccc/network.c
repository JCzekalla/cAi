#include <stdlib.h>
#include <stdio.h>
#include "math.c"

#define hiddenLayerCount 2
#define hiddenLayerSize 2
#define inputLayerSize 2
#define outputLayerSize 1

typedef struct NodeBasic
{
    double storedValue;
    double* weights;
    int weightsLength;
}nodeBasic;
typedef struct Node
{
    double storedValue;
    double* weights;
    int weightsLength;

    nodeBasic* inputNodes;
}node;
void sigmoidMe(node n);
void sigma(node n,double bias);
typedef struct Layer
{
    node* nodesArray;
    int layerSize;
    double bias;
}layer;
void sigma(node n,double bias);
void run(layer* network,double inputs[inputLayerSize])
{
    //For each neuron in Input Layer
    for (int i = 0; i < inputLayerSize; i++)
    {
        network[0].nodesArray[i].storedValue = inputs[i];
    }
    
    //For each layer exludes input layer
    for(int i = 1;i < hiddenLayerCount;i++)
    {
        //For each node
        for (int i1 = 0; i1 < network[i].layerSize; i1++)
        {
            sigma(network[i].nodesArray[i1],network[i].bias);
            sigmoidMe(network[i].nodesArray[i1]);
        }
    }
    printf("Output Values:\n");
    for (int i = 0; i < outputLayerSize; i++)
    {
        printf("%lf\n",network[hiddenLayerCount-1].nodesArray[i].storedValue);
    }
    
}
layer* init()
{
    layer* network = malloc(sizeof(layer)*(hiddenLayerCount+2));
    //Initilize Input Layer
    network[0].nodesArray = malloc(sizeof(node)*inputLayerSize);
    network[0].layerSize = inputLayerSize;
    network[0].bias = getRandom();
    //For each node
    for(int i = 0;i < inputLayerSize;i++)
    {
        network[0].nodesArray[i].storedValue = 0;
        network[0].nodesArray[i].weightsLength = 0;
    }
    //Initilize Hidden Layer
    for(int i = 1;i < hiddenLayerCount-1;i++)
    {
        network[i].nodesArray = malloc(sizeof(node)*hiddenLayerSize);
        network[i].layerSize = hiddenLayerSize;
        network[i].bias = getRandom();
        //For each node
        for (int i1 = 0; i1 < hiddenLayerSize; i1++)
        {
            network[i].nodesArray[i].inputNodes = network[i-1].nodesArray;
            /*network[i].nodesArray[i1].inputNodes->weights = network[i-1].nodesArray->weights;
            network[i].nodesArray[i1].inputNodes->weightsLength = network[i-1].nodesArray->weightsLength;
            network[i].nodesArray[i1].inputNodes->storedValue = network[i-1].nodesArray->storedValue;
            */

            network[i].nodesArray[i1].storedValue = 0;
            network[i].nodesArray[i1].weights = malloc(sizeof(double)*network[i-1].layerSize);   
            network[i].nodesArray[i1].weightsLength = network[i-1].layerSize;

            for (int i2 = 0; i2 < network[i-1].layerSize; i2++)
            {
                network[i].nodesArray[i1].weights[i2] = getRandom();
            }
        }
    }
    //Initilize Output Layer
    network[hiddenLayerCount-1].nodesArray = malloc(sizeof(node)*outputLayerSize);
    network[hiddenLayerCount-1].bias = getRandom();
    for (int i = 0; i < outputLayerSize; i++)
    {
        
        network[hiddenLayerCount-1].nodesArray[i].inputNodes = network[i-1].nodesArray;
        /*network[hiddenLayerCount-1].nodesArray[i].inputNodes->weights = network[i-1].nodesArray->weights;
        network[hiddenLayerCount-1].nodesArray[i].inputNodes->weightsLength = network[i-1].nodesArray->weightsLength;
        network[hiddenLayerCount-1].nodesArray[i].inputNodes->storedValue = network[i-1].nodesArray->storedValue;
        */
        
        network[hiddenLayerCount-1].nodesArray[i].storedValue = 0;
        network[hiddenLayerCount-1].nodesArray[i].weights = malloc(sizeof(double)*network[i-1].layerSize);
        for (int i1 = 0; i1 < network[i-1].layerSize; i1++)
        {
            network[hiddenLayerCount-1].nodesArray[i].weights[i1] = getRandom();
        }
               
        network[hiddenLayerCount-1].nodesArray[i].weightsLength = network[i-1].layerSize;
    }
    network[hiddenLayerCount-1].layerSize = outputLayerSize;

    return network;
}
void sigmoidMe(node n)
{
    n.storedValue = sigmoid(n.storedValue);
}
void sigma(node n,double bias)
{
    //For every neuron in l
    for(int i = 0;i < n.weightsLength;i++)
    {
        n.storedValue += (n.weights[i] * n.inputNodes[i].storedValue);
    }
    n.storedValue += bias;
}