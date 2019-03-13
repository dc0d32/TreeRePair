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
 * This class is used to count the references count of the productions
 * generated during the replacement step, i.e., it is determined how many
 * nodes are labeled with the left-hand side of a certain production.
 */
class ReferencesTreeCounter
	: TreeTraversal
{
public:
	/**
	 * The constructor.
	 * @param pRoot the root of the tree to traverse
	 * @param dagGrammar the optional grammar whose nonterminals are
	 *  replaced by their right-hand sides (i.e., traversing continues
	 *  on the subtrees)
	 * @param mainGrammar the grammar of the productions generated during
	 *  the replacement step; it contains the nonterminals for which the
	 *  number of occurrences shall be determined
	 */
	ReferencesTreeCounter(TreeNode * pRoot,
			Grammar * dagGrammar, Grammar & mainGrammar);
	~ReferencesTreeCounter(void);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * Checks if the specified node is labeled by a nonterminal of the main
	 * grammar and if so updates the reference counter of the corresponding
	 * production.
	 */
	void countReference(TreeNode * pNode);

	/**
	 * The optional grammar whose nonterminals are
	 * replaced by their right-hand sides (i.e., traversing continues
	 * on the subtrees).
	 */
	Grammar * dagGrammar;

	/**
	 * The grammar of the productions generated during
	 * the replacement step. It contains the nonterminals for which the
	 * number of occurrences shall be determined.
	 */
	Grammar & mainGrammar;
};
