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
#include "Grammar.h"

#include <stack>

/**
 * This class creates a deep copy of a tree.
 */
class TreeCopier : TreeTraversal
{
public:
	/**
	 * The constructor.
	 * @param grammar If not null, the copy process continues on
	 *  the right-hand sides of nonterminals encountered during the
	 *  traversal.
	 * @param pRoot The root of the tree to be copied.
	 */
	TreeCopier(Grammar * grammar, TreeNode * pRoot);
	~TreeCopier(void);

	/**
	 * Returns the copy.
	 * @returns the root of the copy
	 */
	TreeNode * getNewRoot() { return pNewRoot; };

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * If not null, the copy process continues on the right-hand sides
	 * of the nonterminals encountered during the traversal.
	 */
	Grammar * grammar;

	/**
	 * The root of the copy.
	 */
	TreeNode * pNewRoot;

	/**
	 * Keeps track of the parent nodes of the current node.
	 */
	stack<TreeNode*> parents;
};
