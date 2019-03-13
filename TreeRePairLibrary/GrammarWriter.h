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

#include "Grammar.h"
#include "TreeRePairTypes.h"
#include "TreeRePairMacros.h"

#include <fstream>

/**
 * The base class used by all grammar writers (i.e., it is used by all
 * classes writing a representation of a linear SLCF tree grammar to
 * an output file).
 */
class GrammarWriter
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar containing the start production of
	 *  the grammar to output
	 * @param mainGrammar the grammar containing the remaining productions
	 * @param pFilePath the output file to use
	 */
	GrammarWriter(Grammar & dagGrammar, Grammar & mainGrammar,
			char * pFilePath);

	virtual ~GrammarWriter();

protected:
	/**
	 * Writes all productions of the grammar to the output file.
	 */
	void writeProductions();

	/**
	 * Writes all productions of the grammar to the output file in their
	 * reverse order.
	 */
	void writeProductionReverse();

	/**
	 * Writes a production of the grammar to the output file. This method
	 * must be implemented by a super class of this class.
	 * @param pProduction the production to output
	 */
	virtual void writeProduction(Production * pProduction) = 0;

	/**
	 * The grammar containing the start production of the linear SLCF tree
	 * grammar to output.
	 */
	Grammar & dagGrammar;

	/**
	 * The grammar containing the remaining productions of the linear SLCF
	 * tree grammar to output.
	 */
	Grammar & mainGrammar;

	/**
	 * The output stream which is used to write to the output file.
	 */
	ofstream fout;

	/**
	 * The output file to use.
	 */
	char * pFilePath;
};
