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

#include <fstream>

/**
 * Writes out a textual (human readable) representation of a
 * linear SLCF tree grammar.
 */
class TextualGrammarWriter: public GrammarWriter
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar containing the start production
	 *  of the linear SLCF tree grammar
	 * @param mainGrammar the grammar containing the remaining
	 *  productions of the linear SLCF tree grammar
	 * @param pFilePath the output file to write to
	 * @param rawMode if true (default), the name (an integer) of the
	 *  symbols is written out; otherwise, the element type is used
	 *  (however, in this case the output file cannot be used to
	 *  reconstruct the grammar)
	 */
	TextualGrammarWriter(Grammar & dagGrammar,
			Grammar & mainGrammar, char * pFilePath,
			bool rawMode = true);
	virtual ~TextualGrammarWriter(void);

protected:
	virtual void writeProduction(Production * pProduction);

private:
	/**
	 * Write the element types of the terminal symbols to the
	 * output file.
	 */
	void writeNameInfo();

	/**
	 * Writes the ranks of the terminal symbols to the output file.
	 */
	void writeRankInfo();

	/**
	 * Writes an enumeration of the terminal symbols with the
	 * specified children characteristic to the output file.
	 */
	void writeRankInfoAux(bool leftChild, bool rightChild);

	/**
	 * If true (default), the name (an integer) of the
	 * symbols is written out. Otherwise, the element type is used
	 * (however, in this case the output file cannot be used to
	 * reconstruct the grammar).
	 */
	bool rawMode;
};

