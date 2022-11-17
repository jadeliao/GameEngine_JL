//
//  A_Star_Node_Priority.cpp
//  Demo Priority Queue
//
//  Created by Gail Harris on 2020-Oct-11.
//  Copyright © 2020 Gail Harris. All rights reserved.
//

#include <stdio.h>

#include "NodeAndPriority.h"

NodeAndPriority::NodeAndPriority(Ref<Node> node_, float priority_ )
{
    node = node_;
    priority = priority_;
}

void NodeAndPriority::print()
{
    printf( "Node: %i, priority: %.2f \n", node->getLabel(), priority);
}
