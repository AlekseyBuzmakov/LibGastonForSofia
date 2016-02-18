# LibGastonForSofia
А wrap-up c++ library for Gaston algorithm implemented by the author, Siegfried Nijssen.

The detailes about the algorithm can be found in 

> S. Nijssen and J. N. Kok. The Gaston Tool for Frequent Subgraph Mining. Electron. _Notes Theor.  Comput. Sci._, 127(1):77–87, mar 2005.

The implementation of the algorithm is taken from [the web cite of Siegfried Nijssen](http://liacs.leidenuniv.nl/~nijssensgr/gaston/index.html). The code is distributed under the GPL license. 

## Some details about the algorithm
Gaston finds frequent graph patterns starting from empty graphs and moving to larger and large ones, i.e., it is a level-wise algorithm.
Moreover, it has three steps. The first step is mining of pathes. The second step is expanison available graphs to trees. And the last step is expanision of threes to general graphs.

## Interface of the LibGastonForSofia library
The interface of the library can be found in the [include file](https://github.com/AlekseyBuzmakov/LibGastonForSofia/blob/master/inc/LibGastonForSofia.h).

It consist of one function **RunGaston** that accepts the same parameters as the original implementation plus a reference to the callback function and an arbitrary data pointer passed to the callback function.
The introduction of the callback function allows for a flexibal interaction with the **Gaston** algorithm.
```c++
/**
 * This function runs gaston on the input dataset.
 * @param inputFileName is the name of the file containing the descriptions of graphs
 * @param support is minimal support (the number of graphs from the dataset) of a discovered graph pattern (a subgraph)
 * @param callback is the callback function, which is used for reporting discovered graphs
 * @param maxsize is the maximal number of nodes in discovered graphs (negative number means 'no limitations')
 * @param mode is the mode of the algorithm. It could discover either all graphs or only trees or pathes.
 * @return is the success of the operation.
 */
typedef bool (LibGastonAPI *RunGastonFunc)( LibGastonDataRef data,
    const char* inputFileName, int support, ReportGraphCallback callback, int maxsize/* = -1*/, TGastonRunningMode mode/* = GRM_All*/ );
bool LibGastonAPI RunGaston( LibGastonDataRef data,
    const char* inputFileName, int support, ReportGraphCallback callback, int maxsize = -1, TGastonRunningMode mode = GRM_All );

```

### Callback function

Below you can see the prototype of the callback function
```c++
/**
 * A type of function that is used for reporting newly found graphs
 * @param graph is the reported graph
 * @return returns if the graph should be further expanded
 */
typedef bool (LibGastonAPI *ReportGraphCallback)( LibGastonDataRef data, const LibGastonGraph* graph );
```

This callback function is used to report found graphs, encoded by structure **LibGastonGraph**. The reference to data coincides with the one passed to **RunGaston** function.
It is never used inside the algorithm and required only for the user that is needed to pass some additional data to callback function (e.g., an instance of the callback-processing class).

### Graph encoding in callback

The graph is encoded by the following structure and basically is just an enumeration of graph vertices, edges, and zero-based objects (transactions) IDs that refer to graphs from the dataset (tidsets). The returning of the tidset of a graph is the second major modification that were made to the original Gaston implementation.

```c++
// Type for storing label of vertices
typedef int VertexLabelType;
// Type for storing label of edges
typedef int EdgeLabelType;
/**
 * A sructure encoding a graph edge
 */
struct LibGastonGraphEdge {
  // The 0-based indices of the vertices for this edge.
  int From;
  int To;
  // Label of the edge.
  EdgeLabelType Label;
};

/**
 * A Sructure encoding a graph
 */
struct LibGastonGraph {
  // Number of vertices and edges in the graph
  int VertexCount;
  int EdgeCount;
  // List of vertices, the size is equal VertexCount
  VertexLabelType* Vertices;
  // List of edges, the size is equal EdgesCount
  LibGastonGraphEdge* Edges;
  // The support of this graph
  int Support;
  // The set of objects covering this pattern. Length of Objects is @var Support.
  int* Objects;
};

// Any data that should be passed to callback.
typedef void* LibGastonDataRef;
```

### The **return** value of the callback function.
The callback function returns a boolean value that adds an aditional control over **Gaston**. It is very usefull for branch cutting. Basically it returns the necessity of the expanision of the current graph pattern. If the user knows somehow that the current graph and all its supergraph is of no use, it can return **false** from the callback function saving some computational time. If the callback function always returns **true**, then the result is exactly the same as the one returned by original **Gaston** with the corresponding parameters.
