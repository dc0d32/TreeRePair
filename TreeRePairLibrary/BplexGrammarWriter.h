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
#pragma once

#include "GrammarWriter.h"
#include "Grammar.h"

#include <fstream>
#include <deque>

/**
 * Write a linear SLCF tree grammar to an output file using a BPLEX-like
 * format.
 */
class BplexGrammarWriter: public GrammarWriter
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar containing the start production of
	 *  the linear SLCF tree grammar
	 * @param mainGrammar the grammar containing the remaining productions
	 * @param pFilePath the file to write to
	 */
	BplexGrammarWriter(Grammar & dagGrammar,
			Grammar & mainGrammar, char * pFilePath);
	virtual ~BplexGrammarWriter(void);

	/**
	 * Returns the name for a symbol to use within the BPLEX-like output.
	 */
	static string getNameForSymbol(Grammar & mainGrammar, Symbol * pSymbol);

private:
	/**
	 * Used to number the parameter symbols within a right-hand side of
	 * a production to write out.
	 */
	static unsigned int parameterIndex;

	virtual void writeProduction(Production * pProduction);
};

