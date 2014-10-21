#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

#define MAX_NUMBER_OF_VERTICES 1000
#define MAX_NUMBER_OF_COLORS 1000
#define BITS 32

typedef pair<int, int> IdDeg;

void setBit(unsigned int &x, int i);
void clearBit(unsigned int &x, int i);
unsigned int getBit(unsigned int x, int i);

class Graph {
 private:
  int nVertex;
  vector< vector<int> > mList;
  vector< vector<int> > mComplementList;

 public:
  Graph();
  Graph(vector< vector<int> >);
  ~Graph();
  int getNumVertices();
  bool isConnected(int, int);
  void calComplementList();
  vector< vector<int> > getAdjList();
  vector< vector<int> > getComplementAdjList();
};

class Clique {
 private:
  //VARIABLES DECLARATION---------------------------------------------------------------------------
  int n;
  // Adjacency matrix
  unsigned int ll[MAX_NUMBER_OF_VERTICES][MAX_NUMBER_OF_VERTICES / BITS + 1];
  vector< IdDeg > deg;

  //set of current clique
  unsigned int Q[MAX_NUMBER_OF_VERTICES / BITS + 1];

  //set of current maximum clique 
  unsigned int Qmax[MAX_NUMBER_OF_VERTICES / BITS + 1];
  
  bool Colored[MAX_NUMBER_OF_VERTICES];
  
  unsigned int c[MAX_NUMBER_OF_VERTICES];

  unsigned int MaxCliqueSize;

  unsigned int CurSet[MAX_NUMBER_OF_VERTICES / BITS + 1];

  vector<int> MaxClique;
  vector< vector<int> > CliqueList;
  //END OF VARIALBLES DECLARATION--------------------------------------------------------------------

  //SUPPORTING FUNCTIONS DECLARATION-----------------------------------------------------------------
  bool isConnected(int, int);
  void intersect(unsigned int * lli, unsigned int * R, unsigned int *Rp);
  int CountOneBit(int start, unsigned int *x);
  
  int VertexColor(unsigned int *, unsigned int *, unsigned int &);
  void EXPAND(unsigned int *, unsigned int); 
  void BASICEXPAND(unsigned int *, unsigned int);
  int MaxCliqueFinding();
  void CliquePartition();

  //END OF SUPPORTING FUNCTIONS DECLARATION-------------------------------------------------------------------

public:
  Clique();
  Clique(Graph);
  ~Clique();
  vector<int> getMaximumCliqueList();
  vector< vector<int> > getCliquePartitionList();
};

class VertexColor {
 private:
  int n;

  int CurUsedColor;
  int BestColorNum; // best number of used colors up to now

  unsigned int ll[MAX_NUMBER_OF_VERTICES][MAX_NUMBER_OF_VERTICES / BITS + 1];
  vector< IdDeg > deg;

  int Color[MAX_NUMBER_OF_VERTICES];
  int CurColor[MAX_NUMBER_OF_VERTICES];
  
  int DSATUR[MAX_NUMBER_OF_VERTICES]; // degree of saturation list

  bool isConnected(int, int);
  
  void HeuristicColoring();

  bool isPossible(int, int);
  void Update(int, int);
  void BBColoring(int, int); // branch and bound coloring
  void Coloring();
  
 public:  
  VertexColor();
  VertexColor(Graph);
  ~VertexColor();
  vector< vector<int> > getColorList();
};
#endif
