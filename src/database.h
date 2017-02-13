// database.h
// Siegfried Nijssen, snijssen@liacs.nl, jan 2004.
#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "legoccurrence.h"
#include "misc.h"

#include <assert.h>

namespace LibGaston {
////////////////////////////////////////////////////////////////////////

using namespace std;

typedef unsigned short InputEdgeLabel;
typedef unsigned short InputNodeLabel;
typedef unsigned int InputNodeId;

struct CombinedInputLabel {
	InputEdgeLabel E;
	InputNodeLabel N1;
	InputNodeLabel N2;
	CombinedInputLabel(InputEdgeLabel e, InputNodeLabel n1, InputNodeLabel n2 ) : N1(n1), N2(n2), E(e) {}
	CombinedInputLabel() : N1(-1), N2(-1), E(-1) {}
	CombinedInputLabel( const CombinedInputLabel& other ) : N1(other.N1), N2(other.N2), E(other.E) {}
};

inline bool operator<(const CombinedInputLabel& l1, const CombinedInputLabel& l2 ) {
	return l1.N2 < l2.N2 || l1.N2==l2.N2 && l1.N1 < l2.N1 
		|| l1.N2==l2.N2 && l1.N1==l2.N1 && l1.E < l2.E;

}
#define combineInputLabels(label1,label2,label3) (CombinedInputLabel(label1,label2,label3))
//#define combineInputLabels(label1,label2,label3) (label1 | ( ((unsigned int) label2 ) << 16 ) | ( ( (unsigned int) label3 ) << 24 ) )
// maximum 255 node labels for now.


#define NOINPUTEDGELABEL ((InputEdgeLabel) -1)
#define NOINPUTNODELABEL ((InputNodeLabel) -1)

template<class T>
class pvector {
public:
  pvector<T> (): array(0), _size(0) { }
  void init(T* ar, int s) { assert( array == 0 && _size == 0); array=ar;_size=s;}
  inline int size () const { return _size; }
  void resize ( int s ) { assert( s <= _size );  _size = s; }
  void clear () { array = 0; _size = 0; } // cannot remove allocation, as we are not managing that memory here 
  T &operator[] ( int i ) { assert(i < _size);return array[i]; }
private:
  T *array;
  int _size;
};

struct DatabaseTreeEdge {
  EdgeLabel edgelabel;
  NodeId tonode;

  DatabaseTreeEdge () { }

  friend ostream &operator<< ( ostream &stream, DatabaseTreeEdge &databasetreeedge );
};

struct DatabaseTreeNode {
  NodeLabel nodelabel;
  bool incycle;
  pvector<DatabaseTreeEdge> edges;

  DatabaseTreeNode () { }

  friend ostream &operator<< ( ostream &stream, DatabaseTreeNode &databasetreenode );
};

struct DatabaseTree {
  Tid tid;
  vector<DatabaseTreeNode> nodes;

  DatabaseTreeEdge *edges;

  DatabaseTree ( Tid tid ): tid ( tid ) { }
  DatabaseTree () { }
  
  friend ostream &operator<< ( ostream &stream, DatabaseTree &databasetree );
};

typedef DatabaseTree *DatabaseTreePtr;

struct DatabaseNodeLabel {
  InputNodeLabel inputlabel;
  Frequency frequency;
  Tid lasttid;

  LegOccurrences occurrences;
  vector<EdgeLabel> frequentedgelabels;

  DatabaseNodeLabel (): frequency ( 1 ) { }
};

struct DatabaseEdgeLabel {
  InputEdgeLabel inputedgelabel;
  NodeLabel tonodelabel, fromnodelabel; 
  EdgeLabel edgelabel; // the (order) edge label to which this entry corresponds during the search
  Frequency frequency;
  // vector<Tid> tidset;
  Tid lasttid;

  DatabaseEdgeLabel (): frequency ( 1 ) { }
};

class Database {
  public:
    Database () { }
    vector<DatabaseTreePtr> trees;
    vector<DatabaseNodeLabel> nodelabels;
    vector<DatabaseEdgeLabel> edgelabels;
    map<InputNodeLabel,NodeLabel> nodelabelmap;
    map<CombinedInputLabel,EdgeLabel> edgelabelmap;
    vector<EdgeLabel> edgelabelsindexes; // given an edge label, returns the index of the element in edgelabels in which
    EdgeLabel frequentEdgeLabelSize () const { return edgelabelsindexes.size (); }
                                         // all information about this edge can be found. Used during the search,
					 // only frequent edge label, node label pairs are stored.

     // NOTE! In the input file, the nodes MUST be listed in pre-order.

     // "read" reads the input file, and determines the frequency counts for node labels and (combined) edge labels
    void read ( FILE *input );

     // after "read", determines the frequency of edges, using DatabaseNodeLabel's edgelasttid/edgelabelfrequency
    void edgecount ();

     // after "edgecount",
     // - removes infrequent data
     // - cleans up the datastructures used until now for counting frequencies
     // - changes the edge label order to optimise the search, fills the database with order numbers in stead of
     //   the numbers assigned in the previous phases; fills edgelabelsindexes.
    void reorder ();

    void printTrees ();
    ~Database ();
  private:
    void readTree ( FILE *input, Tid tid );
    void determineCycledNodes ( DatabaseTreePtr tree, vector<int> &nodestack, vector<bool> &visited1, vector<bool> &visited2 );
};

extern Database database;

/////////////////////////////////////////////////////////////////////
} // end of namespace LibGaston

#endif
