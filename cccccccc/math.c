#include <math.h> 

double getRandom()
{
    return 1.0f;
}
double sigmoid(double x)
{
    return(1/(1+exp(-x)));
}
double meanSquareError(double* outputs, double* ideal, int length)
{
    double value = 0;
    for (int i = 0; i < length; i++)
    {
        value += pow((ideal-outputs),2);
    }
    return value * (1/length);
}