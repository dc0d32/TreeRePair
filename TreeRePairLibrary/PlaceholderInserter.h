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

#include "TreeTraversal.h"
#include "TreeRePairTypes.h"
#include "Grammar.h"

/**
 * This class is for testing purposes only. It replaces terminal symbols
 * of the same element type but different children characteristics by the
 * Symbol instance with the highest number of children and inserts
 * place holder nodes at the corresponding children positions.
 */
class PlaceholderInserter
	: TreeTraversal
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar containing the input tree
	 */
	PlaceholderInserter(Grammar & dagGrammar);
	~PlaceholderInserter(void);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * The grammar containing the input tree.
	 */
	Grammar & dagGrammar;

	/**
	 * The production whose right-hand side is currently processed.
	 */
	Production * pCurrentProduction;

	/**
	 * The terminal symbol which replaces the original terminal symbol with
	 * a lower rank.
	 */
	Symbol * pNewSymbol;

	/**
	 * The terminal symbol which is replaced by a symbol with a higher rank.
	 */
	Symbol * pOldSymbol;


	// unsigned int counter;
	// unsigned int transtiveCounter;
	// bool justCount;
};
