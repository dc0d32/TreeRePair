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
#include "TreeRePairTypes.h"
#include "TreeRePairTypes.h"
#include "Grammar.h"

#include <iostream>
#include <vector>

class Symbol;
class TreeNode;

using namespace std;

/**
 * This class implements a production of a linear SLCF tree grammar. It
 * keeps track of its reference count (how many nodes are labeled by the
 * left-hand side of this production) and of its transitive reference
 * count (how often does the right-hand side of this production occur in
 * the tree which is represented by the corresponding grammar). In addition
 * it maintains a hashed set of all nodes labeled by its left-hand side.
 */
class Production
{
public:
	/**
	 * The constructor.
	 * @param pLeftHandSide the left-hand side of the production
	 * @param pRightHandSide the right-hand side of the production
	 */
	Production(Symbol * pLeftHandSide, TreeNode * pRightHandSide);
	~Production(void);

	/**
	 * Necessary in order to be able to call the method setRightHandSide
	 * from within the Grammar::updateRightHandSide method.
	 */
	friend void Grammar::updateRightHandSide(Production * pProduction, TreeNode * pNewRightHandSide);

	/**
	 * Creates a new instance of the Production class.
	 * @param pLeftHandSide the left-hand side of the production to
	 *  be created
	 * @param pDigram The tree pattern which is represented by this
	 *  digram will serve as the right-hand side of the production.
	 * @returns the generated production
	 */
	static Production * createProduction(Symbol * pLeftHandSide, Digram * pDigram);

	/**
	 * Deletes an instance of the Production class and the right-hand side
	 * of it.
	 */
	static void deleteProduction(Production * pProduction);

#ifdef DEBUG
	static void print(ostream& os, const Production * pProduction);
#endif

	/**
	 * Returns the left-hand side of this production.
	 */
	Symbol * getLeftHandSide() { return pLeftHandSide; };

	/**
	 * Returns the right-hand side of this production.
	 */
	TreeNode * getRightHandSide() { return pRightHandSide; };

	/**
	 * Creates a deep copy of this production.
	 * @returns a deep copy of the production
	 */
	Production * copy();

	/**
	 * Returns the node labeled by a parameter symbol in constant
	 * time.
	 * @param index the index of the parameter symbol
	 * @returns the node labeled by the specified parameter symbol
	 */
	TreeNode * getParameter(unsigned int index) { return pParameters[index]; };

	/**
	 * Updates the node which is labeled by a parameter symbol.
	 * @param index the index of the parameter by which the node is labeled
	 * @param pParameterNode the node labeled by the specified parameter
	 */
	void setParameter(unsigned int index, TreeNode * pParameterNode) { pParameters[index] = pParameterNode; };

	/**
	 * Returns the reference count of this production. The reference count
	 * indicates how many nodes in the corresponding grammar are labeled by
	 * the left-hand side of this production.
	 * @returns the reference count of this production
	 */
	unsigned int getRefCount() { return refCount; };

	/**
	 * Updates the reference count of this production.
	 * @param newRefCount the new reference count to set
	 */
	void setRefCount(unsigned int newRefCount) { refCount = newRefCount; };

	/**
	 * Increments the reference count of this production by one.
	 */
	void incrementRefCount() { refCount++; };

	/**
	 * Decrements the reference count by a certain amount.
	 * @param quantity the amount to subtract from the current
	 *  reference count (default is 1)
	 */
	void decrementRefCount(unsigned int quantity = 1);

	/**
	 * Returns the transitive reference count of this production. The
	 * transitive reference count indicates how often the right-hand side
	 * of this production occurs in the tree which is represented by the
	 * corresponding grammar.
	 * @returns the transitive reference count of this production
	 */
	unsigned int getTransitiveRefCount() { return transitiveRefCount; };

	/**
	 * Updates the transitive reference count of this production.
	 */
	void setTransitiveRefCount(unsigned int newTransitiveRefCount) { transitiveRefCount = newTransitiveRefCount; };

	/**
	 * Increments the transitive reference count by one.
	 */
	void incrementTransitiveRefCount() { transitiveRefCount++; };

	/**
	 * Decrements the transitive reference count by a certain amount.
	 * @param quantity the amount to subtract from the current
	 *  transitive reference count (default is 1)
	 */
	void decrementTransitiveRefCount(unsigned int quantity = 1);

	/**
	 * Returns a hashed set of all nodes labeled by the left-hand side
	 * of this production.
	 */
	HASH_N * getReferencingNodes() { return &referencingNodes; };

	/**
	 * Adds a node to the hashed set of all nodes labeled by the left-hand side
	 * of this production.
	 * @param pNode the node to add
	 */
	void addRefNode(TreeNode * pNode);

	/**
	 * Removes a node to the hashed set of all nodes labeled by the left-hand side
	 * of this production.
	 * @param pNode the node to remove
	 */
	void removeRefNode(TreeNode * pNode);

private:
	/**
	 * Updates the right-hand side of this production. This method
	 * is called by the friendly method Grammar::updateRightHandSide().
	 */
	void setRightHandSide(TreeNode * pNewRightHandSide);

	/**
	 * The left-hand side of this production.
	 */
	Symbol * pLeftHandSide;

	/**
	 * The right-hand side of this production.
	 */
	TreeNode * pRightHandSide;

	/**
	 * The reference count of this production. It indicates how
	 * many nodes of the corresponding grammar are labeled by the
	 * left-hand side of this production.
	 */
	unsigned int refCount;

	/**
	 * The transitive reference count of this production. It indicates
	 * how often the right-hand side of this production occurs in
	 * the tree which is represented by the corresponding grammar.
	 */
	unsigned int transitiveRefCount;

	/**
	 * A hashed set of all nodes of the right-hand sides of the
	 * corresponding grammar labeled by the left-hand side of this
	 * production.
	 */
	HASH_N referencingNodes;

	/**
	 * Pointers to the nodes of the right-hand side of this production
	 * which are labeled by a parameter symbol.
	 */
	TreeNode ** pParameters;
};
