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
#include "Huffman.h"

#include "HuffmanTreeNode.h"
#include "HuffmanTreeDeleter.h"

#include "TreeRePairConstants.h"
#include "Helper.h"

#include <cmath>

using namespace std;

Huffman::Huffman() :
	pHashedHuffmanTreeNodes(0), pRootNode(0)
{
	pHashedHuffmanTreeNodes = new HASH_S_H;
//#if !defined(__GNUC__)
//	pHashedHuffmanTreeNodes->set_empty_key(UINT_MAX);
//#endif
}

Huffman::~Huffman()
{
	HuffmanTreeDeleter treeDeleter;
	treeDeleter.deleteTree(pRootNode);

	delete pHashedHuffmanTreeNodes;
}

#ifdef DEBUG
void Huffman::printCoding()
{
	HASH_S_H::iterator it;
	for (it = pHashedHuffmanTreeNodes->begin(); it
			!= pHashedHuffmanTreeNodes->end(); ++it)
	{
		HuffmanTreeNode * pHuffmanTreeNode = it->second;
		cout << it->first << ": " << pHuffmanTreeNode->getBitRepresentation()
				<< endl;
	}
}
#endif

string Huffman::convertUintToBinary(unsigned int a, unsigned int length)
{
	int exp = length - 1;
	string binary = "";
	while (exp >= 0)
	{
		if (pow((double)2, exp) <= a)
		{
			binary += "1";
			a -= (unsigned int)pow((double)2, exp);
		}
		else
		{
			binary += "0";
		}
		exp--;
	}

	return binary;
}
