#include <math.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <stdio.h>
using namespace std;

//My original code was taking WAY too long to run, and I think a big reason was because I was using vectors and 
//resizing every iteration.  So, I switced to using mostly arrays and a set max value (over SPOJ's max of 200 chambers)
//Using this site's example: http://www.cplusplus.com/doc/tutorial/preprocessor/

//I also used the book example for my max flow algorithm, but decided to split up bfs() and max flow() for readability
//since it was getting too convoluted to check what was going wrong.
#define _max 202
int capacity[_max][_max], maxF[_max][_max]; //capacity from vertex j to vertex i, max flow from j to i.
vector<int> adjacent[_max]; //vector of arrays to check for vertex adjacency
int bfs_arr[_max];          //bfs array, used in bfs() function
int source, sink;           //source is the highest vertex and sink is the lowest
int inf = 999999;           //represents infinity

//described below
bool bfs();
int maxFlow();

int main()
{ //cases - total # of runs, chambers - specific n value for each run, children - # of adjacent chambers to a vertex v, c1 - placeholder vertex before sending to adjacent
  int cases, chambers, children, c1; //int readins from SPOJ.  I switched from cout/cin to scanf/printf because I
  scanf("%i", &cases);               //read several spoj comments saying they were faster
  for (int a = 0; a < cases; a++)    //entire run for loop.
  {
    scanf("%i", &chambers);
    if (chambers == 0)               //edge case if chambers = 0
    {
      cout << 0 << endl;             //0 splunkers can traverse 0 chambers
      continue;
    }
    for (int m = 0; m < _max; m++)    //resetting capactiy and max flow values after every run
    {
      for (int n = 0; n < _max; n++)
      {
        capacity[m][n] = 0;
        maxF[m][n] = 0;
      }
    }
    source = 1, sink = chambers;      //source is node 1 and sink is node # of chambers.
    for (int i = 1; i < chambers; i++)  //from 1 - chambers, read in each line to find which vertexs are adjacent
    {
      scanf("%i", &children);
      if (children == 0)                //edge case where a vertex has no children nodes
        continue;
      for (int j = 0; j < children; j++)
      {
        scanf("%i", &c1);               //read in the vertex c1 that vertex i is connected to
        if (i != 1 && c1 != sink)       //if either vertex is the source or sink, the capacity is 1.  From SPOJ problem definition
          capacity[i][c1] = inf;        //else, the capacity is infinite
        else
          capacity[i][c1] = 1;
        adjacent[i].push_back(c1);      //push back the verticies to each other's adjacency list
        adjacent[c1].push_back(i);
      }
    }
    if (chambers != 1)                  //edge case where there is 1 chamber
      printf("%d\n", maxFlow());        //this gets weird if you try to let max flow run through it when source = sink
    else
      printf("%i", 1);                  //1 splunker can traverse 1 chamber
    scanf;
    for (int i = 1; i < _max; i++)
      adjacent[i].clear();              //clear the adjacency list for each run
  }
  return 0;
}

//maxFlow takes a path from bfs() and determines the amount of splunkers that can traverse the amount of chambers/the
//graph given.  This will happen several times, until the total max flow (mfl) is returned to the main function.
int maxFlow()
{
  int mfl = 0;        //return value for SPOJ POTHOLE
  int flow;           //flow for each node
  while (bfs())       //while there are still paths to be traversed,
  {
    int n = sink;
    flow = inf;
    while (n != source)   //run until the path is traversed backwards
    {
      int i = bfs_arr[n];    //i is the node that is travelling backwards
      flow = min(flow, capacity[i][n] - maxF[i][n]);    //flow will be the minimum of the current flow through the node and the capacity - current max
      n = i;                //traverse backwards
    }
    mfl += flow;            //A path was found, so mark it in the max flow
    n = sink;               //do it again to determine what direction the flow is travelling
    while (n != source)
    {
      int prev = bfs_arr[n];
      maxF[prev][n] += flow;  //if downwards, add to flow
      maxF[n][prev] -= flow;  //if upwards, subtract from flow
      n = prev;
    }
  }
  return mfl;
}


//Breadth First Search is a traversal algorithm that starts at the top node and visits every node at the present level
//before moving down the tree.  This particular algorithm has been modified to exit once the sink vertex is found. 
//That path will then we traversed backwards to determine the max flow from the source to sink of splunkers.
//If no path is found, or all the paths have been found, the return is false.
bool bfs()
{
  for (int m = 0; m < _max; m++)
  {
    bfs_arr[m] = -1;        //represents unvisited nodes
  }
  bfs_arr[source] = -inf;   //source is -inf so it does not get caught up in the traversal
  queue<int> Q;
  Q.push(source);           //bfs utilizes a queue to traverse adjacent nodes, and begins with only the source in it
  while (!Q.empty())        //run until there are no more adjacent nodes in the graph
  {
    int v = Q.front();      //set the vurrent node v
    Q.pop();                //pop it from the queue
    for (int i = 0; i < adjacent[v].size(); i++) //run through all the adjacent verteces of v
    {
      int u = adjacent[v][i];     //u is the current vertex
      if (bfs_arr[u] == -1 && capacity[v][u] - maxF[v][u] > 0)      //if unvisited and has capacity to traverse,
      {
        bfs_arr[u] = v;     //set the previous vertex for u to v (must go through v to get to u)
        if (u == sink)      //if we have found a path,
          return true;      //return 
        Q.push(u);          //otherwise, push u onto the queue and keep traversing
      }
    }
  }
  return false;
}