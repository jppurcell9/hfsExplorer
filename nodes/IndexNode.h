//IndexNode.h

#ifndef INDEX_NODE_H
#define INDEX_NODE_H

#include <sys/types.h>
#include <hfs/hfs_format.h>

#include "Node.h"

class IndexNode : public Node {
public:
	IndexNode(u_int8_t* buffer, u_int16_t size);
};

#endif