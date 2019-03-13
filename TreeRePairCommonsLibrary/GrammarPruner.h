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
#include "TreeRePairTypes.h"
#include "TreeNode.h"

/**
 * Implements the pruning step of the Re-pair for Trees algorithm.
 */
class GrammarPruner
{
public:
	/**
	 * The constructor.
	 * @param dagGrammar the grammar containing the DAG of the start production
	 * @param mainGrammar the grammar containing the productions resulting
	 *  from the digram replacements
	 * @param pruneDagGrammar specifies if the DAG grammar or the
	 *  main grammar shall be pruned
	 */
	GrammarPruner(Grammar & dagGrammar, Grammar & mainGrammar,
			bool pruneDagGrammar = true);
	~GrammarPruner(void);

	/**
	 * Initiates the actual pruning.
	 */
	void pruneGrammar();

#ifdef DEBUG
	void setFilePath(char * pFilePathPar) {
		pFilePath = pFilePathPar;
	}
#endif

private:
	/**
	 * The grammar containing the start production of the linear
	 * SLCF tree grammar to generate.
	 */
	Grammar & dagGrammar;

	/**
	 * The grammar containg the productions (except the start production)
	 * of the linear SLCF tree grammar to generate.
	 */
	Grammar & mainGrammar;

	/**
	 * Indicates whether the DAG grammar or the main grammar
	 * shall be pruned.
	 */
	bool pruneDagGrammar;

#ifdef DEBUG
	char * pFilePath;
#endif
};
