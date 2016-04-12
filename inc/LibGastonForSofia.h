#ifndef _LibGastonForSofia_H_
#define _LibGastonForSofia_H_

#ifndef _WIN32
#define __stdcall
#endif

#define LibGastonAPI __stdcall

#ifdef __cplusplus
extern "C" {
#endif

// Enumeration of possible 'Gaston' algorithm modes.
enum TGastonRunningMode {
  // No limitations on found graphs
  GRM_All = 0,
  // Only tree-like graphs (including pathes)
  GRM_Trees,
  // Only pathes
  GRM_Pathes,

  GRM_EnumCount
};

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

/**
 * A type of function that is used for reporting newly found graphs
 * @param graph is the reported graph
 * @return returns if the graph should be further expanded
 */
typedef bool (LibGastonAPI *ReportGraphCallback)( LibGastonDataRef data, const LibGastonGraph* graph );

/**
 * This function that runs gaston on the input dataset.
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

#ifdef __cplusplus
}
#endif

#endif
