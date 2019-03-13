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
#include "TextualGrammarWriter.h"

#include "Helper.h"
#include "TreeRePairConstants.h"
#include "DefaultTreePrinter.h"
#include "Production.h"

#include <string>

TextualGrammarWriter::TextualGrammarWriter(Grammar & dagGrammarPar,
		Grammar & mainGrammarPar, char * pFilePath,
		bool rawModePar) :
	GrammarWriter(dagGrammarPar, mainGrammarPar, pFilePath),
			rawMode(rawModePar)
{
	string filename = Helper::getFilePathForTextual(pFilePath);
	fout.open(filename.c_str());

	unsigned int name = 1;
	for (unsigned int i = 1; i < Symbol::counter; i++) {
	// for (unsigned int i = 1; i < 56; i++) {
		if (Symbol::isTerminalSymbol(i)) {
			Symbol * pSymbol = Symbol::getSymbolById(i);
			pSymbol->setName(name++);
		}
	}

	Symbol::getParameterSymbol()->setName(name++);

	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
		Symbol * pSymbol = mainGrammar[i]->getLeftHandSide();
		pSymbol->setName(name++);
	}

	fout << "# " << KEYWORD_TERMINALS << " " << Symbol::getNumberOfTerminals() << endl;

	writeRankInfo();
	writeNameInfo();
	writeProductions();

	if (!rawMode) {
		fout << START_TAG;
	} else {
		fout << 0;
	}
	fout << " -> ";

	DefaultTreePrinter treePrinter(&dagGrammar, fout, rawMode);
	Production * pProduction = dagGrammar[0];
	treePrinter.printTree(pProduction->getRightHandSide());

	fout << endl;

	fout.close();
}

TextualGrammarWriter::~TextualGrammarWriter()
{
}

void TextualGrammarWriter::writeNameInfo()
{
	bool first = true;
	fout << "# " << KEYWORD_NAMES << " ";

	for (unsigned int i = 1; i < Symbol::counter; i++) {

		Symbol * pSymbol = Symbol::getSymbolById(i);
		if (Symbol::isTerminalSymbol(pSymbol->getId())) {

			if (!first) {
				fout << " ";
			} else {
				first = false;
			}

			string name = Symbol::getElementTypeById(i);

			// fout << i << " ";
			fout << "<" << name << ">";
		}
	}

	fout << endl;
}

void TextualGrammarWriter::writeRankInfo()
{
	fout << "# " << KEYWORD_RANKS << " 00 ";
	writeRankInfoAux(false, false);
	fout << endl;

	fout << "# " << KEYWORD_RANKS << " 01 ";
	writeRankInfoAux(false, true);
	fout << endl;

	fout << "# " << KEYWORD_RANKS << " 10 ";
	writeRankInfoAux(true, false);
	fout << endl;
}

void TextualGrammarWriter::writeRankInfoAux(bool leftChild, bool rightChild)
{

	bool first = true;
	for (unsigned int i = 1; i < Symbol::counter; i++)
	{
		Symbol * pSymbol = Symbol::getSymbolById(i);
		if (Symbol::isTerminalSymbol(pSymbol->getId())) {

			if (pSymbol->hasLeftChild() == leftChild && pSymbol->hasRightChild()
					== rightChild)
			{
				if (!first) {
					fout << " ";
				} else {
					first = false;
				}

				if (rawMode)
				{
					fout << pSymbol->getName();
				}
				else
				{
					fout << Symbol::getElementTypeById(pSymbol->getId());
				}
			}
		}
	}
}

void TextualGrammarWriter::writeProduction(Production * pProduction)
{
	fout << pProduction->getLeftHandSide()->getName();
	fout << " -> ";

	DefaultTreePrinter treePrinter(0, fout, rawMode);
	treePrinter.printTree(pProduction->getRightHandSide());

	fout << endl;
}
