/**
 * @file graph.c
 * @author Jacob
 * @brief Graph wrapper for googles graphing api
 * @version 0.1
 * @date 2022-03-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define lineSize 150

int lineAmount;
int lineLength = 0;

void openGraph()
{
    system("open -a \"Google Chrome\" ./graphData.html");
}

/**
 * @brief 
 * 
 * @param trainingCount amount of points to add
 * @param prev the amount of points in the graph
 * @return char** the entire graph html file
 */
char** loadGraph(int trainingCount,int prev)
{
    lineAmount = trainingCount + 30 + prev;
    FILE* file = fopen("./graphData.html", "r");
    char** lines = malloc(sizeof(char*)*lineAmount);
    for (int i = 0; i < lineAmount; i++)
    {
        lines[i] = malloc(sizeof(char)*lineSize);
    }
    int i = 0;
    while(fgets(lines[i],lineSize,file) != NULL)
    {
        i++;
    }
    lineLength = i;
    fclose(file);
    return lines;
}
/**
 * @brief Add points to the graph
 * 
 * @param x x points
 * @param y y points
 * @param graph the graph lines
 * @param pointAmount length of the x or y array
 */
void addPoints(double* x, double* y, char** graph, int pointAmount)
{
    //Start adding data to line 11
    char** newGraph = malloc(sizeof(char*)*lineAmount);
    for (int i = 0; i < lineAmount; i++)
    {
        newGraph[i] = malloc(sizeof(char)*lineSize);
    }
    for (int i = 0; i < 10; i++)
    {
        newGraph[i] = graph[i];
    }
    for (int i = 10; i < pointAmount+10; i++)
    {
        if(i != pointAmount+9)
        {
            sprintf(newGraph[i],"            [%lf, %lf],\n",x[i-10],y[i-10]);
        }else
        {
            sprintf(newGraph[i],"            [%lf, %lf]\n",x[i-10],y[i-10]);
        }
    }

    for (int i = 10+pointAmount; i < lineLength+pointAmount; i++)
    {
        newGraph[i] = graph[i-pointAmount];
    }
    
    FILE *out=fopen("./graphData.html","w");
    for (int i = 0; i < lineLength+pointAmount; i++)
    {
        fputs(newGraph[i],out);
    }
    fflush(out);
    fclose(out);
}

/**
 * @brief returns a graph with no points
 * 
 * @param graph the graph with or without points
 * @return char** graph with no points
 */
char** resetGraph(char** graph)
{
    char** newGraph;
    int size = 0;
    for(int i = 10;i < lineLength;i++)
    {
        int hasBracket = 0;
        for(int i1 = 0;i1 < strlen(graph[i]);i1++)
        {
            if(graph[i][i1] == ')')
            {
                hasBracket = 1;
                break;
            }
        }
        
        if(hasBracket == 0)
        {
            size++;
        }else
        {
            break;
        }
    }
    int newGraphStringLength = lineLength-size;
    newGraph = malloc(sizeof(char*)*newGraphStringLength);
    /*for (int i = 0; i < newGraphStringLength; i++)
    {
        newGraph[i] = malloc(sizeof(char)*lineSize);
    }*/
    
    printf("%d\n",size);
    printf("%d\n",newGraphStringLength);
    for(int i = 0;i < 10;i++)
    {
        newGraph[i] = graph[i];
    }
    for (int i = 10+size; i < newGraphStringLength+size; i++)
    {
        newGraph[i-size] = graph[i];
    }
    
    FILE *out=fopen("./graphData.html","w");
    for (int i = 0; i < lineLength; i++)
    {
        fputs("",out);
    }
    
    for (int i = 0; i < newGraphStringLength; i++)
    {
        fputs(newGraph[i],out);
    }
    lineLength = newGraphStringLength;
    fflush(out);
    fclose(out);
    //free(graph);
    return newGraph;
}