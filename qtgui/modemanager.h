/*
 * modemanager.h - modes manager (selecting mode...)
 * created date: 2009/08/04
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

#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include "constants.h"

#include <QObject>
#include <QList>

class ModuleWidget;
class WelcomeWidget;
class KMapWidget;
class CubeWidget;
class QmWidget;

struct Mode {
    Mode() : id(-1) {}
    Mode(int i, const QString &n) : id(i), name(n) {}
    int id;
    QString name;
};

// mode manager class
class ModeManager : public QObject
{
    Q_OBJECT

public:
    // singleton method for returning instance of ModeManager
    static ModeManager *instance();
    // destroy instance method
    static void destroy();
    // returns modes
    static QList<Mode> modes();


    // modes' identifiers
    static const int ID_WELCOME = 0;
    static const int ID_KMAP    = 1;
    static const int ID_QM      = 2;
#if CUBE3D
    static const int ID_CUBE    = 3;
#endif

    // return modules
    QList<ModuleWidget *> getModules() const { return m_modules; }


private:
    ModeManager();

    // static instance of ModeManager
    static ModeManager *s_instance;

    WelcomeWidget *m_welcome;
    KMapWidget *m_kmap;
    QmWidget *m_qm;
#if CUBE3D
    CubeWidget *m_cube;
#endif
    QList<ModuleWidget *> m_modules;

private slots:
    void setMode(int);

signals:
    // emitted when mode is changed
    void modeChanged(int);
    void kmapActivated(bool);
    void qmActivated(bool);
#if CUBE3D
    void cubeActivated(bool);
#endif
};

#endif // MODEMANAGER_H
