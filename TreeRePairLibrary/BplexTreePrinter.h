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
#include "TreeNode.h"

/**
 * Prints the right-hand side of a production of a linear SLCF tree
 * grammar in BPLEX-like format.
 */
class BplexTreePrinter {
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar containing the start production
	 * @param mainGrammar the grammar containing the remaining productions
	 * @param pRoot the root of the tree to print
	 * @param out the output stream to use (default is cout)
	 */
	BplexTreePrinter(Grammar & dagGrammar,
			Grammar & mainGrammar,
			TreeNode * pRoot, ostream & out = cout);
	~BplexTreePrinter(void);

private:
	/**
	 * Initiates the printing process.
	 */
	void traverseTree(void);

	/**
	 * This method is called twice for each node of the tree during a
	 * preorder traversal of it. It must be implemented by the super classes.
	 * @param pNode the current node
	 * @param index the index of the current node
	 * @param depth the depth of the current node
	 * @param closing indicates if the current node is visited on the
	 *  way down to the leafs of the tree (i.e., it is visited for the first
	 *  time) or if it is visited after its children have been visited
	 *  (thats the second and last time it is visited)
	 */
	void processNode(TreeNode * pNode, unsigned int index, int depth,
			bool closing);

	/**
	 * The grammar containing the start production of generated linear SLCF tree
	 * grammar.
	 */
	Grammar & dagGrammar;

	/**
	 * The grammar containing the productions (except the start production)
	 * of the linear SLCF tree grammar.
	 */
	Grammar & mainGrammar;

	/**
	 * The root of the tree to print.
	 */
	TreeNode * pRoot;

	/**
	 * The output stream used to write to the output file.
	 */
	ostream & out;
};
