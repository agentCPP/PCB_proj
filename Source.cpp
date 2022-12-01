#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/extended_graph_alg.h>
#include <ogdf/planarity/ExtractKuratowskis.h>
#include <ogdf/planarity/SubgraphPlanarizer.h>
#include <ogdf/planarity/PlanarSubgraphFast.h>
#include <ogdf/planarity/VariableEmbeddingInserter.h>


using namespace ogdf;

int main()
{
    //Initializes a graph and its data.
    Graph G;
    GraphAttributes GA(G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics);


    /*
    Reads in the input file with the Graph's data.
    Separates each line of the input file into a list.
    Goes through the input file and finds the total number of starting nodes; then adds these primary nodes to a list named Nodes.
    */
    std::string myText;
    std::ifstream MyFile("input.txt");

    std::list<std::string> Nodes;
    Nodes.push_back("1");
    std::list<std::string> lines;
    std::list<std::list<string>> line_elms;
    while (std::getline(MyFile, myText)) {
        lines.push_back(myText);
    }
    MyFile.close();
    for (string line : lines) {
        line_elms.push_back(split(line));
    }

    for (string line : lines) {
        if (!find(line.substr(0, 1), Nodes)) {
            Nodes.push_back(line.substr(0,1));
        }
    }


    /*
    Goes through the list of nodes and creates the edges between nodes based on input file data.
    */
    int i = 0;
    int j = 0;
    for (string new_node : Nodes) {
        string prime_node = new_node;
        node new_node = G.newNode();
        GA.x(new_node) = -5 * (i + 1);
        GA.y(new_node) = -20 * i;
        GA.width(new_node) = 10 * (i + 1);
        GA.height(new_node) = 15;
        i++;
        for (string line : lines) {
            if (line.substr(0, 1) == prime_node) {
                node next_node = G.newNode();
                GA.x(next_node) = 20 * (j - i);
                GA.y(next_node) = 5 * (j + 1 - i);
                GA.width(next_node) = 15;
                GA.height(next_node) = 10 * (j + 1 - i);

                edge e = G.newEdge(new_node, next_node);
                DPolyline& p = GA.bends(e);
                p.pushBack(DPoint(10, -20 * i));
                p.pushBack(DPoint(20 * (j - i), -10));
            }
            j++;

        }

    }
    //Graph should theoretically be fully created now. All that is left is to check for planarity and output graph.


    /*
    Checks if the inputted graph is planar
    If it is planar, create a planar embedding and outputs the graph.
    If it is not planar, divide the graph into subdvisions and output the subdivisions.
    For both cases, the output is a GML file that requires separate software to view in the form of a graph.
    */
    if (isPlanar(G)) {
        planarEmbedPlanarGraph(G);
        GraphIO::write(GA, "planar-pcb.gml", GraphIO::writeGML);
    }
    else {
        SubgraphPlanarizer SP;
        SP.setSubgraph(new PlanarSubgraphFast<int>);
        int crossNum;
        PlanRep PR(G);
        SP.call(PR, 0, crossNum);

        GraphIO::write(PR, "planar-pcb.gml", GraphIO::writeGML);
    }
    return 0;
}

/*
Takes as input a string with only one space.
Separates the string into a list where the first element is the part of the string to the left of the space and vice versa.
It then returns the list of separated items.
*/
std::list<string> split(string s) {
    std::list<std::string> output;
    char separator = ' ';
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == separator) {
            output.push_back(s.substr(0, i - 1));
            output.push_back(s.substr(i + 1, s.length() - 1));
            break;
        }
    }
    return output;
}

/*
Takes as input a list of items you want to search through and a specific item you want to search for.
If the item you are searching for is in the list, returns true.
If the item you are searching for is NOT in the list, returns false.
*/

bool find(std::string item, std::list<std::string> searching) {
    for (auto it = searching.begin(); it != searching.end(); ++it) {
        if (item == *it) {
            return true;
        }
    }
    return false;
}
