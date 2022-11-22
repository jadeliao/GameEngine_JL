#include "Path.h"
#include "WallActor.h"

Path::Path():currentIndex(0) {}

Path::Path(vector<Ref<Node>> pathList_): pathList(pathList_), currentIndex(0) {

}

Path::~Path(){}


Ref<Node> Path::getCurrentNode(){

    if (!pathList.empty()) return pathList[currentIndex];

    return nullptr;
}


void Path::incrementCurrentNode(int increment_){

    currentIndex += increment_;
    if (currentIndex < 0) {
        currentIndex = 0;
    }
    if (currentIndex >= pathList.size()) {
        currentIndex = pathList.size() - 1;
    }
}

bool Path::isLastNode(){

    if (currentIndex == pathList.size() - 1) {
        return true;
    }

    return false;
}
