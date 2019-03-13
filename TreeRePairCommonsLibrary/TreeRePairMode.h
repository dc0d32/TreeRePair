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

/**
 * An instance of this (singleton) class reflects the mode in
 * which a tool of the TreeRePair distribution is executed.
 */
class TreeRePairMode
{
public:
	~TreeRePairMode(void) {};

	enum
	{
		/**
		 * Indicates that we are executing the TreeRepair
		 * tool (compressor).
		 */
		COMPRESS,
		/**
		 * Indicates that we are executing the TreeDePair
		 * tool (decompressor).
		 */
		DECOMPRESS,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to generate a data file which serves as
		 * a input for gnuplot (the class DataFileCreator is
		 * used).
		 */
		TOOL_DATAFILE,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to write out the XML document tree of
		 * an input XML document (the class StructureRetriever is
		 * used).
		 */
		TOOL_STRUCTURE,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to write out a latex file summarizing
		 * multiple runs of different compressors (the class
		 * SummaryCreator is used).
		 */
		TOOL_SUMMARY,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to obtain informations about the minimal
		 * DAG of multiple input XML documents.
		 */
		TOOL_BATCH_MIN_DAG,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to run the TreeRepair tool on multiple
		 * input XML documents in a batch.
		 */
		TOOL_BATCH_TREEREPAIR,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to run the BPLEX tool on multiple
		 * input XML documents in a batch.
		 */
		TOOL_BATCH_BPLEX,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to run the Extended-Repair tool on multiple
		 * input XML documents in a batch.
		 */
		TOOL_BATCH_REPAIR,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to run the gzip tool on multiple
		 * input XML documents in a batch.
		 */
		TOOL_BATCH_GZIP,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to run the bzip2 tool on multiple
		 * input XML documents in a batch.
		 */
		TOOL_BATCH_BZIP,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to run the XMill tool on multiple
		 * input XML documents in a batch.
		 */
		TOOL_BATCH_XMILL,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to create an empty database file.
		 */
		TOOL_CREATE_DATABASE,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to measure the memory usage of another tool.
		 */
		TOOL_MEMORY_USAGE,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to obtain information about a grammar given
		 * by a BPLEX-like formatted file.
		 */
		TOOL_BPLEX_GRAMMAR_ANALYZER,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to collect the character data and attribute
		 * values conatined in an XML document.
		 */
		TOOL_DATA_COLLECTOR,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to obtain informations about a given
		 * XML document.
		 */
		TOOL_ANALYZE,
		/**
		 * Indicates that we are executing the TreeRePairTool
		 * in order to obtain informations about the minimal
		 * DAG of an input XML document tree.
		 */
		TOOL_MINIMAL_DAG_INFO,
		/**
		 * Indicates that we are executing the TreeRePair
		 * tool in file size optimization mode.
		 */
		OPTIMIZE_FILE_SIZE,
		/**
		 * Indicates that we are executing the TreeRePair
		 * tool in number of edges optimization mode.
		 */
		OPTIMIZE_EDGES,
		/**
		 * Indicates that that the pruning step shall be skipped.
		 */
		OPTIMIZE_DISABLE
	};

	/**
	 * Returns the singleton instance of the TreeRePairMode
	 * class.
	 */
	static TreeRePairMode * getInstance(void);

	/**
	 * Deallocates the memory used by the singleton instance
	 * of the TreeRePairMode class.
	 */
	static void deleteInstance();

	/**
	 * Indicates if the optimization mode was set by the user or if
	 * we are using the default mode.
	 */
	bool isSetOptimizationMode() { return optimizationModeSet; };

	/**
	 * Indicates if the maximal rank was set by the user or if
	 * we are using the default value.
	 */
	bool isMaxRankSet() { return maxRankSet; };

	/**
	 * Indicates if we are in verbose mode.
	 */
	bool isVerboseMode() { return verbose; };

	/**
	 * Indicates if we are told to show progress information
	 * during a run of our algorithm.
	 */
	bool isShowProgress() { return showProgress; };

	/**
	 * Indicates whether we are told to write out the final
	 * grammar in a BPLEX-like format, too.
	 */
	bool isPrintBplexLike() { return printBplexLike; };

	/**
	 * Indicates whether we are told to write out the final
	 * grammar in a textual (i.e., human readable) format, too.
	 */
	bool useTextualGrammar() { return textualGammar; };

	/**
	 * Indicates whether we use the DAG representation of the input
	 * tree.
	 */
	bool isUseDag() { return useDag; };

	/**
	 * Indicates if we are told to write out the statistics of the
	 * final grammar to a binary file. This file can be used by the
	 * batch runner of the TreeRePairTool tool to gain informations
	 * about the generated grammar.
	 */
	bool writeBinaryStatistics() { return binaryStatistics; };

	/**
	 * Indicates whether to output informations about the generated
	 * grammar to the standard output.
	 */
	bool isShowStatistics() { return showStatistics; };

	/**
	 * Returns either COMPRESS or DECOMPRESS.
	 */
	int getAlgMode() { return algMode; };

	/**
	 * Returns the optimization mode to use (either OPTIMIZE_FILE_SIZE,
	 * OPTIMIZE_EDGES or OPTIMIZE_DISABLE).
	 */
	int getOptimizationMode() { return optimizationMode; };

	/**
	 * Returns the maximal rank allowed for a nonterminal.
	 */
	unsigned int getMaxRank() { return maxRank; };

	/**
	 * Enables or disables the verbose mode.
	 */
	void setVerboseMode(bool verbosePar) { verbose = verbosePar; };

	/**
	 * Enables or disables the progress information during a run
	 * of our algorithm.
	 */
	void setShowProgress(bool showProgressNew) { showProgress = showProgressNew; };

	/**
	 * Enables or disables the generation of a file containing the final
	 * grammar in BPLEX-like format.
	 */
	void setPrintBplexLike(bool printBplexLikePar) { printBplexLike = printBplexLikePar; };

	/**
	 * Enables or disables the generation of a file containing the final
	 * grammar in a textual format.
	 */
	void setUseTextualGrammar(bool newValue) { textualGammar = newValue; };

	/**
	 * Enables or disables the use of the DAG representation of the
	 * input tree.
	 */
	void setUseDag(bool useDagPar) { useDag = useDagPar; };

	/**
	 * Enables or disables the generation of a binary file containing
	 * statistics about the final grammar.
	 */
	void setWriteBinaryStatistics(bool binaryStatisticsPar) { binaryStatistics = binaryStatisticsPar; };

	/**
	 * Enables or diables the output of statistics about the generated
	 * grammar (using standard output).
	 */
	void setShowStatistics(bool showStatisticsPar) { showStatistics = showStatisticsPar; };

	/**
	 * Sets the algorithm mode (either COMPRESS or DECOMPRESS).
	 */
	void setAlgMode(int newAlgMode) { algMode = newAlgMode; };

	/**
	 * Sets the optimization mode (either OPTIMIZE_FILE_SIZE,
	 * OPTIMIZE_EDGES or OPTIMIZE_DISABLE).
	 */
	void setOptimizationMode(int newMode) {
		optimizationModeSet = true;
		optimizationMode = newMode;
	};

	/**
	 * Sets the maximal rank allowed for a nonterminal.
	 */
	void setMaxRank(unsigned int maxRankPar) {
		this->maxRank = maxRankPar;
		maxRankSet = true;
	};

private:
	TreeRePairMode(void);

	/**
	 * The singleton instance of the TreeRePairMode class.
	 */
	static TreeRePairMode * pSingleton;

	/**
	 * Indicates the algorithm mode (either COMPRESS or DECOMPRESS).
	 */
	int algMode;

	/**
	 * Indicates the optimization mode (either OPTIMIZE_FILE_SIZE,
	 * OPTIMIZE_EDGES or OPTIMIZE_DISABLE).
	 */
	int optimizationMode;

	/**
	 * Indicates the use of the DAG representation of the input tree.
	 */
	bool useDag;

	/**
	 * Indicates whether we are in verbose mode.
	 */
	bool verbose;

	/**
	 * Indicates if we are told to write out the statistics of the
	 * final grammar to a binary file.
	 */
	bool binaryStatistics;

	/**
	 * Indicates whether to output informations about the generated
	 * grammar to the standard output.
	 */
	bool showStatistics;

	/**
	 * Indicates if we are told to show progress information
	 * during a run of our algorithm.
	 */
	bool showProgress;

	/**
	 * Indicates whether we are told to write out the final
	 * grammar in a BPLEX-like format, too.
	 */
	bool printBplexLike;

	/**
	 * Indicates whether we are told to write out the final
	 * grammar in a textual (i.e., human readable) format, too.
	 */
	bool textualGammar;

	/**
	 * Indicates if the optimization mode was set by the user or if
	 * we are using the default mode.
	 */
	bool optimizationModeSet;

	/**
	 * Indicates if the maximal rank was set by the user or if
	 * we are using the default value.
	 */
	bool maxRankSet;

	/**
	 * The maximal rank allowed for a nonterminal.
	 */
	unsigned int maxRank;
};
