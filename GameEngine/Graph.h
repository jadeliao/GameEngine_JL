#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <queue>
#include "Node.h"

class Graph {
private:
	float manhattanHeuristic(Ref<Node> start_, Ref<Node> goal_);
public:
	Graph();
	~Graph();
	void addWeightConnection(int fromNode, int toNode, float weight);
	bool OnCreate(std::vector<Ref<Node>> nodes_);

	// List of nodes
	std::map<int, Ref<Node>> node;
	// 2D Matrix of connection costs
	std::vector<std::vector<float>> cost;

	int numNodes();
	std::vector<int> neighbours(int fromNode);
	std::vector<int> Dijkstra(int startNode, int goalNode);
	std::vector<int> AStar(int startNode, int goalNode);

	Ref<Node> getNode(int label);
};

#endif

