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
#include "TextualGrammarReader.h"

#include "TreeRePairTypes.h"
#include "Helper.h"
#include "TreeRePairMode.h"
#include "TreeRePairConstants.h"
#include "Production.h"

#include <sstream>

TextualGrammarReader::TextualGrammarReader(const char * pFilePath,
		Grammar & dagGrammarPar, Grammar & mainGrammarPar) :
	dagGrammar(dagGrammarPar), mainGrammar(mainGrammarPar),
	counter(1)
{
	ifs = new ifstream;
	ifs->open(pFilePath);

	parse();
}

TextualGrammarReader::~TextualGrammarReader(void)
{
	ifs->close();
	delete ifs;
}

void TextualGrammarReader::parse() {

	string line;
	while (getline(*ifs, line)) {

		parseLine(line);

	}

}

Production * TextualGrammarReader::createProductionAux(
		unsigned int newSymbolId, const string &strRightSide)
{
	unsigned int numberOfParameters = 0;
	TreeNode * pParentNode = 0, *pNode = 0;
	unsigned int currentSymbolName;
	Symbol * pCurrentSymbol;

	istringstream ss(strRightSide);
	ss >> currentSymbolName;

	Symbol * pRootSymbol = Symbol::getSymbolById(currentSymbolName);
	TreeNode * pRoot = new TreeNode(pRootSymbol);

	pParentNode = pRoot;

	stack<unsigned int*> sumOfRanksStack;
	sumOfRanksStack.push(new unsigned int(0));

	while (!ss.eof())
	{

		ss >> currentSymbolName;
		pCurrentSymbol = Symbol::getSymbolById(currentSymbolName);
		if (pCurrentSymbol == Symbol::getParameterSymbol()) {
			numberOfParameters++;
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

#ifdef DEBUG
		if (*sumOfRanks > pParentNode->getSymbol()->getRank())
		{
			cout << "Fatal error: Sum of incoming ranks of children is "
				"bigger than outgoing rank of parent symbol. Exiting ..."
					<< endl;
			exit(1);
		}
#endif

		if (pCurrentSymbol->getRank() > 0)
		{
			pParentNode = pNode;
			sumOfRanksStack.push(new unsigned int(0));
		}

	}


	// symbol with id newSymbolName exists if we are running the
	// TextualGrammarReader within TreeRePair (instead within TreeDePair)
	// to read the replacements
	Symbol * pNewSymbol = Symbol::getSymbolById(newSymbolId);
	if (pNewSymbol == 0)
	{
		pNewSymbol = new Symbol();
		pNewSymbol->setRank(numberOfParameters);
	}

	Production * pProduction = new Production(pNewSymbol, pRoot);

	return pProduction;
}

void TextualGrammarReader::parseLine(const string &line)
{

	istringstream ss(line);
	string token;
	ss >> token;
	if (token == "#")
	{
		parseInfo(line.substr(token.length(), line.length()));
	}
	else
	{
		if (TreeRePairMode::getInstance()->isVerboseMode() && (counter % 10) == 0)
		{
			cout << "\rParsing replacements ... " << counter;
		}

		counter++;

		parseProduction(line);
	}
}

void TextualGrammarReader::parseProduction(const string &line)
{

	istringstream ss(line);
	unsigned int newSymbolId;
	string strArrow, strRightSide, strTemp;
	ss >> newSymbolId;

	ss >> strArrow;

	while (!ss.eof()) {
		ss >> strTemp;
		strRightSide += strTemp;
		if (!ss.eof()) {
			strRightSide += " ";
		}
	}

	Production * pProduction = createProductionAux(newSymbolId, strRightSide);
	if (newSymbolId == 0) {
		dagGrammar.addProduction(pProduction);
	} else {
		mainGrammar.addProduction(pProduction);
	}
}

void TextualGrammarReader::parseInfo(const string &line)
{

	istringstream ss(line);
	string token;

	ss >> token;

	if (token == KEYWORD_TERMINALS)
	{
		unsigned int numberOfTerminals;
		Symbol * pSymbol = 0;
		ss >> numberOfTerminals;
		for (unsigned int i = 1; i <= numberOfTerminals; i++) {
			pSymbol = new Symbol(true, true);
		}
		Symbol::getParameterSymbol();
		cout << "parameter: " + Symbol::getParameterSymbol()->getId() << endl;
	}
	else if (token == KEYWORD_RANKS)
	{
		string childInfo;
		ss >> childInfo;

		unsigned int id;
		Symbol * pSymbol;
		while (!ss.eof()) {
			ss >> id;

			pSymbol = Symbol::getSymbolById(id);
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
	else if (token == KEYWORD_NAMES)
	{
		unsigned int i = 1;
		string name;
		while (!ss.eof()) {
			ss >> name;
			name = name.substr(1, name.length() - 2);

			Symbol * pSymbol = Symbol::getSymbolById(i);

			if (pSymbol == 0) {
				cout << "Fatal error when reading names of terminals: " << "Can't find symbol with id " << i << ". Exiting ...";
				exit(1);
			}

			pSymbol->addSymbolToHashMaps(name);
			i++;
		}
	}

}
