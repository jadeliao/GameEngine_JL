//
//  node.h
//  Demo Priority Queue
//
//  Created by Gail Harris on 2020-Oct-11.
//  Copyright © 2020 Gail Harris. All rights reserved.
//

#ifndef NODE_H
#define NODE_H

#include "MMath.h"
#include "WallActor.h"
using namespace std;
using namespace MATH;

template<typename T>
using Ref = std::shared_ptr<T>;

class Node
{
private:
    // member variables
    int label;
    Vec3 position;
    Ref<WallActor> tile;

public:
    // constructors
    Node();
    Node(int label_, Vec3 position_ = Vec3()) {
        label = label_;
        position = position_;
        tile = nullptr;
    }
    ~Node(){}
    // useful functions
    void print();
    int getLabel() { return label; }
    Vec3 getPos() { return position; }
    Ref<WallActor> getTile() { return tile; }
    void setTile(Ref<WallActor> tile_) { tile = tile_; }
};

#endif /* NODE_H */
