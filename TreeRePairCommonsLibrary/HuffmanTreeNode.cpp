/*
 * TreeRePair is an implementation of the Re-pair for Trees algorithm.
 *
 * Copyright (C) 2010  Roy Mennicke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License Version 3 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not,
 * see <http://www.gnu.org/licenses/gpl-3.0.txt>.
 */
#include "HuffmanTreeNode.h"

HuffmanTreeNode::HuffmanTreeNode(void)
: frequency(0), index(-1), symbol(0), bitRepresentation(""), pLeftChild(0), pRightChild(0), pParent(0)
{
}

HuffmanTreeNode::~HuffmanTreeNode(void)
{
}

HuffmanTreeNode * HuffmanTreeNode::getChild(unsigned int index) {

	if (index == 0) {
		return pLeftChild;
	} else if (index == 1) {
		return pRightChild;
	} else {
		return 0;
	}
}

void HuffmanTreeNode::setChild(unsigned int index, HuffmanTreeNode * pChild) {

	if (index == 0) {
		pLeftChild = pChild;
	} else if (index == 1) {
		pRightChild = pChild;
	} else {
		// do nothing
	}
}
