/*
 * quinemccluskeydata.h - debugging dat for Quine-McCluskey algorithm
 * created date: 5/1/2010
 *
 * Copyright (C) 2010 Jakub Zelenka.
 *
 * Bmin is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Bmin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Bmin; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QUINEMCCLUSKEYDATA_H
#define QUINEMCCLUSKEYDATA_H

#include "term.h"
#include "termssortinglist.h"

#include <vector>
#include <list>
#include <set>

class QuineMcCluskeyData
{
public:
    struct Combination
    {
        Combination(Term *pleft, Term *pright, Term *pcombined) :
                left(*pleft), right(*pright), combined(*pcombined) {}
        Term left;
        Term right;
        Term combined;
    };

    QuineMcCluskeyData();
    QuineMcCluskeyData(const QuineMcCluskeyData &qmd);
    ~QuineMcCluskeyData();
    // initialization
    void initImpls(int vars, bool sp);
    void initCover(std::vector<Term> *row, std::vector<Term> *col);

    void setEmpty(bool sp) { empty = true; sop = sp; }
    bool isEmpty() { return empty; }
    bool isSoP() { return sop; }
    bool isCovered(int row, int col);
    void setCover(int row, int col);

    void addImpl(int missings, int explicits, Term *t);
    TermsSortingList *getImpls(int missings, int explicits);
    void addCombination(Term *left, Term *right, Term *combined);
    std::list<Combination> *getCombinations();
    void setPrimes(std::vector<Term> primes);
    int getVarsCount() { return varsCount; }
    int getMaxMissings() { return maxMissings; }

    int firstExplicitTerm();
    int lastExplicitTerm();

    std::vector<Term> *getCoverHeadRow() { return &coverHeadRow; }
    std::vector<Term> *getCoverHeadCol() { return &coverHeadCol; }

private:
    inline int getImplsIdx(int missings, int explicits);
    inline int getCoverIdx(int row, int col);

    bool empty;
    bool sop;
    int varsCount;
    int maxMissings;
    int coverRowsCount;
    int coverColsCount;
    TermsSortingList *impls;
    size_t implsSize;
    std::vector<Term> coverHeadRow;
    std::vector<Term> coverHeadCol;
    std::set<int> coverTable;
    std::list<Combination> combinations;
};

#endif // QUINEMCCLUSKEYDATA_H
