#include "Graph.h"
#include "NodeAndPriority.h"

using namespace std;


Graph::Graph(){

}

bool Graph::OnCreate(std::vector<Node*> nodes_)
{
	//ASSUMPTION: nodes_ vector has teh node with label "i" in the "i" position of the vector
	// 
	// Given a list of nodes initialize a matrix of costs with 0.0 weights
	int numNodes = nodes_.size();
	cost.resize(numNodes);

	for (int i = 0; i < numNodes; i++)
	{
		// Add nodes to internal 
		if (i != nodes_[i]->getLabel()) {
			return false;
		}

		node[i] = nodes_[i];

		// Set connections
		cost[i].resize(numNodes);
		for (int j = 0; j < numNodes; j++)
		{
			// Init connections
			cost[i][j] = 0.0f;
		}
	}
	return true;
}

Graph::~Graph()
{
}

int Graph::numNodes()
{
	return node.size();
	// Can also return cost.size();
}

void Graph::addWeightConnection(int fromNode, int toNode, float weight)
{
	cost[fromNode][toNode] = weight;
}

std::vector<int> Graph::neighbours(int fromNode)
{
	std::vector<int> result = {};
	for (int j = 0; j < numNodes(); j++)
	{
		if (cost[fromNode][j] > 0.0f)
		{
			result.push_back(j);
		}
	}
	return result;
}

std::vector<int> Graph::Dijkstra(int startNode, int goalNode) {
	//declare helper variables
	float new_cost;
	int current = startNode;
	//declare current NodeAndPriority
	NodeAndPriority* currentNodeAndPriority;
	currentNodeAndPriority = new NodeAndPriority(node[startNode], 0.0f);
	//set up priority queue for frontier of nodes

	std::priority_queue<NodeAndPriority, deque<NodeAndPriority>, ComparePriority> frontier;
	frontier.push(*currentNodeAndPriority);

	//track solution path
	vector<int> came_from;
	came_from.resize(numNodes());

	//store cost
	map<int, float> cost_so_far;
	cost_so_far[startNode] = 0.0f;

	//TODO implement the algorithm
	//start looping through frontier
	while (!frontier.empty()) {
		//get the node from the top of  the frontier and pop it off
		*currentNodeAndPriority = frontier.top();
		frontier.pop();
		//if its the goal, then break out of the loop
		if (currentNodeAndPriority->node == node[goalNode]) {
			break;
		}
		//for the neibours of current node
		vector<int> neighbourList = neighbours(current);
		for (int next : neighbourList) {
			//calculate new cost
			float new_cost = cost_so_far[current] + cost[current][next];
			//if neighbour is not in cost_so_far or new cost is lower
			if (cost_so_far.find(next) != cost_so_far.end() || new_cost < cost_so_far[next]) {
				cost_so_far[next] = new_cost;
				currentNodeAndPriority->priority = new_cost;
				frontier.push(*currentNodeAndPriority);
				came_from[next] = current;
			}	
		}
	}
	return came_from;
	
}

std::vector<int> Graph::AStar(int startNode, int goalNode)
{
	//declare helper variables
	float new_cost;
	int current = startNode;
	//declare current NodeAndPriority
	NodeAndPriority* currentNodeAndPriority;
	currentNodeAndPriority = new NodeAndPriority(node[startNode], 0.0f);
	
	//set up priority queue for frontier of nodes
	std::priority_queue<NodeAndPriority, deque<NodeAndPriority>, ComparePriority> frontier;
	frontier.push(*currentNodeAndPriority);
	//track solution path
	vector<int> came_from;
	came_from.resize(numNodes());
	//store cost
	map<int, float> cost_so_far;
	cost_so_far[startNode] = 0.0f;

	//Start looping through frontier
	while (!frontier.empty()) {
		//Get the node from the top of  the frontier and pop it off
		*currentNodeAndPriority = frontier.top();
		frontier.pop();
		//If reaches the goal, then break out of the loop
		if (currentNodeAndPriority->node == node[goalNode]) {
			break;
		}
		//Loop through the neibours of current node
		vector<int> neighbourList = neighbours(current);
		for (int next : neighbourList) {
			//Calculate new cost
			float new_cost = cost_so_far[current] + cost[current][next];
			if (cost_so_far.find(next) != cost_so_far.end() || new_cost < cost_so_far[next]) {
				cost_so_far[next] = new_cost;
				//Use Manhattan Heuristic for 4 direction movement
				currentNodeAndPriority->priority = new_cost + manhattanHeuristic(node[next], node[goalNode]);
				frontier.push(*currentNodeAndPriority);
				came_from[next] = current;
			}
		}
	}
	return came_from;

}

float Graph::manhattanHeuristic(Node* start_, Node* goal_) {
	float D = 1.0f; //test for this value, use 1 for now
	float dx = abs(start_->getPos().x - goal_->getPos().x);
	float dy = abs(start_->getPos().y - goal_->getPos().y);
	return D * (dx + dy);
}
Node* Graph::getNode(int label)
{
	return node[label];
}
