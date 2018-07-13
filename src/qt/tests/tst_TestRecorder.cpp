/*
** Copyright (C) 2018 Sylvain Fargier
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
** Created on: 2018-07-12T15:51:17+02:00
**     Author: Sylvain Fargier <fargie_s> <fargier.sylvain@free.fr>
**
*/

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <QApplication>
#include <QLineEdit>
#include <QFile>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>

#include "tst_TestRecorder.hpp"
#include "TestRecorder.hpp"
#include "test_helpers_stddump.hpp"

AutoRecorder::AutoRecorder(QWidget *subject, bool doSave, QObject *parent) :
    QObject(parent),
    m_subject(subject),
    m_doSave(doSave)
{}

void AutoRecorder::onAskForUpdate()
{
    QWidget *widget = qApp->activeModalWidget();
    CPPUNIT_ASSERT(widget);
    qApp->postEvent(widget, new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier, ""));

    QTimer::singleShot(500, this, SLOT(onRecording()));
}

void AutoRecorder::onRecording()
{
    /* qApp->activeWindow() doesn't work with Qt4, it doesn't seem to give back focus properly */
    QWidget *widget = m_subject;
    CPPUNIT_ASSERT(widget);

    qApp->postEvent(widget, new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::NoModifier, "t"));
    qApp->postEvent(widget, new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::NoModifier, "e"));
    qApp->postEvent(widget, new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::NoModifier, "s"));
    qApp->postEvent(widget, new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::NoModifier, "t"));

    qApp->postEvent(widget, new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::ControlModifier, "c"));
}

void AutoRecorder::onValidate()
{
    QWidget *widget = qApp->activeModalWidget();
    CPPUNIT_ASSERT(widget);

    if (m_doSave)
        qApp->postEvent(widget, new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier, ""));
    else
        qApp->postEvent(widget, new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier, ""));
}

class TestRecorderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestRecorderTest);
    CPPUNIT_TEST(simpleRecord);
    CPPUNIT_TEST(autoRecordSave);
    CPPUNIT_TEST(autoRecordNoSave);
    CPPUNIT_TEST_SUITE_END();

protected:
    void simpleRecord() {
        QLineEdit edit;
        TestRecorder recorder("Simple Record", &edit);

        recorder.run("<br/>Enter the following text: \"test\"<br/>"
            "Do not copy/paste it !<br/>");
        CPPUNIT_ASSERT_EQUAL(QString("test"), edit.text());
    }

    void autoRecordSave() {
        QLineEdit edit;
        AutoRecorder autoRec(&edit, true);

        {
            TestRecorder recorder("Auto Record", &edit);
            QFile::remove(recorder.getDataFileName());

            QMetaObject::invokeMethod(&autoRec, "onAskForUpdate",
                Qt::QueuedConnection);

            recorder.run("<br/>Enter the following text: \"test\"<br/>"
                "Do not copy/paste it !<br/>");

            /* will ask to save */
            QMetaObject::invokeMethod(&autoRec, "onValidate",
                Qt::QueuedConnection);
        }
        CPPUNIT_ASSERT_EQUAL(QString("test"), edit.text());
    }

    void autoRecordNoSave() {
        QLineEdit edit;
        AutoRecorder autoRec(&edit, false);

        {
            TestRecorder recorder("Auto Record", &edit);
            QFile::remove(recorder.getDataFileName());

            QMetaObject::invokeMethod(&autoRec, "onAskForUpdate",
                Qt::QueuedConnection);

            recorder.run("<br/>Enter the following text: \"test\"<br/>"
                "Do not copy/paste it !<br/>");

            /* will ask to save */
            QMetaObject::invokeMethod(&autoRec, "onValidate",
                Qt::QueuedConnection);
        }
        CPPUNIT_ASSERT_EQUAL(QString("test"), edit.text());
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestRecorderTest);
