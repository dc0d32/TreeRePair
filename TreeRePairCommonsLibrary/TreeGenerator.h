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

#include "Grammar.h"
#include "TreeNode.h"

class Symbol;

#include <string>

using namespace std;

/**
 * This class is the base class used by all tree generators. These
 * tree generators are used to generate artificial trees for
 * testing purposes only.
 */
class TreeGenerator
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar which serves as a container for
	 *  the tree
	 */
	TreeGenerator(Grammar & dagGrammar);
	~TreeGenerator(void) {};

protected:
	/**
	 * Creates a start production with the specified right-hand side.
	 * @param pRightHandSide the right-hand side to use
	 */
	void createStartProduction(TreeNode * pRightHandSide);

	/**
	 * The grammar which serves as a container for the generated tree.
	 */
	Grammar & dagGrammar;
};

/**
 * A tree generator generating the set of trees which is used
 * in the paper to show that a maximal rank of 1 can lead to
 * a better compression performance than an unlimited maximal
 * rank.
 */
class DegeneratedTreeGenerator : public TreeGenerator
{
public:
	/**
	 * the constructor of the class.
	 * @param dagGrammar the grammar which serves as a container for the
	 *  tree
	 * @param depth the depth of the tree to generate
	 */
	DegeneratedTreeGenerator(Grammar & dagGrammar, unsigned int depth);
	~DegeneratedTreeGenerator() {};
private:
	/**
	 * Recursive method used to construct the tree.
	 */
	TreeNode * createTreeAux(unsigned int curDepth, unsigned int maxDepth);

	/**
	 * The symbol labeling all inner nodes of the tree.
	 */
	Symbol * pBinaryTreeInnerSymbol;

	/**
	 * The symbols used to label the leaves of the tree.
	 */
	vector<Symbol *> leafSymbols;
};

/**
 * A tree generator generating a perfect binary tree of which each inner node
 * is labeled by the terminal symbol f, each left leaf is labeled by a
 * and each right leaf is labeled by b.
 */
class BinaryTreeGenerator : public TreeGenerator
{
public:
	/**
	 * The constructor of the tree.
	 * @param dagGrammar the grammar which serves as a container for the
	 *  tree
	 * @param depth the depth of the tree to generate
	 */
	BinaryTreeGenerator(Grammar & dagGrammar, int depth);
	~BinaryTreeGenerator() {};
private:
	/**
	 * Recursive method used to construct the tree.
	 */
	TreeNode * createBinaryTreeAux(unsigned int curDepth, unsigned int maxDepth, bool left = true);

	/**
	 * The symbol used to label all inner nodes of the tree.
	 */
	Symbol * pBinaryTreeInnerSymbol;

	/**
	 * The symbol used to label all left leaves of the binary tree.
	 */
	Symbol * pBinaryTreeLeftLeafSymbol;

	/**
	 * The symbol used to label all right leavess of the binary tree.
	 */
	Symbol * pBinaryTreeRightLeafSymbol;
};

/**
 * A tree generator generating a perfect binary tree of which each inner node
 * is labeled by the terminal symbol f and each leaf is labeled by a unique
 * terminal symbol, i.e., there do not exist to different leafs which are
 * labeled by the same terminal symbol.
 */
class BinaryTreeGenerator2 : public TreeGenerator
{
public:
	/**
	 * The constructor of the tree.
	 * @param dagGrammar the grammar which serves as a container for the
	 *  tree
	 * @param depth the depth of the tree to generate
	 */
	BinaryTreeGenerator2(Grammar & dagGrammar, int depth);
	~BinaryTreeGenerator2() {};
private:
	/**
	 * Recursive method used to construct the tree.
	 */
	TreeNode * createBinaryTreeAux(unsigned int curDepth, unsigned int maxDepth);

	/**
	 * The terminal symbol used to label all inner nodes.
	 */
	Symbol * pBinaryTreeInnerSymbol;

	/**
	 * Used to assign every leaf a unique symbol.
	 */
	unsigned int counter;
};
