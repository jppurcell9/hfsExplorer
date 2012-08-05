//HeaderNode.cc

#include "HeaderNode.h"
#include "../hfsUtils.h"

HeaderNode::HeaderNode(u_int8_t* buffer, u_int16_t nodeSize)
	: Node(buffer, nodeSize)
{
	this->_header = (BTHeaderRec*)(this->_buffer + *(this->_startOffset));
	byteSwapHeaderRecord(this->_header);

	// skip user data and map records for now
}