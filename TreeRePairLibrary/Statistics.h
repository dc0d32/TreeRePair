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
#include "TreeRePairMacros.h"

/**
 * This class encapsulates the statistics of a grammar generated
 * by TreeRePair, BPLEX, Extended-Repair or another compression
 * algorithm.
 */
class Statistics
{
public:
	/**
	 * A constructor of this class.
	 * @param pFilePath the final containing the encoded final grammar
	 * @param dagGrammar the grammar containing the start production
	 *  of the generated linear SLCF tree grammar
	 * @param timeInSeconds the runtime of the algorithm
	 * @param numberOfInputEdges the number of edges of the input tree
	 * @param mainGrammar the grammar containing the productions of
	 *  the generated linear SLCF tree grammar except the start
	 *  production
	 * @param mode the mode in which the algorithm was executed
	 */
	Statistics(const char * pFilePath, Grammar & dagGrammar,
			int timeInSeconds, int numberOfInputEdges,
			Grammar & mainGrammar, string mode);

	/**
	 * A constructor of this class.
	 * @param filesystemInputSize the file size of the input XML document
	 * @param filesystemOutputSize the size of the output file containing
	 *  the encoded generated grammar
	 * @param timeInSeconds the runtime of the algorithm
	 * @param numberOfOutputEdges the number of edges of the generated
	 *  linear SLCF tree grammar
	 * @param numberOfInputEdges the number edges of the input tree
	 * @param numberOfNonTerminals the number of nonterminals of the generated
	 *  linear SLCF tree grammar
	 * @param mode the mode in which the algorithm was executed
	 * @param memVmRSS the memory usage of the algorithm during its execution
	 *  in megabytes
	 */
	Statistics(int filesystemInputSize, int filesystemOutputSize,
			int timeInSeconds, int numberOfOutputEdges,
			int numberOfInputEdges, int numberOfNonTerminals,
			string mode, int memVmRSS);

	/**
	 * A constructor of this class.
	 * @param pFilePath the final containing the encoded final grammar
	 * @param timeInSeconds the runtime of the algorithm
	 * @param numberOfOutputEdges the number of edges of the generated
	 *  linear SLCF tree grammar
	 * @param numberOfInputEdges the number edges of the input tree
	 * @param numberOfNonTerminals the number of nonterminals of the generated
	 *  linear SLCF tree grammar
	 * @param mode the mode in which the algorithm was executed
	 * @param memVmRSS the memory usage of the algorithm during its execution
	 *  in megabytes
	 */
	Statistics(const char * pFilePath, int timeInSeconds,
			int numberOfOutputEdges, int numberOfInputEdges,
			int numberOfNonTerminals, string mode, int memVmRSS);

	/**
	 * An constructor taking no arguments. It is needed to deserialize
	 * an instance of the Statistics class (which was written to a
	 * binary file).
	 */
	Statistics(void);

	~Statistics(void);

	/**
	 * Writes a textual representation of the Statistics instance to an
	 * output file (using the file returned by
	 * Helper::getFilePathForStatistics()).
	 */
	void writeStatisticsFile();

	/**
	 * Writes a textual representation of the Statistics instance to the
	 * specified output stream.
	 * @param out the output stream to use
	 */
	void writeStatistics(ostream & out);

	/**
	 * Returns the number of edges of the input tree.
	 */
	int getEdgesInputSize() {
		return numberOfInputEdges;
	};

	/**
	 * Returns the number of edges of the generated linear SLCF tree grammar.
	 */
	int getEdgesOutputSize() {
		return edgesOutputSize;
	};

	/**
	 * Returns the file size of the input XML document.
	 */
	int getFilesystemInputSize() {
		return filesystemInputSize;
	};

	/**
	 * Returns the size of the output file containing the succinct coded
	 * final grammar.
	 */
	int getFilesystemOutputSize() {
		return filesystemOutputSize;
	};

	/**
	 * Returns the number of nonterminals of the generated grammar.
	 */
	size_t getNumberOfNonTerminals() {
		return numberOfNonTerminals;
	};

	/**
	 * Returns the number of edges of the right-hand side of the start
	 * production of the generated grammar.
	 */
	size_t getInitialSize() {
		return initialSize;
	};

	/**
	 * Returns the compression factor with respect to the number of edges
	 * of the input tree and the generated grammar, respectively.
	 */
	double getEdgesCompression() {
		return edgesCompression;
	};

	/**
	 * Returns the compression factor with respect to the file size of the
	 * input XML document and the size of the output file containing the
	 * final grammar, respectively.
	 */
	double getFilesystemCompression() {
		return filesystemCompression;
	};

	/**
	 * Returns the factor between the size of the right-hand side of the
	 * start production of the generated grammar (in number of edges) and
	 * the total number of edges of the generated grammar.
	 */
	double getInitialSizeFactor() {
		return initialSizeFactor;
	};

	/**
	 * Returns the mode in which the algorithm was executed.
	 */
	string getMode() {
		return mode;
	};

	/**
	 * Returns the runtime of the algorithm.
	 */
	int getRunTime();

	/**
	 * Returns the maximal amount of memory used during the execution of
	 * the algorithm (in megabytes).
	 */
	int getVmRSS() { return memVmRSS; };

	/**
	 * Sets the maximal amount of memory used during the execution of the
	 * algorithm (in megabytes).
	 */
	void setVmRSS(int memVmRSSPar) { memVmRSS = memVmRSSPar; };

	/**
	 * Serializes the instance of the Statistics class using the specified
	 * output stream.
	 */
	void serialize(ofstream & arc);

	/**
	 * Deserializes an instance of the Statisitcs class using the specified
	 * input stream.
	 */
	void deserialize(ifstream & arc);

private:
	/**
	 * Computes the size of the input file.
	 */
	void computeFilesystemOutputSize();

	/**
	 * Computes the size of the output file of the algorithm.
	 */
	void computeFilesystemInputSize();

	/**
	 * Computes the factor with respect to the number of input
	 * and output edges.
	 */
	void computeEdgesCompression();

	/**
	 * Computes the factor with respect to the sizes of the
	 * input and output file.
	 */
	void computeFilesystemCompression();

	/**
	 * Computes the size (number of edges) of the right-hand side
	 * of the start production of the generated grammar.
	 */
	void computeInitialSize(Grammar & dagGrammar);

	/**
	 * Computes the factor with respect to the size of the input tree
	 * and the size of the right-hand side of the start production
	 * of the generated grammar.
	 */
	void computeInitialSizeFactor();

	/**
	 * Prints the specified value with an appended percentage sign.
	 */
	static void printPercentage(ostream & out, double percentage);

	/**
	 * The input file of the algorithm.
	 */
	const char * pFilePath;

	/**
	 * The runtime of the algorithm.
	 */
	int timeInSeconds;

	/**
	 * The number of edges of the input tree.
	 */
	int numberOfInputEdges;

	/**
	 * The number of terminal symbols of the generated grammar.
	 */
	unsigned int numberOfTerminals;

	/**
	 * The number of edges of the generated grammar.
	 */
	int edgesOutputSize;

	/**
	 * The number of nonterminals of the generated grammar.
	 */
	size_t numberOfNonTerminals;

	/**
	 * The size of the input file (in bytes).
	 */
	int filesystemInputSize;

	/**
	 * The size of the output file of the algorithm (in bytes).
	 */
	int filesystemOutputSize;

	/**
	 * The size (number of edges) of the right-hand side of the
	 * start production of the generated grammar.
	 */
	size_t initialSize;

	/**
	 * The edges compression factor.
	 */
	double edgesCompression;

	/**
	 * The file size compression factor.
	 */
	double filesystemCompression;

	/**
	 * The factor with respect to the number of edges of the generated
	 * grammar and the number of edges of the right-hand side of the
	 * generated grammar.
	 */
	double initialSizeFactor;

	/**
	 * The mode of the algorithm executed.
	 */
	string mode;

	/**
	 * The maximal amount of main memory which was used by the algorithm
	 * during its execution.
	 */
	int memVmRSS;
};
