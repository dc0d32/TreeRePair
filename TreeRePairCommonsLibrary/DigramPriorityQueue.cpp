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
#include "DigramPriorityQueue.h"

#include "Helper.h"
#include "Digram.h"
#include "TreeRePairConstants.h"

#include <cmath>

using namespace std;

DigramPriorityQueue::DigramPriorityQueue(const unsigned int n,
		HASH_D_D * pDigramHashMapPar) :
	pDigramHashMap(pDigramHashMapPar) {

	numberOfDigramLists = (int) sqrt((float) n);
	currentDigramListIndex = numberOfDigramLists - 1;

	pDigramLists = new Digram*[numberOfDigramLists];
	for (unsigned int i = 0; i < numberOfDigramLists; i++) {
		pDigramLists[i] = 0;
	}
}

DigramPriorityQueue::~DigramPriorityQueue(void) {

	delete[] pDigramLists;
}

unsigned int DigramPriorityQueue::calculateScore(const Digram * pDigram) {

	unsigned int score = pDigram->getNumberOfOccs();

//	if (pDigram->getChildSymbol()->getRank() == 0 && score > 1) {
//		score *= 2;
//	}

	return score;
}

Digram * DigramPriorityQueue::top() {

	// unsigned int bound = TreeRePairMode::getInstance()->getBound();

	Digram * pTopDigram = 0;

	if (currentDigramListIndex == numberOfDigramLists - 1) {
		if (pDigramLists[numberOfDigramLists - 1] != 0) {
			Digram * pDigram = pDigramLists[numberOfDigramLists - 1];
			while (pDigram != 0
					&& (pDigram->getRankOfTreePattern() > TreeRePairMode::getInstance()->getMaxRank()
							|| pDigram->getNumberOfOccs() < 2)) {
				pDigram = pDigram->getFollowingDigram();
			}

			if (pDigram != 0) {
				unsigned int score = calculateScore(pDigram);
				Digram * pCurDigram = pDigram->getFollowingDigram();
				while (pCurDigram != 0) {
					unsigned int currentScore = calculateScore(pCurDigram);
					if (currentScore > score
							&& pCurDigram->getRankOfTreePattern() <= TreeRePairMode::getInstance()->getMaxRank()
							&& pCurDigram->getNumberOfOccs() >= 2) {

						score = currentScore;
						pDigram = pCurDigram;
					}
					pCurDigram = pCurDigram->getFollowingDigram();
				}
				pTopDigram = pDigram;
			}
		}
	}

	if (pTopDigram == 0) {

		unsigned int beginWithDigramListIndex = min(currentDigramListIndex, numberOfDigramLists - 2);

		// we cannot pay attention to the score of the digrams when
		// in the queues number size-2 - number 0; otherwise our
		// run time would drastically increase;
		// this restriction had no negative consequences on the compression
		// results concerning the test files
		for (int i = beginWithDigramListIndex; i >= 0; i--) {
			if (pDigramLists[i] != 0) {
				Digram * pDigram = pDigramLists[i];

				while (pDigram != 0
						&& (pDigram->getRankOfTreePattern() > TreeRePairMode::getInstance()->getMaxRank()
								|| pDigram->getNumberOfOccs() < 2)) {
					pDigram = pDigram->getFollowingDigram();
				}

				if (pDigram == 0) {
					continue;
				}

				pTopDigram = pDigram;

				currentDigramListIndex = i;

				break;
			}
		}
	} else {
		// cout << "found digram in top digram list ..." << endl;
	}

	return pTopDigram;
}

void DigramPriorityQueue::pop(Digram * pDigram, int digramListIndex) {

	if (digramListIndex >= 0) {
		// if the digram already was an element of a digram list, we need to
		// fix this digram list

		if (pDigramLists[digramListIndex] == pDigram) {
			// if the digram to be updated is the first digram in the
			// old digram list ...

			Digram * pNextDigram = pDigram->getFollowingDigram();
			if (pNextDigram != 0) {
				pNextDigram->setPrecedingDigram(0);
			}
			pDigramLists[digramListIndex] = pNextDigram;
		} else {
			Digram * pOldPrecedingDigram = pDigram->getPrecedingDigram();
			Digram * pOldFollowingDigram = pDigram->getFollowingDigram();

			// pPrecedingDigram == 0 is only allowed if in special binary mode
			pOldPrecedingDigram->setFollowingDigram(pOldFollowingDigram);

			if (pOldFollowingDigram != 0) {
				pOldFollowingDigram->setPrecedingDigram(pOldPrecedingDigram);
			}
		}
	}

	pDigram->setPrecedingDigram(0);
	pDigram->setFollowingDigram(0);
}

void DigramPriorityQueue::setDigramQuantityToZero(Digram * pDigram) {

	pop(pDigram, getDigramListIndex(pDigram));

	pDigram->setFirstOccurrence(0);
	pDigram->setLastOccurrence(0);
	pDigram->setNumberOfOccsToZero();

	deleteDigram(pDigram);
}

void DigramPriorityQueue::decrementQuantity(Digram * pDigram, unsigned int subQuantity) {

	if (pDigram->getNumberOfOccs() == 0) {
		return;
	}

	updateNumberOfOccs(pDigram, -((int)subQuantity));
}

void DigramPriorityQueue::incrementQuantity(Digram * pDigram, unsigned int addQuantity) {

	updateNumberOfOccs(pDigram, addQuantity);
}

void DigramPriorityQueue::setQuantity(Digram * pDigram,
		const unsigned int newQuantity) {

	int oldDigramListIndex = getDigramListIndex(pDigram);

	pDigram->setNumberOfOccs(newQuantity);

	int digramListIndex = getDigramListIndex(pDigram);
	// unsigned int quantity = pPair->getNumberOfOccs();

	if (oldDigramListIndex == digramListIndex) {
		return;
	}

	pop(pDigram, oldDigramListIndex);

	// if (calculateScore(pPair) == 0) {
	if (pDigram->getNumberOfOccs() == 0) {
		deleteDigram(pDigram);
	} else {
		Digram * pFirstDigram = pDigramLists[digramListIndex];
		if (pFirstDigram == 0) {
			// do nothing
		} else {
			pFirstDigram->setPrecedingDigram(pDigram);
			pDigram->setFollowingDigram(pFirstDigram);
		}
		pDigramLists[digramListIndex] = pDigram;
	}
}

void DigramPriorityQueue::updateNumberOfOccs(Digram * pDigram,
		int addQuantity) {

	int oldDigramListIndex = getDigramListIndex(pDigram);

	if (addQuantity > 0) {
		pDigram->incrementNumberOfOccs(addQuantity);
	} else {
		pDigram->decrementNumberOfOccs(-addQuantity);
	}

	int digramListIndex = getDigramListIndex(pDigram);
	// unsigned int quantity = pDigram->getNumberOfOccs();

	if (oldDigramListIndex == digramListIndex) {
		return;
	}

	pop(pDigram, oldDigramListIndex);

	// if (calculateScore(pDigram) == 0) {
	if (pDigram->getNumberOfOccs() == 0) {
		deleteDigram(pDigram);
	} else {
		Digram * pFirstDigram = pDigramLists[digramListIndex];
		if (pFirstDigram == 0) {
			// do nothing
		} else {
			pFirstDigram->setPrecedingDigram(pDigram);
			pDigram->setFollowingDigram(pFirstDigram);
		}
		pDigramLists[digramListIndex] = pDigram;
	}
}

void DigramPriorityQueue::deleteDigram(Digram * pDigram) {

	HASH_D_D::iterator it = pDigramHashMap->find(pDigram);
	if (it != pDigramHashMap->end()) {
		pDigramHashMap->erase(it);
	}

	delete pDigram;
}

int DigramPriorityQueue::getDigramListIndex(const Digram * pDigram) {

	unsigned int score = calculateScore(pDigram);

	if (pDigram->getNumberOfOccs() == 0) {
		return -1;
	}

	int digramListIndex = -1;
	if (score < (numberOfDigramLists - 1)) {
		digramListIndex = score;
	} else {
		digramListIndex = numberOfDigramLists - 1;
	}

	return digramListIndex;
}

#ifdef DEBUG
void DigramPriorityQueue::print(ostream& os,
		const DigramPriorityQueue * pPriorityQueue) {
	os << "content of the priority queue:" << endl;
	os << "==============================" << endl;
	os << endl;

	for (unsigned int i = 0; i < pPriorityQueue->numberOfDigramLists; i++) {
		os << "digram list of all digrams with score ";

		if (i < pPriorityQueue->numberOfDigramLists - 1) {
			os << i + 1;
		} else {
			os << pPriorityQueue->numberOfDigramLists;
		}
		os << ":" << endl;
		;

		Digram * pDigram = pPriorityQueue->pDigramLists[i];
		if (pDigram == 0) {
			os << "empty" << endl << endl;
		} else {
			os << "-";
			Digram::print(os, pDigram);
			os << endl;
			// Helper::parseOccurrences(pDigram);

			pDigram = pDigram->getFollowingDigram();

			while (pDigram != 0) {
				os << "-";
				Digram::print(os, pDigram);
				os << endl;
				// Helper::parseOccurrences(pDigram);

				pDigram = pDigram->getFollowingDigram();
			}
			os << endl;
		}
	}
}
#endif
