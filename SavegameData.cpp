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

#include "SavegameData.h"
#include <QTextCodec>
#include <QDebug>
#include <QFile>

SavegameData::SavegameData(QString fileName, QObject *parent) : QObject(parent), savegameFileName(fileName)
{
    // PARSE INT INIT - DO NOT CHANGE THIS VALUES
    savegameHeaderLength = 260;
    verificationValue = QString::fromLatin1("00000001");

    // INIT SAVEGAME
    savegameStr = "";
    savegameOk = 0;
}

bool SavegameData::readingSavegame()
{
    // Start opening file
    // lastStep is like currentStep

    QFile *saveFile = new QFile(savegameFileName);
    if (!saveFile->open(QFile::ReadOnly))
    {
        lastStep = "1;/1,OpenFile," + convertDrawStringForLog(savegameFileName);
        saveFile->deleteLater();
        delete saveFile;
        return false;
    }

    // Reading Savegame Header
    if (!saveFile->isReadable())
    {
        lastStep = "2;/3,ReadingFile," + convertDrawStringForLog(savegameFileName) + ",1,NOHEADER";
        saveFile->close();
        saveFile->deleteLater();
        delete saveFile;
        return false;
    }
    QByteArray savegameHeaderLine = saveFile->read(savegameHeaderLength);
    if (QString::fromLatin1(savegameHeaderLine.left(4).toHex()) == verificationValue)
    {
        savegameStr = getSavegameDataString(savegameHeaderLine);
        if (savegameStr.length() >= 1)
        {
            savegameOk = true;
        }
    }
    saveFile->close();
    saveFile->deleteLater();
    delete saveFile;
    return savegameOk;
}

QString SavegameData::getSavegameDataString(QByteArray savegameHeader)
{
    QString savegameTitle;
    QByteArray savegameBytes = savegameHeader.left(savegameHeaderLength);
    QList<QByteArray> savegameBytesList = savegameBytes.split(char(0x01));
    savegameBytes = savegameBytesList.at(1);

    int savegameLength = savegameBytes.length();
    int parsedBytes = 0;

    while (parsedBytes <= savegameLength)
    {
        QList<QByteArray> parseByteList;
        parseByteList.append(savegameBytes.mid(parsedBytes-1, 1));
        parseByteList.append(savegameBytes.mid(parsedBytes-2, 1));
        if (parseByteList.at(0).toHex() == "00")
        {
            // Latin character
            savegameTitle.append(QString::fromLatin1(parseByteList.at(1)));
        }
        else if (parseByteList.at(0).toHex() == "30")
        {
            // Japanese character
            QByteArray japaneseHex;
            japaneseHex.append(QByteArray::fromHex("A5"));
            japaneseHex.append(parseByteList.at(1));
            savegameTitle.append(QTextCodec::codecForName("EUC-JP")->toUnicode(japaneseHex));
        }
        else
        {
            // Unsupported
        }
        parsedBytes = parsedBytes + 2;
        parseByteList.clear();
    }

    savegameBytesList.clear();
    savegameBytes.clear();
    return savegameTitle;
}

bool SavegameData::readingSavegameFromFile(QString fileName)
{
    if (fileName != "")
    {
        savegameFileName = fileName;
        return readingSavegame();
    }
    else
    {
        return false;
    }
}

QString SavegameData::convertDrawStringForLog(QString inputStr)
{
    return inputStr.replace("&","&u;").replace(",","&c;");
}

QString SavegameData::convertLogStringForDraw(QString inputStr)
{
    return inputStr.replace("&c;",",").replace("&u;","&");
}

bool SavegameData::isSavegameOk()
{
    return savegameOk;
}

QString SavegameData::getSavegameStr()
{
    return savegameStr;
}

QString SavegameData::getLastStep()
{
    return lastStep;
}
