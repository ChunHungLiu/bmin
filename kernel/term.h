/*
 * term.h - Term class for Quine–McCluskey algorithm
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

#ifndef TERM_H
#define TERM_H

#include "exceptions.h"

#include <iostream>
#include <vector>
#include <string>
#include <limits.h>

// term mask
typedef int term_t;

// maximum number of literals ( -1 means sign in int
#define TERM_MAX_SIZE (sizeof (term_t) * 8 - 1)

enum TermValue { ZERO, ONE, DC };

// Class represented product term
class Term
{
private:
    term_t liters;     // literals value
    term_t missing;    // which literals are missing literals
    int size;		   // number of literals
    bool dc;		   // dont care term

    // term initialization
    void init(term_t lit, term_t mis, int s, bool isDC);

public:
    // checks the variables validity
    static void checkVars(const std::vector<char> & var_names)
            throw(InvalidVarsExc);

    // constructor - the term of size s with all variables setted to missing value
    Term(int s = 0, bool isDC = false);
    // constructor - makes the variables array with size s by index idx
    Term(int idx, int s, bool isDC = false);
    // constructor - internal usage
    Term(term_t lit, term_t miss, int size, bool isDC = false);

    // true if this is don't care term
    bool isDC() const { return dc; }
    // sets whether this is dont care term
    void setDC(bool is_dc) { dc = is_dc; }
    // returns size of term, if all is false returns size reduced of dont cares
    int getSize(bool all = true) const;
    // returns terms index of boolean function
    int getIdx() const;
    // returns the count of values in term
    int valuesCount(int value) const;
    inline int valuesCount(const LiteralValue & value) const
    {
        return valuesCount(value.getValue());
    }
    // returns the new term combined (only by difference of one varible)
    // with *this and t, for example 0010 & 0000 => 00X0
    Term *combine(const Term & t) const;
    // replace first dont care by zero and one
    Term *expandMissingValue() const;
    // returns true if *this term implies term t
    bool implies(Term & t) const;

    // eqaulity operators
    bool operator==(const Term & t) const;
    bool operator<(const Term & t) const;
    bool operator>(const Term & t) const;
    // index operator
    LiteralValue operator[](int position);
    LiteralValue at(int position) throw(BadIndexExc);
    int getValueAt(int position);


    // term in string form: { 0 X 1 0 }
    std::string toString() const;
    // term in string form: A'BC
    std::string toString(std::vector<char> var_names) const throw(InvalidVarsExc);

    // friend function to place term to ostream
    friend std::ostream & operator<<(std::ostream & os, const Term & t);
};

#endif /* TERM_H */