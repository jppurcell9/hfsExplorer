//Node.cc

#include "Node.h"
#include "../hfsUtils.h"


Node::Node(u_int8_t* buffer, u_int16_t size) 
	: _buffer(buffer), _nodeSize(size)
{
	this->_descriptor = (BTNodeDescriptor*)this->_buffer;
	this->_startOffset = ((u_int16_t*)(this->_buffer + this->_nodeSize)) - 1;
	this->_freespaceOffset = (this->_startOffset - this->_descriptor->numRecords) - 1;

	// byteswap record pointers
	for (u_int16_t* offset = this->_startOffset; offset >= this->_freespaceOffset; offset--) {
		*offset = bswap_16(*offset);
	}
}

Node::~Node() {
	if (this->_buffer != NULL) {
		delete[] this->_buffer;
		this->_buffer = NULL;
	}
}