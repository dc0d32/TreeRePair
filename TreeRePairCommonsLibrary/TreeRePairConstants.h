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
#define KEYWORD_RANKS "ranks"
#define KEYWORD_NAMES "names"
#define KEYWORD_TERMINALS "terminals"

#define START_TAG "xml_collapse_start"
#define PROGRESS_RESOLUTION 50
#define DEFAULT_ARITY 2

#define MAX_RANK 4

#define CODE_LENGTH_BITS 18

#define RLC_NON_NULL_CODE_LENGTH_BITS 2
#define RLC_NULL_CODE_LENGTH_FEW_BITS 3
#define RLC_NULL_CODE_LENGTH_MANY_BITS 7
#define RLC_MAX_RUN_LENGTH 3

#define COL_SEP 1
