//
//  A_Star_Node_Priority.h
//  Demo Priority Queue
//
//  Created by Gail Harris on 2020-Oct-11.
//  Copyright © 2020 Gail Harris. All rights reserved.
//

#ifndef NodeAndPriority_h
#define NodeAndPriority_h

#include "Node.h"

class NodeAndPriority
{
public:
    
    // member variables
    Node *node;
    float priority;
    
    // constructors
    NodeAndPriority( Node *node_, float priority);
    
    // useful functions
    void print();
};


struct ComparePriority
{
    bool operator()(NodeAndPriority const& lhs, NodeAndPriority const& rhs)
    {
        // make it a min queue
        return lhs.priority > rhs.priority;
    }
};

#endif /* A_Star_Node_Priority_h */
