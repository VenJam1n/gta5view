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

#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"
#include "AppEnv.h"
#include "config.h"
#include <QMessageBox>
#include <QStringList>
#include <QLocale>
#include <QString>
#include <QDebug>
#include <QList>
#include <QDir>

OptionsDialog::OptionsDialog(ProfileDatabase *profileDB, QWidget *parent) :
    QDialog(parent), profileDB(profileDB),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    setupTreeWidget();
    setupLanguageBox();
}

OptionsDialog::~OptionsDialog()
{
    foreach(QTreeWidgetItem *playerItem, playerItems)
    {
        delete playerItem;
    }
    delete ui;
}

void OptionsDialog::setupTreeWidget()
{
    foreach(const QString &playerIDStr, profileDB->getPlayers())
    {
        bool ok;
        int playerID = playerIDStr.toInt(&ok);
        if (ok)
        {
            QString playerName = profileDB->getPlayerName(playerID);

            QStringList playerTreeViewList;
            playerTreeViewList << playerIDStr;
            playerTreeViewList << playerName;

            QTreeWidgetItem *playerItem = new QTreeWidgetItem(playerTreeViewList);
            ui->twPlayers->addTopLevelItem(playerItem);
            playerItems.append(playerItem);
        }
    }
    ui->twPlayers->sortItems(1, Qt::AscendingOrder);
}

void OptionsDialog::setupLanguageBox()
{
    QSettings settings(GTA5SYNC_APPVENDOR, GTA5SYNC_APPSTR);
    settings.beginGroup("Interface");
    currentLanguage = settings.value("Language","System").toString();
    settings.endGroup();

    QString cbSysStr = tr("%1 (%2 if available) [sys]", "System like PC System = %1, System Language like Deutsch = %2").arg(tr("System",
    "System like PC System"), QLocale::system().nativeLanguageName());
    ui->cbLanguage->addItem(cbSysStr, "System");

    QString cbEngStr = "English (English) [en]";
    ui->cbLanguage->addItem(QIcon::fromTheme("flag-us"), cbEngStr, "en");
    if (currentLanguage == "en")
    {
        ui->cbLanguage->setCurrentText(cbEngStr);
    }

    QDir langDir;
    langDir.setNameFilters(QStringList("gta5sync_*.qm"));
    langDir.setPath(AppEnv::getLangFolder());
    QStringList langFiles;
    langFiles << langDir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::NoSort);
    langDir.setPath(":/tr");
    langFiles << langDir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::NoSort);
    langFiles.removeDuplicates();

    foreach(const QString &langFile, langFiles)
    {
        QString lang = langFile;
        lang.remove("gta5sync_");
        lang.remove(".qm");

        QLocale langLocale(lang);
        QString languageNameInternational = QLocale::languageToString(langLocale.language());
        QString languageNameNative = langLocale.nativeLanguageName();

        QString cbLangStr = languageNameNative + " (" + languageNameInternational + ") [" + lang + "]";
        QString langIconStr = "flag-" + lang;

        ui->cbLanguage->addItem(QIcon::fromTheme(langIconStr), cbLangStr, lang);
        if (currentLanguage == lang)
        {
            ui->cbLanguage->setCurrentText(cbLangStr);
        }
    }
}

void OptionsDialog::on_cmdOK_clicked()
{
    applySettings();
    close();
}

void OptionsDialog::applySettings()
{
    QSettings settings(GTA5SYNC_APPVENDOR, GTA5SYNC_APPSTR);
    settings.beginGroup("Interface");
    settings.setValue("Language", ui->cbLanguage->currentData());
    settings.endGroup();

    if (ui->cbLanguage->currentData().toString() != currentLanguage)
    {
        QMessageBox::information(this, tr("%1", "%1").arg(GTA5SYNC_APPSTR), tr("The language change will take effect after you restart %1.").arg(GTA5SYNC_APPSTR));
    }
}