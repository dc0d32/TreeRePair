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
#include "SymbolInfoContainerHasher.h"
#include "TreeRePairTypes.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

/**
 * This class serves as a container for several hash maps.
 */
class SymbolsHashMaps
{
public:
	/**
	 * Hash map which allows constant time access to a symbol
	 * using its id.
	 */
    HASH_U_S symbolByIdHashMap;

    /**
     * Hash map which allows constant time access to a symbol
     * by specifying its element type and its children characteristic.
     */
    HASH_S_S symbolsBySicHashMap;

    /**
     * Hashed set storing pointers to all terminal symbols.
     */
    HASH_U terminalSymbols;

    /**
     * Hash map which allows constant time access to the element
     * type of a symbol by using its id.
     */
	HASH_I_S idElementTypeHashMap;
};

/**
 * This class implements a symbol which can either be a terminal
 * symbol or a nonterminal.
 */
class Symbol
{
public:
	/**
	 * The constructor of the class. The parameters of it are used
	 * when creating terminal symbols in the binary model. If a
	 * terminal symbol exhibits only one child then, with their
	 * help, it can be easily determined if it is a left or right
	 * child.
	 * @param leftChild specifies if the symbol has a left child
	 *  (default is false)
	 * @param rightChild specifies if the symbol has a right child
	 *  (default is false)
	 */
	Symbol(bool leftChild = false, bool rightChild = false);
	~Symbol(void) {};

	/**
	 * Returns the unique symbol which is used to label parameter
	 * nodes.
	 */
	static Symbol * getParameterSymbol();

	/**
	 * Returns the unique symbol which is used to label place holder
	 * nodes.
	 */
	static Symbol * getPlaceholderSymbol();

	/**
	 * Returns the unique symbol which is used as the left-hand
	 * side of the start production of the generated grammar.
	 */
	static Symbol * getStartSymbol();

	/**
	 * Returns the element type of a symbol.
	 * @param id the ID of the symbol
	 * @returns the element type
	 */
	static string getElementTypeById(unsigned int id);

	/**
	 * Returns the symbol with the specified ID.
	 * @param id the ID of the symbol which shall be returned
	 * @returns the symbol with the specified ID
	 */
	static Symbol * getSymbolById(unsigned int id) {
		return hashMaps.symbolByIdHashMap[id];
	}

	/**
	 * Inserts a symbol into the terminal symbols hash map.
	 * @param id the ID of the symbol which shall be inserted
	 */
	static void setTerminalSymbol(unsigned int id) {
		hashMaps.terminalSymbols.insert(id);
	}

	/**
	 * Returns the number of terminal symbols.
	 */
	static unsigned int getNumberOfTerminals() {
		return hashMaps.terminalSymbols.size();
	}

	/**
	 * Returns true if the specified symbol is a terminal symbol.
	 * @param id the ID of the symbol to check
	 * @returns true if the symbol is a terminal symbol; otherwise false
	 */
	static bool isTerminalSymbol(unsigned int id) {
		if (hashMaps.terminalSymbols.find(id) != hashMaps.terminalSymbols.end()) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Adds the symbol to the symbolsBySicHashMap hash map and
	 * the idElementTypeHashMap hashmap.
	 * @param elementType the element type of the symbol
	 */
	void addSymbolToHashMaps(string elementType);

	/**
	 * Returns the ID of the symbol.
	 */
	unsigned int getId() const
	{
#ifdef DEBUG
		if (id > Symbol::counter) {
			cout << "Fatal error: Id of symbol incorrect. Exiting ..." << endl;
			exit(1);
		}
#endif
		return id;
	}
	;

	/**
	 * Returns the name (an integer) of the symbol.
	 */
	unsigned int getName() const {
		return name;
	};

	/**
	 * Sets the name (an integer) of the symbol.
	 */
	void setName(unsigned int newName) {
		name = newName;
	};

	/**
	 * Returns the rank of the symbol.
	 */
	unsigned int getRank() const {
		return rank;
	};

	/**
	 * Sets the rank of the symbol.
	 */
	void setRank(unsigned int newRank) {
		this->rank = newRank;
	};

	/**
	 * Returns true if and only if this symbol has a left child.
	 */
	bool hasLeftChild() const {
		return leftChild;
	};

	/**
	 * Sets if the symbol has a left child
	 */
	void setHasLeftChild(bool newLeftChild) {
		this->leftChild = newLeftChild;
	};

	/**
	 * Returns true if and only if this symbol has a right child.
	 */
	bool hasRightChild() const {
		return rightChild;
	};

	/**
	 * Sets if the symbol has a right child
	 */
	void setHasRightChild(bool newRightChild) {
		this->rightChild = newRightChild;
	};

	/**
	 * Counts the number of instantiations of the Symbol class.
	 */
	static unsigned int counter;

#ifdef DEBUG
	static void print(ostream& os, const Symbol * symbol);
#endif

	/**
	 * The hash maps used to allow constant time access to the instances
	 * of the Symbol class, to the element type of a symbol and to all
	 * terminal symbols.
	 */
	static SymbolsHashMaps hashMaps;

private:
	/**
	 * The symbol used to label all parameter nodes.
	 */
	static Symbol * pParameterSymbol;

	/**
	 * The symbol used to label all place holder nodes.
	 */
	static Symbol * pPlaceholderSymbol;

	/**
	 * The symbol used as the left-hand side of the start production
	 * of the linear SLCF tree grammar to generate.
	 */
	static Symbol * pStartSymbol;

	/**
	 * Indicates if this symbol has a left child.
	 */
	bool leftChild;

	/**
	 * Indicates if this symbol has a right child.
	 */
	bool rightChild;

	/**
	 * The unique ID of this Symbol instance.
	 */
	unsigned int id;

	/**
	 * The name (an integer) of this Symbol instance.
	 */
	unsigned int name;

	/**
	 * The rank of this symbol.
	 */
	unsigned int rank;
};
