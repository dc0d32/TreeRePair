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

#include <string>
#include <functional>

using namespace std;

class TreeNode;

/**
 * Used to check if two tree nodes are identical.
 */
struct NodeEqualityTester
    : std::binary_function<TreeNode*, TreeNode*, bool>
{
    /**
     * Returns true if the two specified tree nodes are
     * identical.
     */
    bool operator()(const TreeNode* pNode1,
        const TreeNode* pNode2) const;
};

/**
 * Used to compute a hash value for a tree node.
 */
struct NodeHasher
    : std::unary_function<TreeNode*, std::size_t>
{
    /**
     * Returns a hash value for the specified tree node.
     */
    std::size_t operator()(const TreeNode * pNode) const;
};
