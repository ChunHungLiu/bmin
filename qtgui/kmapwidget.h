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

#ifndef KMAPWIDGET_H
#define KMAPWIDGET_H

#include "modulewidget.h"

#include <QWidget>

class GUIManager;
class Kernel;
class KMap;

class KMapWidget : public ModuleWidget
{
    Q_OBJECT

public:
    KMapWidget(const QString &name, int pos);

private:
    GUIManager *m_gm;
    Kernel *m_kernel;
    KMap *m_kmap;

public slots:
    void setActivity(bool a);
    void updateData();
};

#endif // KMAPWIDGET_H
