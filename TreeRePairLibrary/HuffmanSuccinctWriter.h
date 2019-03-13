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

#include "GrammarWriter.h"
#include "HuffmanCoding.h"

/**
 * This class implements our succinct coding for linear SLCF
 * tree grammars (see Chap. Succinct Coding). It uses techniques
 * based on the DEFLATE Compressed Data Format Specification
 * (see http://www.ietf.org/rfc/rfc1951.txt).
 */
class HuffmanSuccinctWriter: public GrammarWriter
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar containing the start production
	 *  of the linear SLCF tree grammar for which the succinct coding
	 *  shall be generated
	 * @param mainGrammar the grammar containing the remaining
	 *  productions
	 * @param pFilePath the output file to use for our succinct coding
	 */
	HuffmanSuccinctWriter(Grammar & dagGrammar,
			Grammar & mainGrammar, char * pFilePath);

	virtual ~HuffmanSuccinctWriter(void);

protected:
	virtual void writeProduction(Production * pProduction);

private:
	/**
	 * Converts the specified integer to a bit string of the
	 * specified length.
	 * @param n the integer to convert
	 * @param numberOfBits Specifies the number of bits to use. If not
	 *  sufficient an error message is produced and the application
	 *  exits.
	 * @returns the binary representation of the input value
	 */
	static string decToBinary(const unsigned int n,
			unsigned int numberOfBits);

	/**
	 * Computes the number of bits needed to represent the specified
	 * integer as a binary number.
	 */
	static unsigned int calculateNeededBits(unsigned int n);

	/**
	 * Makes sure that all run indicators (see Chap. Succinct Coding)
	 * are considered by the super Huffman coding.
	 */
	void addValuesForRunlengthCoding(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Writes out the number of terminals of the generated linear SLCF
	 * tree grammar using the specified Huffman coding.
	 */
	void writeNumberOfTerminals(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Writes out the number of productions (minus the start production)
	 * of the generated linear SLCF tree grammar.
	 */
	void writeNumberOfProductions(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Counts the number of terminal symbols of the generated linear SLCF
	 * tree grammar with the specified chilren characteristic.
	 */
	unsigned int countSymbols(bool leftChild, bool rightChild);

	/**
	 * Counts the number of occurrences of the symbols labeling the nodes
	 * of the right-hand side of the start production.
	 * @param pBaseHuffmanCoding the Huffman coding which will later be used
	 *  to encode the start production
	 */
	void countSymbolsInStartProduction(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Counts the number of occurrences of the symbols labeling the nodes
	 * of the right-hand side of the productions (except the start
	 * production) of the generated linear SLCF tree grammar.
	 * @param pBaseHuffmanCoding the base Huffman coding which will later be used
	 *  to encode the productions (except the start production)
	 */
	void countSymbolsInProductions(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Counts the number of occurrences of the symbols to be written
	 * out to make the ranks of the terminal symbols available to the
	 * decompressor.
	 * @param pBaseHuffmanCoding the base Huffman coding which will later be used
	 *  to encode the ranks information
	 */
	void countSymbolsInRankInfo(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Counts the number of occurrences of the symbols to be written
	 * out to make the element types of the terminal symbols available
	 * to the decompressor.
	 * @param pBaseHuffmanCoding the base Huffman coding which will later be used
	 *  to encode the element types
	 */
	void countSymbolsInNameInfo(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Counts the values which need to be written out when serializing
	 * the specified base Huffman coding by writing out an enumeration
	 * of its code lengths.
	 */
	void countBaseHuffmanTree(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Counts the specified number of occurrences of the specified
	 * base code length of a base Huffman coding which will later
	 * be encoded by the super Huffman coding.
	 */
	void countBaseCodeLength(unsigned int codeLength,
			unsigned int occurrences);

	/**
	 * Writes the enumeration of code lengths of the specified base
	 * Huffman coding to the output file using the super Huffman
	 * coding and a run-length coding.
	 */
	void writeBaseHuffmanTree(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Writes a base code length of a base Huffman coding to the output
	 * file using the super Huffman coding.
	 */
	void writeBaseCodeLength(unsigned int codeLength,
			unsigned int occurrences);

	/**
	 * Writes the enumeration of code lengths of the super Huffman
	 * coding to the output file using a fixed-length coding.
	 */
	void writeSuperHuffmanTree();

	/**
	 * Writes the productions (except the start production) of the
	 * generated linear SLCF tree grammar to the output file using
	 * the specified base Huffman coding).
	 */
	void writeProductions(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Writes the start production of the generated linear SLCF tree
	 * grammar to the output file using the specified base Huffman
	 * coding.
	 */
	void writeStartProduction(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Writes the ranks of the terminal symbols to the output file
	 * using the specified base Huffman coding.
	 */
	void writeRankInfo(HuffmanCoding * pBaseHuffmanCoding);

	/**
	 * Enumerates all terminal symbols with the specified children
	 * characteristic using the specified base Huffman coding. The
	 * enumeration is written to the output file.
	 */
	void writeRankInfoAux(HuffmanCoding * pBaseHuffmanCoding, bool leftChild,
			bool rightChild);

	/**
	 * Writes the element types of the terminal symbols to the output
	 * file using the specified base Huffman coding.
	 */
	void writeNameInfo(HuffmanCoding * pHuffmanCoding);

	/**
	 * Computes the maximal base code length to be encoded by the super
	 * Huffman coding.
	 */
	void computeMaximalBaseCodeLength();

	/**
	 * Computes the number of bits needed to encode the super code
	 * lengths of the super Huffman coding by a fixed-length coding.
	 */
	unsigned int computeBitsPerSuperCodeLength();

	/**
	 * Writes out the specified super code length using the specified
	 * number of bits (i.e., we use a fixed-length coding).
	 */
	void writeSuperCodeLength(const int n,
			unsigned int bitsPerCodeLength);

	/**
	 * The base Huffman coding used to encode the start production of
	 * the generate linear SLCF tree grammar.
	 */
	HuffmanCoding * pHuffmanCodingStartProduction;

	/**
	 * The base Huffman coding used to encode the productions (except
	 * the start production), the children characteristics of the
	 * terminal symbols and the numbers of terminals and nonterminals
	 * of the generated linear SLCF tree grammar.
	 */
	HuffmanCoding * pHuffmanCodingProductions;

	/**
	 * The base Huffman coding used to encode the element types of the
	 * terminal symbols.
	 */
	HuffmanCoding * pHuffmanCodingNames;

	/**
	 * The super Huffman coding used to encode the enumeration of base
	 * code lengths.
	 */
	HuffmanCoding * pSuperHuffmanCoding;

	/**
	 * The maximal base code length to be encoded by the super Huffman
	 * coding.
	 */
	unsigned int maxBaseCodeLength;

	/**
	 * Keeps track of the highest run indicator used (see Chap.
	 * Succinct Coding for more information).
	 */
	unsigned int highestRunIndicator;

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

	/**
	 * Indicates whether the values to be written to the output file
	 * shall be left unencoded (for debugging purposes only).
	 */
	bool unencoded;

	/**
	 * The bit writer which is used to write to the output file.
	 */
	BitWriter bitWriter;
};
