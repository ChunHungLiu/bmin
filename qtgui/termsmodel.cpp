/*
 *
 * created date: 3/9/2010
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

#include "termsmodel.h"
// kernel
#include "term.h"
#include "formula.h"
// shell - parsing
#include "parser.h"

TermsModel::TermsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_formula = 0;
}

TermsModel::~TermsModel()
{
    delete m_formula;
}

int TermsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (m_formula) {
        return m_formula->getMaxSize();
    }
    else
        return 0;
}

int TermsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant TermsModel::data(const QModelIndex &index, int role) const
{
    if (!m_formula || !index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    try {
        Parser::PrintForm form;
        if (m_formula->getRepre() == Formula::REP_SOP)
            form = Parser::PF_PROD;
        else
            form = Parser::PF_SUM;

        return QString::fromStdString(Parser::termToString(
                Term(index.row(), m_formula->getVarsCount()), m_formula->getVars(), form));

    }
    catch (std::exception &) {
        return QVariant();
    }

}

QVariant TermsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation == Qt::Horizontal)
        return QVariant();
    else
        return section;
}

void TermsModel::setFormula(Formula *formula)
{
    delete m_formula;
    if (formula)
        m_formula = new Formula(*formula);
    else
        m_formula = 0;
    emit layoutChanged();
}

void TermsModel::clearFormula()
{
    setFormula(0);
}
