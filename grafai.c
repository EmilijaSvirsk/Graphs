#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define CITY_MAX 20 

void read(char *fl, int *cities, int **graph, int *start, int *end)
{
    FILE *fp = fopen(fl, "r");
    int roads;
    fscanf(fp, "%d%d%*[^\n]", cities, &roads);

    for(int i = 0; i < *cities; ++i)
    {
        graph[i] = (int*) calloc(*cities, sizeof(int));
        for(int j = 0; j < *cities; ++j)
            graph[i][j] = 0;
    }
    
    for(int i = 0; i < roads; ++i)
    {
        int cit1, cit2, len;
        fscanf(fp, "%d%d%d%*[^\n]", &cit1, &cit2, &len);
        graph[cit1][cit2] = len;
        graph[cit2][cit1] = len;
    }
    fscanf(fp, "%d%d%*[^\n]", start, end);
    fclose(fp);
}


void print_path(int *parent, int end)
{
    if(parent[end] == -1)
    {
        printf("%d ", end);
        return;
    }
    print_path(parent, parent[end]);
    printf("%d ", end);
}

int find_distance(int n, int **graph, int *parent, int start, int end)
{
    int *dist = (int*) calloc(n, sizeof(int));
    int *visited = (int*) calloc(n, sizeof(int));
    
    for(int i = 0; i < n; ++i)
    {
        dist[i] = INT_MAX;
        visited[i] = -1;
        parent[i] = -1;
    }
    dist[start] = 0;
    for(int i = 0; i < n - 1; ++i)
    {
        int min = INT_MAX;
        int index = -1;
        for(int j = 0; j < n; ++j)
        {
            if(visited[j] == -1 && dist[j] <= min)
            {
                min = dist[j];
                index = j;
            }
        }
        visited[index] = 1;
        for(int j = 0; j < n; ++j)
        {
            if(visited[j] == -1 && graph[index][j] != 0 && dist[index] != INT_MAX
                && dist[index] + graph[index][j] < dist[j])
            {
                parent[j] = index;
                dist[j] = dist[index] + graph[index][j];
            }
        }
    }
    
    int distance = dist[end];
    free(dist);
    free(visited);
    
    if(distance == INT_MAX)
        return -1;
    else
        return distance;
}


int main()
{
    char *fl_read = (char*) calloc(20, sizeof(char));
    strcpy(fl_read, "duom.txt");
    int **graph = (int**) calloc(CITY_MAX, sizeof(int*));
    int n;
    int start, end;
    read(fl_read, &n, graph, &start, &end);
    
    int *parent = (int*) calloc(n, sizeof(int));
    int dist = find_distance(n, graph, parent, start, end);
    
    if(dist == -1)
        printf("No possible route.\n");
    else
    {
        printf("%d\n", dist);
        print_path(parent, end);
        printf("\n");
    }
    
    free(fl_read);
    free(graph);
    free(parent);
}
