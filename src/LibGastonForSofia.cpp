// LibGastonForSofia.cpp
// Aleksey Buzmakov, avbuzmakov@hse.ru, jan 2016
// The file is based on the file main.cpp written by 
//  Siegfried Nijssen, snijssen@liacs.nl, jan 2004.

#include <LibGastonForSofia.h>

#include <iostream>
#include <fstream>
#include "database.h"
#include "path.h"
#include "misc.h"
#include "graphstate.h"
#include <time.h>

#include <assert.h>

// BU: added for compilation under Ubuntu
#include <stdlib.h>
#include <unistd.h>

using namespace std;

namespace LibGaston {
////////////////////////////////////////////////////////////////////////

Frequency minfreq = 1;
Database database;
Statistics statistics;
int phase = 3;
int maxsize = ( 1 << ( sizeof(NodeId)*8 ) ) - 1; // safe default for the largest allowed pattern
LibGastonDataRef callbackData = 0; // A data that is always passed to the callback.
ReportGraphCallback callback = 0; // The callback function that is used to report graphs.

FILE* output;
// A simple callback function that just print the result in a similar way as the original algorith did (it prints the graph to 'ouput' file)
bool LibGastonAPI PrintToFileCallback( LibGastonDataRef, const LibGastonGraph* graph )
{
  assert( graph != 0 );

  putc ( '#', output );
  putc ( ' ', output );
  puti ( output, graph->Support );
  putc ( '\n', output );

  static int counter = 0;
  counter++;
  putc ( 't', output );
  putc ( ' ', output );
  puti ( output, (int) counter );
  putc ( '\n', output );
  for ( int i = 0; i < graph->VertexCount; i++ ) {
    putc ( 'v', output );
    putc ( ' ', output );
    puti ( output, (int) i );
    putc ( ' ', output );
    puti ( output, (int) graph->Vertices[i] );
    putc ( '\n', output );
  }
  for ( int i = 0; i < graph->EdgeCount; i++ ) {
    putc ( 'e', output );
    putc ( ' ', output );
    puti ( output, (int) graph->Edges[i].From );
    putc ( ' ', output );
    puti ( output, (int) graph->Edges[i].To );
    putc ( ' ', output );
    puti ( output, (int) graph->Edges[i].Label );
    putc ( '\n', output );
  }
  putc( 'x', output );
  for( int i = 0; i < graph->Support; ++i ) {
	  putc( ' ', output );
	  puti( output, (int) graph->Objects[i] );
  }
  putc( '\n', output );
  
  return true;
}

void Statistics::print () {
  int total = 0, total2 = 0, total3 = 0;
  for ( int i = 0; i < frequenttreenumbers.size (); i++ ) {
    cout << "Frequent " << i + 2
         << " cyclic graphs: " << frequentgraphnumbers[i]
         << " real trees: " << frequenttreenumbers[i]
         << " paths: " << frequentpathnumbers[i]
         << " total: " << frequentgraphnumbers[i] + frequenttreenumbers[i] + frequentpathnumbers[i] << endl;
    total += frequentgraphnumbers[i];
    total2 += frequenttreenumbers[i];
    total3 += frequentpathnumbers[i];
  }
  cout << "TOTAL:" << endl
       << "Frequent cyclic graphs: " << total << " real trees: " << total2 << " paths: " << total3 << " total: " << total + total2 + total3 << endl;
}

void puti ( FILE *f, int i ) {
  char array[100];
  int k = 0;
  do {
    array[k] = ( i % 10 ) + '0';
    i /= 10;
    k++;
  }
  while ( i != 0 );
  do {
    k--;
    putc ( array[k], f );
  } while ( k );
}


/////////////////////////////////////////////////////////////////////
} // end of namespace LibGaston

using namespace LibGaston;

bool LibGastonAPI RunGaston( LibGastonDataRef data,
    const char* inputFileName, int support, ReportGraphCallback callbackArg, int maxsizeArg /*= -1*/, TGastonRunningMode mode /*= GRM_All*/ )
{
  
  clock_t t1 = clock ();
  cerr << "GASTON GrAph, Sequences and Tree ExtractiON algorithm" << endl;
  cerr << "Version 1.0 with Occurrence Lists" << endl;
  cerr << "Siegfried Nijssen, LIACS, 2004" << endl;

  // Converting options to internal Gaston representation
  minfreq = support;
  callbackData = data;
  callback = callbackArg;
  if( maxsizeArg > 0 ) {
    maxsize = maxsizeArg;
  }
  switch( mode ) {
  case GRM_All:
    phase = 3;
    break;
  case GRM_Trees:
    phase = 2;
    break;
  case GRM_Pathes:
    phase = 1;
    break;
  default:
    return false;
  }
  
  cerr << "Read" << endl;
  FILE *input = fopen ( inputFileName, "r" );
  database.read ( input );
  fclose ( input );

  cerr << "Edgecount" << endl;
  database.edgecount ();
  cerr << "Reorder" << endl;
  database.reorder ();

  cerr << "InitLegStatics" << endl;
  initLegStatics();
  cerr << "graphstate.init()" << endl;
  graphstate.init ();

  cerr << "Main Procedure" << endl;

  for ( int i = 0; i < database.nodelabels.size (); i++ ) {
    if ( database.nodelabels[i].frequency >= minfreq &&
         database.nodelabels[i].frequentedgelabels.size () ) {
      Path path ( i );
      path.expand ();
    }
  }

  statistics.print ();
}

// A test wrap-up function for the implementation of the api.
int main ( int argc, char *argv[] ) {
  clock_t t1 = clock ();

  char opt;
  int maxsizeArg = -1;
  TGastonRunningMode mode = GRM_All;

  while ( ( opt = getopt ( argc, argv, "m:tp" ) ) != -1 ) {
    switch ( opt ) {
      case 'm': maxsizeArg = atoi ( optarg ) - 1; break;
      case 't': mode = GRM_Trees; break;
      case 'p': mode = GRM_Pathes; break;
    }
  }
    
  if ( argc - optind < 2 || argc - optind > 3 ) {
    cerr << "Parameters: [-m size] [-p] [-t] support input [output]" << endl;
    return 1;
  }
  
  const int support = atoi ( argv[optind] );
  const char* inputFileName = argv[optind+1];
  ReportGraphCallback callback = 0;
  if ( argc - optind == 3 ) {
    callback = PrintToFileCallback;
    output = fopen ( argv[optind+2], "w" );
  }

  RunGaston( 0, inputFileName, support, callback, maxsizeArg, mode );

  clock_t t2 = clock ();
  cout << "Approximate total runtime: " << ( (float) t2 - t1 ) / CLOCKS_PER_SEC << "s" << endl;
}
