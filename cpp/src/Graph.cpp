#include "Graph.h"

#define BITS 32

/* 
This file contains three classes:
1. Graph: A class holds the information of a graph in form of adjacent list: mList
+ Graph(): Constructor; creates a graph from file for testing purpose
+ Graph(vector<ES>): Constructor; creates a graph from list of ES. There is an edge between two ES instances if 
they are compatible
+ calReversedList(): Creates a reversed graph from the current graph and holds the information of this graph in 
mReversedList. There is an edge between two vertices in the new graph if there is no edge between these two 
vertices in the original graph. This function is used for Vertex Coloring Problem

2. CliqueCover: Split the graph into subsets of vertices. Each set forms a clique.
+ CliqueCover(Graph graph): Constructor; Creates an instance of CliqueCover from a graph
+ vector< vector<int> > getCliqueCoverList(): Returns list of each sets

3. VertexColor: Solve the same problem as above but using a different approach
+ VertexColor(Graph graph): Constructor; Creates an instance of VertexColor from a graph
+ vector< vector<int> > getColorList(): Returns the list of colors. Each list contains vertices with the same 
color. In other words, each list forms a clique.
*/

bool comp(const IdDeg &x, const IdDeg &y) {
  if (x.second > y.second) return true;
  if (x.second < y.second) return false;
  if (x.first < y.first) return true;
  return false;
}

void setBit(unsigned int &x, int i) { 
  x = x | (1 << i);
} 

void clearBit(unsigned int &x, int i) { 
  x = x & (~(1 << i));
} 

unsigned int getBit(unsigned int x, int i) { 
  return ((x & 1 << i) != 0); 
}

//////////////////////////////////////////////////////////////////////////////////////////////
Graph::Graph(vector< vector<int> > list) {
  mList.clear();
  mComplementList.clear();
  nVertex = 0;
  if (list.size() == 0) return;
  nVertex = list.size();
  for (int i = 0; i < nVertex; i ++) {
    vector<int> newList;
    for (int j = 0; j < list[i].size(); j ++) {
      newList.push_back(list[i][j]);
    }
    sort(newList.begin(), newList.end());
    mList.push_back(newList);
  }
  calComplementList();
}

Graph::~Graph() {
  for (int i = 0; i < mList.size(); i ++)
    mList[i].clear();
  mList.clear();
  for (int i = 0; i < mComplementList.size(); i ++)
    mComplementList[i].clear();
  mComplementList.clear();
}

void Graph::calComplementList() {
  mComplementList.clear();
  for (int i = 0; i < nVertex; i ++) {
    vector<int> newList;
    bool adjacent[MAX_NUMBER_OF_VERTICES];
    for (int j = 0; j < nVertex; j ++) {
      adjacent[j] = false;
    }
    adjacent[i] = true;
    for (int j = 0; j < mList[i].size(); j ++) {
      adjacent[mList[i][j]] = true;
    }
    for (int j = 0; j < nVertex; j ++) 
      if (!adjacent[j]) {
	newList.push_back(j);
      }
    sort(newList.begin(), newList.end());
    mComplementList.push_back(newList);
  }
}

vector< vector<int> > Graph::getAdjList() {
  return mList;
}

vector< vector<int> > Graph::getComplementAdjList() {
  return mComplementList;
}

bool Graph::isConnected(int i, int j) {
  if (mList[i].size() == 0) return false;
  int lo = 0;
  int hi = mList[i].size() - 1;
  while (lo < hi) {
    int mid = (lo + hi) / 2;
    if (mList[i][mid] == j) return true;
    if (mList[i][mid] > j) hi = mid - 1;
    else lo = mid + 1;
  }
  return false;
}

int Graph::getNumVertices() {
  return nVertex;
}

//////////////////////////////////////////////////////////////////////////////////

void Clique::intersect(unsigned int * lli, unsigned int * R, unsigned int *Rp) {
  for (int i = 0; i <= (n - 1) / BITS; i ++) {
    Rp[i] = (lli[i] & R[i]);
  }
}

int Clique::CountOneBit(int start, unsigned int *x) {
  int ret = 0;
  for (int i = start; i <= (n - 1) / BITS; i ++)
    ret = ret + __builtin_popcount(x[i]);
  return ret;
}

bool Clique::isConnected(int i, int j) {
  return (getBit(ll[i][j / BITS], j % BITS) == 1);
}

void Clique::BASICEXPAND(unsigned int * Rem, unsigned int CurCliqueSize) {
  int NumRem = 0;
  NumRem = CountOneBit(0, Rem);
  if (NumRem == 0) {
    if (MaxCliqueSize < CurCliqueSize) {
      MaxCliqueSize = CurCliqueSize;
      for (int i = 0; i < (n - 1) / BITS; i ++) {
	Qmax[i] = Q[i];
      }
    }
    return;
  }
  if (CurCliqueSize + NumRem <= MaxCliqueSize) {
    return;
  }
  while (NumRem > 0) {
    for (int p = 0; p < n; p ++)
      if (getBit(Rem[p / BITS], p % BITS)) {
	if (CurCliqueSize + NumRem > MaxCliqueSize) {
	  unsigned int Rp[MAX_NUMBER_OF_VERTICES / BITS + 1];
	  setBit(Q[p / BITS], p % BITS);
	  intersect(ll[p], Rem, Rp);
	  BASICEXPAND(Rp, CurCliqueSize + 1);
	  clearBit(Q[p / BITS], p % BITS);
	}
	clearBit(Rem[p / BITS], p % BITS);
	NumRem --;
      }
  }
}

int Clique::VertexColor(unsigned int *Rem, unsigned int *Col, unsigned int &CurListSize) {
  unsigned int Color[MAX_NUMBER_OF_VERTICES];
  unsigned int CurList[MAX_NUMBER_OF_VERTICES];
  bool Choosed[MAX_NUMBER_OF_VERTICES];
  CurListSize = 0;
  int CurColor = 0;
  for (int i = 0; i < n; i ++)
    if (getBit(Rem[deg[i].first / BITS], deg[i].first % BITS)) {
      CurList[CurListSize ++] = deg[i].first;
    }
  for (int i = 0; i < CurListSize; i ++) {
    Color[i] = 0;
    for (int j = 1; j <= CurColor; j ++) {
      Choosed[j] = false;
    }
    for (int j = i - 1; j >= 0; j --)
      if (isConnected(CurList[i], CurList[j])) {
	Choosed[ Color[j] ] = true;
      }
    for (int j = 1; j <= CurColor; j ++)
      if (!Choosed[j]) {
	Color[i] = j;
	break;
      }
    if (Color[i] == 0) {
      CurColor ++;
      Color[i] = CurColor;
    }
  }
  int k = 0;
  for (int i = 1; i <= CurColor; i ++)
    for (int j = 0; j < CurListSize; j ++)
      if (Color[j] == i) {
	Col[k ++] = CurList[j];
      }
  return CurColor;
}

void Clique::EXPAND(unsigned int * Rem, unsigned int CurCliqueSize) {
  unsigned int Col[MAX_NUMBER_OF_VERTICES];
  unsigned int CurListSize;
  //  int NumRem = CountOneBit(0, Rem);
  int ColorNum = VertexColor(Rem, Col, CurListSize);
  if (CurListSize == 0) {
    if (MaxCliqueSize < CurCliqueSize) {
      MaxCliqueSize = CurCliqueSize;
      for (int i = 0; i <= (n - 1) / BITS; i ++) {
	Qmax[i] = Q[i];
      }
    }
    return;
  }
  if (CurCliqueSize + ColorNum <= MaxCliqueSize) {
    return;
  }
  for (int i = CurListSize - 1; i >= 0; i --) {
    int p = Col[i];
    if (getBit(Rem[p / BITS], p % BITS)) {
      unsigned int Rp[MAX_NUMBER_OF_VERTICES / BITS + 1];
      setBit(Q[p / BITS], p % BITS);
      intersect(ll[p], Rem, Rp);
      EXPAND(Rp, CurCliqueSize + 1);
      clearBit(Q[p / BITS], p % BITS);
      clearBit(Rem[p / BITS], p % BITS);
    }
  }
}

int Clique::MaxCliqueFinding() {
  unsigned int Rem[MAX_NUMBER_OF_VERTICES / BITS + 1];
  MaxCliqueSize = 0;
  for (int i = 0; i <= (n - 1) / BITS; i ++) {
    Rem[i] = Q[i] = Qmax[i] = 0;
  }
  for (int i = 0; i < n; i ++) 
    if (!Colored[i]) {
      setBit(Rem[i / BITS], i % BITS);
    }
  EXPAND(Rem, 0);
  MaxClique.clear();
  for (int i = 0; i < n; i ++) {
    if (getBit(Qmax[i / BITS], i % BITS)) {
      MaxClique.push_back(i);
    }
  }
  MaxCliqueSize = MaxClique.size();
  return MaxCliqueSize;
}

void Clique::CliquePartition() {
  MaxClique.clear();
  CliqueList.clear();
  for (int i = 0; i < n; i ++) {
    Colored[i] = false;
  }
  // clique partition by finding MAXIMUM clique each time
  // while (MaxCliqueFinding() > 0) {
  //   vector<int> newClique;
  //   for (int i = 0; i < n; i ++) {
  //     if (getBit(Qmax[i / BITS], i % BITS)) {
  // 	Colored[i] = true;
  // 	newClique.push_back(i);
  //     }
  //   }
  //   CliqueList.push_back(newClique);
  // }

  // Clique partition by finding MAXIMAL clique each time
  int id = 0;
  while (id < n) {
    int u = deg[id].first;
    if (Colored[u]) {
      id ++;
      continue;
    }
    vector<int> newClique;
    newClique.push_back(u);
    for (int i = id + 1; i < n; i ++) {
      int v = deg[i].first;
      if (Colored[v]) continue;
      bool ok = true;
      for (int j = 0; j < newClique.size(); j ++)
	if (!isConnected(newClique[j], v)) {
	  ok = false;
	  break;
	}
      if (ok) {
	newClique.push_back(v);
      }
    }
    for (int i = 0; i < newClique.size(); i ++) {
      Colored[newClique[i]] = true;
    }
    CliqueList.push_back(newClique);
  }
  for (int i = 0; i < n; i ++) {
    Colored[i] = false;
  }
}

Clique::Clique() {
}

Clique::Clique(Graph OriginalGraph) {
  vector< vector<int> > graph = OriginalGraph.getAdjList();
  if (graph.size() == 0) {
    cout << "Error: empty adjacent list." << endl;
  }
  n = graph.size();
  for (int i = 0; i < n; i ++) {
    deg.push_back(pair<int, int>(i, graph[i].size()));
    for (int j = 0; j <= (n - 1) / BITS; j ++) {
      ll[i][j] = 0;
    }
  }
  sort(deg.begin(), deg.end(), comp);
  for (int i = 0; i < n; i ++) {
    for (int t = 0; t < graph[i].size(); t ++) {
      int j = graph[i][t];
      setBit(ll[i][j / BITS], j % BITS);
    }
  }
};

vector<int> Clique::getMaximumCliqueList() {
  MaxCliqueFinding();
  return MaxClique;
}

vector< vector<int> > Clique::getCliquePartitionList() {
  CliquePartition();
  return CliqueList;
}

Clique :: ~Clique() {
  deg.clear();
  MaxClique.clear();
  CliqueList.clear();
}

///////////////////////////////////////////////////////////////////////////////////

bool VertexColor::isConnected(int i, int j) {
  return (getBit(ll[i][j / BITS], j % BITS) == 1);
}

VertexColor::VertexColor() {
  deg.clear();
}

VertexColor::VertexColor(Graph OriginalGraph) {
  vector< vector<int> > graph = OriginalGraph.getAdjList();
  if (graph.size() == 0) {
    cout << "Error: empty adjacent list." << endl;
  }
  n = graph.size();
  for (int i = 0; i < n; i ++) {
    deg.push_back(pair<int, int>(i, graph[i].size()));
    for (int j = 0; j <= (n - 1) / BITS; j ++) {
      ll[i][j] = 0;
    }
  }
  sort(deg.begin(), deg.end(), comp);
  for (int i = 0; i < n; i ++) {
    CurColor[i] = -1;
    DSATUR[i] = 0;
    for (int t = 0; t < graph[i].size(); t ++) {
      int j = graph[i][t];
      setBit(ll[i][j / BITS], j % BITS);
    }
  }
  HeuristicColoring();
  cout << "Heuristic result: " << BestColorNum << endl;
  Clique clique(OriginalGraph);
  vector<int> InitialList = clique.getMaximumCliqueList();
  for (int i = 0; i < InitialList.size(); i ++) {
    CurColor[InitialList[i]] = i;
    for (int j = 0; j < n; j ++)
      if (isConnected(InitialList[i], j)) {
	DSATUR[j] ++;
      }
  }
  CurUsedColor = InitialList.size();
}

VertexColor::~VertexColor() {
  deg.clear();
}

void VertexColor::HeuristicColoring() {
  int CurColor = 0;
  for (int i = 0; i < n; i ++) {
    int u = deg[i].first;
    Color[u] = -1;
    bool Used[MAX_NUMBER_OF_VERTICES];
    for (int j = 0; j < CurColor; j ++) {
      Used[j] = false;
    }
    for (int j = 0; j < i; j ++) {
      int v = deg[j].first;
      if (isConnected(u, v)) {
	Used[Color[v]] = true;
      }
    }
    for (int j = 0; j < CurColor; j ++)
      if (!Used[j]) {
	Color[u] = j;
	break;
      }
    if (Color[u] == -1) {
      CurColor ++;
      Color[u] = CurColor - 1;
      vector<int> newColor;
      newColor.push_back(u);
    }
  }
  BestColorNum = CurColor;
}

// uu can be colored by color j
bool VertexColor::isPossible(int uu, int j) {
  for (int i = 0; i < n; i ++)
    if (CurColor[i] != -1 && isConnected(i, uu) && CurColor[i] == j)
      return false;
  return true;
}

void VertexColor::Update(int uu, int t) {
  for (int i = 0; i < n; i ++)
    if (isConnected(i, uu)) {
      DSATUR[i] += t;
    }
}

void VertexColor::BBColoring(int Remain, int CurUsedColor) {
  if (Remain <= 0) {
    if (CurUsedColor < BestColorNum) {
      BestColorNum = CurUsedColor;
      cout << "New val: " << BestColorNum << endl;
      for (int i = 0; i < n; i ++) {
	Color[i] = CurColor[i];
      }
    }
    return;
  }
  if (CurUsedColor >= BestColorNum) return;
  int MaxDSATUR = -1;
  int uu = -1;
  for (int i = 0; i < n; i ++) {
    int u = deg[i].first;
    if (CurColor[u] != -1) continue;
    if (DSATUR[u] > MaxDSATUR) {
      MaxDSATUR = DSATUR[u];
      uu = u;
    }
  }
  for (int j = 0; j < CurUsedColor + 1; j ++) 
    if (isPossible(uu, j)) {
      CurColor[uu] = j;
      Update(uu, 1);
      BBColoring(Remain - 1, max(CurUsedColor, j + 1));
      CurColor[uu] = -1;
      Update(uu, -1);
    }
}

void VertexColor::Coloring() {
  // Find maximum clique in the given graph to determine initial colors
  int Remain = n - CurUsedColor;
  BBColoring(Remain, CurUsedColor);
}

vector< vector<int> > VertexColor::getColorList() {
  Coloring();
  vector< vector<int> > ColorList;
  for (int i = 0; i < BestColorNum; i ++) {
    vector<int> newColor;
    ColorList.push_back(newColor);
  }
  for (int i = 0; i < n; i ++) {
    ColorList[Color[i]].push_back(i);
  }
  return ColorList;
}

//////////////////////////////////////////////////////////////////////////////////
