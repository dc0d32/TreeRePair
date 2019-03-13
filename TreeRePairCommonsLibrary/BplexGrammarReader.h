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

#include "TreeRePairTypes.h"
#include "Grammar.h"

#include <stack>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * This class retrieves the linear SLCF tree grammar from a file
 * in BPLEX-like format.
 */
class BplexGrammarReader
{
public:
	/**
	 * The constructor.
	 * @param pFilePath the file containing the linear SLCF tree
	 *  grammar in BPLEX-like format
	 * @param dagGrammar the grammar instance to which the start
	 *  production shall be written to
	 * @param mainGrammar the grammar instance which serves as a
	 *  container of the remaining productions
	 */
	BplexGrammarReader(char * pFilePath,
			Grammar & dagGrammar, Grammar & mainGrammar);
	virtual ~BplexGrammarReader();

private:
	/**
	 * Reconstructs the production represented by the specified string.
	 * @param grammar the grammar to which the production shall be added
	 * @param strProduction the string which represents the production
	 *  in a BPLEX-like format
	 */
	void retrieveProduction(Grammar & grammar, string & strProduction);

	/**
	 * Reconstructs the rank of a nonterminal by parsing its right-hand
	 * side.
	 * @param strRightHandSide the right-hand side to parse
	 * @returns the rank of the nonterminal
	 */
	unsigned int retrieveRank(string strRightHandSide);

	/**
	 * Retrieves the corresponding Symbol instance from the symbols hash
	 * map.
	 * @param strName the name of the symbol to retrieve
	 * @returns the corresponding Symbol instance
	 */
	Symbol * retrieveSymbol(string & strName);

	/**
	 * Creates a Symbol instance to use for the specified symbol name. If
	 * the specified name starts with an "y" the unique parameter symbol
	 * is returned (see Symbol::getParameterSymbol()).
	 */
	Symbol * createTerminalSymbol(string & strName);

	/**
	 * Creates a Symbol instance which represents the nonterminal with
	 * the specified name and rank.
	 */
	Symbol * createNonTerminalSymbol(string & strName, unsigned int rank);

	/**
	 * Reads the next symbol name (relative to the specified index) within a
	 * tree given by a BPLEX-like formatted string.
	 */
	string readNextSymbol(string & strTree, unsigned int * currentIndex);

	/**
	 * Reconstructs the right-hand side which is represented by the specified
	 * string in BPLEX-like format.
	 */
	TreeNode * readRightHandSide(string & strTree);

	/**
	 * The grammar which serves as a container for the start production of
	 * the linear SLCF tree grammar to reconstruct.
	 */
	Grammar & dagGrammar;

	/**
	 * The grammar which serves as a container of the productions (except the
	 * start production) of the linear SLCF tree grammar to reconstruct.
	 */
	Grammar & mainGrammar;

	/**
	 * The hash map allowing constant time access to the Symbol instances by
	 * specifying their name.
	 */
	HASH_ST_S nameSymbolsHashMap;
};
