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
#include "TreeNode.h"

/**
 * This class is used to update the reference counters and the hashed
 * sets containing the referencing nodes of the productions of the
 * specified grammar.
 */
class RefCountUpdater : TreeTraversal
{
public:
	/**
	 * The constructor.
	 * @param mainGrammar the grammar to update
	 * @param pNode the node which shall be added or removed from the
	 *  hashed sets of referencing nodes
	 * @param add (please see the implementation of the processNode method)
	 */
	RefCountUpdater(Grammar & mainGrammar, TreeNode * pNode, int add);
	~RefCountUpdater();

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * The grammar to update.
	 */
	Grammar & mainGrammar;

	/**
	 * (please see the implementation of the processNode method)
	 */
	int add;
};
