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
#include "BplexGrammarReader.h"

#include "TreeRePairConstants.h"
#include "Symbol.h"
#include "Digram.h"
#include "Production.h"

#include <vector>

using namespace std;

BplexGrammarReader::BplexGrammarReader(char * pFilePath,
		Grammar & dagGrammarPar, Grammar & mainGrammarPar) :
	dagGrammar(dagGrammarPar), mainGrammar(mainGrammarPar) {

	cout << "Grammar file: " << pFilePath << endl;

	ifstream ifs;
	ifs.open(pFilePath);

	vector<string> lines;

	string str;
	while (getline(ifs, str)) {
		lines.push_back(str);
	}

	ifs.close();

	for (unsigned int i = lines.size() - 1; i > 0; i--) {
		retrieveProduction(mainGrammar, lines[i]);
	}

	retrieveProduction(dagGrammar, lines[0]);
}

BplexGrammarReader::~BplexGrammarReader() {
}

void BplexGrammarReader::retrieveProduction(Grammar & grammar, string & strProduction) {

	string::size_type endOfLeftHandSide = min(strProduction.find('(', 0), strProduction.find("->", 0) - 1);
	string strLeftHandSide = strProduction.substr(0, endOfLeftHandSide);

	string strTemp = strProduction.substr(endOfLeftHandSide, strProduction.length() - endOfLeftHandSide);
	unsigned int rank = retrieveRank(strTemp);
	Symbol * pLeftHandSide = createNonTerminalSymbol(strLeftHandSide, rank);

	if (pLeftHandSide == Symbol::getPlaceholderSymbol()) {
		return;
	}

	string::size_type indexOfTree = strProduction.find("->", 0) + 3;
	strTemp = strProduction.substr(indexOfTree, strProduction.length() - indexOfTree);
	TreeNode * pRightHandSide = readRightHandSide(strTemp);

	Production * pProduction = new Production(pLeftHandSide, pRightHandSide);
	grammar.addProduction(pProduction);
}

TreeNode * BplexGrammarReader::readRightHandSide(string & strTree) {

	unsigned int currentIndex = 0;

	TreeNode * pParentNode = 0, *pNode = 0;
	string currentSymbolName;
	Symbol * pCurrentSymbol;

	currentSymbolName = readNextSymbol(strTree, &currentIndex);

	Symbol * pRootSymbol = retrieveSymbol(currentSymbolName);
	if (pRootSymbol == 0) {
		pRootSymbol = createTerminalSymbol(currentSymbolName);
	}
	TreeNode * pRoot = new TreeNode(pRootSymbol);

	pParentNode = pRoot;

	stack<unsigned int*> sumOfRanksStack;
	sumOfRanksStack.push(new unsigned int(0));

	while (currentIndex < strTree.length() - 1)
	{

		currentSymbolName = readNextSymbol(strTree, &currentIndex);
		if (currentSymbolName.length() == 0) {
			break;
		}

		pCurrentSymbol = retrieveSymbol(currentSymbolName);
		if (pCurrentSymbol == 0) {
			pCurrentSymbol = createTerminalSymbol(currentSymbolName);
		}

		pNode = new TreeNode(pCurrentSymbol);

		unsigned int * sumOfRanks = sumOfRanksStack.top();
		while (*sumOfRanks >= pParentNode->getSymbol()->getRank())
		{
			if (pParentNode->getParent() != 0)
			{
				pParentNode = pParentNode->getParent();
				unsigned int * tempUInt = sumOfRanksStack.top();
				sumOfRanksStack.pop();
				delete tempUInt;
				sumOfRanks = sumOfRanksStack.top();
			}
			else
			{
				cout << "Fatal error: Could not parse input tree. Exiting ..."
						<< endl;
				exit(1);
			}
		}

		pParentNode->addChild(pNode);
		*sumOfRanks += 1;

		if (pCurrentSymbol->getRank() > 0)
		{
			pParentNode = pNode;
			sumOfRanksStack.push(new unsigned int(0));
		}

	}

	return pRoot;

}

string BplexGrammarReader::readNextSymbol(string & strTree, unsigned int * currentIndex) {

	while (*currentIndex < strTree.length() && (strTree.at(*currentIndex) == ' '
			|| strTree.at(*currentIndex) == ','
			|| strTree.at(*currentIndex) == '('
			|| strTree.at(*currentIndex) == ')')) {
		(*currentIndex)++;
	}

	if (*currentIndex >= strTree.length()) {
		return "";
	}

	string::size_type endOfSymbol = min(strTree.find('(', *currentIndex), strTree.find(')', *currentIndex));
	endOfSymbol = min(endOfSymbol, strTree.find(',', *currentIndex));
	endOfSymbol = min(endOfSymbol, strTree.find(' ', *currentIndex));

	string strSymbol = strTree.substr(*currentIndex, endOfSymbol - *currentIndex);
	(*currentIndex) += (endOfSymbol - *currentIndex);

	return strSymbol;
}

unsigned int BplexGrammarReader::retrieveRank(string str) {

	string::size_type indexOfArrow = str.find("->", 0);
	string::size_type indexOfOpeningBracket = str.find('(', 0);

	if (indexOfOpeningBracket == string::npos ||
			(indexOfArrow != string::npos && indexOfArrow < indexOfOpeningBracket)) {
		return 0;
	}

	string::size_type indexOfClosingBracket = str.find(')', 0);

	str = str.substr(indexOfOpeningBracket, indexOfClosingBracket - indexOfOpeningBracket);

	unsigned int numberOfCommas = 0;
	string::size_type indexOfComma = str.find (',', 0);
	while (indexOfComma != string::npos) {
		numberOfCommas++;
		indexOfComma = str.find (',', indexOfComma + 1);
	}

	return numberOfCommas + 1;
}

Symbol * BplexGrammarReader::retrieveSymbol(string & strName) {

	HASH_ST_S::iterator result = nameSymbolsHashMap.find(strName);
	if (result == nameSymbolsHashMap.end()) {
		return 0;
	} else {
		return result->second;
	}
}

Symbol * BplexGrammarReader::createTerminalSymbol(string & strName) {

	Symbol * pSymbol = 0;
	if (strName.length() >= 2 && strName.at(0) == 'y'
			&& (strName.at(1) == '0' || strName.at(1) == '1' || strName.at(1) == '2'
			|| strName.at(1) == '3' || strName.at(1) == '4' || strName.at(1) == '5'
			|| strName.at(1) == '6' || strName.at(1) == '7' || strName.at(1) == '8'
			|| strName.at(1) == '9')) {

		pSymbol = Symbol::getParameterSymbol();
	} else {
		pSymbol = new Symbol();
		pSymbol->setRank(2);
		pSymbol->addSymbolToHashMaps(strName);
	}
	nameSymbolsHashMap[strName] = pSymbol;
	return pSymbol;
}

Symbol * BplexGrammarReader::createNonTerminalSymbol(string & strName, unsigned int rank) {

	Symbol * pSymbol = 0;
	if (strName != "A0") {
		pSymbol = new Symbol();
		pSymbol->setRank(rank);
	} else {
		pSymbol = Symbol::getPlaceholderSymbol();
	}
	nameSymbolsHashMap[strName] = pSymbol;

	return pSymbol;
}
