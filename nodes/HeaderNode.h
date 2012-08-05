//HeaderNode.h

#ifndef HEADER_NODE_H
#define HEADER_NODE_H

#include <sys/types.h>
#include <hfs/hfs_format.h>

#include "Node.h"

class HeaderNode : public Node {
public:
	HeaderNode(u_int8_t* buffer, u_int16_t size);

protected:
	BTHeaderRec* _header;
};

#endif