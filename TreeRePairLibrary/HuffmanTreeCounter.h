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

#include "HuffmanCoding.h"
#include "TreeTraversal.h"
#include "TreeRePairTypes.h"

#include <vector>

/**
 * This class traverses a tree which shall be Huffman coded. It counts
 * the occurrences of each occurring symbol and adds it to the number
 * of occurrences counter of the specified Huffman coding.
 */
class HuffmanTreeCounter
	: TreeTraversal
{
public:
	/**
	 * The constructor of the class.
	 * @param dagGrammar an optional grammar whose nonterminals are
	 *  replaced by their right-hand sides when traversing a tree
	 * @param pHuffmanCoding the Huffman coding by which the traversed
	 *  tree shall be encoded later on
	 * @param pRoot the root of the tree to traverse
	 */
	HuffmanTreeCounter(Grammar * dagGrammar,
			HuffmanCoding * pHuffmanCoding,
			TreeNode * pRoot);
	~HuffmanTreeCounter(void);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * An optional grammar whose nonterminals are
	 * replaced by their right-hand sides when traversing a tree.
	 */
	Grammar * dagGrammar;

	/**
	 * The Huffman coding which will later be used to
	 * encode this tree.
	 */
	HuffmanCoding * pHuffmanCoding;
};
