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

#include "BitReader.h"
#include "HuffmanDecoding.h"
#include "Production.h"

using namespace std;

/**
 * This class is used to retrieve the linear SLCF tree grammar
 * from the succinct coding.
 */
class HuffmanSuccinctReader
{
public:
	/**
	 * The constructor of the class.
	 * @param pFilePath the file containing the succinct coding
	 * @param dagGrammar the grammar containing the start production
	 * @param mainGrammar the grammar containing the remaining productions
	 */
	HuffmanSuccinctReader(char * pFilePath,
			Grammar & dagGrammar, Grammar & mainGrammar);
	virtual ~HuffmanSuccinctReader();

	/**
	 * Converts a binary number into a decimal number.
	 * @param strRep the binary number to convert
	 * @returns the corresponding decimal number
	 */
	static unsigned int binaryToDec(string strRep);

private:
	/**
	 * Reconstructs the ranks of the symbols of the linear SLCF
	 * tree grammar using the specified Huffman coding.
	 */
	void readRanks(HuffmanDecoding & pHuffmanDecoding);

	/**
	 * Reconstructs the element types of the symbols of the linear SLCF
	 * tree grammar using the specified Huffman coding.
	 */
	void readNames(HuffmanDecoding & pHuffmanDecoding);

	/**
	 * Reconstructs the productions of the linear SLCF
	 * tree grammar (except the start prodcution) using the
	 * specified Huffman coding.
	 */
	void readProductions(HuffmanDecoding & pHuffmanDecoding);

	/**
	 * Reconstructs the start production of the linear SLCF
	 * tree grammar using the specified Huffman coding.
	 */
	void readStartProduction(HuffmanDecoding & pHuffmanDecoding);

	/**
	 * Reconstructs a tree (i.e., a right-hand side of the linear
	 * SLCF tree grammar) using the specified Huffman coding.
	 * @param pHuffmanDecoding the Huffman coding to use
	 * @param pNumberOfParameters indicates the number of parameter
	 *  nodes of the retrieved tree after the right-hand side has
	 *  been completely reconstructed
	 * @returns the root node of the right-hand side which was
	 *  reconstructed
	 */
	TreeNode * readTree(HuffmanDecoding & pHuffmanDecoding, unsigned int * pNumberOfParameters);

	/**
	 * The bit reader to use to read the binary representation of the
	 * linear SLCF tree grammar.
	 */
	BitReader * pBitReader;

	/**
	 * The input stream used by the bit reader.
	 */
	ifstream * pIfs;

	/**
	 * Keeps track of the number of terminals of the linear SLCF
	 * tree grammar to be reconstructed.
	 */
	unsigned int numberOfTerminals;

	/**
	 * Keeps track of the number of nonterminals (except the start
	 * nonterminal) of the grammar to be reconstructed.
	 */
	unsigned int numberOfProductionsMinusStartProd;

	/**
	 * The grammar which will serve as a container for the start
	 * production of the linear SLCF tree grammar to reconstruct.
	 */
	Grammar & dagGrammar;

	/**
	 * The grammar which will serve as a container of the productions
	 * (except the start production) of the linear SLCF tree grammar
	 * to reconstruct.
	 */
	Grammar & mainGrammar;
};
