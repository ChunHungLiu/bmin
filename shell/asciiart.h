/*
 *
 * created date: 1/26/2010
 *
 * Copyright (C) 2009 Jakub Zelenka.
 *
 * Bmin is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * Bmin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Bmin; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#ifndef ASCIIART_H
#define ASCIIART_H

#include <iostream>
#include <list>

class QuineMcCluskeyData;

class AsciiArt
{
public:
    static const int MAX_VARS_QM   = 9;
    static const int MAX_VARS_MAP  = 4;
    static const int MAX_VARS_CUBE = 3;
    static const int CELL_SIZE     = 3;

    static const char EMPTY   = ' ';
    static const char SEP_COL = '|';
    static const char SEP_ROW = '-';

    AsciiArt(std::ostream *s) : os(s) {}

    void showQm(QuineMcCluskeyData *data);
    void showQmImpls(QuineMcCluskeyData *data);
    void showQmCover(QuineMcCluskeyData *data);

private:
    void maxError(std::ostream *os, const char *msg, int max);

    std::ostream *os;
};

class Implicants {
public:
    static const int PREF_SPACE = 1;
    static const int SUFF_SPACE = 2;

    Implicants() : maxSize(0) {}

    void addImpl(const char *str);
    void addImpl(std::string str);
    void addEmpty();
    void addSep();
    void frontSep(); // push sep front

    unsigned getWidth() { return maxSize + PREF_SPACE + SUFF_SPACE; }

    void printInit(std::ostream *s, char empty, char sep);
    void printNext();
    bool hasNext();

private:
    std::list<std::string> impls;
    std::list<std::string>::iterator it;
    std::ostream *os;
    unsigned maxSize;
    unsigned width;
    char separator;
    char empty;
};

static const char * const MSG_NO_DATA           = "No formula for Quine-McCluskey algorithm";
static const char * const MSG_P_IMPLS_FINDING   = "Finding Prime Implicants";
static const char * const MSG_P_IMPLS_COVERING  = "Prime Implicants Covering Table";
static const char * const MSG_FINDING_IMPLS     = "Finding Prime Implicants";
static const char * const MSG_SIZE_IMPLS        = "Size %d Implicants";
static const char * const MSG_NUMBER_OF_1S      = "Number of 1s";
static const char * const MSG_NUMBER_OF_0S      = "Number of 0s";
static const char * const MSG_MINTERM           = "Minterm";
static const char * const MSG_MAXTERM           = "Maxterm";
static const char * const MSG_CUBE              = "%d-Cube";
static const char * const MSG_MAX_VARS_QM       = "Too many variables for QM algorithm (max %d)";


#endif // ASCIIART_H