//IndexNode.cc

#include "IndexNode.h"
#include "../hfsUtils.h"

IndexNode::IndexNode(u_int8_t* buffer, u_int16_t nodeSize)
	: Node(buffer, nodeSize)
{
	// byteswap the keylength fields on all the records
	for (u_int16_t* offset = this->_startOffset; offset > this->_freespaceOffset; offset--) {
		u_int16_t* keyLength = (u_int16_t*)(this->_buffer + *offset);
		*keyLength = bswap_16(*keyLength);
	}
}