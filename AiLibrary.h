#ifndef _AiLibrary_h
#define _AiLibrary_h

Network* initMyNetwork(int* model);
void trainMyNetwork(Network* network,int epochCount, double** inputs,double** desired);
Result* runMyNetwork(Network* network,double* inputs);

#endif