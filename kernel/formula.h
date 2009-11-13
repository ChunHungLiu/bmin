/*
 * formula.h - Formula class for Quine–McCluskey algorithm
 * created date: 2007/08/24
 *
 * Copyright (C) 2007-2009 Jakub Zelenka.
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

#ifndef FORMULA_H
#define FORMULA_H

#include "term.h"
#include "termscontainer.h"
#include "kernelexc.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <exception>
#include <stdexcept>


struct FormulaDecl
{
    FormulaDecl(const std::vector<char> *v = 0, char n = 'f') : vars(v), name(n) {}
    ~FormulaDecl() { delete vars; }
    const std::list<char> *vars;
    char name;
};

struct FormulaSpec
{
    FormulaSpec(std::set<int> *_f = 0, std::set<int> *_d = 0, std::set<int> *_r = 0)
                : f(_f), d(_d), r(_r) {}
    ~FormulaSpec();
    std::set<int> *f; // one
    std::set<int> *d; // dc
    std::set<int> *r; // zero
};


// class represented logical formula
class Formula
{
public:
    static void checkVars(const vector<char> &v) throw(InvalidVarsExc);

    // Constructors
    Formula::Formula(std::vector<Term> &t, char name = 'f', std::vector<char> *v = 0)
            throw(InvalidTermExc, NoTermExc);
    Formula::Formula(Term *t, int n, char name = 'f', std::vector<char> *v = 0)
            throw(InvalidTermExc, NoTermExc);
    Formula::Formula(FormulaSpec *spec, FormulaDecl *decl)
            throw(InvalidTermExc, NoTermExc);

    // adds new term to formula
    void pushTerm(int idx, bool isDC = false) throw(InvalidIndexExc);
    // removes term with idx
    void removeTerm(int idx) throw(InvalidIndexExc);
    // finds out whether term t is in TermsContainer
    inline bool hasTerm(const Term &t) { return terms->hasTerm(t); }
    // returns value of term with idx
    inline Term::OutputValue getTermValue(int idx) { terms->getTermValue(idx); }
    // returns terms id with val from original terms
    inline std::vector<int> getTermsIdx(Term::OutputValue val) { terms->getTermsIdx(val); }
    // returns actual minterms
    inline std::vector<Term> getMinterms() { terms->getMinterms(); }
    // returns number of terms
    inline int getSize() { return terms.getSize(); }

    // whether formula is minimized
    inline bool isMinimized() { return minimized; }

    // equality
    bool operator==(const Formula &f) { return terms == f.terms; }
    bool equal(const Formula &f, inclVars = true);


    // name setter
    inline void setName(char n) { name = n; }
    // name getter
    inline char getName() { return name; }

    // set default names for n variables
    void setVars(int n);
    // sets variables name by array of characters v
    void setVars(char * v, int n) throw(InvalidVarsExc);
    // sets variables name by vector v
    void setVars(std::vector<char> &v) throw(InvalidVarsExc);
    // returns variables
    std::vector<char> getVars() { return vars; }
    // returns number of varibles
    inline int getVarsCount() { return varsSize; }

    // friend function to place term to ostream (to debugging)
    friend std::ostream &operator<<(std::ostream &os, Formula &t);

    friend class MinimizingAlgorithm;


private:
    void init();
    inline void minimized(bool m) { minimized = m; }


    TermsContainer *terms;

    // formula name
    char name;
    // names of variables
    std::vector<char> vars;

    int varsSize;
    bool minimized;
    int maxIdx;
};

#endif /* FORMULA_H */

