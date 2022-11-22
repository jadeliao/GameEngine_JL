#ifndef PATH_H
#define PATH_H

#include "Node.h"
#include "Vector.h"
#include "VMath.h"
#include <vector>

using namespace std;

template<typename T>
using Ref = std::shared_ptr<T>;

class Path{
private:
	vector<Ref<Node>> pathList;
	int currentIndex;

public:
	Path();
	Path(vector<Ref<Node>> pathList);
	~Path();
	Ref<Node> getCurrentNode();
	void incrementCurrentNode(int increment_);
	bool isLastNode();
};

#endif
