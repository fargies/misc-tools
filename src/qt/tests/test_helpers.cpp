/**
 ** Copyright (C) 2014 fargie_s
 **
 ** This software is provided 'as-is', without any express or implied
 ** warranty.  In no event will the authors be held liable for any damages
 ** arising from the use of this software.
 **
 ** Permission is granted to anyone to use this software for any purpose,
 ** including commercial applications, and to alter it and redistribute it
 ** freely, subject to the following restrictions:
 **
 ** 1. The origin of this software must not be misrepresented; you must not
 **    claim that you wrote the original software. If you use this software
 **    in a product, an acknowledgment in the product documentation would be
 **    appreciated but is not required.
 ** 2. Altered source versions must be plainly marked as such, and must not be
 **    misrepresented as being the original software.
 ** 3. This notice may not be removed or altered from any source distribution.
 **
 ** test_helpers.cpp
 **
 **        Created on: 07/12/2014
 **   Original Author: fargie_s
 **
 **/

#include <QCoreApplication>
#include <QFile>
#include <QDebug>

#include "test_helpers.hpp"

#include "config.h"

namespace testHelpers
{

#if !defined(SRC_DATADIR)
#warning SRC_DATADIR undefined
#define SRC_DATADIR "."
#endif

#if !defined(BIN_DATADIR)
#warning BIN_DATADIR undefined
#define BIN_DATADIR "."
#endif

QString getDataPath()
{
    static QString dataDir;

    if (dataDir.isEmpty())
    {
        QString dir;

        dir = QCoreApplication::applicationDirPath() + "/data";
        if (QFile::exists(dir))
            dataDir = dir;
        else if (QFile::exists(SRC_DATADIR))
            dataDir = SRC_DATADIR;
        else if (QFile::exists(BIN_DATADIR))
            dataDir = BIN_DATADIR;
    }
    return dataDir;
}

QString getDataFile(const QString &fileName)
{
    QString appFile = QCoreApplication::applicationDirPath() + "/data/" + fileName;
    if (QFile::exists(appFile))
        return appFile;
    else if (QFile::exists(SRC_DATADIR "/" + fileName))
        return SRC_DATADIR "/" + fileName;
    else if (QFile::exists(BIN_DATADIR "/" + fileName))
        return BIN_DATADIR "/" + fileName;
    else
        return QString();
}

}
