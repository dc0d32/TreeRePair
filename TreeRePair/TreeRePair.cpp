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
#include "TreeRePairAlgorithm.h"
#include "Helper.h"
#include "TreeRePairVersion.h"
#include "TreeRePairConstants.h"

static void printHelp() {
	string blank = Helper::getBlankCharacters(1);

	cout << endl;
	cout << "TreeRePair is an implementation of the Re-pair for Trees algorithm." << endl;
	cout << endl;
	cout << "Copyright (c) 2010 Roy Mennicke, Build date: " << VERSION << endl;
	cout << endl;
	cout << "This is free software.  You may redistribute copies of it under the terms of" << endl;
	cout << "the GNU General Public License Version 3 which is available at the webpage" << endl;
	cout << "<http://www.gnu.org/licenses/gpl-3.0.txt>. There is NO WARRANTY." << endl;

	cout << endl;
	cout << "Usage: TreeRePair [-optimize MODE] [-statistics] input_file" << endl;

	cout << endl << "Parameters:" << endl;

	cout << blank << "input_file" << endl;
	cout << blank << blank << "input_file must be a well-formed XML file." << endl;

	cout << blank << "-optimize MODE" << endl;
	cout << blank << blank << "Optimizes either the file size (\"filesize\") or the number of " << endl;
	cout << blank << blank << "edges (\"edges\") of the generated linear SLCF tree grammar by " << endl;
	cout << blank << blank << "eliminating inefficient productions. The default is file size " << endl;
	cout << blank << blank << "optimization. Setting MODE to \"disable\" skips the pruning " << endl;
	cout << blank << blank << "step." << endl;

	cout << blank << "-statistics" << endl;
	cout << blank << blank << "Prints various informations about the generated grammar." << endl;

	cout << blank << "-help" << endl;
	cout << blank << blank << "Shows this help screen." << endl;

	cout << endl << "Experimental Parameters:" << endl;

	cout << blank << "-v" << endl;
	cout << blank << blank << "Enables verbose mode which prints out various additional" << endl;
	cout << blank << blank << "informations." << endl;

	cout << blank << "-p" << endl;
	cout << blank << blank << "Shows progress of the digram replacement process." << endl;

	cout << blank << "-max_rank MAX_RANK" << endl;
	cout << blank << blank << "MAX_RANK specifies the maximal rank of any nonterminal. It " << endl;
	cout << blank << blank << "must be of type Integer. Default is " << MAX_RANK << "." << endl;

	cout << blank << "-bplex" << endl;
	cout << blank << blank << "Writes out the generated grammar in BPLEX format." << endl;

	cout << blank << "-textual" << endl;
	cout << blank << blank << "Writes out a human readable but still parseable representation" << endl;
	cout << blank << blank << "of the generated grammar." << endl;

	cout << blank << "-no_dag" << endl;
	cout << blank << blank << "Doesn't create the minimal DAG of the input tree but loads the" << endl;
	cout << blank << blank << "whole input tree." << endl;

	cout << endl;
}

static char * processTreeRePairArgs(int argc, char * args[])
{
	char * pFilePath = 0;

	for (int i = 1; i < argc; i++)
	{
		char * arg = args[i];
		char *par = 0;
		int maxRank = MAX_RANK;
		if (arg[0] == '-')
		{
			switch (arg[1])
			{
			case 'b':
				if (arg[2] == 'i')
				{ // binary statistics
					TreeRePairMode::getInstance()->setWriteBinaryStatistics(
							true);
				}
				else if (arg[2] == 'p')
				{ // bplex-like
					TreeRePairMode::getInstance()->setPrintBplexLike(true);
				}
				else
				{
					cout << "Unknown switch: " << arg << endl;
					exit(0);
				}
				break;
			case 'm': // max_rank
				par = args[i + 1];
				maxRank = atoi(par);
				TreeRePairMode::getInstance()->setMaxRank(maxRank);
				i++;
				break;
			case 't': // textual
				TreeRePairMode::getInstance()->setUseTextualGrammar(true);
				break;
			case 'n': // no_dag
				TreeRePairMode::getInstance()->setUseDag(false);
				break;
			case 'o':
				if (arg[2] == 'p') { // optimize
					par = args[i + 1];
					if (par[0] == 'f' && par[1] == 'i')
					{ // file size
						TreeRePairMode::getInstance()->setOptimizationMode(
								TreeRePairMode::OPTIMIZE_FILE_SIZE);
					}
					else if (par[0] == 'e' && par[1] == 'd')
					{
						TreeRePairMode::getInstance()->setOptimizationMode(
								TreeRePairMode::OPTIMIZE_EDGES);
					}
					else if (par[0] == 'd' && par[1] == 'i')
					{
						TreeRePairMode::getInstance()->setOptimizationMode(
								TreeRePairMode::OPTIMIZE_DISABLE);
					}
					else
					{
						cout << "Invalid value for optimization mode." << endl;
						exit(0);
					}
					i++;
				}
				else
				{
					cout << "Unknown switch: " << arg << endl;
					exit(0);
				}
				break;
				//				case 'd':
				//					TreeRePairMode::getInstance()->setUseDag(true);
				//					break;
			case 'p':
				TreeRePairMode::getInstance()->setShowProgress(true);
				break;
			case 'v':
				TreeRePairMode::getInstance()->setVerboseMode(true);
				break;
			case 's':
				if (arg[2] == 't')
				{ // statistics
					TreeRePairMode::getInstance()->setShowStatistics(true);
				}
				else
				{
					cout << "Unknown switch: " << arg << endl;
					exit(0);
				}
				break;
			case 'h': // -help
				printHelp();
				break;
			case '\0':
				cout << "Unknown switch." << endl;
			default:
				cout << "Unknown switch: " << arg << endl;

				exit(1);
				break;
			}
		}
		else
		{
			pFilePath = arg;
		}
	}

	return pFilePath;
}

int main (int argc, char* args[]) {

	if (argc == 1) {
		cout << "Please specify an input file. Use the -help switch to show the help screen." << endl;
		// printHelp();
		exit(1);
	}

	char * pFilePath = processTreeRePairArgs(argc, args);

	if (pFilePath == 0) {
		cout << "Please specify a input file. Use the -help switch to show the help screen." << endl;
	} else if (!Helper::fileExists(pFilePath)) {
		cout << "Input file does not exist. Use the -help switch to show the help screen." << endl;
	} else {
		TreeRePairAlgorithm * pAlg = new TreeRePairAlgorithm(pFilePath);
		delete pAlg;
	}

#ifdef DEBUG
	// getchar();
#endif

	return 0;
}
