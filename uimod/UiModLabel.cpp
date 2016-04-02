/*****************************************************************************
* gta5sync GRAND THEFT AUTO V SYNC
* Copyright (C) 2016 Syping
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "UiModLabel.h"

UiModLabel::UiModLabel(const QString &text, QWidget *parent) : QLabel(parent)
{
    setText(text);
}

UiModLabel::UiModLabel(QWidget *parent, const QString &text) : QLabel(parent)
{
    setText(text);
}

UiModLabel::UiModLabel(QWidget *parent) : QLabel(parent)
{
}

UiModLabel::~UiModLabel()
{
}

void UiModLabel::mouseMoveEvent(QMouseEvent *ev)
{
    QLabel::mouseMoveEvent(ev);
    emit mouseMoved();
}

void UiModLabel::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent(ev);
    emit mousePressed();
}

void UiModLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    QLabel::mouseReleaseEvent(ev);
    emit mouseReleased();
}

void UiModLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QLabel::mouseDoubleClickEvent(ev);
    emit mouseDoubleClicked();
}