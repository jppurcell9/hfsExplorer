//Node.h

#ifndef NODE_H
#define NODE_H

#include <sys/types.h>
#include <hfs/hfs_format.h>

class Node {
protected:
	u_int8_t* _buffer;
	u_int16_t _nodeSize;

	BTNodeDescriptor* _descriptor;
	u_int16_t* _startOffset;
	u_int16_t* _freespaceOffset;

	Node(u_int8_t* buffer, u_int16_t size);
	virtual ~Node();
};

#endif