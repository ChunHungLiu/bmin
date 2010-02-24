/*
 *
 * created date: 2/20/2010
 *
 * Copyright (C) 2009-2010 Jakub Zelenka.
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

#include "kmap.h"
#include "formula.h"
#include "outputvalue.h"

#include <vector>
using namespace std;

const int GreyCode::code[1 << GC_MAX_VC] = {0, 1, 3, 2, 6, 7, 5, 4};

GreyCode::GreyCode()
{
    varsCount = 0;
}

void GreyCode::generate(int vc)
{
    varsCount = vc;
    size = (vc <= GC_MAX_VC)? (1 << vc): 0;
}

int GreyCode::getCode(int idx)
{
    return (idx < static_cast<int>(size))? code[idx]: -1;
}

KMap::KMap()
{
    varsCount = 0;
    cellsCount = 0;
}

void KMap::setFormula(Formula *f)
{
    formula = f;
    if (f == 0)
        error = NO_FORMULA;
    else if (f->getVarsCount() > MAX_VARS)
        error = TOO_MANY_VARS;
    else {
        error = OK;
        varsCount = f->getVarsCount();
        cellsCount = 1 << varsCount;
        // setting side and top vars and grey codes
        vars = vector<char>(f->getVars().begin(), f->getVars().end());
        makeVarsAndGC();
    }
}

void KMap::makeVarsAndGC()
{
    topVars.clear();
    sideVars.clear();
    switch (varsCount) {
    case 1:
        topVars.push_back(vars[0]);
        topGC.generate(1);
        sideGC.generate(0);
        break;
    case 2:
        topVars.push_back(vars[1]);
        sideVars.push_back(vars[0]);
        topGC.generate(1);
        sideGC.generate(1);
        break;
    case 3:
        topVars.push_back(vars[2]);
        topVars.push_back(vars[1]);
        sideVars.push_back(vars[0]);
        topGC.generate(2);
        sideGC.generate(1);
        break;
    case 4:
        topVars.push_back(vars[3]);
        topVars.push_back(vars[2]);
        sideVars.push_back(vars[1]);
        sideVars.push_back(vars[0]);
        topGC.generate(2);
        sideGC.generate(2);
        break;
    case 5:
        topVars.push_back(vars[4]);
        topVars.push_back(vars[3]);
        sideVars.push_back(vars[2]);
        sideVars.push_back(vars[1]);
        sideVars.push_back(vars[0]);
        topGC.generate(2);
        sideGC.generate(3);
        break;
    case 6:
        topVars.push_back(vars[5]);
        topVars.push_back(vars[4]);
        topVars.push_back(vars[0]);
        sideVars.push_back(vars[3]);
        sideVars.push_back(vars[2]);
        sideVars.push_back(vars[1]);
        topGC.generate(3);
        sideGC.generate(3);
        break;
    }
}

OutputValue KMap::getCellValue(unsigned row, unsigned col)
{
    if (row >= sideGC.getSize() || col >= topGC.getSize())
        return OutputValue(OutputValue::UNDEFINED);
    else {
        int sideCode = sideGC.getCode(row);
        int topCode = topGC.getCode(col);
        if (sideCode < 0 || topCode < 0)
            return OutputValue(OutputValue::UNDEFINED);
        // composition of Grey codes
        int idx;
        if (topGC.getSize() == 3) // 6 variables (the first bit of topCode is before sideCode)
           idx = (sideCode << 2) | (~3 & topCode) | ((~4 & topCode) << 3);
        else
            idx = (sideCode << topGC.getSize()) | topCode;

        return formula->getTermValue(idx);
    }
}
