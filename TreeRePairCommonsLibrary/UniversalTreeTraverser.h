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
 * This class is used for testing purposes only. It traverses
 * a tree and can execute custom actions.
 */
class UniversalTreeTraverser : public TreeTraversal
{
public:
	/**
	 * The constructor of the class.
	 */
	UniversalTreeTraverser(Grammar * pGrammar, TreeNode * pRoot);
	virtual ~UniversalTreeTraverser(void);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * The optional grammar to use to continue the traversal on
	 * subtrees represented by a nonterminal of the grammar.
	 */
	Grammar * pGrammar;
};
