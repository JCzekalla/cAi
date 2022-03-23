#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "graph.c"
#include "AiLibrary.h"

#define learningRate 0.05
#define trainingCount 50000

typedef struct
{
    double* output;
    double outputLength;
}Result;
typedef struct
{
    double value;
    double otherDelta;

    double* weights;
    int weightCount;

    double EOverO;
    double OOverN;
    double NOverW;
    double delta;
}Node;
typedef struct
{
    Node* Nodes;
    int nodesCount;
}Layer;
typedef struct
{
    Layer* layers;
    int layersCount;
}Network;
double meanSquared(double* errors,double length)
{

    double mean = 0;
    for(int i = 0;i < length;i++)
    {
        mean += errors[i];
        //mean += pow(errors[i],2);
    }
    //mean = pow(mean, 2);
    mean *= 1/length;
    return mean;
}
double sigmoid1(double x)
{
    return x / (1 + x);
}
double sigmoid(double x)
{
    return 1 / (1+exp(-x));
}
double sigmoid2(double x)
{
    return x;
}
double relu(double x)
{
    if(x > 0)
    {
        return x;
    }else
    {
        return 0;
    }
}
double getRandom()
{
    return ((rand() % RAND_MAX) / (double)RAND_MAX);
}
void init(Network* net,int layersSize,int* model)
{
    srand(time(NULL));
    net->layersCount = layersSize;
    net->layers = malloc(sizeof(Layer)*net->layersCount);    
    for (int i = 0; i < net->layersCount; i++)
    {
        net->layers[i].nodesCount = model[i];
        net->layers[i].Nodes = malloc(sizeof(Node)*net->layers[i].nodesCount);
        for (int i2 = 0; i2 < net->layers[i].nodesCount; i2++)
        {
            net->layers[i].Nodes[i2].delta = 0;
            net->layers[i].Nodes[i2].value = 0;
            
            if(i == net->layersCount-1)
            {
                net->layers[i].Nodes[i2].weightCount = 0;
            }else
            {
                net->layers[i].Nodes[i2].weightCount = model[i+1];
                net->layers[i].Nodes[i2].weights = malloc(sizeof(double)*net->layers[i].Nodes[i2].weightCount);
                for (int i3 = 0; i3 < net->layers[i].Nodes[i2].weightCount; i3++)
                {
                    net->layers[i].Nodes[i2].weights[i3] = getRandom();
                }
                
            }
        }
    }
    
}
Result* run(Network *network)
{
    for (int i = 0; i < network->layersCount-1; i++)
    {
        for (int i2 = 0; i2 < network->layers[i+1].nodesCount; i2++)
        {
            for (int i3 = 0; i3 < network->layers[i].nodesCount; i3++)
            {
                network->layers[i+1].Nodes[i2].value += network->layers[i].Nodes[i3].weights[i2] * network->layers[i].Nodes[i3].value;
                //printf("Node %d: %lf\n",i3,network->layers[i].Nodes[i3].value);
            }
            network->layers[i+1].Nodes[i2].value = sigmoid(network->layers[i+1].Nodes[i2].value);
        }
    }
    double* outputs = malloc(sizeof(double)*network->layers[network->layersCount-1].nodesCount);
    for (int i = 0; i < network->layers[network->layersCount-1].nodesCount; i++)
    {
        outputs[i] = network->layers[network->layersCount-1].Nodes[i].value;
    }
    Result *res = malloc(sizeof(Result));
    res->outputLength = network->layers[network->layersCount-1].nodesCount; 
    res->output = outputs;
    return res;
}

int lengthOF(int* arr)
{
    int iterator = 0;
    while(arr[iterator] >= 1)
    {
        iterator++;
    }
    return iterator;
}

void setInput(Network* net,double* inputs)
{
    for (int i = 0; i < net->layers[0].nodesCount; i++)
    {
        net->layers[0].Nodes[i].value = inputs[i];
    }
    
}
double train(Network* network,double error)
{
    for (int i = network->layersCount-1; i >= 0; i--)
    {
        //Layers
        for (int i1 = 0; i1 < network->layers[i].nodesCount; i1++)
        {
            //Nodes
            network->layers[i].Nodes[i1].delta = 0;

            if(i == network->layersCount-1)
            {
                network->layers[i].Nodes[i1].delta = 1;
            }
            
            for (int i2 = 0; i2 < network->layers[i].Nodes[i1].weightCount; i2++)
            {
                //Weights
                network->layers[i].Nodes[i1].delta += network->layers[i].Nodes[i1].weights[i2] * network->layers[i+1].Nodes[i2].delta;
                
                network->layers[i].Nodes[i1].weights[i2] -= learningRate*(network->layers[i].Nodes[i1].value * error *  network->layers[i+1].Nodes[i2].delta);
            }
        }
    }
    return error;
}
double train2(Network* network,double* out,double* target,double totalGlobalError)
{
    for (int i = network->layersCount-1; i >= 0; i--)
    {
        if(i == network->layersCount-1)
        {
            for (int i1 = 0; i1 < network->layers[network->layersCount-1].nodesCount; i1++)
            {
                network->layers[network->layersCount-1].Nodes[i1].delta = 1;
            }   
        }
        if(i == network->layersCount-2)
        {
            //Second last layer
            for (int i1 = 0; i1 < network->layers[i].nodesCount; i1++)
            {
                for (int i2 = 0; i2 < network->layers[i].Nodes[i1].weightCount; i2++)
                {
                    network->layers[i].Nodes[i1].delta = network->layers[i].Nodes[i1].weights[i2] * network->layers[i+1].Nodes[i2].delta;
                    network->layers[i].Nodes[i1].EOverO = ((target[i2]*-1) - (out[i2]*-1));
                    network->layers[i].Nodes[i1].OOverN = (out[i2]*1)-(out[i2]*out[i2]);
                    network->layers[i].Nodes[i1].NOverW = network->layers[i].Nodes[i1].value;

                    network->layers[i].Nodes[i1].otherDelta = network->layers[i].Nodes[i1].EOverO * network->layers[i].Nodes[i1].OOverN * network->layers[i].Nodes[i1].NOverW;

                    network->layers[i].Nodes[i1].weights[i2] -= learningRate * network->layers[i].Nodes[i1].otherDelta;
                }
            }
        }else
        {
            if(i != network->layersCount-1)
            {
                for (int i1 = 0; i1 < network->layers[i].nodesCount; i1++)
                {
                    for (int i2 = 0; i2 < network->layers[i].Nodes[i1].weightCount; i2++)
                    {
                        network->layers[i].Nodes[i1].delta = network->layers[i].Nodes[i1].weights[i2] * network->layers[i+1].Nodes[i2].delta;
                        
                        double EOverOh1 = 0;
                        double Oh1OverNh1 = 0;
                        double Nh1OverW = 0;
                        for (int i3 = 0; i3 < network->layers[network->layersCount-1].nodesCount; i3++)
                        {
                            network->layers[i].Nodes[i1].EOverO = network->layers[i+1].Nodes[i2].EOverO;
                            network->layers[i].Nodes[i1].OOverN = network->layers[i+1].Nodes[i2].OOverN;

                            double Eo1OverNo1 = network->layers[i+1].Nodes[i2].EOverO * network->layers[i+1].Nodes[i2].OOverN;
                            double No1OverOh1 = network->layers[i+1].Nodes[i2].delta;//w5

                            double EoOverO = Eo1OverNo1 * No1OverOh1;

                            EOverOh1 += EoOverO;
                        }
                        Oh1OverNh1 = (network->layers[i+1].Nodes[i2].value*1) - (network->layers[i+1].Nodes[i2].value*network->layers[i+1].Nodes[i2].value);
                        Nh1OverW = network->layers[i].Nodes[i1].value;
                        double newDelta = EOverOh1*Oh1OverNh1*Nh1OverW;

                        network->layers[i].Nodes[i1].weights[i2] -= learningRate * newDelta;
                    }
                }
            }
        }
    }
    return totalGlobalError;
}
void resetNetwork(Network* network)
{
    for (int i = 0; i < network->layersCount; i++)
    {
        for (int i1 = 0; i1 < network->layers[i].nodesCount; i1++)
        {
            network->layers[i].Nodes[i1].value = 0;
            //network->layers[i].Nodes[i1].delta = 0;
        }
        
    }
    
}
void freeNetwork(Network *net)
{
    for (int i = 0; i < net->layersCount; i++)
    {
        if(i!=net->layersCount-1)
        {
            for (int i1 = 0; i1 < net->layers[i].nodesCount; i1++)
            {
                free(net->layers[i].Nodes[i1].weights);
            }
        }
        
        free(net->layers[i].Nodes);
    }
    free(net->layers);
}
void dumpNetwork(Network *net)
{
	printf("********\n");
	for (int i = 0; i < net->layersCount; i++)
	{
		Layer *l = &net->layers[i];
		for (int j = 0; j < l->nodesCount; j++)
		{
			Node *n = &l->Nodes[j];

			if (n->weightCount > 0)
			{
				for (int k = 0; k < n->weightCount; k++)
				{
					printf("L%d N%d W%d=%lf Delta=%lf I=%lf\n", i, j, k, n->weights[k], n->delta, n->value);
				}
			}
			else
			{
				printf("L%d N%d Delta=%lf I=%lf\n", i, j, n->delta, n->value);
			}
		}
	}
	printf("********\n");
}
double getZeroOrOne()
{
    if(getRandom() >= 0.5)
    {
        return 1;
    }
    return 0;
}
void createTrainingSet(double** inputs,double* desired)
{
    //Xor
    inputs[0] = malloc(sizeof(double)*2);
    inputs[0][0] = 0.01;
    inputs[0][1] = 0.01;
    desired[0] =   0;

    inputs[1] = malloc(sizeof(double)*2);
    inputs[1][0] = 1.01;
    inputs[1][1] = 0.01;
    desired[1] =   1;

    inputs[2] = malloc(sizeof(double)*2);
    inputs[2][0] = 0.01;
    inputs[2][1] = 1.01;
    desired[2] =   1;

    inputs[3] = malloc(sizeof(double)*2);
    inputs[3][0] = 1.01;
    inputs[3][1] = 1.01;
    desired[3] =   0;
}
void createTrainingSet1(double** inputs,double** desired)
{
    //Xor
    inputs[0] = malloc(sizeof(double)*2);
    desired[0] = malloc(sizeof(double)*2);
    inputs[0][0] = 0.01;
    inputs[0][1] = 0.01;
    desired[0][0] =   0;
    desired[0][1] =   1;

    inputs[1] = malloc(sizeof(double)*2);
    desired[1] = malloc(sizeof(double)*2);
    inputs[1][0] = 1.01;
    inputs[1][1] = 0.01;
    desired[1][0] =   1;
    desired[1][1] =   0;

    inputs[2] = malloc(sizeof(double)*2);
    desired[2] = malloc(sizeof(double)*2);
    inputs[2][0] = 0.01;
    inputs[2][1] = 1.01;
    desired[2][0] =   1;
    desired[2][1] =   0;

    inputs[3] = malloc(sizeof(double)*2);
    desired[3] = malloc(sizeof(double)*2);
    inputs[3][0] = 1.01;
    inputs[3][1] = 1.01;
    desired[3][0] =   0;
    desired[3][1] =   1;
}
int main1()
{
    Network network;
    int model[] = {2,2,2,1,-1};
    int modelLength = lengthOF(model);
    char** graph = loadGraph(trainingCount,230001);

    init(&network,modelLength,model);
    dumpNetwork(&network);
    double** inputs = malloc(sizeof(double*)*4);
    double* desired = malloc(sizeof(double)*4);    
    createTrainingSet(inputs,desired);
    graph = resetGraph(graph);
    double* x = malloc(sizeof(double)*trainingCount*4);
    double* y = malloc(sizeof(double)*trainingCount*4);

    for (int i = 0; i < trainingCount; i++)
    {
        for (int i1 = 0; i1 < 4; i1++)
        {
            x[i*4+i1] = i*4+i1;
            resetNetwork(&network);
            setInput(&network,inputs[i1]);
            Result *res = run(&network);
            double output = fabs(res->output[0]);
            double error = desired[i1]-output;
            y[i*4+i1] = train(&network,error);
        }
    }
    dumpNetwork(&network);
    addPoints(x,y,graph,trainingCount);
    openGraph();
    return 1;
}

int main()
{
    Network network;
    int model[] = {2,2,2,-1};
    int modelLength = lengthOF(model);
    char** graph = loadGraph(trainingCount,53001);

    init(&network,modelLength,model);
    dumpNetwork(&network);
    double** inputs = malloc(sizeof(double*)*4);
    double** desired = malloc(sizeof(double*)*4);    
    createTrainingSet1(inputs,desired);
    graph = resetGraph(graph);
    double* x = malloc(sizeof(double)*trainingCount*4);
    double* y = malloc(sizeof(double)*trainingCount*4);

    for (int i = 0; i < trainingCount; i++)
    {
        for (int i1 = 0; i1 < 4; i1++)
        {
            x[i*4+i1] = i*4+i1;
            resetNetwork(&network);
            setInput(&network,inputs[i1]);
            Result *res = run(&network);

            double* errors = malloc(sizeof(double)*res->outputLength);
            for (int i2 = 0; i2 < res->outputLength; i2++)
            {
                errors[i2] = desired[i1][i2] - res->output[i2];
            }
            double Etotal = meanSquared(errors,res->outputLength);

            //double error = desired[i1] - errors[0];
            y[i*4+i1] = train2(&network,res->output,desired[i1],Etotal);
        }
        
    }
    dumpNetwork(&network);
    addPoints(x,y,graph,trainingCount);
    openGraph();
    int runs = 0;
    while(runs == 0)//Test the network after training
    {
        resetNetwork(&network);
        double inputs[2];
        inputs[0] = fgetc(stdin);
        if(inputs[0] == '1')
        {
            inputs[0] = 1.01;
        }else
        {
            inputs[0] = 0.01;
        }
        fflush(stdin);
        inputs[1] = fgetc(stdin);
        fflush(stdin);
        if(inputs[1] == '1')
        {
            inputs[1] = 1;
        }else
        {
            inputs[1] = 0;
        }
        //scanf("[&d &d]",&inputs[0],&inputs[1]);
        setInput(&network,inputs);
        Result* res = run(&network);//Run the network
        printf("1: %lf\n",res->output[0]);
        printf("2: %lf\n",res->output[1]);
    }
    return 1;
}


//Api functions
Network* initMyNetwork(int* model)
{
    Network* network = malloc(sizeof(Network));
    int modelLength = lengthOF(model);
    init(&network,modelLength,model);
    network;
}
void trainMyNetwork(Network* network,int epochCount, double** inputs,double** desired)
{
    for (int i = 0; i < trainingCount; i++)
    {
        resetNetwork(network);
        setInput(network,inputs[i]);
        Result *res = run(network);

        double* errors = malloc(sizeof(double)*res->outputLength);
        for (int i1 = 0; i1 < res->outputLength; i1++)
        {
            errors[i1] = desired[i][i1] - res->output[i1];
        }
        double Etotal = meanSquared(errors,res->outputLength);

        //double error = desired[i1] - errors[0];
        train2(&network,res->output,desired[i],Etotal);        
    }
}
Result* runMyNetwork(Network* network,double* inputs)
{
    resetNetwork(network);
    setInput(network,inputs);

    Result *res = run(network);
    return res;
}