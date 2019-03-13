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

#include "Symbol.h"
#include "TreeRePairMacros.h"
#include "TreeRePairMode.h"
#include "Grammar.h"

// #include <map>
#include <iostream>

#if !defined(__GNUC__)
#include <algorithm>
#else
#include <limits.h>
#endif

using namespace std;

/**
 * This class implements a tree node. Furthermore, it implements
 * a doubly linked list of digram occurrences.
 */
class TreeNode {

public:
	/**
	 * The constructor.
	 * @param pSymbol the label of the node
	 * @param rank Specifies the rank of the node (optional parameter). If no
	 *  rank is specified the rank of the symbol is used.
	 */
	TreeNode(Symbol * pSymbol, unsigned int rank = UINT_MAX);

	~TreeNode(void);

	/**
	 * Returns the unique ID of the node.
	 */
	unsigned int getId() const { return id;	};

	/**
	 * Returns the label of the node.
	 */
	Symbol * getSymbol() const { return pSymbol; };

	/**
	 * Sets the label of the node.
	 */
	void setSymbol(Symbol * pSymbolPar);

	/**
	 * Returns the current number of children of the node.
	 */
	unsigned int getCurrentChildrenCount() const {
		return currentChildrenCount - offset;
	};

	/**
	 * Returns a child of the node.
	 * @param i the index of the child to return
	 */
	TreeNode * getChild(unsigned int i) const;

	/**
	 * Sets the child of the node.
	 * @param index the index of the child to set
	 * @param pChild the child node to set
	 */
	void setChild(unsigned int index, TreeNode * pChild);

	/**
	 * Adds a child to the node using the first
	 * unused index.
	 */
	void addChild(TreeNode * pNode);

	/**
	 * Returns the parent node of the node.
	 */
	TreeNode * getParent() const { return pParent; };

	/**
	 * Sets the parent of the node.
	 * @param pNode the parent to set
	 */
	void setParent(TreeNode * pNode) {
		if (pNode == 0) {
			index = 0;
		}
		this->pParent = pNode;
	};

	/**
	 * Sets the next occurrence of the digram formed by the label of this
	 * node and the label of the child with the specified index. This
	 * method is part of the implementation of the digram occurrence queue.
	 * @param index the index of the digram
	 * @param pNextDigramOccurrence the next occurrence of the digram
	 */
	void setNextDigramOccurrence(unsigned int index, TreeNode* pNextDigramOccurrence);

	/**
	 * Returns the next occurrence of the digram formed by the label of this
	 * node and the label of the child with the specified index. This
	 * method is part of the implementation of the digram occurrence queue.
	 * @param index the index of the digram
	 * @returns the next occurrence of the digram
	 */
	TreeNode * getNextDigramOccurrence(unsigned int index);

	/**
	 * Returns the child of the node with the specified index. If this
	 * child is labeled by a nonterminal of the given grammar, then
	 * the root of the right-hand side of this nonterminal is returned.
	 * @param grammar the grammar to be used to resolve nonterminals
	 * @param pNode the node whose child shall be retrieved
	 * @param index the index of the child to be retrieved
	 * @returns the child of the node
	 */
	static TreeNode * getChild(Grammar & grammar, TreeNode * pNode, unsigned int index);

	/**
	 * Sets the previous occurrence of the digram formed by the label of this
	 * node and the label of the child with the specified index. This
	 * method is part of the implementation of the digram occurrence queue.
	 * @param index the index of the digram
	 * @param pPreviousDigramOccurrence the previous occurrence of the digram
	 */
	void setPreviousDigramOccurrence(unsigned int index, TreeNode* pPreviousDigramOccurrence);

	/**
	 * Returns the previous occurrence of the digram formed by the label of this
	 * node and the label of the child with the specified index. This
	 * method is part of the implementation of the digram occurrence queue.
	 * @param index the index of the digram
	 * @returns the previous occurrence of the digram
	 */
	TreeNode * getPreviousDigramOccurrence(unsigned int index);

	/**
	 * Sets the index of this node.
	 */
	void setIndex(unsigned int index) {
		this->index = index;
	};

	/**
	 * Returns the index of this node.
	 */
	unsigned int getIndex() const {
		return index;
	};

	/**
	 * Sets the transitive reference count of this node. The transitive
	 * reference count indicates how many nodes of the tree represented
	 * by the grammar of which the current node is part of are represented
	 * by the current node.
	 */
	void setTransitiveRefCount(unsigned int transitiveRefCountPar) {
		transitiveRefCount = transitiveRefCountPar;
	};

	/**
	 * Returns the transitive reference count of this node.
	 */
	unsigned int getTransitiveRefCount();

	/**
	 * Returns true if and only if this node is an occurrence of the
	 * digram formed by the label of this node and the label of the
	 * child with the specified index.
	 */
	bool isRegisteredOccurrence(HASH_D_D * pDigramHashMap, Grammar & dagGrammar, int index);

#ifdef DEBUG
	static void print(ostream& os, const TreeNode* pNode, bool recursive = true);
#endif

	/**
	 * Keeps track of the number of instances of the TreeNode class.
	 */
	static unsigned int idCounter;

#ifdef DEBUG
	bool isVisited() const {
		return visited;
	};

	void setVisited(bool visitedPar) {
		this->visited = visitedPar;
	};
#endif

private:
	/**
	 * The unique ID of the node.
	 */
	unsigned int id;

	/**
	 * The label of the node.
	 */
	Symbol * pSymbol;

	/**
	 * The rank of the node.
	 */
	unsigned int rank;

	/**
	 * The current children count of the node (less than or equal
	 * to the rank of the node).
	 */
	unsigned int currentChildrenCount;

	/**
	 * The child nodes of this node.
	 */
	TreeNode ** pChildren;

	/**
	 * An array keeping track of all next occurrences of the digrams
	 * formed by the label of this node and the label of the child
	 * with the corresponding array index.
	 */
	TreeNode ** pNextDigramOccurrences;

	/**
	 * An array keeping track of all previous occurrences of the digrams
	 * formed by the label of this node and the label of the child
	 * with the corresponding array index.
	 */
	TreeNode ** pPreviousDigramOccurrences;

	/**
	 * The parent of this node.
	 */
	TreeNode * pParent;

	/**
	 * The child index of this node.
	 */
	unsigned int index;

	/**
	 * The transitive reference count of this node. It indicates how
	 * many nodes of the input tree are represented by this node.
	 */
	unsigned int transitiveRefCount;

	/**
	 * Used to turn a node which was originally created with rank 2
	 * into a node with rank 1 or 0.
	 */
	unsigned int offset;

#ifdef DEBUG
	bool visited;
#endif
};
