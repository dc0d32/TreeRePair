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
#include "GrammarPruner.h"

#include "Production.h"
#include "Grammar.h"
#include "SymbolReplacer.h"
#include "EdgeCounter.h"
#include "RefCountUpdater.h"
#include "Helper.h"

#ifdef DEBUG
#include "XmlTreePrinter.h"
#endif

#include <vector>

GrammarPruner::GrammarPruner(Grammar & dagGrammarPar,
		Grammar & mainGrammarPar, bool pruneDagGrammarPar) :
	dagGrammar(dagGrammarPar), mainGrammar(mainGrammarPar),
			pruneDagGrammar(pruneDagGrammarPar) {
#ifdef DEBUG
	pFilePath = 0;
#endif
}

void GrammarPruner::pruneGrammar() {
	Grammar * grammar = &dagGrammar;
	if (!pruneDagGrammar) {
		grammar = &mainGrammar;
	}

	vector<Production*>::iterator it, itTemp;

#ifdef DEBUG
	int testcounter = 0;
#endif

	for (it = grammar->end() - 1; it >= grammar->begin();) {
		// needed for MSVC++ compiler
		itTemp = it;
		bool stop = false;
		if (it != grammar->begin()) {
			it--;
		} else {
			stop = true;
		}

		Production * pProduction = *itTemp;

		if (pProduction->getLeftHandSide() != Symbol::getStartSymbol()
				&& pProduction->getRefCount() <= 1) {
//#ifdef DEBUG
//			cout << pProduction->getReferencingNodes()->size() << endl;
//			cout << "Removing production:" << endl;
//			Production::print(cout, pProduction);
//#endif

			TreeNode * pReference =
					*(pProduction->getReferencingNodes()->begin());
			TreeNode * pParent = pReference->getParent();

			Grammar::applyProduction(pReference, pProduction);

			if (pParent != 0) {
				pParent->setChild(pReference->getIndex(),
						pProduction->getRightHandSide());

//#ifdef DEBUG
//				XmlTreePrinter treePrinter;
//				treePrinter.printTree(pProduction->getRightHandSide(), cout, true, false);
//#endif
			} else {
				Production * pRefProduction =
						grammar->getProductionByRightHandSide(pReference);
				TreeNode * pRightHandSide = pProduction->getRightHandSide();
				grammar->updateRightHandSide(pProduction, 0);
				grammar->updateRightHandSide(pRefProduction, pRightHandSide);

//#ifdef DEBUG
//				XmlTreePrinter treePrinter;
//				treePrinter.printTree(pRightHandSide, cout, true, false);
//#endif
			}

			grammar->removeProduction(itTemp);
			delete pReference;
			delete pProduction;

#ifdef DEBUG
			if (pFilePath != 0) {
				// if (testcounter == 1 && !Helper::areGrammarsValid(dagGrammar, mainGrammar, pFilePath)) {
				if (!Helper::areGrammarsValid(dagGrammar, mainGrammar, pFilePath)) {
					cout << "Grammar is invalid." << endl;
					exit(1);
				}
				testcounter++;
			}
#endif
		}

		if (stop) {
			break;
		}
	}

	// roll back inefficient replacements
	if (!pruneDagGrammar) {

		unsigned int counter = 0;
		SymbolReplacer symbolReplacer(dagGrammar, mainGrammar);

		int limit = 0;
		if (TreeRePairMode::getInstance()->getOptimizationMode() == TreeRePairMode::OPTIMIZE_FILE_SIZE) {
			limit = 2;
		}

		//		if (TreeRePairMode::getInstance()->isVerboseMode()) {
		//			cout << "Rolling back inefficient replacements ..." << endl;
		//		}

		// cout << "size of grammar: " << grammar->size() << endl;
		// unsigned int t = 0;

		for (it = grammar->end() - 1; it >= grammar->begin();) {
			itTemp = it;

			// needed for MSVC++ compiler
			bool stop = false;
			if (it != grammar->begin()) {
				it--;
			} else {
				stop = true;
			}


			Production * pProduction = *itTemp;

			EdgeCounter edgeCounter(0, false);
			edgeCounter.countEdges(pProduction->getRightHandSide());
			unsigned int edges = edgeCounter.getNumberOfEdges();

			int savValue = (int)pProduction->getRefCount() * (int)edges - (int)pProduction->getLeftHandSide()->getRank() - (int)edges;

			//Production::print(cout, pProduction);
			//cout << "refCount: " << pProduction->getRefCount() << ", ";
			//cout << "sav-value:" << savValue << endl;

			if (savValue <= limit) {

				//cout << "Eliminating production ..." << endl;
//				cout << "refCount: " << pProduction->getRefCount() << ", ";
//				Production::print(cout, pProduction);

				symbolReplacer.replaceNonterminal(pProduction);

				RefCountUpdater updater(mainGrammar,
						pProduction->getRightHandSide(),
						pProduction->getRefCount() - 1);

				grammar->removeProduction(itTemp);
				Production::deleteProduction(pProduction);

				counter++;

//				cout << "after pruning: " << endl;
//				for (unsigned int i = 0; i < grammar->size(); i++) {
//					Production * pProduction = (*grammar)[i];
//					Production::print(cout, pProduction);
//				}
//				cout << ".................." << endl;
			}

			if (stop) break;
		}

		if (TreeRePairMode::getInstance()->isVerboseMode()) {
			cout << "Eliminated " << counter << " inefficient production(s)."
					<< endl;
		}

		// cout << "processed " << t << " productions" << endl;

	}
}

GrammarPruner::~GrammarPruner(void) {
}
