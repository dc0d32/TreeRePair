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
#pragma once

#include "HuffmanTreeNode.h"
#include "TreeRePairMacros.h"

/**
 * This class is used to traverse a Huffman tree in preorder.
 */
class HuffmanTreeTraversal
{
public:
	/**
	 * The constructor.
	 * @param pRoot the root of the Huffman tree to traverse
	 */
	HuffmanTreeTraversal(HuffmanTreeNode * pRoot);
	virtual ~HuffmanTreeTraversal(void);

protected:
	/**
	 * Initiates the traversal.
	 */
	void traverseTree(void);

	/**
	 * This method is called twice for each node of the tree during a
	 * preorder traversal of it.
	 * @param pNode the current node
	 * @param index the index of the current node
	 * @param depth the depth of the current node
	 * @param closing indicates if the current node is visited on the
	 *  way down to the leafs of the tree (i.e., it is visited for the first
	 *  time) or if it is visited after its children have been visited
	 *  (thats the second and last time it is visited)
	 */
	virtual void processNode(HuffmanTreeNode * pNode, unsigned int index, int depth, bool closing) = 0;

	/**
	 * The root of the Huffman tree to traverse.
	 */
	HuffmanTreeNode * pRoot;
};
