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
#include "TreeRePairTypes.h"
#include "NodeHasher.h"

#include <vector>

class TreeNode;
class Production;

/**
 * This class implements a linear SLCF tree grammar using the vector class.
 */
class Grammar
{
public:
	/**
	 * The constructor.
	 */
	Grammar(void);
	~Grammar(void);

	/**
	 * This operator allows constant time access to the productions of the
	 * grammar.
	 * @param index the index of the production
	 * @returns the requested production
	 */
	Production * operator[] (const int index);

	/**
	 * This method returns the current size of the grammar.
	 * @returns the size of the grammar
	 */
	unsigned int size() { return productions.size(); };

	/**
	 * This method returns an iterator referring to the
	 * first production of the grammar.
	 * @returns a iterator to the first production
	 */
	vector<Production *>::iterator begin() { return productions.begin(); };

	/**
	 * This method returns an iterator referring to the
	 * last production of the grammar.
	 * @returns a iterator to the last production
	 */
	vector<Production *>::iterator end() { return productions.end(); };

	/**
	 * This method adds a production to the grammar.
	 * @param pProduction the production to be added
	 */
	void addProduction(Production * pProduction);

	/**
	 * Removes a production from the grammar using a iterator. Needs constant
	 * time plus the number of productions after the removed production.
	 * @param it an iterator referring to the production to be removed
	 */
	void removeProduction(vector<Production *>::iterator it);

	/**
	 * Removes a production from the grammar by searching for the
	 * specified production and then removing it. Needs linear time
	 * (on the size of the grammar) plus the number of productions after
	 * the removed production.
	 * @param pProduction the production to be removed
	 */
	void removeProduction(Production * pProduction);

	/**
	 * Updates the right-hand side of a production.
	 * @param pProduction the production to be updated
	 * @param pNewRightHandSide the right-hand side to set
	 */
	void updateRightHandSide(Production * pProduction, TreeNode * pNewRightHandSide);

	/**
	 * Returns a production with a certain left-hand side in constant time.
	 * @param pSymbol the left-hand side of the production
	 * @returns the production with the specified left-hand side
	 */
	Production * getProductionBySymbol(Symbol * pSymbol);

	/**
	 * Returns a production with a certain right-hand side in constant time.
	 * @param pRightHandSide the right-hand side of the production
	 * @returns the production with the specified right-hand side
	 */
	Production * getProductionByRightHandSide(TreeNode * pRightHandSide);

	/**
	 * Checks if a given symbol is a nonterminal.
	 * @param pSymbol the symbol to check
	 * @returns true if the specified symbol is a nonterminal
	 */
	bool isNonTerminal(Symbol * pSymbol);

	/**
	 * Replaces a nonterminal by its right-hand side.
	 * @param pNodeToBeReplaced tree node labeled by a nonterminal of the grammar
	 * @param pProduction the corresponding production
	 */
	static void applyProduction(TreeNode * pNodeToBeReplaced, Production * pProduction);

	/**
	 * Deep copies the grammar.
	 * @returns a deep copy of the grammar
	 */
	Grammar * copy();

private:
	/**
	 * The productions of the grammar.
	 */
	vector<Production*> productions;

	/**
	 * The hash map which allows constant time access to the productions
	 * of the grammar by specifying their left-hand side.
	 */
	HASH_I_P productionBySymbolIdHashMap;

	/**
	 * The hash map which allows constant time access to the productions
	 * of the grammar by specifying the root of their right-hand side.
	 */
	HASH_N_P productionByRightHandSideHashMap;
};
