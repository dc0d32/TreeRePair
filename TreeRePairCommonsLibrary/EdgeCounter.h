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
#include "TreeRePairMacros.h"
#include "Grammar.h"

/**
 * This class is used to count the edges of a tree.
 */
class EdgeCounter : public TreeTraversal
{
public:
	/**
	 * The constructor of the class.
	 * @param pDagGrammar Can contain an optional DAG grammar. If specified,
	 *  every encountered nonterminal of it is substituted by the subtree
	 *  representing it (i.e., the edges of the subtrees are considered too).
	 * @param countParameters Specifies if edges to parameter nodes shall
	 *  be counted too (default).
	 */
	EdgeCounter(Grammar * pDagGrammar = 0, bool countParameters = true);
	virtual ~EdgeCounter(void);

	/**
	 * This method initiates the counting of the edges.
	 * @param pRoot the root of the tree whose edges shall be counted
	 */
	void countEdges(TreeNode * pRoot);

	/**
	 * Counts the edges of the right-hand sides of the productions of a grammar.
	 * @param grammar the grammar whose size shall be determined
	 */
	void countEdges(Grammar & grammar);

	/**
	 * This method returns the number of edges counted.
	 * @returns the number of edges counted
	 */
	unsigned int getNumberOfEdges() { return numberOfEdges; };

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * If not null, every encountered nonterminal of it is substituted by
	 * the subtree representing it (i.e., the edges of the subtrees are
	 * considered too).
	 */
	Grammar * pDagGrammar;

	/**
	 * Keeps track of the number of edges counted so far.
	 */
	unsigned int numberOfEdges;

	/**
	 * Indicates if edges to parameter nodes shall be counted too.
	 */
	bool countParameters;
};
