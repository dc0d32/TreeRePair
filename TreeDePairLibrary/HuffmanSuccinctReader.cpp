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
#include "HuffmanSuccinctReader.h"

#include "TreeRePairConstants.h"
#include "HuffmanDecoding.h"
#include "Symbol.h"
#include "Digram.h"

#include <stack>
#include <iostream>
#include <cmath>

using namespace std;

HuffmanSuccinctReader::HuffmanSuccinctReader(char * pFilePath,
		Grammar & dagGrammarPar, Grammar & mainGrammarPar) :
	numberOfTerminals(0), numberOfProductionsMinusStartProd(0), dagGrammar(dagGrammarPar),
	mainGrammar(mainGrammarPar) {

	pIfs = new ifstream;
	pIfs->open(pFilePath, ios_base::binary);

	pBitReader = new BitReader(pIfs);

	// read super huffman tree
	HuffmanDecoding superHuffmanCoding(pBitReader);
	superHuffmanCoding.processCodeLengths();

	// reader other huffman trees

//	cout << "base Huffman coding for start production: " << endl;
	HuffmanDecoding huffmanCodingStartProduction(pBitReader);
	huffmanCodingStartProduction.processCodeLengths(&superHuffmanCoding);
	// pHuffmanDecodingTree->printCoding();

//	cout << "base Huffman coding for productions, numbers, children characteristics: " << endl;
	HuffmanDecoding huffmanCodingProductions(pBitReader);
	huffmanCodingProductions.processCodeLengths(&superHuffmanCoding);

//	cout << "base Huffman coding for terminal names: " << endl;
	HuffmanDecoding huffmanCodingNames(pBitReader);
	huffmanCodingNames.processCodeLengths(&superHuffmanCoding);

//	cout << "base Huffman coding for start production: " << endl;
//	huffmanCodingStartProduction.printCoding();
//	cout << "base Huffman coding for productions, numbers, children characteristics: " << endl;
//	huffmanCodingProductions.printCoding();
//	cout << "base Huffman coding for terminal names: " << endl;
//	huffmanCodingNames.printCoding();
//	cout << "super Huffman coding: " << endl;
//	superHuffmanCoding.printCoding();

	// run length coding information
	numberOfTerminals = huffmanCodingProductions.readNext();
	numberOfProductionsMinusStartProd = huffmanCodingProductions.readNext();

//	cout << "number of terminals: " << numberOfTerminals << endl;

	// create terminal symbols
	Symbol * pSymbol = 0;
	for (unsigned int i = 1; i <= numberOfTerminals; i++) {
		pSymbol = new Symbol(true, true);
		Symbol::setTerminalSymbol(pSymbol->getId());
	}

	Symbol::getParameterSymbol();

	readRanks(huffmanCodingProductions); // 00
	readRanks(huffmanCodingProductions); // 01
	readRanks(huffmanCodingProductions); // 10

	readNames(huffmanCodingNames);

	readProductions(huffmanCodingProductions);

//	cout << "reading start production" << endl;
	readStartProduction(huffmanCodingStartProduction);
}

HuffmanSuccinctReader::~HuffmanSuccinctReader() {
	delete pBitReader;

	pIfs->close();
	delete pIfs;
}

void HuffmanSuccinctReader::readStartProduction(HuffmanDecoding & huffmanDecoding) {

	TreeNode * pRoot = readTree(huffmanDecoding, new unsigned int(0));
	Production * pProduction = new Production(Symbol::getStartSymbol(), pRoot);
	dagGrammar.addProduction(pProduction);

}

TreeNode * HuffmanSuccinctReader::readTree(HuffmanDecoding & huffmanDecoding,
		unsigned int * pNumberOfParameters) {

	TreeNode * pParentNode = 0, *pNode = 0;
	unsigned int currentSymbolId = huffmanDecoding.readNext();
	Symbol * pCurrentSymbol = 0;

	Symbol * pRootSymbol = Symbol::getSymbolById(currentSymbolId);
	TreeNode * pRoot = new TreeNode(pRootSymbol);

	pParentNode = pRoot;

	stack<unsigned int*> sumOfRanksStack;
	sumOfRanksStack.push(new unsigned int(0));

	while (huffmanDecoding.readNext(true) != 0) {

//		cout << currentSymbolId << endl;

		currentSymbolId = huffmanDecoding.readNext(true);
		if (currentSymbolId == Symbol::getParameterSymbol()->getId()) {
			(*pNumberOfParameters)++;
		}
		pCurrentSymbol = Symbol::getSymbolById(currentSymbolId);

		unsigned int * sumOfRanks = sumOfRanksStack.top();
		while (*sumOfRanks >= pParentNode->getSymbol()->getRank()) {
			if (pParentNode->getParent() != 0) {
				pParentNode = pParentNode->getParent();
				sumOfRanksStack.pop();
				sumOfRanks = sumOfRanksStack.top();
			} else {
				return pRoot; // done with tree
			}
		}

		huffmanDecoding.readNext(false); // really read id
		pNode = new TreeNode(pCurrentSymbol);
		pParentNode->addChild(pNode);
		*sumOfRanks += 1;

#ifdef DEBUG
		if (*sumOfRanks > pParentNode->getSymbol()->getRank()) {
			cout << "Fatal error: Sum of incoming ranks of children is "
				"bigger than outgoing rank of parent symbol. Exiting ..."
					<< endl;
			exit(1);
		}
#endif

		if (pCurrentSymbol->getRank() > 0) {
			pParentNode = pNode;
			sumOfRanksStack.push(new unsigned int(0));
		}

	}

	return pRoot;

}

void HuffmanSuccinctReader::readProductions(HuffmanDecoding & huffmanDecoding) {

	for (unsigned int i = 0; i < numberOfProductionsMinusStartProd; i++) {

		unsigned int * pNumberOfParameters = new unsigned int(0);
		TreeNode * pRoot = readTree(huffmanDecoding, pNumberOfParameters);

		Symbol * pNewSymbol = new Symbol();
		pNewSymbol->setRank(*pNumberOfParameters);

		Production * pProduction = new Production(pNewSymbol, pRoot);

		mainGrammar.addProduction(pProduction);
	}

}

void HuffmanSuccinctReader::readNames(HuffmanDecoding & huffmanDecoding) {

	for (unsigned int i = 1; i <= numberOfTerminals; i++) {

		char character = huffmanDecoding.readNext();
		string name = "";

		while (character != 3) {

			name += character;
			character = huffmanDecoding.readNext();

		}

		Symbol * pSymbol = Symbol::getSymbolById(i);

		if (pSymbol == 0) {
			cout << "Fatal error when reading names of terminals: "
				"Can't find symbol with id " << i << ". Exiting ...";
			exit(1);
		}

		pSymbol->addSymbolToHashMaps(name);
	}

}

void HuffmanSuccinctReader::readRanks(HuffmanDecoding & huffmanDecoding) {

	string childInfo = pBitReader->readBitString(2);

	unsigned int numberOfSymbols = huffmanDecoding.readNext();

	for (unsigned int i = 0; i < numberOfSymbols; i++) {

		unsigned int id = huffmanDecoding.readNext();

		Symbol * pSymbol = Symbol::getSymbolById(id);
		if (pSymbol == 0) {
			cout << "Fatal error when reading ranks: Can't find symbol "
				"with id " << id << ". Exiting ...";
			exit(1);
		}

		unsigned int childrenCount = 0;

		if (childInfo.at(0) == '1') {
			pSymbol->setHasLeftChild(true);
			childrenCount++;
		} else {
			pSymbol->setHasLeftChild(false);
		}
		if (childInfo.at(1) == '1') {
			pSymbol->setHasRightChild(true);
			childrenCount++;
		} else {
			pSymbol->setHasRightChild(false);
		}

		pSymbol->setRank(childrenCount);
	}
}

unsigned int HuffmanSuccinctReader::binaryToDec(string binaryRep) {

	unsigned int dec = 0;

	for (short int i = (short int) binaryRep.length() - 1; i >= 0; i--) {

		if (binaryRep.at(i) == '1') {
			dec += (unsigned int) pow(2.0, (int) (binaryRep.length() - i - 1));
		}

	}

	return dec;
}
