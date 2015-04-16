#include "OsdNode.h"
using namespace std;

OsdNode::OsdNode(string s, Effect e)
{
	text = s;
	effect = e;
}

OsdNode::OsdNode(string s)
{
	text = s;
	effect = NONE;
}

OsdNode::~OsdNode(void)
{
}

OsdNode* OsdNode::addChild(OsdNode* n) {
	children.push_back(n);
	n->parent = this;
	return n;
}