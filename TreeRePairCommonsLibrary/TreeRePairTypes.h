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

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <vector>
#include <string>

using namespace std;

class TreeNode;
class TreeHasher;
class SymbolInfoContainer;
class Digram;
class Symbol;
class TreeNode;
class ProductionTreeContainer;
class HuffmanTreeNode;
class ProductionReference;
class Production;
struct NodeHasher;
struct NodeEqualityTester;
struct ProductionReferenceHasher;
struct ProductionReferenceEqualityTester;
struct SubtreeHasher;
struct SubtreeEqualityTester;
struct DigramHasher;
struct DigramEqualityTester;
struct SymbolInfoContainerHasher;
struct SymbolInfoContainerEqualityTester;


typedef boost::unordered_set<TreeNode*, NodeHasher, NodeEqualityTester> HASH_N;
typedef boost::unordered_set<unsigned int> HASH_U;
typedef boost::unordered_map<unsigned int, Production *> HASH_I_P;
typedef boost::unordered_map<unsigned int, unsigned int> HASH_I_I;
typedef boost::unordered_map<unsigned int, HuffmanTreeNode*> HASH_S_H;
typedef boost::unordered_map<int, string> HASH_I_S;
typedef boost::unordered_map<unsigned int, Symbol*> HASH_U_S;
typedef boost::unordered_map<string, vector<Symbol*> * > HASH_ST_VS;
typedef boost::unordered_map<string, Symbol*> HASH_ST_S;
typedef boost::unordered_map<TreeNode*, ProductionTreeContainer*, SubtreeHasher, SubtreeEqualityTester> HASH_T_T;
typedef boost::unordered_map<Digram*, Digram*, DigramHasher, DigramEqualityTester> HASH_D_D;
typedef boost::unordered_map<TreeNode*, Production*, NodeHasher, NodeEqualityTester> HASH_N_P;
typedef boost::unordered_map<TreeNode*, TreeNode*, NodeHasher, NodeEqualityTester> HASH_N_N;
typedef boost::unordered_map<SymbolInfoContainer*, Symbol*, SymbolInfoContainerHasher, SymbolInfoContainerEqualityTester> HASH_S_S;
