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

#include "HuffmanTreeTraversal.h"
#include "HuffmanTreeNode.h"

/**
 * This class is used to deallocate the memory used by
 * a Huffman tree.
 */
class HuffmanTreeDeleter :
	public HuffmanTreeTraversal
{
public:
	DLLEXPORT HuffmanTreeDeleter(void);
	DLLEXPORT ~HuffmanTreeDeleter(void);

	/**
	 * Initiates the deletion of a Huffman tree.
	 */
	DLLEXPORT void deleteTree(HuffmanTreeNode * pRootPar);

protected:
	void processNode(HuffmanTreeNode * pNode, unsigned int index, int depth, bool closing);
};
