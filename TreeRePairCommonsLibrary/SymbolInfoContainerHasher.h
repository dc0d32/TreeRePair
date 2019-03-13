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

class SymbolInfoContainer;

/**
 * Checks two symbol info containers for equality.
 */
struct SymbolInfoContainerEqualityTester
    : std::binary_function<SymbolInfoContainer*, SymbolInfoContainer*, bool>
{
    /**
     * Returns true if and only if the two symbol info containers are equal.
     */
    bool operator()(const SymbolInfoContainer* pProductionReference1,
        const SymbolInfoContainer* pProductionReference2) const;
};

/**
 * Computes a hash value of a given symbol info container.
 */
struct SymbolInfoContainerHasher
    : std::unary_function<SymbolInfoContainer*, std::size_t>
{
    /**
     * Returns a hash value for the specified symbol info container.
     */
    std::size_t operator()(const SymbolInfoContainer * pSymbolNameContainer) const;
};
