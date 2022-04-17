#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define LearningRate 0.05

typedef struct Neuron
{
    int neuronIndex;

    double input;

    double* outWeights;
    int oWLength;

    double** inWeights;
    int iWLength;

    double delta;
}Neuron;
typedef struct Layer
{
    int isLastLayer;
    int isFirstLayer;
    
    double layerBias;

    Neuron* Neurons;
    int nLength;
}Layer;
typedef struct Network
{
    Layer* layers;
    Layer* firstLayer;
    Layer* lastLayer;
    int lLength;

}Network;
typedef struct Result
{
    double* outputs;
    int winningIndex;
    int equalCases;
}Result;

double sigmoid(double* input)
{
    double newValue = (1)/(1+exp((-1)*(*input)));
    *input = newValue;
    return newValue;
}
double randome()
{
	return (rand() % RAND_MAX) / (double)RAND_MAX;
}
int arrayLength(int* arr)
{
    int i = 0;
    while (arr[i] != -1)
    {
        i++;
    }
    return i;
}
void freeNetwork(Network* network)
{
    for (int i = 0; i < network->lLength; i++)
    {
        for (int i1 = 0; i1 < network->layers[i].nLength; i1++)
        {
            free(network->layers[i].Neurons[i1].outWeights);
        }
        free(network->layers[i].Neurons);
    }
    free(network->layers);
    free(network);
}
int zeroNetwork(Network* network)
{
    for (int i = 0; i < network->lLength; i++)
    {
        Layer* currentLayer = &network->layers[i];
        Layer* previousLayer;
        currentLayer->isFirstLayer = 0;
        if(i == 0)
        {
            currentLayer->isFirstLayer = 1;
        }else
        {
            previousLayer = &network->layers[i-1];
        }
        currentLayer->layerBias = randome();
        for (int i1 = 0; i1 < currentLayer->nLength; i1++)
        {
            Neuron* currentNode = &currentLayer->Neurons[i1];
            currentNode->input = 0;
            if(!currentLayer->isLastLayer)
            {
                for (int i2 = 0; i2 < currentNode->oWLength; i2++)
                {
                    currentNode->outWeights[i2] = randome();
                }
            }
            if(!currentLayer->isFirstLayer)
            {
                for (int i2 = 0; i2 < previousLayer->nLength; i2++)
                {
                    currentNode->inWeights[i2] = &previousLayer->Neurons[i2].outWeights[i1];
                }
            }
        }
    }
    return 0;
}

int init(Network* network,int* model)
{
    srand(time(NULL));
    int layerLength = arrayLength(model);
    network->lLength = layerLength;
    network->layers = malloc(sizeof(Layer)*network->lLength);
    network->firstLayer = &network->layers[0];
    network->lastLayer = &network->layers[network->lLength-1];

    for (int i = 0; i < network->lLength; i++)
    {
        Layer* currentLayer = &network->layers[i];
        Layer* previousLayer;
        currentLayer->isFirstLayer = 0;
        if(i == 0)
        {
            currentLayer->isFirstLayer = 1;
        }else
        {
            previousLayer = &network->layers[i-1];
        }

        currentLayer->isLastLayer = 0;
        if(i == layerLength-1)
        {
            currentLayer->isLastLayer = 1;
        }
        currentLayer->layerBias = randome();
        currentLayer->nLength = model[i];
        currentLayer->Neurons = malloc(sizeof(Neuron)*currentLayer->nLength);

        for (int i1 = 0; i1 < currentLayer->nLength; i1++)
        {
            Neuron* currentNode = &currentLayer->Neurons[i1];

            currentNode->neuronIndex = i1;
            currentNode->input = 0;
            if(!currentLayer->isLastLayer)
            {
                currentNode->outWeights = malloc(sizeof(double)*model[i+1]);
                currentNode->oWLength = model[i+1];
                printf("%d\n",currentNode->oWLength);                
                for (int i2 = 0; i2 < model[i+1]; i2++)
                {
                    currentNode->outWeights[i2] = randome();
                }
            }
            if(!currentLayer->isFirstLayer)
            {
                currentNode->inWeights = malloc(sizeof(double)*previousLayer->nLength);
                for (int i2 = 0; i2 < previousLayer->nLength; i2++)
                {
                    currentNode->inWeights[i2] = &previousLayer->Neurons[i2].outWeights[i1];
                }
                
                currentNode->iWLength = previousLayer->nLength;
            }
            
            
        }
    }
    return 0;
}
Result* forwardPass(Network* network,double* inputs)
{
    for (int i = 0; i < network->lLength; i++)
    {
        Layer* currentLayer = &network->layers[i];
        Layer* previousLayer = &network->layers[i-1];
        for (int i1 = 0; i1 < currentLayer->nLength; i1++)
        {
            Neuron* currentNode = &currentLayer->Neurons[i1];
            
            if(i == 0)
            {
                currentNode->input = inputs[i1];
                continue;
            }
            currentNode->input = 0;
            for (int i2 = 0; i2 < previousLayer->nLength; i2++)
            {
                Neuron* previousNeuron = &previousLayer->Neurons[i2];
                currentNode->input +=  previousNeuron->input*previousNeuron->outWeights[i1];
            }
            currentNode->input += currentLayer->layerBias;
            sigmoid(&currentNode->input);
        }
    }
    
    Result* result = malloc(sizeof(result));
    result->equalCases = 0;
    if(network->lastLayer->nLength == 1)
    {
        result->winningIndex = 0;
        result->outputs = &network->lastLayer->Neurons[0].input;
        return result;
    }
    double outputs[network->lastLayer->nLength];
    double highscore = network->lastLayer->Neurons[0].input;
    int index = 0;

    for (int i = 1; i < network->lastLayer->nLength; i++)
    {
        outputs[i] = network->lastLayer->Neurons[i].input;
        if(network->lastLayer->Neurons[i].input > highscore)
        {
            highscore = network->lastLayer->Neurons[i].input;
            index = i;
        }else
        {
            if(network->lastLayer->Neurons[i].input == highscore)
            {
                result->equalCases++;
            }
        }
    }
    result->winningIndex = index;
    result->outputs = outputs;

    return result;
}
double backwardPass(Network* network, double* desired)
{
    double totalOutut = 0;
    
    for (int i = 0; i < network->layers[network->lLength-1].nLength; i++)
    {
        totalOutut += pow((desired[i] - network->layers[network->lLength-1].Neurons[i].input) * 0.5,2);
        totalOutut /= network->layers[network->lLength-1].nLength;
        //network->layers[network->lLength-1].Neurons[i1].delta = 1;
    }

    for (int i = network->lLength-1; i >= 0; i--)//Will go to first and last layer
    {
        for (int i1 = 0; i1 < network->layers[i].nLength; i1++)
        {
            double V1 = 1;
            double V2 = 1;
            if(network->layers[i].isLastLayer)
            {
                V1 = -1*(desired[i1] - network->layers[i].Neurons[i1].input);
                V2 = network->layers[i].Neurons[i1].input * (1-network->layers[i].Neurons[i1].input);
            }
            for(int i2 = 0; i2 < network->layers[i].Neurons[i1].iWLength;i2++)
            {
                if(network->layers[i].isLastLayer)
                {
                    double V3 = 1 * network->layers[i-1].Neurons[i2].input * 1;
                    double editV = V1 * V2 * V3; 
                    *network->layers[i].Neurons[i1].inWeights[i2] -= (LearningRate * editV);
                }
            }
            
        }
    }
    

    return totalOutut;
}
int dumpNetwork(Network* network)
{
    for (int i = 0; i < network->lLength; i++)
    {
        printf("Layer %d:\n",i);
        for (int i1 = 0; i1 < network->layers[i].nLength; i1++)
        {
            printf("  Neuron[%d] %lf & %lf:\n",i1,network->layers[i].Neurons[i1].input,network->layers[i].Neurons[i1].delta);
            for (int i2 = 0; i2 < network->layers[i].Neurons[i1].oWLength; i2++)
            {
                printf("    Weight[%d] %lf:\n",i2,network->layers[i].Neurons[i1].outWeights[i2]);
            }
        }
    }
    return 0;
}


int isForwardWorks(Network* network)
{
    double inputs[] = {2,3};
    for (int i = 0; i < network->lLength; i++)
    {
        Layer* currentLayer = &network->layers[i];
        currentLayer->layerBias = 0;
        for (int i1 = 0; i1 < currentLayer->nLength; i1++)
        {
            Neuron* currenetNode = &currentLayer->Neurons[i1];
            if(currentLayer->isFirstLayer)
            {
                if(i1 == 0)
                {
                    currenetNode->outWeights[0] = 0.11;
                    currenetNode->outWeights[1] = 0.12;
                }else
                {
                    currenetNode->outWeights[0] = 0.21;
                    currenetNode->outWeights[1] = 0.08;
                }
            }else if(i == 1)
            {
                if(i1 == 0)
                {
                    currenetNode->outWeights[0] = 0.14;
                }else
                {
                    currenetNode->outWeights[0] = 0.15;
                }
            }
        }
    }
    Result* r = forwardPass(network,inputs);
    printf("%lf\n",r->outputs[r->winningIndex]);
    if(r->outputs[r->winningIndex] == 0.191)
    {
        return 1;
    }else
    {
        return 0;
    }
}

void trainNetwork(Network* network, double inputs[4][2],double desired[4][1],int epoch,int dataIndexes)
{
    for (int i = 1; i < epoch+1; i++)
    {
        for (int i1 = 0; i1 < dataIndexes; i1++)
        {
            Result* r = forwardPass(network,inputs[i1]);
            //printf("Result[%d]%lf\n",((i-1)*dataIndexes)+i1,r->outputs[r->winningIndex]); 
            backwardPass(network,desired[i1]);
            double error = (*(desired[i1]))-(r->outputs[r->winningIndex]);
            //printf("  Error: %lf\n",error);
        }
    }
    return;
}