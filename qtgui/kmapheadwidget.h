/*
 * kmapheadwidget.h - K-map header cell
 * created date: 3/9/2010
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

#ifndef KMAPHEADWIDGET_H
#define KMAPHEADWIDGET_H

#include <vector>

#include <QGraphicsWidget>

// Class for head row in k-map
class KMapHeadWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    enum Direction { VERTICAL, HORIZONTAL, CORNER };
    enum Mode { LINES, BINARY };

    static const int SIZE         = 50;
    static const int FONT_SIZE    = 10;
    static const int LINE_SIZE    = 2;
    static const int LINE_SPACING = 14;


    KMapHeadWidget(Mode m, Direction d, QGraphicsItem *parent = 0);

    void setMode(Mode m);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    Mode m_mode;
    Direction m_direction;

    unsigned m_varsCount;
    qreal m_width;
    qreal m_height;

    QFont m_font;

private:
    QAction *m_linesAction;
    QAction *m_binaryAction;

private slots:
    void setLinesMode();
    void setBinaryMode();

signals:
    void modeChanged(KMapHeadWidget::Mode);
};

// Class for showing variables next to description in line mode
class KMapLineVarsWidget : public KMapHeadWidget
{
public:
    KMapLineVarsWidget(std::vector<char> *vars, Direction d, QGraphicsItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setVars(std::vector<char> *vars) { m_vars = vars; }

private:
    std::vector<char> *m_vars;
};

// Class for showing variables next to description in line mode
class KMapBinaryVarsWidget : public KMapHeadWidget
{
public:
    KMapBinaryVarsWidget(std::vector<char> *hVars, std::vector<char> *vVars, QGraphicsItem *parent = 0);\

    void setVars(std::vector<char> *hVars, std::vector<char> *vVars);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    std::vector<char> *m_hVars;
    std::vector<char> *m_vVars;

    QString m_hVarsStr;
    QString m_vVarsStr;

    QPoint m_hVarsPoint;
    QPoint m_vVarsPoint;
};


// Class for head description of cells
class KMapDescWidget : public KMapHeadWidget
{
public:
    KMapDescWidget(int code, unsigned vc, qreal size, Mode m, Direction d, QGraphicsItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int m_code;
    QString m_codeStr;
};

#endif // KMAPHEADWIDGET_H
