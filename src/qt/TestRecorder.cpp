/*
** Copyright (C) 2013 Fargier Sylvain <fargier.sylvain@free.fr>
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
** TestRecorder.cpp
**
**        Created on: Apr 06, 2013
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QFile>

#include "config.h"

#include "TestRecorder.hpp"
#include "InputEventRecorder.hpp"
#include "SignalWaiter.hpp"

/* should be defined in config.h */
#if !defined(SRC_DATADIR)
#warning SRC_DATADIR undefined
#define SRC_DATADIR "."
#endif

#if !defined(BIN_DATADIR)
#warning BIN_DATADIR undefined
#define BIN_DATADIR "."
#endif

static bool s_forced = false;

TestRecorder::TestRecorder(
        const QString &name,
        QObject *observee,
        QObject *parent) :
    QObject(parent),
    m_recorder(new InputEventRecorder(observee)),
    m_dataName(name),
    m_update(false)
{
    qApp->setQuitOnLastWindowClosed(false);
    m_recorder->nameAllWidgets(observee);
    connect(observee, SIGNAL(destroyed()),
            m_recorder, SLOT(stop()));
}

TestRecorder::~TestRecorder()
{
    if (m_update && askToSave())
        m_recorder->save(getDataFileName());
    delete m_recorder;
}

void TestRecorder::run(const QString &msg, float speed)
{
    if (m_recorder->obj())
    {
        QMetaObject::invokeMethod(m_recorder->obj(), "show");
        QMetaObject::invokeMethod(m_recorder->obj(), "setFocus");
    }

    if ((s_forced || !QFile::exists(getDataFileName())) && askForUpdate(msg))
    {
        m_update = true;
        SignalWaiter w(m_recorder, SIGNAL(recordDone()));
        m_recorder->record();
        w.wait(-1);
    }
    else
    {
        SignalWaiter w(m_recorder, SIGNAL(replayDone()));
        m_recorder->load(getDataFileName());
        m_recorder->replay(speed);
        w.wait(-1);
    }
}

void TestRecorder::stopOnLastWin()
{
    connect(qApp, SIGNAL(lastWindowClosed()),
            this, SLOT(stop()));
}

void TestRecorder::setForced(bool value)
{
    s_forced = value;
}

void TestRecorder::stop()
{
    m_recorder->stop();
}

bool TestRecorder::askForUpdate(const QString &msg) const
{
    int ret = QMessageBox::question(0, "Update interface record",
            QString("<b>Update this test record ?</b>"
                "<br/><br/>Test: <br/>%1"
                "<br/><br/><small>(type-in ctrl-c (or cmd-c) once done)</small>"
            ).arg(msg),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    return ret == QMessageBox::Yes;
}

bool TestRecorder::askToSave() const
{
    int ret = QMessageBox::question(0, "Save interface record",
            "<b>Save the new interface record ?</b>",
            QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Save);
    return ret == QMessageBox::Save;
}

QString TestRecorder::getDataFileName() const
{
    QString dir;

    dir = QCoreApplication::applicationDirPath() + QDir::separator() + "data";
    if (!QFile::exists(dir))
    {
        if (QFile::exists(SRC_DATADIR))
            dir = SRC_DATADIR;
        else if (QFile::exists(BIN_DATADIR))
            dir = BIN_DATADIR;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    return dir + QDir::separator() + m_dataName + "_Qt5.events";
#else
    return dir + QDir::separator() + m_dataName + "_Qt4.events";
#endif
}
