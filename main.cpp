/*****************************************************************************
* gta5sync GRAND THEFT AUTO V SYNC
* Copyright (C) 2016 Syping Gaming Team
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

#include "SnapmaticPicture.h"
#include "PictureDialog.h"
#include <QApplication>
#include <QStringList>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList applicationArgs = a.arguments();
    QString selectedAction;
    QString arg1;

    foreach(QString currentArg, applicationArgs)
    {
        QString reworkedArg;
        if (currentArg.left(9) == "-showpic=" && selectedAction == "")
        {
            reworkedArg = currentArg.remove(0,9);
            arg1 = reworkedArg;
            selectedAction = "showpic";
        }
    }

    if (selectedAction == "showpic")
    {
        PictureDialog picDialog;
        SnapmaticPicture picture;
        picture.readingPictureFromFile(arg1);
        picDialog.setWindowFlags(picDialog.windowFlags()^Qt::WindowContextHelpButtonHint);
        picDialog.setWindowTitle(picture.getPictureStr());
        picDialog.setSnapmaticPicture(picture.getPixmap());
        picDialog.setJsonString(&picture);
        picDialog.show();

        return a.exec();

        qDebug() << "showpic runned";
    }

    return a.exec();
}
