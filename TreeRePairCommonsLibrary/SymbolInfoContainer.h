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

#include <string>

using namespace std;

/**
 * This class stores the combination of an XML element type and its
 * children characteristics as it occurs in the (binary or multiary)
 * ranked representation of the input tree.
 */
class SymbolInfoContainer
{
public:
	/**
	 * The constructor.
	 * @param elementType the element type
	 * @param childCount the number of children of the corresponding occurrence
	 *  of this element type (only used when constructing a multiary tree)
	 */
	SymbolInfoContainer(string elementType, unsigned int childCount = 0);
	~SymbolInfoContainer(void);

	/**
	 * Returns true if the combination of the element type and children
	 * characteristic which is represented by this container has a left
	 * child. This method is only used when dealing with the binary model.
	 */
	bool hasLeftChild() const { return leftChild; };

	/**
	 * Specifies that the combination of the element type and children
	 * characteristic which is represented by this container has a left
	 * child. This method is only used when dealing with the binary model.
	 */
	void setHasLeftChild(bool leftChildPar) { this->leftChild = leftChildPar; };

	/**
	 * Returns true if the combination of the element type and children
	 * characteristic which is represented by this container has a right
	 * child. This method is only used when dealing with the binary model.
	 */
	bool hasRightChild() const { return rightChild; };

	/**
	 * Specifies that the combination of the element type and children
	 * characteristic which is represented by this container has a right
	 * child. This method is only used when dealing with the binary model.
	 */
	void setHasRightChild(bool rightChildPar) { this->rightChild = rightChildPar; };

	/**
	 * Returns the number of children of the combination of element type
	 * and children characteristic which is represented by this container.
	 * Only used when dealing with a multiary tree.
	 */
	unsigned int getChildCount() const { return childCount; };

	/**
	 * Returns the element type represented by this container.
	 */
	string getElementType() const { return elementType; };

private:
	/**
	 * The element type represented by this container.
	 */
	string elementType;

	/**
	 * Indicates that the combination of the element type and children
	 * characteristic which is represented by this container has a left
	 * child. This method is only used when dealing with the binary model.
	 */
	bool leftChild;

	/**
	 * Indicates that the combination of the element type and children
	 * characteristic which is represented by this container has a right
	 * child. This method is only used when dealing with the binary model.
	 */
	bool rightChild;

	/**
	 * The number of children of the combination of element type
	 * and children characteristic which is represented by this container.
	 * Only used when dealing with a multiary tree.
	 */
	unsigned int childCount;
};
