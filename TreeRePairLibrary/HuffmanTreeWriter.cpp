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
#include "HuffmanTreeWriter.h"

#include "HuffmanSuccinctWriter.h"
#include "TreeRePairTypes.h"
#include "TreeRePairConstants.h"
#include "Helper.h"
#include "Production.h"

HuffmanTreeWriter::HuffmanTreeWriter(Grammar * grammarPar,
		HuffmanCoding * pHuffmanCodingPar,
		BitWriter & bitWriterPar, TreeNode * pRootPar) :
	TreeTraversal(pRootPar), dagGrammar(grammarPar), pHuffmanCoding(pHuffmanCodingPar), bitWriter(
			bitWriterPar)
{
	traverseTree();
}

HuffmanTreeWriter::~HuffmanTreeWriter(void)
{
}

void HuffmanTreeWriter::processNode(TreeNode * pParent, unsigned int index,
		int depth, bool closing)
{

	TreeNode * pNode;
	if (index != UINT_MAX)
	{
		pNode = pParent->getChild(index);
	}
	else
	{
		pNode = pParent;
	}

	if (closing)
	{
		return;
	}

	if (dagGrammar != 0 && dagGrammar->isNonTerminal(pNode->getSymbol())) {
		Production * pProduction = dagGrammar->getProductionBySymbol(pNode->getSymbol());
		HuffmanTreeWriter treeWriter(dagGrammar, pHuffmanCoding, bitWriter, pProduction->getRightHandSide());
	} else {
//		cout << pNode->getSymbol()->getName() << endl;
		pHuffmanCoding->writeHuffmanCode(pNode->getSymbol()->getName(),
			true);
	}

	return;
}
