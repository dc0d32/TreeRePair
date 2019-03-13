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
#include "HuffmanCoding.h"

#include "HuffmanTreeNode.h"
#include "HuffmanTreeDeleter.h"

#include "TreeRePairConstants.h"
#include "Helper.h"

#include <queue>

using namespace std;

HuffmanCoding::HuffmanCoding(BitWriter & bitWriterPar, ofstream & foutPar,
		bool unencodedPar) :
	Huffman(), pHuffmanTreeNodesVector(0), bitWriter(bitWriterPar), fout(
			foutPar), unencoded(unencodedPar), biggestDecimal(0)
{
	pHuffmanTreeNodesVector = new vector<HuffmanTreeNode*> ;
}

HuffmanCoding::~HuffmanCoding()
{
}

unsigned int HuffmanCoding::getMaximumCodeLength()
{

	unsigned int maximumCodeLength = 0;

	HASH_S_H::iterator it;
	for (it = pHashedHuffmanTreeNodes->begin(); it
			!= pHashedHuffmanTreeNodes->end(); ++it)
	{
		HuffmanTreeNode * pHuffmanTreeNode = it->second;
		unsigned int codeLength =
				pHuffmanTreeNode->getBitRepresentation().length();

		if (codeLength > maximumCodeLength)
		{
			maximumCodeLength = codeLength;
		}
	}

	return maximumCodeLength;

}

void HuffmanCoding::countSymbol(const unsigned int symbol,
		const unsigned int add)
{
	if (biggestDecimal < symbol)
	{
		biggestDecimal = symbol;
	}

	HASH_S_H::iterator result = pHashedHuffmanTreeNodes->find(symbol);
	HuffmanTreeNode * pHuffmanTreeNode = 0;
	if (result == pHashedHuffmanTreeNodes->end())
	{
		pHuffmanTreeNode = addHuffmanTreeNode(symbol);
	} else {
		pHuffmanTreeNode = result->second;
	}
	pHuffmanTreeNode->addToFrequency(add);
}

HuffmanTreeNode * HuffmanCoding::addHuffmanTreeNode(const unsigned int symbol)
{
	HuffmanTreeNode * pHuffmanTreeNode = new HuffmanTreeNode();
	(*pHashedHuffmanTreeNodes)[symbol] = pHuffmanTreeNode;

	pHuffmanTreeNodesVector->push_back(pHuffmanTreeNode);

	return pHuffmanTreeNode;
}

void HuffmanCoding::computeHuffmanCodes(HuffmanTreeNode * pNode,
		const string &representation)
{

	if (pNode->getLeftChild() != 0)
	{
		computeHuffmanCodes(pNode->getLeftChild(), representation + "0");
		computeHuffmanCodes(pNode->getRightChild(), representation
				+ "1");
	}
	else
	{
		pNode->setBitRepresentation(representation);
	}

}

void HuffmanCoding::computeHuffmanTree()
{

	// cout << "biggest decimal is " << biggestDecimal << endl;

	if (pHuffmanTreeNodesVector->size() == 1)
	{
		pRootNode = (*pHuffmanTreeNodesVector)[0];
		return;
	}

	sort(pHuffmanTreeNodesVector->begin(), pHuffmanTreeNodesVector->end(),
			cmpHuffmanTreeNodes);

	queue<HuffmanTreeNode*> pNodesQueue;
	queue<HuffmanTreeNode*> pSubTreesQueue;

	for (unsigned int i = 0; i < pHuffmanTreeNodesVector->size(); i++)
	{
		pNodesQueue.push((*pHuffmanTreeNodesVector)[i]);
	}

	delete pHuffmanTreeNodesVector;

	while (true)
	{

		int frequencyNodesQueue = INT_MAX;
		int frequencySubTreesQueue = INT_MAX;

		if (!pNodesQueue.empty())
		{
			frequencyNodesQueue = pNodesQueue.front()->getFrequency();
		}

		if (!pSubTreesQueue.empty())
		{
			frequencySubTreesQueue = pSubTreesQueue.front()->getFrequency();
		}

		HuffmanTreeNode * pFirstNode;
		if (frequencyNodesQueue < frequencySubTreesQueue)
		{
			pFirstNode = pNodesQueue.front();
			pNodesQueue.pop();
			if (!pNodesQueue.empty())
			{
				frequencyNodesQueue = pNodesQueue.front()->getFrequency();
			}
			else
			{
				frequencyNodesQueue = INT_MAX;
			}
		}
		else
		{
			pFirstNode = pSubTreesQueue.front();
			pSubTreesQueue.pop();
			if (!pSubTreesQueue.empty())
			{
				frequencySubTreesQueue = pSubTreesQueue.front()->getFrequency();
			}
			else
			{
				frequencySubTreesQueue = INT_MAX;
			}
		}

		// huffman tree is complete
		if (frequencyNodesQueue == INT_MAX && frequencySubTreesQueue == INT_MAX)
		{
			pSubTreesQueue.push(pFirstNode);
			break;
		}

		HuffmanTreeNode * pSecondNode = 0;
		if (frequencyNodesQueue < frequencySubTreesQueue)
		{
			pSecondNode = pNodesQueue.front();
			pNodesQueue.pop();
		}
		else
		{
			pSecondNode = pSubTreesQueue.front();
			pSubTreesQueue.pop();
		}

		HuffmanTreeNode * pNewNode = new HuffmanTreeNode();
		pNewNode->setFrequency(pFirstNode->getFrequency()
				+ pSecondNode->getFrequency());
		pNewNode->setLeftChild(pFirstNode);
		pNewNode->setRightChild(pSecondNode);

		pSubTreesQueue.push(pNewNode);
	}

	pRootNode = pSubTreesQueue.front();

	computeHuffmanCodes(pRootNode, string(""));

	return;
}

bool HuffmanCoding::cmpHuffmanTreeNodes(HuffmanTreeNode * pNode1,
		HuffmanTreeNode * pNode2)
{
	return pNode1->getFrequency() < pNode2->getFrequency();
}

void HuffmanCoding::writeHuffmanCode(const int symbol, bool isSymbol)
{

	isSymbol = false;

	if (unencoded)
	{
		if (isSymbol)
		{
			fout << "[" << Symbol::getElementTypeById(symbol) << "]";
		}
		else
		{
			fout << "[" << symbol << "]";
		}
	}
	else
	{
		HASH_S_H::iterator result = pHashedHuffmanTreeNodes->find(symbol);
		if (result != pHashedHuffmanTreeNodes->end()) {
			HuffmanTreeNode * pHuffmanTreeNode = result->second;
			bitWriter.writeBitString(pHuffmanTreeNode->getBitRepresentation());
		} else {
			cout << "Fatal error: Can't encode value " << symbol << ". Exiting ...";
			exit(1);
		}
	}

}

void HuffmanCoding::reorganizeHuffmanCodes()
{
	unsigned int ** bl_count = new unsigned int *[getMaximumCodeLength() + 1];
	unsigned int ** next_code = new unsigned int *[getMaximumCodeLength() + 1];

	for (unsigned int i = 0; i <= getMaximumCodeLength(); i++)
	{
		bl_count[i] = new unsigned int(0);
		next_code[i] = new unsigned int(0);
	}

	for (unsigned int symbol = 0; symbol <= biggestDecimal; symbol++)
	{

		HASH_S_H::iterator it = pHashedHuffmanTreeNodes->find(symbol);
		if (it != pHashedHuffmanTreeNodes->end())
		{
			HuffmanTreeNode * pHuffmanTreeNode = it->second;

			// cout << "old: " << symbol << " : " << pHuffmanTreeNode->getBitRepresentation() << endl;

			unsigned int codeLength =
					pHuffmanTreeNode->getBitRepresentation().length();
			unsigned int * count = bl_count[codeLength];
			(*count)++;
		}
	}

	unsigned int code = 0;
	(*(bl_count[0])) = 0;

	for (unsigned int bits = 1; bits <= getMaximumCodeLength(); bits++)
	{
		code = (code + (*(bl_count[bits - 1]))) << 1;
		(*(next_code[bits])) = code;
	}

	for (unsigned int symbol = 0; symbol <= biggestDecimal; symbol++)
	{

		HASH_S_H::iterator it = pHashedHuffmanTreeNodes->find(symbol);
		if (it != pHashedHuffmanTreeNodes->end())
		{
			HuffmanTreeNode * pHuffmanTreeNode = it->second;
			unsigned int codeLength =
					pHuffmanTreeNode->getBitRepresentation().length();
			if (codeLength != 0)
			{
				pHuffmanTreeNode->setBitRepresentation(convertUintToBinary(
						(*(next_code[codeLength])), codeLength));
				(*(next_code[codeLength]))++;

				// cout << "new: " << symbol << " : " << pHuffmanTreeNode->getBitRepresentation() << endl;
			}
		}
	}


	for (unsigned int i = 0; i <= getMaximumCodeLength(); i++)
	{
		delete bl_count[i];
		delete next_code[i];
	}

	delete [] bl_count;
	delete [] next_code;
}
