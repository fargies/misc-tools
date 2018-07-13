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
** Created on: 2018-07-12T17:13:12+02:00
**     Author: Sylvain Fargier <fargie_s> <fargier.sylvain@free.fr>
**
*/


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <QKeyEvent>
#include <QDataStream>
#include <QSharedPointer>
#include <QEvent>

#include "InputEventSerializer.hpp"
#include "test_helpers_stddump.hpp"

class InputEventSerializerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(InputEventSerializerTest);
    CPPUNIT_TEST(contextMenuEvent);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    CPPUNIT_TEST(keyEventQt5);
    CPPUNIT_TEST(mouseEventQt5);
    CPPUNIT_TEST(wheelEventQt5);
#else
    CPPUNIT_TEST(keyEventQt4);
    CPPUNIT_TEST(mouseEventQt4);
    CPPUNIT_TEST(wheelEventQt4);
#endif
    CPPUNIT_TEST_SUITE_END();

protected:
    void streamIn(QInputEvent &evt, QByteArray &buffer) {
        QDataStream in(&buffer, QIODevice::WriteOnly);
        in << &evt;
        CPPUNIT_ASSERT(in.status() == QDataStream::Ok);
    }

    void streamOut(QInputEvent *&evt, QByteArray &buffer) {
        QDataStream out(&buffer, QIODevice::ReadOnly);
        out >> evt;
        CPPUNIT_ASSERT(out.atEnd());
    }

    void contextMenuEvent() {
        QContextMenuEvent evt(QContextMenuEvent::Mouse,
            QPoint(123, 124), QPoint(234, 345), Qt::AltModifier);

        QByteArray buffer;
        streamIn(evt, buffer);

        QInputEvent *evt2;
        streamOut(evt2, buffer);
        QSharedPointer<QInputEvent> evtShared(evt2);
        CPPUNIT_ASSERT(evt2);
        QContextMenuEvent *kevt2 = dynamic_cast<QContextMenuEvent *>(evt2);
        CPPUNIT_ASSERT(kevt2);
        CPPUNIT_ASSERT_EQUAL(QContextMenuEvent::Mouse, kevt2->reason());
        CPPUNIT_ASSERT_EQUAL(QPoint(123, 124), kevt2->pos());
        CPPUNIT_ASSERT_EQUAL(QPoint(234, 345), kevt2->globalPos());
        CPPUNIT_ASSERT_EQUAL(Qt::KeyboardModifiers(Qt::AltModifier), kevt2->modifiers());
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    void keyEventQt5() {
        QKeyEvent evt(QEvent::KeyPress, Qt::Key_F1, Qt::ControlModifier,
            12, 24, 32, "F1");

        QByteArray buffer;
        streamIn(evt, buffer);

        QInputEvent *evt2;
        streamOut(evt2, buffer);
        QSharedPointer<QInputEvent> evtShared(evt2);
        CPPUNIT_ASSERT(evt2);
        QKeyEvent *kevt2 = dynamic_cast<QKeyEvent *>(evt2);
        CPPUNIT_ASSERT(kevt2);
        CPPUNIT_ASSERT_EQUAL(evt.modifiers(), kevt2->modifiers());
        CPPUNIT_ASSERT_EQUAL(evt.key(), kevt2->key());
    }

    void mouseEventQt5() {
        QMouseEvent evt(QEvent::MouseButtonRelease, QPointF(1.2, 2.3),
            QPointF(3.4, 3.5), QPointF(4.4, 5.5),
            Qt::LeftButton, Qt::LeftButton, Qt::AltModifier);

        QByteArray buffer;
        streamIn(evt, buffer);

        QInputEvent *evt2;
        streamOut(evt2, buffer);
        QSharedPointer<QInputEvent> evtShared(evt2);
        CPPUNIT_ASSERT(evt2);
        QMouseEvent *kevt2 = dynamic_cast<QMouseEvent *>(evt2);
        CPPUNIT_ASSERT(kevt2);
        CPPUNIT_ASSERT_EQUAL(QPointF(1.2, 2.3), kevt2->localPos());
        CPPUNIT_ASSERT_EQUAL(QPointF(3.4, 3.5), kevt2->windowPos());
        CPPUNIT_ASSERT_EQUAL(QPointF(4.4, 5.5), kevt2->screenPos());
        CPPUNIT_ASSERT_EQUAL(QEvent::MouseButtonRelease, kevt2->type());
        CPPUNIT_ASSERT_EQUAL(Qt::LeftButton, kevt2->button());
        CPPUNIT_ASSERT_EQUAL(Qt::MouseButtons(Qt::LeftButton),
            kevt2->buttons());
        CPPUNIT_ASSERT_EQUAL(Qt::KeyboardModifiers(Qt::AltModifier),
            kevt2->modifiers());
    }

    void wheelEventQt5() {
        QWheelEvent evt(QPointF(1.2, 1.3), QPointF(2.2, 2.3), QPoint(1, 2),
            QPoint(3, 4), 42, Qt::Vertical, Qt::LeftButton, Qt::AltModifier,
            Qt::ScrollBegin, Qt::MouseEventSynthesizedBySystem, true);

        QByteArray buffer;
        streamIn(evt, buffer);

        QInputEvent *evt2;
        streamOut(evt2, buffer);
        QSharedPointer<QInputEvent> evtShared(evt2);
        CPPUNIT_ASSERT(evt2);
        QWheelEvent *kevt2 = dynamic_cast<QWheelEvent *>(evt2);
        CPPUNIT_ASSERT(kevt2);
        CPPUNIT_ASSERT_EQUAL(QPointF(1.2, 1.3), kevt2->posF());
        CPPUNIT_ASSERT_EQUAL(QPointF(2.2, 2.3), kevt2->globalPosF());
        CPPUNIT_ASSERT_EQUAL(QPoint(1, 2), kevt2->pixelDelta());
        CPPUNIT_ASSERT_EQUAL(QPoint(3, 4), kevt2->angleDelta());
        CPPUNIT_ASSERT_EQUAL(42, kevt2->delta());
        CPPUNIT_ASSERT_EQUAL(Qt::Vertical, kevt2->orientation());
        CPPUNIT_ASSERT_EQUAL(Qt::MouseButtons(Qt::LeftButton), kevt2->buttons());
        CPPUNIT_ASSERT_EQUAL(Qt::KeyboardModifiers(Qt::AltModifier),
            kevt2->modifiers());
        CPPUNIT_ASSERT_EQUAL(Qt::ScrollBegin, kevt2->phase());
        CPPUNIT_ASSERT_EQUAL(Qt::MouseEventSynthesizedBySystem, kevt2->source());
        CPPUNIT_ASSERT_EQUAL(true, kevt2->inverted());
    }

#else
    void keyEventQt4() {

    }

    void mouseEventQt4() {

    }

    void wheelEventQt4() {

    }
#endif
};

CPPUNIT_TEST_SUITE_REGISTRATION(InputEventSerializerTest);
