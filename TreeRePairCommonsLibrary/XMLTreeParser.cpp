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
#include "XMLTreeParser.h"

#include "TreeConstructor.h"
#include "BinaryTreeConstructor.h"
#include "MultiaryTreeConstructor.h"
#include "SymbolsRanksCounter.h"
#include "XMLParser.h"
#include "XmlEcho.h"

#include <iostream>

using namespace std;

XMLTreeParser::XMLTreeParser(char * pFilePath,
		Grammar & dagGrammarPar, bool binary) :
	dagGrammar(dagGrammarPar)

{
	if (TreeRePairMode::getInstance()->isVerboseMode()) {
		cout << "Constructing binary tree in memory ..." << endl;
	}

	TreeConstructor * treeConstructor = 0;
	if (binary) {
		treeConstructor = new BinaryTreeConstructor(dagGrammar);
	} else {
		SymbolsRanksCounter ranksCounter;
		XMLParser xmlParserChildren(pFilePath, ranksCounter);

		treeConstructor = new MultiaryTreeConstructor(dagGrammar);
	}

	XMLParser xmlParserTree(pFilePath, *treeConstructor);

	nodeCount = treeConstructor->getNodeCount();

	delete treeConstructor;
}

XMLTreeParser::~XMLTreeParser()
{
}
