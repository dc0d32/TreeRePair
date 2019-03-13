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
#include "TreeNode.h"

#include <set>
#include <deque>
#include <vector>

/**
 * This class is used by TreeDePair (the decompressor) to replace
 * all nonterminals of the input linear SLCF tree grammar by their
 * right-hand sides.
 */
class SymbolReplacer :
	public TreeTraversal
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar containing the start production
	 * @param mainGrammar the grammar containing the prodcutions (except
	 *  the start production) of the input linear SLCF tree grammar
	 */
	SymbolReplacer(Grammar & dagGrammar, Grammar & mainGrammar);
	~SymbolReplacer(void);

	/**
	 * Initiates the replacement process for all nonterminals of the specified
	 * grammar.
	 * @param symbolReplacer the SymbolReplacer instance to use for the
	 *  replacement process
	 * @param mainGrammar the grammar of which all nonterminals shall be replaced
	 *  by their right-hand sides
	 */
	static void replaceNonterminals(SymbolReplacer & symbolReplacer, Grammar & mainGrammar);

	/**
	 * Replaces all nodes which are labeled by the left-hand side of the
	 * specified production.
	 */
	void replaceNonterminal(Production * pProduction);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * Replaces the specified tree node (which is supposed to be labeled
	 * by the left-hand side of the current production) with the right-hand
	 * side of the current production.
	 */
	TreeNode * replaceTreeNode(TreeNode * pNode);

	/**
	 * Replaces every node contained in the occurrences queue by the
	 * right-hand side of the current production.
	 */
	void processQueue();

	/**
	 * The grammar containing the start production of the
	 * linear SLCF tree grammar.
	 */
	Grammar & dagGrammar;

	/**
	 * The grammar containing the productions (except the start
	 * production) of the linear SLCF tree grammar.
	 */
	Grammar & mainGrammar;

	/**
	 * The current production whose left-hand side is replaced
	 * its right-hand side.
	 */
	Production * pCurrentProduction;

	/**
	 * Contains all nodes labeled by the left-hand side of the current
	 * production.
	 */
	deque<TreeNode *> occurrencesQueue;

	/**
	 * It is used to check in constant time if a particular node is
	 * already contained in the occurrences queue.
	 */
	set<TreeNode *> occurrencesSet;
};
