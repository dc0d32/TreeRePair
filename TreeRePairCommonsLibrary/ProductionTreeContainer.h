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

class Production;
class TreeNode;

/**
 * This class is used when on-the-fly generating the DAG of a tree. It
 * saves a pointer to the first occurrence of a tree and a pointer to
 * the production representing this tree, respectively.
 *
 * More detailed, if a shareable tree is encountered for the first time
 * a new instance of the ProductionTreeContainer class is created. The
 * latter contains a pointer to this tree. Later, if this tree is
 * encountered a second time, the production tree container is retrieved
 * using a hash table. A production representing the tree is created
 * and the current occurrence and the first occurrence (which can be
 * accessed using the pointer of the production tree container) is
 * replaced by the left-hand side of this production. The production
 * tree container is updated such that in points to the production
 * instance. That way, the latter can be reused if another copy of the
 * corresponding tree is encountered.
 */
class ProductionTreeContainer
{
public:
	/**
	 * The constructor.
	 * @param pNode the first occurrence of the tree represented by this
	 *  container
	 */
	ProductionTreeContainer(TreeNode * pNode);
	~ProductionTreeContainer(void);

	/**
	 * Sets the first occurrence pointer to null. This is done after the
	 * first occurrence of the tree was replaced by a nonterminal representing
	 * the tree.
	 */
	void setFirstOccurrencePointerToNull() { pNode = 0; };

	/**
	 * Sets the production whose right-hand side is equal to the tree
	 * which is represented by this container.
	 * @param pNewProduction the production to set
	 */
	void setProduction(Production * pNewProduction) { pProduction = pNewProduction; };

	/**
	 * Returns the first occurrence of the tree represented by this
	 * cotnainer.
	 */
	TreeNode * getFirstOccurrenceOfTree() { return pNode; };

	/**
	 * Returns the production whose right-hand side is equal to the tree
	 * which is represented by this container.
	 */
	Production * getProduction() { return pProduction; };

private:
	/**
	 * The production whose right-hand side equals the tree represented
	 * by this container.
	 */
	Production * pProduction;

	/**
	 * The root of the tree which is represented by this container.
	 */
	TreeNode * pNode;
};
