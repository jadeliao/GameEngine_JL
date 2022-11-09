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
//#include "Tile.h"
using namespace std;
using namespace MATH;

class Node
{
private:
    // member variables
    int label;
    Vec3 position;
    //Tile* tile;

public:
    // constructors
    Node();
    Node(int label_, Vec3 position_ = Vec3()) {
        label = label_;
        position = position_;
        //tile = nullptr;
    }
    ~Node(){}
    // useful functions
    void print();
    int getLabel() { return label; }
    Vec3 getPos() { return position; }
    //void setTile(Tile* tile_) { tile = tile_; }
    //Tile* getTile() { return tile; }
};

#endif /* NODE_H */
