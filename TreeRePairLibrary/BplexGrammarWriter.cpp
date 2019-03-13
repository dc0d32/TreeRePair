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
#include "BplexGrammarWriter.h"

#include "TreeRePairConstants.h"
#include "BplexTreePrinter.h"
#include "Helper.h"
#include "Production.h"

#include <string>

unsigned int BplexGrammarWriter::parameterIndex = 0;

BplexGrammarWriter::BplexGrammarWriter(Grammar & dagGrammarPar,
		Grammar & mainGrammarPar, char * pFilePath) :
	GrammarWriter(dagGrammarPar, mainGrammarPar, pFilePath)
{
	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
		Production * pProduction = mainGrammar[i];
		pProduction->getLeftHandSide()->setName(i + 1);
	}

	Symbol::getPlaceholderSymbol()->setName(0);

	string filename = Helper::getFilePathForBplexLike(pFilePath);
	fout.open(filename.c_str());

	fout << START_TAG;
	fout << " -> ";

	Production * pProduction = dagGrammar[0];
	TreeNode * pRoot = pProduction->getRightHandSide();
	BplexTreePrinter treePrinter(dagGrammar, mainGrammar, pRoot, fout);

	fout << endl;

	writeProductionReverse();

	fout << "A0 -> _" << endl;

	fout.close();
}

BplexGrammarWriter::~BplexGrammarWriter()
{
}

string BplexGrammarWriter::getNameForSymbol(Grammar & mainGrammar, Symbol * pSymbol) {

	if (pSymbol == Symbol::getParameterSymbol()) {
		return "y" + Helper::stringify((size_t) BplexGrammarWriter::parameterIndex++);
	} else if (mainGrammar.isNonTerminal(pSymbol)) { // non terminal
		return "A" + Helper::stringify((size_t) pSymbol->getName());
	} else {
		return Symbol::getElementTypeById(pSymbol->getId());
	}

}

void BplexGrammarWriter::writeProduction(Production * pProduction)
{
	Symbol * pNewNonTerminal = pProduction->getLeftHandSide();

	fout << getNameForSymbol(mainGrammar, pNewNonTerminal);

	if (pNewNonTerminal->getRank() > 0) {
		fout << "(";
		for (unsigned int i = 0; i < pNewNonTerminal->getRank(); i++) {
			fout << "y" + Helper::stringify((size_t) i);
			if (i < pNewNonTerminal->getRank() - 1) {
				fout << ", ";
			}
		}
		fout << ")";
	}

	fout << " -> ";

	BplexGrammarWriter::parameterIndex = 0;
	TreeNode * pRoot = pProduction->getRightHandSide();
	BplexTreePrinter treePrinter(dagGrammar, mainGrammar, pRoot, fout);

	fout << endl;
}
