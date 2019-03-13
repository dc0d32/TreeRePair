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
#include "GrammarWriter.h"

#include "Helper.h"
#include "TreeRePairConstants.h"

#include <vector>

GrammarWriter::GrammarWriter(Grammar & dagGrammarPar,
		Grammar & mainGrammarPar, char * pFilePathPar) :
	dagGrammar(dagGrammarPar), mainGrammar(mainGrammarPar), pFilePath(
			pFilePathPar)
{
}

GrammarWriter::~GrammarWriter()
{
}

void GrammarWriter::writeProductions()
{
	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
		writeProduction(mainGrammar[i]);
	}
}

void GrammarWriter::writeProductionReverse()
{
	for (unsigned int i = mainGrammar.size(); i > 0; i--) {
		writeProduction(mainGrammar[i - 1]);
	}
}
