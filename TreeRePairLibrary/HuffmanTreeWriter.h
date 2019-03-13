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
#include "BitWriter.h"
#include "TreeTraversal.h"
#include "TreeRePairTypes.h"
#include "Grammar.h"

/**
 * This class uses a Huffman coding to write out a tree.
 */
class HuffmanTreeWriter : TreeTraversal
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar If not null, then every nonterminal of this
	 *  grammar encountered during the traversal of the specified tree
	 *  is replaced by its right-hand side (i.e., the printing process
	 *  continues on the subtree instead of printing out the nonterminal).
	 * @param pHuffmanCoding the Huffman coding to use when writing out
	 *  the specified tree
	 * @param bitWriter the bit writer to use when writing out the
	 *  Huffman codes
	 * @param pRoot the root of the tree to be printed
	 */
	HuffmanTreeWriter(Grammar * dagGrammar,
			HuffmanCoding * pHuffmanCoding, BitWriter & bitWriter,
		TreeNode * pRoot);
	~HuffmanTreeWriter(void);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth,
			bool closing);

private:
	/**
	 * If not null, then every nonterminal of this
	 * grammar encountered during the traversal of the specified tree
	 * is replaced by its right-hand side (i.e., the printing process
	 * continues on the subtree instead of printing out the nonterminal).
	 */
	Grammar * dagGrammar;

	/**
	 * The Huffman coding used when writing out the specified tree.
	 */
	HuffmanCoding * pHuffmanCoding;

	/**
	 * The bit writer used when writing out the Huffman codes.
	 */
	BitWriter & bitWriter;
};
