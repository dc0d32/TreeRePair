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
#include "TreeRePairTypes.h"

using namespace std;

/**
 * The base class used by the HuffmanCoding and HuffmanDecoding classes.
 */
class Huffman
{
public:
	Huffman();
	virtual ~Huffman();

#ifdef DEBUG
	void printCoding();
#endif

protected:
	/**
	 * Returns the binary representation of a decimal. The least significant
	 * bit is the rightmost bit.
	 * @param a the decimal to convert to a binary number
	 * @param length Specifies the number of bits to use for the binary
	 *  representation. If length is too small, the binary representation
	 *  will be truncated.
	 * @returns the binary representation of the input decimal
	 */
	string convertUintToBinary(unsigned int a, unsigned int length);

	/**
	 * A hash table allowing constant time access to the Huffman code
	 * which is assigned to a certain integer.
	 */
	HASH_S_H * pHashedHuffmanTreeNodes;

	/**
	 * The root of the Huffman tree used for encoding and decoding,
	 * respectively.
	 */
	HuffmanTreeNode * pRootNode;
};
