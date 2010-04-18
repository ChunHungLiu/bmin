/*
 *
 * created date: 4/10/2010
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

#include "espresso.h"
#include "formula.h"
#include "literalvalue.h"
#include "espressocover.h"

using namespace std;

Espresso::~Espresso()
{
    delete of;
}

Formula *Espresso::minimize(Formula *formula, bool dbg)
{
    debug = dbg;

    delete of;
    of = new Formula(*formula, true);
    vc = formula->getVarsCount();
    fullRow = Term::getFullLiters(vc);

    EspressoCover f, d, r;
    Term *pcube;

    /* tautology test
    EspressoCover c;
    c.add(Term("0212"));
    c.add(Term("0122"));
    c.add(Term("1121"));
    c.add(Term("2220"));
    bool taut = tautology(c);*/

    /* cofactor test
    EspressoCover in, out;
    in.add(Term("2011"));
    in.add(Term("0210"));
    in.add(Term("1111"));
    cofactor(Term("2211"), in, out);*/


    // main loop
    unsigned cost, initCost;
    do {
        of->getCovers(f.cover, d.cover, r.cover);

        foreach_cube(f, pcube) {
            pcube->setPrime(false);
        }

        initCost = f.cost();
        expand(f, r);
        irredundant(f, d);

        do {
            do {
                cost = f.cost();
                reduce(f, d);
                expand(f, r);
                irredundant(f, d);
            } while (cost > f.cost());

            cost = f.cost();
            lastGasp(f, d, r);
        } while (cost > f.cost());
    } while (initCost < f.cost());

    mf = new Formula(*formula, f.cover);
    mf->setMinimized(true);
    formula->setMinimized(true);

    return mf;
}


// COFACTOR AND TAUTOLOGY

// returns cofactor (to out) of cover c with respect to cube p
void Espresso::cofactor(const Term &p, EspressoCover &c, EspressoCover &out, int flags)
{
    out.clear();
    Term *pcube;
    foreach_cube(c, pcube) {
        if (!flags || pcube->hasFlags(flags)) {
            Term t = pcube->cofactor(p, fullRow);
            if (t.isOne())
                out.add(t);
        }
    }
}

// Shannon expansion for variable at position pos of cover c, cofactors are save to out0 and out1
void Espresso::shannon(unsigned pos, EspressoCover &c, EspressoCover &out0, EspressoCover &out1)
{
    out0.clear();
    out1.clear();
    Term *pcube;
    foreach_cube(c, pcube) {
        Term t0 = pcube->cofactor(pos, false, fullRow);
        if (t0.isOne())
            out0.add(t0);
        Term t1 = pcube->cofactor(pos, true, fullRow);
        if (t1.isOne())
            out1.add(t1);
    }
}

// tautology algorithm for cover c
bool Espresso::tautology(EspressoCover &c, unsigned pos)
{
    // tautology if it has row with all 2's
    Term *pcube;
    foreach_cube (c, pcube) {
        if ((pcube->getMissing() & fullRow) == fullRow)
            return true;
    }

    EspressoCover c0, c1;
    shannon(pos, c, c0, c1);
    if (c0.isEmpty() || c1.isEmpty())
        return false;
    else
        return tautology(c0, pos + 1) && tautology(c1, pos + 1);
}


// EXPAND

// expand each nonprime cube of F into a prime implicant
void Espresso::expand(EspressoCover &f, EspressoCover &r)
{
     // tautology test
    if (r.isEmpty()) {
        f.setTautology();
        return;
    }


    // sorts in decreasing order - larger cube first
    f.sort();
    f.setCovered(false);

    Term *pcube;
    foreach_cube(f, pcube) {
        if (!pcube->isPrime() && !pcube->isCovered()) {
            // expand the cube pcube, result is raise
            expand1(*pcube, r, f);
        }
    }

    f.activeCount = 0;
    bool change = false;
    foreach_cube(f, pcube) {
        if (!pcube->isPrime() && pcube->isCovered()) {
            pcube->setActive(false);
            change = true;
        }
        else {
            pcube->setActive();
            f.activeCount++;
        }
    }
    if (change)
        f.removeInactived();

}

// expand a single cube against the OFF-set
void Espresso::expand1(Term &cube, EspressoCover &r, EspressoCover &f)
{
    EspressoCover bb = r;
    EspressoCover cc = f;
    matrices(cube, bb, cc);

    term_t lower = 0;
    term_t raise = 0;
    term_t essen, inessen, maxFeasible;

    while ((lower | raise) != fullRow && !bb.isCovered() && !cc.isCovered()) {
        essen = essential(bb);
        lower |= essen;
        elim1bb(essen, bb);
        elim1cc(essen, cc);

        maxFeasible = mfc(cc);
        raise |= maxFeasible;
        elim2(maxFeasible, bb, cc);

        inessen = inessential(bb);
        raise |= inessen;
        elim2(inessen, bb, cc);
    }

    if (!bb.isCovered())
        lower |= minlow(bb);

    // lowers variables
    cube.lower(lower);

    // sets covered cubes
    cc.setCovered(false);
    elim1cc(lower, cc);
    f.setCovering(cc);

    // cube is prime
    cube.setPrime();
}

// set up the blocking and covering cover
void Espresso::matrices(Term &cube, EspressoCover &bb, EspressoCover &cc)
{
    Term *pcube;

    foreach_cube(bb, pcube) {
        pcube->makeBB(cube);
        pcube->setCovered(false);
    }
    foreach_cube(cc, pcube) {
        pcube->makeCC(cube);
        pcube->setCovered(false);
    }
}

// essential columns are positions of one in single one row
term_t Espresso::essential(EspressoCover &bb)
{
    Term *row;
    term_t essen = 0;
    foreach_cube(bb, row) {
        if (!row->isCovered() && row->valuesCount(LiteralValue::ONE, bb.colMask) == 1)
            essen |= row->getFirstOnePos(bb.colMask);
    }
    return essen;
}

// inessential columns are zero columns of bb
term_t Espresso::inessential(EspressoCover &bb)
{
    Term *row;
    term_t inessen = fullRow & ~bb.colMask;
    bool hasCovered = false;
    foreach_cube(bb, row) {
        if (!row->isCovered()) {
            hasCovered = true;
            inessen &= ~row->getLiters();
        }
    }
    return hasCovered? inessen: 0;
}

// maximal fesible covering - column with the most 1's in matrix
term_t Espresso::mfc(EspressoCover &mat)
{
    vector<int> counts(vc, 0);
    term_t liters, pos;
    Term *row;
    foreach_cube(mat, row) {
        if (!row->isCovered()) {
            liters = row->getLiters() & ~mat.colMask;
            pos = 1;
            for (unsigned i = 0; i < vc; i++, pos <<= 1) {
                if (liters & pos)
                    counts[i]++;
            }
        }
    }

    int maxCol = -1;
    int max = 0;
    for (unsigned i = 0; i < counts.size(); i++) {
        if (counts[i] > max) {
            max = counts[i];
            maxCol = i;
        }
    }

    if (maxCol == -1) // no column with some 1's
        return 0;
    else
        return 1 << maxCol;
}

// minimal column covering of blocking matrix
term_t Espresso::minlow(EspressoCover &bb)
{
    term_t lower = 0, best;
    do {
        best = mfc(bb);
        lower |= best;
        elim1bb(best, bb);
    } while (best);
    return lower;
}

// eliminates rows and columns from blocking matrix
void Espresso::elim1bb(term_t columns, EspressoCover &bb)
{
    if (columns == 0) // nothing for removing
        return;

    // blocking matrix
    bb.colMask |= columns;
    Term *row;
    foreach_cube(bb, row) {
        if (!row->isCovered() && (columns & row->getLiters()))
            row->setCovered();
    }
}

// eliminates rows and columns from covering matrix
void Espresso::elim1cc(term_t columns, EspressoCover &cc)
{
    if (columns == 0) // nothing for removing
        return;

    // covering matrix
    cc.colMask |= columns;
    Term *row;
    foreach_cube(cc, row) {
        if (!row->isCovered() && (columns & ~row->getLiters()) == columns)
            row->setCovered();
    }
}

// eliminates columns from bb and cc
void Espresso::elim2(term_t columns, EspressoCover &bb, EspressoCover &cc)
{
    bb.colMask |= columns;
    cc.colMask |= columns;
}


// IRREDUNDANT

// returns a minimal subset of F
void Espresso::irredundant(EspressoCover &f, EspressoCover &d)
{
    f.appendDC(d);
    f.setActived(true);

    redundant(f);
    partialyRedundant(f);
    minimalIrredundant(f);

    f.removeDC();
    f.removeRedundant();
}

// finds essential and redundant cubes
void Espresso::redundant(EspressoCover &fd)
{
    EspressoCover cof;
    Term *pcube;
    foreach_cube(fd, pcube) {
        if (!pcube->isDC()) {
            pcube->setActive(false);
            cofactor(*pcube, fd, cof, Term::ACTIVE);
            bool isTaut = tautology(cof);
            pcube->setRedundant(isTaut);
            pcube->setRelativelyEssential(!isTaut);
            pcube->setActive(true);
        }
    }
}

// finds partialy and totaly redundant cubes
void Espresso::partialyRedundant(EspressoCover &fd)
{
    int flags = Term::ACTIVE | Term::DC | Term::RELESSEN;
    EspressoCover cof;
    Term *pcube;
    foreach_cube(fd, pcube) {
        if (pcube->isRedundant()) {
            pcube->setActive(false);
            cofactor(*pcube, fd, cof, flags);
            if (tautology(cof))
                pcube->setRedundant(false);
            pcube->setActive(true);
        }
    }
}

// finds minimal irredundant cover from partial redundant set
void Espresso::minimalIrredundant(EspressoCover &fd)
{
    EspressoCover cof;
    Term *pcube;
    foreach_cube(fd, pcube) {
        if (!pcube->hasFlags(Term::DC | Term::RELESSEN | Term::REDUND))  {
            cofactor(*pcube, fd, cof,  Term::DC | Term::RELESSEN);
            if (tautology(cof))
                pcube->setRedundant(true);
            else
                pcube->setRelativelyEssential(true);
        }
    }

}


// REDUCE

void Espresso::reduce(EspressoCover &f, EspressoCover &d)
{

}

void Espresso::lastGasp(EspressoCover &f, EspressoCover &d, EspressoCover &r)
{

}

