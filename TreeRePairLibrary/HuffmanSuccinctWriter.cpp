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
#include "HuffmanSuccinctWriter.h"

#include "TreeRePairConstants.h"
#include "Helper.h"
#include "HuffmanCoding.h"
#include "HuffmanTreeCounter.h"
#include "HuffmanTreeWriter.h"
#include "ReferencesTreeCounter.h"
#include "TreeCopier.h"
#include "Production.h"
#include "Grammar.h"

#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

HuffmanSuccinctWriter::HuffmanSuccinctWriter(Grammar & dagGrammarPar,
		Grammar & mainGrammarPar, char * pFilePath) :
	GrammarWriter(dagGrammarPar, mainGrammarPar, pFilePath),
			pHuffmanCodingStartProduction(0), pHuffmanCodingProductions(0),
			pSuperHuffmanCoding(0), maxBaseCodeLength(0),
			highestRunIndicator(0),
			unencoded(false),
			bitWriter(fout, unencoded)
{
	string filename = Helper::getFilePathForBinary(pFilePath);
	if (unencoded)
	{
		fout.open(filename.c_str());
	}
	else
	{
		fout.open(filename.c_str(), ios_base::binary);
	}

	// maximal repetitions of each run type of the run length coding
	rlcMaxRepNonNullCodeLength = (unsigned int) pow(2.0, RLC_NON_NULL_CODE_LENGTH_BITS) + RLC_MAX_RUN_LENGTH;
	rlcMaxRepNullCodeLengthFew = (unsigned int) pow(2.0, RLC_NULL_CODE_LENGTH_FEW_BITS) + RLC_MAX_RUN_LENGTH;
	rlcMaxRepNullCodeLengthMany = (unsigned int) pow(2.0, RLC_NULL_CODE_LENGTH_MANY_BITS) + rlcMaxRepNullCodeLengthFew;

//	cout << rlcMaxRepNonNullCodeLength << endl;
//	cout << rlcMaxRepNullCodeLengthFew << endl;
//	cout << rlcMaxRepNullCodeLengthMany << endl;

	unsigned int name = 1;
	for (unsigned int i = 1; i < Symbol::counter; i++) {
		if (Symbol::isTerminalSymbol(i)) {
			Symbol * pSymbol = Symbol::getSymbolById(i);

			/*Symbol::print(cout, pSymbol);
			cout << " -> " << name << endl;*/

			pSymbol->setName(name++);
		}
	}

	Symbol::getParameterSymbol()->setName(name++);

//	Symbol::getPlaceholderSymbol()->setName(name++);

	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
		Symbol * pSymbol = mainGrammar[i]->getLeftHandSide();

		/*Symbol::print(cout, pSymbol);
		cout << " -> " << name << endl;*/

		pSymbol->setName(name++);
	}

//	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
//		Rule::print(cout, mainGrammar[i]);
//	}

	pHuffmanCodingStartProduction = new HuffmanCoding(bitWriter, fout, unencoded);
	pHuffmanCodingProductions = new HuffmanCoding(bitWriter, fout, unencoded);
	pHuffmanCodingNames = new HuffmanCoding(bitWriter, fout, unencoded);
	pSuperHuffmanCoding = new HuffmanCoding(bitWriter, fout, unencoded);

	pHuffmanCodingProductions->countSymbol(mainGrammar.size());
	pHuffmanCodingProductions->countSymbol(Symbol::getNumberOfTerminals());
	countSymbolsInStartProduction(pHuffmanCodingStartProduction);
	countSymbolsInProductions(pHuffmanCodingProductions);
	countSymbolsInRankInfo(pHuffmanCodingProductions);
	countSymbolsInNameInfo(pHuffmanCodingNames);

	pHuffmanCodingStartProduction->computeHuffmanTree();
	pHuffmanCodingProductions->computeHuffmanTree();
	pHuffmanCodingNames->computeHuffmanTree();

	pHuffmanCodingStartProduction->reorganizeHuffmanCodes();
	pHuffmanCodingProductions->reorganizeHuffmanCodes();
	pHuffmanCodingNames->reorganizeHuffmanCodes();

	computeMaximalBaseCodeLength();

	countBaseHuffmanTree(pHuffmanCodingProductions);
	countBaseHuffmanTree(pHuffmanCodingStartProduction);
	countBaseHuffmanTree(pHuffmanCodingNames);
	addValuesForRunlengthCoding(pSuperHuffmanCoding);

	pSuperHuffmanCoding->computeHuffmanTree();
	pSuperHuffmanCoding->reorganizeHuffmanCodes();
	writeSuperHuffmanTree();

//	cout << "base Huffman coding for start production: " << endl;
//	pHuffmanCodingStartProduction->printCoding();
//	cout << "base Huffman coding for productions, numbers, children characteristics: " << endl;
//	pHuffmanCodingProductions->printCoding();
//	cout << "base Huffman coding for terminal names: " << endl;
//	pHuffmanCodingNames->printCoding();
//	cout << "super Huffman coding: " << endl;
//	pSuperHuffmanCoding->printCoding();

//	cout << "base Huffman coding for start production: " << endl;
	writeBaseHuffmanTree(pHuffmanCodingStartProduction);
//	cout << "base Huffman coding for productions, numbers, children characteristics: " << endl;
	writeBaseHuffmanTree(pHuffmanCodingProductions);
//	cout << "base Huffman coding for terminal names: " << endl;
	writeBaseHuffmanTree(pHuffmanCodingNames);

	writeNumberOfTerminals(pHuffmanCodingProductions);
	writeNumberOfProductions(pHuffmanCodingProductions);
	writeRankInfo(pHuffmanCodingProductions);
	writeNameInfo(pHuffmanCodingNames);
	writeProductions(pHuffmanCodingProductions);

	writeStartProduction(pHuffmanCodingStartProduction);

	pHuffmanCodingStartProduction->writeHuffmanCode(0, false);

	bitWriter.flushBuffer();
	fout.close();
}

HuffmanSuccinctWriter::~HuffmanSuccinctWriter(void)
{
	delete pHuffmanCodingProductions;
	delete pHuffmanCodingStartProduction;
	delete pHuffmanCodingNames;
	delete pSuperHuffmanCoding;
}

void HuffmanSuccinctWriter::addValuesForRunlengthCoding(HuffmanCoding * pHuffmanCoding) {
	for (unsigned int i = highestRunIndicator; i < 3; i++) {
		pHuffmanCoding->countSymbol(pHuffmanCoding->getBiggestDecimal() + 1);
	}
}

void HuffmanSuccinctWriter::writeNumberOfTerminals(HuffmanCoding * pHuffmanCoding)
{

	if (unencoded)
	{
		fout << "[" << Symbol::getNumberOfTerminals() << "]" << endl;
	}
	else
	{
		/*bitWriter.writeBitString(decToBinary(Symbol::getNumberOfTerminals(),
				CODE_LENGTH_BITS));*/
		pHuffmanCoding->writeHuffmanCode(Symbol::getNumberOfTerminals(), false);
	}
}

void HuffmanSuccinctWriter::writeNumberOfProductions(HuffmanCoding * pHuffmanCoding)
{

	if (unencoded)
	{
		fout << "[" << mainGrammar.size() << "]" << endl;
	}
	else
	{
		pHuffmanCoding->writeHuffmanCode(mainGrammar.size(), false);
	}
}

void HuffmanSuccinctWriter::computeMaximalBaseCodeLength()
{

	unsigned int codeLengthProductions =
			pHuffmanCodingProductions->getMaximumCodeLength();
	unsigned int codeLengthStartProduction = pHuffmanCodingStartProduction->getMaximumCodeLength();
	unsigned int codeLengthNames = pHuffmanCodingNames->getMaximumCodeLength();

	maxBaseCodeLength = max(codeLengthProductions, codeLengthStartProduction);
	maxBaseCodeLength = max(maxBaseCodeLength, codeLengthNames);
}

void HuffmanSuccinctWriter::countBaseHuffmanTree(HuffmanCoding * pHuffmanCoding)
{
	HASH_S_H * pHashedHuffmanTreeNodes = pHuffmanCoding->getHuffmanTreeNodes();
	unsigned int biggestDecimal = pHuffmanCoding->getBiggestDecimal();
	unsigned int lastCodeLength = UINT_MAX;
	unsigned int occurrences = 0;

	for (unsigned int symbol = 0; symbol <= biggestDecimal; symbol++)
	{

		HASH_S_H::iterator it = pHashedHuffmanTreeNodes->find(symbol);
		unsigned int codeLength = 0;
		if (it != pHashedHuffmanTreeNodes->end())
		{
			HuffmanTreeNode * pHuffmanTreeNode = it->second;
			codeLength = pHuffmanTreeNode->getBitRepresentation().length();
		}
		else
		{
			// do nothing
		}

		if (lastCodeLength == codeLength)
		{
			occurrences++;
		}
		else
		{
			if (lastCodeLength != UINT_MAX)
			{
				countBaseCodeLength(lastCodeLength, occurrences);
			}
			lastCodeLength = codeLength;
			occurrences = 1;
		}

	}

	countBaseCodeLength(lastCodeLength, occurrences);

}

void HuffmanSuccinctWriter::countBaseCodeLength(
		unsigned int codeLength, unsigned int occurrences)
{

	if (occurrences <= RLC_MAX_RUN_LENGTH)
	{
		pSuperHuffmanCoding->countSymbol(codeLength, occurrences);
	}
	else
	{ // occurrences > RUN_LENGTH_MAX_OCC
		if (codeLength > 0)
		{
			pSuperHuffmanCoding->countSymbol(codeLength, 1);

			// occurrences--;

			unsigned int result = occurrences / rlcMaxRepNonNullCodeLength;
			pSuperHuffmanCoding->countSymbol(maxBaseCodeLength + 1, result);

			result = occurrences % rlcMaxRepNonNullCodeLength;
			if (result > RLC_MAX_RUN_LENGTH)
			{
				pSuperHuffmanCoding->countSymbol(maxBaseCodeLength + 1, 1);
			}
			else if (result > 0)
			{
				pSuperHuffmanCoding->countSymbol(codeLength, result);
			}

			if (highestRunIndicator < 1)
			{
				highestRunIndicator = 1;
			}
		}
		else
		{ // codeLength == 0

			unsigned int result = occurrences / rlcMaxRepNullCodeLengthMany;
			pSuperHuffmanCoding->countSymbol(maxBaseCodeLength + 3, result);
			highestRunIndicator = 3;

			result = occurrences % rlcMaxRepNullCodeLengthMany;
			if (result > rlcMaxRepNullCodeLengthFew)
			{
				pSuperHuffmanCoding->countSymbol(maxBaseCodeLength + 3, 1);
				highestRunIndicator = 3;
			}
			else if (result > RLC_MAX_RUN_LENGTH)
			{
				pSuperHuffmanCoding->countSymbol(maxBaseCodeLength + 2, 1);
				if (highestRunIndicator < 2)
				{
					highestRunIndicator = 2;
				}
			}
			else if (result > 0)
			{
				pSuperHuffmanCoding->countSymbol(codeLength, result);
			}
		}
	}

}

void HuffmanSuccinctWriter::writeBaseHuffmanTree(HuffmanCoding * pHuffmanCoding)
{
	if (unencoded)
	{
		fout << endl;
	}

	HASH_S_H * pHashedHuffmanTreeNodes = pHuffmanCoding->getHuffmanTreeNodes();
	unsigned int biggestDecimal = pHuffmanCoding->getBiggestDecimal();
	unsigned int lastCodeLength = UINT_MAX;
	unsigned int occurrences = 0;

//	cout << "number of code lengths: " << biggestDecimal + 1 << endl;

	// number of code lengths
	if (unencoded)
	{
		fout << "[" << biggestDecimal + 1 << "]" << endl;
	}
	else
	{
		bitWriter.writeBitString(decToBinary(biggestDecimal + 1,
				CODE_LENGTH_BITS));
	}

	// code lengths
	for (unsigned int symbol = 0; symbol <= biggestDecimal; symbol++)
	{

		HASH_S_H::iterator it = pHashedHuffmanTreeNodes->find(symbol);
		unsigned int codeLength = 0;
		if (it != pHashedHuffmanTreeNodes->end())
		{
			HuffmanTreeNode * pHuffmanTreeNode = it->second;
			codeLength = pHuffmanTreeNode->getBitRepresentation().length();

			// cout << symbol << " : " << pHuffmanTreeNode->getBitRepresentation() << endl;
		}
		else
		{
			// do nothing
		}

		if (lastCodeLength == codeLength)
		{
			occurrences++;
		}
		else
		{
			if (lastCodeLength != UINT_MAX)
			{
				writeBaseCodeLength(lastCodeLength, occurrences);
			}
			lastCodeLength = codeLength;
			occurrences = 1;
		}

	}

	writeBaseCodeLength(lastCodeLength, occurrences);

	if (unencoded)
	{
		fout << endl << endl;
	}
}

void HuffmanSuccinctWriter::writeBaseCodeLength(
		unsigned int codeLength, unsigned int occurrences)
{

	// cout << "code length: " << codeLength << "; occurrences: " << occurrences << endl;

	if (occurrences <= RLC_MAX_RUN_LENGTH)
	{
		for (unsigned int i = 0; i < occurrences; i++)
		{
			pSuperHuffmanCoding->writeHuffmanCode(codeLength,
					false);
			// cout << "runlength 0 (" << codeLength << ")" << endl;
		}
	}
	else
	{ // occurrences > RUN_LENGTH_MAX_OCC
		if (codeLength > 0)
		{
			pSuperHuffmanCoding->writeHuffmanCode(codeLength,
					false);

			// occurrences--;

			unsigned int result = occurrences / rlcMaxRepNonNullCodeLength;
			while (result > 0)
			{
				pSuperHuffmanCoding->writeHuffmanCode(
						maxBaseCodeLength + 1, false);
				bitWriter.writeBitString(decToBinary(rlcMaxRepNonNullCodeLength
						- RLC_MAX_RUN_LENGTH - 1, RLC_NON_NULL_CODE_LENGTH_BITS));
				result--;

				// cout << "runlength 1: " << runLength1Divisor << endl;
			}

			result = occurrences % rlcMaxRepNonNullCodeLength;
			if (result > RLC_MAX_RUN_LENGTH)
			{
				pSuperHuffmanCoding->writeHuffmanCode(
						maxBaseCodeLength + 1, false);
				bitWriter.writeBitString(decToBinary(result
						- RLC_MAX_RUN_LENGTH - 1, RLC_NON_NULL_CODE_LENGTH_BITS));

				// cout << "runlength 1: " << result << endl;
			}
			else if (result > 0)
			{
				writeBaseCodeLength(codeLength, result);
			}
		}
		else
		{ // codeLength == 0

			unsigned int result = occurrences / rlcMaxRepNullCodeLengthMany;
			while (result > 0)
			{
				pSuperHuffmanCoding->writeHuffmanCode(
						maxBaseCodeLength + 3, false);
				bitWriter.writeBitString(decToBinary(rlcMaxRepNullCodeLengthMany
						- rlcMaxRepNullCodeLengthFew - 1, RLC_NULL_CODE_LENGTH_MANY_BITS));
				result--;

				// cout << "runlength 3: " << runLength3Val << endl;
			}

			result = occurrences % rlcMaxRepNullCodeLengthMany;
			if (result > rlcMaxRepNullCodeLengthFew)
			{
				pSuperHuffmanCoding->writeHuffmanCode(
						maxBaseCodeLength + 3, false);
				bitWriter.writeBitString(decToBinary(result - rlcMaxRepNullCodeLengthFew
						- 1, RLC_NULL_CODE_LENGTH_MANY_BITS));

				// cout << "runlength 3: " << result << endl;
			}
			else if (result > RLC_MAX_RUN_LENGTH)
			{
				pSuperHuffmanCoding->writeHuffmanCode(
						maxBaseCodeLength + 2, false);
				bitWriter.writeBitString(decToBinary(result
						- RLC_MAX_RUN_LENGTH - 1, RLC_NULL_CODE_LENGTH_FEW_BITS));

				// cout << "runlength 2: " << result << endl;
			}
			else if (result > 0)
			{
				writeBaseCodeLength(codeLength, result);
			}
		}
	}

}

void HuffmanSuccinctWriter::writeSuperHuffmanTree()
{
	unsigned int bitsPerCodeLength = computeBitsPerSuperCodeLength();
//	cout << "bits per code length: " << bitsPerCodeLength << endl;

	if (calculateNeededBits(bitsPerCodeLength) > CODE_LENGTH_BITS)
	{
		cout
				<< "Fatal error: Can't encode number of bits used to encode code lengths."
					"Exiting ..." << endl;
		exit(1);
	}

	if (unencoded)
	{
		fout << endl;
	}

	// bits per code length
	if (unencoded)
	{
		fout << "[" << bitsPerCodeLength << "]" << endl << endl;
	}
	else
	{
		bitWriter.writeBitString(decToBinary(bitsPerCodeLength,
				CODE_LENGTH_BITS));
	}

	HASH_S_H * pHashedHuffmanTreeNodes =
			pSuperHuffmanCoding->getHuffmanTreeNodes();
	unsigned int biggestDecimal =
			pSuperHuffmanCoding->getBiggestDecimal();

	// number of code lengths
	if (unencoded)
	{
		fout << "[" << biggestDecimal + 1 << "]" << endl;
	}
	else
	{
		bitWriter.writeBitString(decToBinary(biggestDecimal + 1,
				CODE_LENGTH_BITS));
	}

//	cout << "number of code lengths: " << biggestDecimal + 1 << endl;

	// code lengths
	for (unsigned int symbol = 0; symbol <= biggestDecimal; symbol++)
	{

		HASH_S_H::iterator it = pHashedHuffmanTreeNodes->find(symbol);
		int codeLength = 0;
		if (it != pHashedHuffmanTreeNodes->end())
		{
			HuffmanTreeNode * pHuffmanTreeNode = it->second;
			codeLength = pHuffmanTreeNode->getBitRepresentation().length();

			// cout << symbol << " : " << pHuffmanTreeNode->getBitRepresentation() << endl;
		}
		else
		{
			// do nothing
		}

		writeSuperCodeLength(codeLength, bitsPerCodeLength);

	}

	if (unencoded)
	{
		fout << endl;
	}
}

void HuffmanSuccinctWriter::writeSuperCodeLength(const int n,
		unsigned int bitsPerCodeLength)
{
	if (unencoded)
	{
		fout << "[" << n << "]";
	}
	else
	{
		bitWriter.writeBitString(decToBinary(n, bitsPerCodeLength));
	}
}

unsigned int HuffmanSuccinctWriter::computeBitsPerSuperCodeLength()
{
	HASH_S_H * pHashedHuffmanTreeNodes = pSuperHuffmanCoding->getHuffmanTreeNodes();

	unsigned int maxCodeLength = 0;

	HASH_S_H::iterator it;
	for (it = pHashedHuffmanTreeNodes->begin(); it
			!= pHashedHuffmanTreeNodes->end(); ++it)
	{
		HuffmanTreeNode * pHuffmanTreeNode = it->second;
		unsigned int currentCodeLength =
				pHuffmanTreeNode->getBitRepresentation().length();
		if (currentCodeLength > maxCodeLength)
		{
			maxCodeLength = currentCodeLength;
		}
	}

	// + 1 since ceil(log2(16)) is 4 but needs 5 bits
	return calculateNeededBits(maxCodeLength);
}

void HuffmanSuccinctWriter::countSymbolsInStartProduction(HuffmanCoding * pHuffmanCoding)
{
	Production * pProduction = dagGrammar[0];
	HuffmanTreeCounter treeCounter(&dagGrammar, pHuffmanCoding, pProduction->getRightHandSide());

	pHuffmanCoding->countSymbol(0);
}

void HuffmanSuccinctWriter::countSymbolsInProductions(
		HuffmanCoding * pHuffmanCoding)
{
	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
		Production * pProduction = mainGrammar[i];
		HuffmanTreeCounter treeCounter(0, pHuffmanCoding, pProduction->getRightHandSide());
		// pHuffmanCoding->countSymbol(0, 1);
	}
}

void HuffmanSuccinctWriter::countSymbolsInRankInfo(
		HuffmanCoding * pHuffmanCoding)
{
	for (unsigned int i = 1; i < Symbol::counter; i++)
	{
		Symbol * pSymbol = Symbol::getSymbolById(i);
		if (Symbol::isTerminalSymbol(pSymbol->getId())) {

			if (pSymbol->hasLeftChild() != true || pSymbol->hasRightChild() != true)
			{
				pHuffmanCoding->countSymbol(pSymbol->getName());
			}
		}
	}

	pHuffmanCoding->countSymbol(countSymbols(false, false), false);
	pHuffmanCoding->countSymbol(countSymbols(false, true), false);
	pHuffmanCoding->countSymbol(countSymbols(true, false), false);
}

void HuffmanSuccinctWriter::countSymbolsInNameInfo(
		HuffmanCoding * pHuffmanCoding)
{
	for (unsigned int i = 1; i < Symbol::counter; i++)
	{
		Symbol * pSymbol = Symbol::getSymbolById(i);
		if (Symbol::isTerminalSymbol(pSymbol->getId())) {

			string name = Symbol::getElementTypeById(i);

			for (unsigned int j = 0; j < name.length(); j++)
			{
				char character = name.at(j);
				pHuffmanCoding->countSymbol(character);
			}

			pHuffmanCoding->countSymbol(3); // ETX
		}
	}
}

void HuffmanSuccinctWriter::writeStartProduction(HuffmanCoding * pHuffmanCoding)
{
	Production * pProduction = dagGrammar[0];
	HuffmanTreeWriter treeWriter(&dagGrammar, pHuffmanCoding, bitWriter, pProduction->getRightHandSide());
}

// this inherited method is not needed
void HuffmanSuccinctWriter::writeProduction(Production * pProduction)
{
	cout << "Method not implemented. Exiting ..." << endl;
	exit(1);
}

void HuffmanSuccinctWriter::writeProductions(HuffmanCoding * pHuffmanCoding)
{
	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
		Production * pProduction = mainGrammar[i];
		HuffmanTreeWriter treeWriter(0, pHuffmanCoding, bitWriter, pProduction->getRightHandSide());
	}
}

void HuffmanSuccinctWriter::writeRankInfo(HuffmanCoding * pHuffmanCoding)
{
	// TODO optimize dont count symbols when constructing huffman tree
	// and when writing out (i.e. don't do it twice)
	bitWriter.writeBitString(string("00"));
	pHuffmanCoding->writeHuffmanCode(countSymbols(false, false), false);
	writeRankInfoAux(pHuffmanCoding, false, false);

	bitWriter.writeBitString(string("01"));
	pHuffmanCoding->writeHuffmanCode(countSymbols(false, true), false);
	writeRankInfoAux(pHuffmanCoding, false, true);

	bitWriter.writeBitString(string("10"));
	pHuffmanCoding->writeHuffmanCode(countSymbols(true, false), false);
	writeRankInfoAux(pHuffmanCoding, true, false);
}

void HuffmanSuccinctWriter::writeRankInfoAux(HuffmanCoding * pHuffmanCoding,
		bool leftChild, bool rightChild)
{

	for (unsigned int i = 1; i < Symbol::counter; i++)
	{
		Symbol * pSymbol = Symbol::getSymbolById(i);
		if (Symbol::isTerminalSymbol(pSymbol->getId())) {

			Symbol * pSymbol = Symbol::getSymbolById(i);
			if (pSymbol->hasLeftChild() == leftChild && pSymbol->hasRightChild()
					== rightChild)
			{
				pHuffmanCoding->writeHuffmanCode(pSymbol->getName(), true);
			}
		}

	}
}

void HuffmanSuccinctWriter::writeNameInfo(HuffmanCoding * pHuffmanCoding)
{
	for (unsigned int i = 1; i < Symbol::counter; i++) {
		Symbol * pSymbol = Symbol::getSymbolById(i);
		if (Symbol::isTerminalSymbol(pSymbol->getId())) {

			string name = Symbol::getElementTypeById(i);

			for (unsigned int j = 0; j < name.length(); j++) {
				char character = name.at(j);
				pHuffmanCoding->writeHuffmanCode(character, false);
			}

			pHuffmanCoding->writeHuffmanCode(3, false);
		}
	}
}

string HuffmanSuccinctWriter::decToBinary(const unsigned int n,
		unsigned int numberOfBits)
{

	// #ifdef DEBUG
	if (n > (pow(2.0, (int) numberOfBits) - 1))
	{
		cout << "Error: Couldn't encode decimal " << n << ". "
			"Not enough available bits. Exiting ..." << endl;
		exit(1);
	}
	// #endif

	int rem = n;
	string bitStr = "";
	for (int e = (numberOfBits - 1); e >= 0; e--)
	{
		if (((double) rem / pow(2.0, e)) >= 1.0)
		{
			bitStr += "1";
			rem = rem - (int) floor(pow(2.0, e));
		}
		else
		{
			bitStr += "0";
		}
	}

	return bitStr;
}

unsigned int HuffmanSuccinctWriter::countSymbols(bool leftChild,
		bool rightChild)
{
	unsigned int count = 0;

	for (unsigned int i = 1; i < Symbol::counter; i++) {

		Symbol * pSymbol = Symbol::getSymbolById(i);

		if (Symbol::isTerminalSymbol(pSymbol->getId())) {
			if (pSymbol->hasLeftChild() == leftChild && pSymbol->hasRightChild()
					== rightChild)
			{
				count++;
			}
		}

	}

	return count;

}

unsigned int HuffmanSuccinctWriter::calculateNeededBits(unsigned int n)
{
	return ceil(log((double) (n + 1)) / log(2.0));
}
