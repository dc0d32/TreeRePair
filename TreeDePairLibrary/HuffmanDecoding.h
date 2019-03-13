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

#include "Huffman.h"
#include "BitReader.h"
#include "HuffmanTreeNode.h"
#include "TreeRePairTypes.h"

#include <vector>

using namespace std;

/**
 * This class reconstructs a Huffman tree from its list of code
 * lengths which is either encoded using another Huffman coding
 * or using a fixed-length coding.
 */
class HuffmanDecoding: public Huffman
{
public:
	/**
	 * The constructor of the class.
	 * @param pBitReader specifies the BitReader instance from
	 *  which the encoded list of code lengths is to be read
	 */
	HuffmanDecoding(BitReader * pBitReader);
	virtual ~HuffmanDecoding();

	/**
	 * This method instructs the HuffmanDecoding instance to begin
	 * with decoding the list of code lengths supplied by the BitReader
	 * instance. If the enumeration of code lengths is encoded by
	 * another Huffman coding, the corresponding HuffmanDecoding
	 * instance must be provided.
	 * @param pDecoding This parameter must be not null if the
	 *  list of code lengths is encoded by another Huffman coding.
	 */
	void processCodeLengths(HuffmanDecoding * pDecoding = 0);

	/**
	 * This method reads the next bits from the BitReader instance
	 * and tries to decode it using the Huffman tree which was
	 * constructed before using the method processCodeLengths.
	 * @param lookAhead If this parameter is set to true the bits
	 *  are removed from the input stream used by the BitReader
	 *  instance, i.e., they cannot be read again. The default is
	 *  false.
	 * @returns the integer which was read from the BitReader instance
	 */
	unsigned int readNext(bool lookAhead = false);

	/**
	 * Returns the number of code lengths which is necessary to represent
	 * the Huffman tree by the enumeration of its code lengths.
	 */
	unsigned int getNumberOfCodeLengths() {
		return numberOfCodeLengths;
	};

private:
	/**
	 * This method reconstructs the Huffman codes of the Huffman coding
	 * after the code lengths have been retrieved (and decoded) from
	 * the input stream.
	 * @param codeLengths the code lengths by which the Huffman tree
	 *  is represented
	 * @returns a hash map allowing constant time access to the Huffman
	 *  code of each input symbol encoded by the Huffman coding
	 */
	HASH_I_S reconstructHuffmanCodes(vector<unsigned int> codeLengths);

	/**
	 * Reconstructs the Huffman tree after the Huffman codes have
	 * been reconstructed from the enumeration of code lengths.
	 * @param codesHashMap the hash map assigning each input symbol
	 *  its Huffman code
	 */
	void reconstructHuffmanTree(HASH_I_S codesHashMap);

	/**
	 * Decodes the code lengths supplied by the bit reader using
	 * the specified Huffman coding.
	 */
	vector<unsigned int> readHuffmanCodedCodeLengths(
			HuffmanDecoding * pDecoding);

	/**
	 * Decodes the code lengths supplied by the bit reader and assumes
	 * that they are encoded by a fixed-length coding.
	 */
	vector<unsigned int> readFixedCodedCodeLengths();

	/**
	 * The number of code lengths which is necessary to represent
	 * the Huffman tree by the enumeration of its code lengths.
	 */
	unsigned int numberOfCodeLengths;

	/**
	 * The bit reader to read the enumeration of code lengths, which
	 * is used to reconstruct the Huffman tree, from.
	 */
	BitReader * pBitReader;

	/**
	 * Indicates the maximal number of repetitions of a base code length
	 * (except the code length 0) which can be run-length encoded by
	 * the "run indicator" n + 1 (see Chap. Succinct Coding).
	 */
	unsigned int rlcMaxRepNonNullCodeLength;

	/**
	 * Indicates the maximal number of repetitions of the base code length 0
	 * which can be run-length encoded by the "run indicator" n + 2
	 * (see Chap. Succinct Coding).
	 */
	unsigned int rlcMaxRepNullCodeLengthFew;

	/**
	 * Indicates the maximal number of repetitions of the base code length 0
	 * which can be run-length encoded by the "run indicator" n + 3
	 * (see Chap. Succinct Coding).
	 */
	unsigned int rlcMaxRepNullCodeLengthMany;
};
