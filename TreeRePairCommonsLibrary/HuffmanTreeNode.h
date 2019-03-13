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
 * This tree implements a node of a Huffman tree. In particular, it
 * is used to keep track of the frequency of a input symbol which
 * needs to be encoded and of the Huffman code which is assigned to
 * this value.
 */
class HuffmanTreeNode
{
public:
	HuffmanTreeNode(void);
	~HuffmanTreeNode(void);

	/**
	 * Increases the frequency counter of the input symbol which is
	 * represented by this Huffman tree node.
	 * @param add the value to add to the frequency counter
	 */
	void addToFrequency(unsigned int add) { frequency = frequency + add; };

	/**
	 * Returns the frequency of the input symbol which is represented
	 * by this tree node.
	 * @returns the frequency of the input symbol
	 */
	int getFrequency() { return frequency; };

	/**
	 * Updates the frequency counter of the input symbol which is
	 * represented by this tree node.
	 * @param newValue the new value of the frequency counter
	 */
	void setFrequency(int newValue) { frequency = newValue; };

	/**
	 * Updates the left child of this tree node.
	 * @param pNode the new left child
	 */
	void setLeftChild(HuffmanTreeNode * pNode) {
		pLeftChild = pNode;
		pLeftChild->setParent(this);
		pLeftChild->setIndex(0);
	};

	/**
	 * Updates the right child of this tree node.
	 * @param pNode the new right child
	 */
	void setRightChild(HuffmanTreeNode * pNode) {
		pRightChild = pNode;
		pRightChild->setParent(this);
		pRightChild->setIndex(1);
	};

	/**
	 * Updates one of the childs of the tree node
	 * @param index the index of the child to update
	 * @param pChild the new child at the specified index
	 */
	void setChild(unsigned int index, HuffmanTreeNode * pChild);

	/**
	 * Updates the parent of this tree node.
	 * @param pNode the new parent node to set
	 */
	void setParent(HuffmanTreeNode * pNode) { pParent = pNode; };

	/**
	 * Returns the parent of this tree node.
	 * @returns the parent node
	 */
	HuffmanTreeNode * getParent() { return pParent; };

	/**
	 * Updates the index of this tree node which indicates if
	 * this tree node is a left or right child.
	 * @param newIndex the new index to set
	 */
	void setIndex(int newIndex) { index = newIndex; };

	/**
	 * Returns the index of this tree node.
	 * @returns the index
	 */
	int getIndex() { return index; };

	/**
	 * Sets the symbol which shall be represented by this Huffman tree
	 * node.
	 * @param newSymbol the symbol to set
	 */
	void setSymbol(unsigned int newSymbol) { symbol = newSymbol; };

	/**
	 * Returns the symbol which is represented by this Huffman tree node.
	 * @returns the symbol represented
	 */
	unsigned int getSymbol() { return symbol; };

	/**
	 * Sets the Huffman code which represents the input symbol of
	 * this Huffman tree node.
	 * @param newValue the Huffman code of the input symbol represented by
	 *  this tree node
	 */
	void setBitRepresentation(const string &newValue) { bitRepresentation = newValue; };

	/**
	 * Returns the Huffman code for the input symbol represented by this
	 * Huffman tree node.
	 * @returns the Huffman code
	 */
	string getBitRepresentation() { return bitRepresentation; };

	/**
	 * Returns the left child of this tree node.
	 */
	HuffmanTreeNode * getLeftChild() { return pLeftChild; };

	/**
	 * Returns the right child of this tree node.
	 */
	HuffmanTreeNode * getRightChild() { return pRightChild; };

	/**
	 * Returns a child of this tree node.
	 * @param index the index of the child to be returned
	 * @returns the child with the specified index
	 */
	HuffmanTreeNode * getChild(unsigned int index);

private:
	/**
	 * The frequency counter for the input symbol represented by
	 * this tree node.
	 */
	int frequency;

	/**
	 * The child index of this node.
	 */
	int index;

	/**
	 * The input symbol (an integer) which is represented by this
	 * Huffman tree node.
	 */
	unsigned int symbol;

	/**
	 * The Huffman code assigned to the input symbol represented by
	 * this node.
	 */
	string bitRepresentation;

	/**
	 * The left child of this tree node.
	 */
	HuffmanTreeNode * pLeftChild;

	/**
	 * The right child of this tree node.
	 */
	HuffmanTreeNode * pRightChild;

	/**
	 * The parent node of this tree node.
	 */
	HuffmanTreeNode * pParent;
};
