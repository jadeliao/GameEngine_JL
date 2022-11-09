#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <queue>
#include "Node.h"

class Graph {
private:
	float manhattanHeuristic(Node* start_, Node* goal_);
public:
	Graph();
	~Graph();
	void addWeightConnection(int fromNode, int toNode, float weight);
	bool OnCreate(std::vector<Node*> nodes_);

	// List of nodes
	std::map<int, Node*> node;
	// 2D Matrix of connection costs
	std::vector<std::vector<float>> cost;

	int numNodes();
	std::vector<int> neighbours(int fromNode);
	std::vector<int> Dijkstra(int startNode, int goalNode);
	std::vector<int> AStar(int startNode, int goalNode);

	Node* getNode(int label);
};

#endif

