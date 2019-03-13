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

#include "TreePrinter.h"
#include "Grammar.h"

/**
 * This tree printer prints a human readable representation
 * of a tree (which can be given as a DAG). It is used by the
 * TextualGrammarWriter class.
 */
class DefaultTreePrinter :
	public TreePrinter
{
public:
	/**
	 * The constructor of the class.
	 * @param grammar The grammar representing the tree which shall
	 *  be printed. If, during the printing, a nonterminal of this
	 *  grammar is encountered, it is automatically substituted by
	 *  the subtree it represents.
	 * @param out Specifies the output stream which shall be used.
	 *  Default is cout (standard output).
	 * @param rawMode If true, for each symbol its assigned integer is
	 *  written to the output stream. If false, the corresponding element
	 *  type is written out. Default is false.
	 */
	DefaultTreePrinter(Grammar * grammar, ostream & out = cout, bool rawMode = false);
	~DefaultTreePrinter(void);

	/**
	 * Initiates the printing of the subtree which is rooted at the
	 * specified node.
	 * @param pNode the root of the tree which shall be printed
	 */
	void printTree(TreeNode * pNode);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * The grammar representing the tree which shall
	 * be printed. If, during the printing, a nonterminal of this
	 * grammar is encountered, it is automatically substituted by
	 * the subtree it represents.
	 */
	Grammar * grammar;
};
