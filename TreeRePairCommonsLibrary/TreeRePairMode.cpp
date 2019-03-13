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
#include "TreeRePairMode.h"

#include "TreeRePairConstants.h"

TreeRePairMode::TreeRePairMode()
{

	optimizationModeSet = false;
	maxRankSet = false;

	verbose = false;
	showProgress = false;
	printBplexLike = false;
	textualGammar = false;
	useDag = true;
	binaryStatistics = false;
	showStatistics = false;

	algMode = COMPRESS;
	optimizationMode = OPTIMIZE_FILE_SIZE;

	maxRank = MAX_RANK;
}

TreeRePairMode * TreeRePairMode::getInstance()
{
	if (pSingleton == 0)
	{
		pSingleton = new TreeRePairMode();
	}
	return pSingleton;
}

void TreeRePairMode::deleteInstance()
{
	delete pSingleton;
	pSingleton = 0;
}

TreeRePairMode * TreeRePairMode::pSingleton = 0;
