//
//  node.cpp
//  Demo Priority Queue
//
//  Created by Gail Harris on 2020-Oct-11.
//  Copyright © 2020 Gail Harris. All rights reserved.
//

#include <stdio.h>
#include "Node.h"

Node::Node() {
    label = 0;
}

void Node::print()
{
    printf ("%i ", label );
}
