// graphstate.h
// Siegfried Nijssen, snijssen@liacs.nl, feb 2004.
#ifndef GRAPHSTATE_H
#define GRAPHSTATE_H
#include "misc.h"
#include <vector>
#include <iostream>
#include "database.h"
#include "closeleg.h"
#include "patterntree.h"

#include <assert.h>

using namespace std;

namespace LibGaston {
////////////////////////////////////////////////////////////////////////

typedef unsigned int Mark;

class GraphState;

extern GraphState graphstate;

class GraphState {
  public:
    struct GSDeletedEdge {
      NodeId tonode, fromnode;
      EdgeLabel edgelabel;
      int postonode, posfromnode;
      bool close;
      Mark cyclemark;
    };
    vector<GSDeletedEdge> deletededges;
    // the current pattern
    vector<Tuple> *treetuples;
    vector<CloseTuple> *closetuples;
    vector<NodeId> nodesinpreorder;
    int backbonelength; // the length of the backbone, in number of nodes
    int startsecondpath; // the position of the second part of the backbone in
                         // the treetuples.
    bool nasty;  // nasty == A-B-A-B-A-B -like cases
    NodeLabel centerlabel;
    EdgeLabel bicenterlabel;
    int closecount;
    bool selfdone; // set by isnormal to store whether the original graph has been
                   // normal-checked; we try to delay this until the very last moment,
                   // as we know that on this graph the normalisation procedure will
                   // have to go through all phases
    
    struct GSEdge {
      int tonode;
      int postonode; // position in the adjacency list of the corresponding reverse edge
      EdgeLabel edgelabel;
      Mark cyclemark;
      bool close; // closing edge
      GSEdge ():cyclemark ( 0 ), close ( false ) { }
      GSEdge ( int tonode, int postonode, EdgeLabel edgelabel, bool close = false ): tonode ( tonode ), postonode ( postonode ), edgelabel ( edgelabel ), cyclemark ( 0 ), close ( close ) { }
    };
    struct GSNode {
      NodeLabel label;
      short unsigned int maxdegree;
      vector<GSEdge> edges;
    };
    //keep for debugging purposes
    void makeState ( DatabaseTree *databasetree );
    void undoState ();
    void insertNode ( NodeLabel nodelabel, short unsigned int maxdegree );
    void deleteNode2 ();
    vector<GSNode> nodes;
    int edgessize;
    short unsigned int getNodeDegree ( int i ) const { return nodes[i].edges.size (); }
    short unsigned int getNodeMaxDegree ( int i ) const { return nodes[i].maxdegree; }
    GraphState ();
    void determineCycles ( unsigned int usedbit );
    int enumerateSpanning ();
    int isnormal ();
    int normalizetree ();
    int normalizeSelf ();
    void init ();
    void insertStartNode ( NodeLabel nodelabel );
    void deleteStartNode ();
    void insertNode ( int from, EdgeLabel edgelabel, short unsigned int maxdegree );
    void deleteNode ();
    void insertEdge ( int from, int to, EdgeLabel edgelabel );
    void deleteEdge ( int from, int to );
    void deleteEdge ( GSEdge &edge ); // pushes deleted edge on stack
    void reinsertEdge (); // reinserts last edge on the stack
    NodeId lastNode () const { return nodes.size () - 1; }
    void print ( FILE *f );

    // Saves graph to a graph structures
    void getGraph(LibGastonGraph& graph);
    // Clears allocated memory by getGraph();
    void deleteGraph(LibGastonGraph& graph);

};

extern LibGastonDataRef  callbackData;

inline bool OUTPUT( const LegOccurrences& occs ) {
  if( callback == 0 ) {
    return true;
  }
  LibGastonGraph graph;
  graph.Support = occs.frequency;

  // Collecting unique tids
  graph.Objects = new int[graph.Support];
  const int elemsCount = occs.elements.size(); 
  Tid lastTid = -1;
  int uniqueTidCount = 0;
  for( int i = 0; i < elemsCount; ++i ) {
	  if( lastTid == occs.elements[i].tid ) {
		  continue;
	  }
	  assert(lastTid == -1 || lastTid < occs.elements[i].tid);

	  assert( uniqueTidCount < graph.Support );
	  lastTid = occs.elements[i].tid;
	  graph.Objects[uniqueTidCount] = lastTid; 
	  ++uniqueTidCount;
  }
  assert( uniqueTidCount == graph.Support );
  //const bool isSupportKnown=( uniqueTidCount == graph.Support );
  //graph.Support=uniqueTidCount;
  
  graphstate.getGraph(graph);
  const bool res = callback( callbackData, &graph);

  graphstate.deleteGraph(graph);

  //if(!isSupportKnown) {
	//assert( occs.tidset == 0 || occs.frequency == occs.tidset->size() );
	//cerr << occs.frequency << ": ";
	//for( int i = 0; i < occs.tidset->size(); ++i ) {
		//cerr << (*occs.tidset)[i] << " ";
	//}	
	//cerr << endl;
	//assert(false);
  //}

  return res;
}

inline bool OUTPUT( const CloseLegOccurrences& occs ) {
  if( callback == 0 ) {
    return true;
  }
  LibGastonGraph graph;
  graph.Support = occs.frequency;

  // Collecting unique tids
  graph.Objects = new int[graph.Support];
  const int elemsCount = occs.elements.size(); 
  Tid lastTid = -1;
  int uniqueTidCount = 0;
  for( int i = 0; i < elemsCount; ++i ) {
	  if( lastTid == occs.elements[i].tid ) {
		  continue;
	  }

	  assert(lastTid == -1 || lastTid < occs.elements[i].tid);

	  assert( uniqueTidCount < graph.Support );
	  lastTid = occs.elements[i].tid;
	  graph.Objects[uniqueTidCount] = lastTid; 
	  ++uniqueTidCount;
  }
  assert( uniqueTidCount == graph.Support );
  
  graphstate.getGraph(graph);
  const bool res = callback( callbackData, &graph);

  graphstate.deleteGraph(graph);
  return res;
}

/////////////////////////////////////////////////////////////////////
} // end of namespace LibGaston

#endif
