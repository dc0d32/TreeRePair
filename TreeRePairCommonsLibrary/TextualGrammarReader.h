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

#include "TreeRePairMacros.h"
#include "Grammar.h"

#include <fstream>
#include <stack>

/**
 * This class is used to retrieve a linear SLCF tree grammar from
 * its textual representation (which was written out using the
 * TextualGrammarWriter class).
 */
class TextualGrammarReader
{
public:
	/**
	 * The constructor of the class.
	 * @param pFilePath the file containing the textual representation
	 *  of the linear SLCF tree grammar
	 * @param dagGrammar the grammar which shall be used as a container
	 *  of the start production of the linear SLCF tree grammar
	 * @param mainGrammar the grammar which will contain all other
	 *  productions of the grammar retrieved
	 */
	TextualGrammarReader(const char * pFilePath,
			Grammar & dagGrammar, Grammar & mainGrammar);

	~TextualGrammarReader(void);

private:
	/**
	 * Constructs a Production instance using the specified symbol
	 * (as the left-hand side) and by parsing the given textual
	 * representation of the right-hand side.
	 */
	Production * createProductionAux(unsigned int newSymbolName,
				const string &strRightSide);

	/**
	 * Initiates the parsing of the input file.
	 */
	void parse();

	/**
	 * Parses one line of the input file.
	 */
	void parseLine(const string &line);

	/**
	 * Interpretes a line of the input file as a textual
	 * representation of a production and tries to reconstruct
	 * it. If successful, it adds it to the grammar.
	 */
	void parseProduction(const string &line);

	/**
	 * Parses lines of the input file containing informations
	 * about the ranks of the symbols, the names of the symbols
	 * etc. and incorporates this information into the linear
	 * SLCF tree grammar.
	 */
	void parseInfo(const string &line);

	/**
	 * The grammar which will serve as a container for the start
	 * production of the grammar to reconstruct.
	 */
	Grammar & dagGrammar;

	/**
	 * the grammar which will serve as a contianer for the productions
	 * (except the start production) of the grammar to reconstruct.
	 */
	Grammar & mainGrammar;

	/**
	 * Keeps track of the number of productions parsed until
	 * now.
	 */
	int counter;

	/**
	 * The input stream which is used to read from the file
	 * containing the textual representation of the linear
	 * SLCF tree grammar.
	 */
	ifstream * ifs;
};
