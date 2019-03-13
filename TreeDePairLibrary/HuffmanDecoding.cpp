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
#include "HuffmanDecoding.h"

#include "HuffmanTreeNode.h"
#include "HuffmanSuccinctReader.h"

#include "TreeRePairConstants.h"
#include "Helper.h"

#include <string>
#include <cmath>

using namespace std;

HuffmanDecoding::HuffmanDecoding(BitReader * pBitReaderPar) :
	Huffman(), numberOfCodeLengths(0), pBitReader(pBitReaderPar)
{
	rlcMaxRepNonNullCodeLength = (unsigned int)pow(2.0, RLC_NON_NULL_CODE_LENGTH_BITS) + RLC_MAX_RUN_LENGTH;
	rlcMaxRepNullCodeLengthFew = (unsigned int)pow(2.0, RLC_NULL_CODE_LENGTH_FEW_BITS) + RLC_MAX_RUN_LENGTH;
	rlcMaxRepNullCodeLengthMany = (unsigned int)pow(2.0, RLC_NULL_CODE_LENGTH_MANY_BITS) + rlcMaxRepNullCodeLengthFew;
}

HuffmanDecoding::~HuffmanDecoding()
{
}

unsigned int HuffmanDecoding::readNext(bool lookAhead)
{
	HuffmanTreeNode * pCurrentNode = pRootNode;
	string currentBit;
	unsigned int childIndex;

	unsigned int length = 1;
	while (true)
	{
		currentBit = pBitReader->readBitString(length, true);
		if (currentBit.at(length - 1) == '0')
		{
			childIndex = 0;
		}
		else
		{
			childIndex = 1;
		}

		if (pCurrentNode->getChild(childIndex) != 0)
		{
			pCurrentNode = pCurrentNode->getChild(childIndex);
		}
		else
		{
			break;
		}

		length++;
	}
	// 00

	if (!lookAhead)
	{
		pBitReader->readBitString(length - 1, false);
	}

	return pCurrentNode->getSymbol();
}

void HuffmanDecoding::processCodeLengths(HuffmanDecoding * pDecoding)
{

	vector<unsigned int> codeLengths;

	if (pDecoding == 0)
	{
		codeLengths = readFixedCodedCodeLengths();
	}
	else
	{
		codeLengths = readHuffmanCodedCodeLengths(pDecoding);
	}

	HASH_I_S codesHashMap = reconstructHuffmanCodes(codeLengths);
	reconstructHuffmanTree(codesHashMap);

	// printCoding();
}

vector<unsigned int> HuffmanDecoding::readFixedCodedCodeLengths()
{
	vector<unsigned int> codeLengths;

	unsigned int bitsPerCodeLength = HuffmanSuccinctReader::binaryToDec(
			pBitReader->readBitString(CODE_LENGTH_BITS));
//	cout << "bits per code length: " << bitsPerCodeLength << endl;

	numberOfCodeLengths = HuffmanSuccinctReader::binaryToDec(
			pBitReader->readBitString(CODE_LENGTH_BITS));
//	cout << "number of code lengths: " << numberOfCodeLengths << endl;

	unsigned int codeLength;
	for (unsigned int i = 0; i < numberOfCodeLengths; i++)
	{
		codeLength = HuffmanSuccinctReader::binaryToDec(pBitReader->readBitString(
				bitsPerCodeLength));
		codeLengths.push_back(codeLength);
		// cout << codeLength << endl;
	}

	return codeLengths;
}

vector<unsigned int> HuffmanDecoding::readHuffmanCodedCodeLengths(
		HuffmanDecoding * pSuperHuffmanCoding)
{
	unsigned int numberOfSymbols = pSuperHuffmanCoding->getNumberOfCodeLengths() - 3;

	// decode code lengths of first huffman tree

	unsigned int numberOfCodeLengths = HuffmanSuccinctReader::binaryToDec(
			pBitReader->readBitString(CODE_LENGTH_BITS));

//	cout << "number of code lengths: " << numberOfCodeLengths << endl;

	unsigned int counter = 0;
	unsigned int currentCodeLength = 0;
	vector<unsigned int> codeLengths;

	while (counter < numberOfCodeLengths)
	{
		currentCodeLength = pSuperHuffmanCoding->readNext();

		if (currentCodeLength < numberOfSymbols)
		{
			// cout << "Code length: " << currentSymbol << endl;

			unsigned int nextCodeLength = pSuperHuffmanCoding->readNext(true);

			unsigned int runLength = 0;

			if (counter + 1 < numberOfCodeLengths && nextCodeLength >= numberOfSymbols)
			{

				unsigned int runlengthCodingType = nextCodeLength
						- (numberOfSymbols - 1);

				while (runlengthCodingType == 1 &&
						((runLength > 0 && (counter + runLength) < (numberOfCodeLengths))
								|| (runLength == 0 && (counter + 1) < (numberOfCodeLengths))
						))
				{ // run length coding type 1

					pSuperHuffmanCoding->readNext(false);

					unsigned int add = HuffmanSuccinctReader::binaryToDec(
							pBitReader->readBitString(RLC_NON_NULL_CODE_LENGTH_BITS))
							+ RLC_MAX_RUN_LENGTH + 1;
					runLength += add;

					// cout << "runlength 1: " << add << endl;

					nextCodeLength = pSuperHuffmanCoding->readNext(true);

					runlengthCodingType = nextCodeLength - (numberOfSymbols - 1);
				}
			}

			if (runLength == 0) {
				runLength++;
			}

			for (unsigned int i = 0; i < runLength; i++)
			{
				codeLengths.push_back(currentCodeLength);
			}

			//			if (runLength == 1)
			//			{
			//				cout << "runlength 0 (" << currentSymbol << ")" << endl;
			//			}

			// cout << "code length: " << currentCodeLength << "; occurrences: " << runLength << endl;

			counter += runLength;

		}
		else // runlength coding of codelength 0
		{
			unsigned int runlengthCodingType = currentCodeLength - (numberOfSymbols
					- 1);

			// cout << "run length coding type: " << runlengthCodingType << endl;

			unsigned int runLength = 0;

			while (runlengthCodingType == 3 && counter + runLength
					< numberOfCodeLengths)
			{
				unsigned int add = HuffmanSuccinctReader::binaryToDec(
						pBitReader->readBitString(RLC_NULL_CODE_LENGTH_MANY_BITS))
						+ rlcMaxRepNullCodeLengthFew + 1;
				runLength += add;

				// cout << "runlength 3: " << add << endl;

				currentCodeLength = pSuperHuffmanCoding->readNext(true);
				if (currentCodeLength >= numberOfSymbols && counter + runLength
						< numberOfCodeLengths)
				{
					pSuperHuffmanCoding->readNext(false);
					runlengthCodingType = currentCodeLength - (numberOfSymbols - 1);
				}
				else
				{
					break;
				}
			}

			if (runlengthCodingType == 2 && counter + runLength
					< numberOfCodeLengths)
			{
				unsigned int add = HuffmanSuccinctReader::binaryToDec(
						pBitReader->readBitString(RLC_NULL_CODE_LENGTH_FEW_BITS))
						+ RLC_MAX_RUN_LENGTH + 1;
				runLength += add;

				// cout << "runlength 2: " << add << endl;
			}

			// cout << "run length coding: " << runLength << endl;

			for (unsigned int i = 0; i < runLength; i++)
			{
				codeLengths.push_back(0);
			}

			// cout << "code length: " << 0 << "; occurrences: " << runLength << endl;

			counter += runLength;
		}
	}

	// cout << "run length info end" << endl;

	return codeLengths;
}

void HuffmanDecoding::reconstructHuffmanTree(HASH_I_S codesHashMap)
{

	pRootNode = new HuffmanTreeNode();

	HASH_I_S::iterator it;
	for (it = codesHashMap.begin(); it != codesHashMap.end(); it++)
	{
		HuffmanTreeNode * pCurrentNode = pRootNode;

		string strRepr = it->second;
		unsigned int childIndex = 0;
		for (unsigned int j = 0; j < strRepr.length(); j++)
		{
			if (strRepr.at(j) == '1')
			{
				childIndex = 1;
			}
			else
			{
				childIndex = 0;
			}

			if (pCurrentNode->getChild(childIndex) == 0)
			{
				pCurrentNode->setChild(childIndex, new HuffmanTreeNode());
			}

			pCurrentNode = pCurrentNode->getChild(childIndex);
		}

		pCurrentNode->setBitRepresentation(it->second);
		pCurrentNode->setSymbol(it->first);
		(*pHashedHuffmanTreeNodes)[it->first] = pCurrentNode;
	}

}

// see page 7 in DEFLATE specfication
HASH_I_S HuffmanDecoding::reconstructHuffmanCodes(
		vector<unsigned int> codeLengths)
{
	unsigned int maximumCodeLength = 0;
	for (unsigned int i = 0; i < codeLengths.size(); i++)
	{
		if (maximumCodeLength < codeLengths.at(i))
		{
			maximumCodeLength = codeLengths.at(i);
		}
	}

	unsigned int ** bl_count = new unsigned int *[maximumCodeLength + 1];
	unsigned int ** next_code = new unsigned int *[maximumCodeLength + 1];

	for (unsigned int i = 0; i <= maximumCodeLength; i++)
	{
		bl_count[i] = new unsigned int(0);
		next_code[i] = new unsigned int(0);
	}

	for (unsigned int symbol = 0; symbol < codeLengths.size(); symbol++)
	{
		unsigned int codeLength = codeLengths.at(symbol);
		unsigned int * count = bl_count[codeLength];
		(*count)++;
	}

	unsigned int code = 0;
	(*(bl_count[0])) = 0;

	for (unsigned int bits = 1; bits <= maximumCodeLength; bits++)
	{
		code = (code + (*(bl_count[bits - 1]))) << 1;
		(*(next_code[bits])) = code;
	}

	HASH_I_S codesHashMap;
	for (unsigned int symbol = 0; symbol < codeLengths.size(); symbol++)
	{
		unsigned int codeLength = codeLengths.at(symbol);
		if (codeLength != 0)
		{
			codesHashMap[symbol] = convertUintToBinary(
					(*(next_code[codeLength])), codeLength);
			(*(next_code[codeLength]))++;
		}
	}

	return codesHashMap;
}

