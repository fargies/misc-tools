/* Copyright 2011 Stanislaw Adaszewski. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY STANISLAW ADASZEWSKI ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of Stanislaw Adaszewski. */

#include "InputEventRecorder.hpp"

#include <QContextMenuEvent>
#include <QFile>
#include <QDataStream>
#include <QTimer>
#include <QWidget>
#include <QApplication>
#include <QDebug>

#include "InputEventSerializer.hpp"

//
// Helper routines
//

static QString objectPath(QObject *obj)
{
    bool first = true;
    QString res;
    for(; obj; obj = obj->parent())
    {
        if (!first)
            res.prepend("/");
        else
            first = false;
        res.prepend(obj->objectName());
    }
    return res;
}

static bool isChild(QObject *obj, QObject *parent)
{
    while ((obj = obj->parent()))
    {
        if (obj == parent)
            return true;
    }

    return false;
}

//
// InputEventRecorder::EventDelivery
//

InputEventRecorder::EventDelivery::EventDelivery(int timeOffset, QObject *obj, QEvent *ev):
    m_TimeOffset(timeOffset),
    m_ClsName(obj->metaObject()->className()),
    m_ObjName(objectPath(obj)),
    m_Ev(ev)
{
}

//
// InputEventRecorder
//

QEvent* InputEventRecorder::cloneEvent(QEvent *ev)
{
    if (dynamic_cast<QContextMenuEvent*>(ev))
        return new QContextMenuEvent(*static_cast<QContextMenuEvent*>(ev));
    else if (dynamic_cast<QKeyEvent*>(ev))
        return new QKeyEvent(*static_cast<QKeyEvent*>(ev));
    else if (dynamic_cast<QMouseEvent*>(ev))
        return new QMouseEvent(*static_cast<QMouseEvent*>(ev));
    else if (dynamic_cast<QWheelEvent*>(ev))
        return new QWheelEvent(*static_cast<QWheelEvent*>(ev));

    return 0;
}

bool InputEventRecorder::isRecordable(const QEvent *evt) const
{
    switch (evt->type())
    {
    case QEvent::Wheel:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::ShortcutOverride:
    case QEvent::ContextMenu:
        return true;
    default:
        return false;
    }
}

InputEventRecorder::InputEventRecorder(QObject *obj):
    m_Obj(obj),
    m_InReplay(false),
    m_Timer(new QTimer),
    m_uniqueId(0)
{
    m_Timer->setSingleShot(true);
    QObject::connect(m_Timer, SIGNAL(timeout()), this, SLOT(replay()));
}

InputEventRecorder::~InputEventRecorder()
{
    delete m_Timer;
}

bool InputEventRecorder::eventFilter(QObject *obj, QEvent *ev)
{
    if (m_InReplay || !m_ReplayFilter.isEmpty())
    {
        if (!m_ReplayFilter.remove(ev) && isRecordable(ev))
            return true; /* filtering external events during replays */
        else if (m_ReplayFilter.isEmpty() && !m_InReplay)
            qApp->removeEventFilter(this);
        return false;
    }

    if ((obj != m_Obj) && !isChild(obj, m_Obj))
        return false;

    /* stop recording when we detect ctrl-c */
    if (ev && ev->type() == QEvent::KeyPress)
    {
        QKeyEvent *kev = static_cast<QKeyEvent *>(ev);
        if ((kev->modifiers() == Qt::ControlModifier) &&
                (kev->key() == Qt::Key_C))
        {
            stop();
            return false;
        }
    }

    QEvent *clonedEv = cloneEvent(ev);

    if (clonedEv)
    {
        int timeOffset;
        QDateTime curDt(QDateTime::currentDateTime());
        timeOffset = m_RecordingStartTime.daysTo(curDt) * 24 * 3600 * 1000 + m_RecordingStartTime.time().msecsTo(curDt.time());
        m_Recording.push_back(EventDelivery(timeOffset, obj, clonedEv));
    }

    return false;
}

void InputEventRecorder::save(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::WriteOnly))
        return;
    QDataStream ds(&f);
    foreach(EventDelivery ed, m_Recording)
    {
        ds << (qint32) ed.timeOffset() << ed.clsName() << ed.objName();
        QEvent *ev(ed.event());
        ds << static_cast<QInputEvent*>(ev);
    }
}

void InputEventRecorder::load(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly))
        return;

    m_Recording.clear();

    QDataStream ds(&f);
    while (!ds.atEnd())
    {
        qint32 timeOffset;
        QString clsName, objName;
        ds >> timeOffset >> clsName >> objName;
        QInputEvent *ev;
        ds >> ev;
        m_Recording.push_back(EventDelivery(timeOffset, clsName, objName, ev));
    }
}

void InputEventRecorder::nameAllWidgets(QObject *w)
{
    QObjectList children = w->children();
    foreach(QObject *o, children)
    {
        if (dynamic_cast<QWidget*>(o))
        {
            if (o->objectName().isEmpty())
                o->setObjectName(QString("unique_%1").arg(m_uniqueId++));
            nameAllWidgets(static_cast<QWidget*>(o));
        }
    }
}

void InputEventRecorder::record()
{
    qApp->installEventFilter(this);
    m_RecordingStartTime = QDateTime::currentDateTime();
}

void InputEventRecorder::stop()
{
    qApp->removeEventFilter(this);
    m_Timer->stop();
    emit recordDone();
}

void InputEventRecorder::replay(float speedFactor)
{
    if (m_Recording.size() == 0)
    {
        emit replayDone();
        return;
    }

    qApp->installEventFilter(this);
    m_InReplay = true;
    m_ReplayPos = 0;
    m_ReplaySpeedFactor = speedFactor;
    m_Timer->setInterval(0);
    m_Timer->start();
}

void InputEventRecorder::replay()
{
    EventDelivery& rec(m_Recording[m_ReplayPos++]);
    if (m_ReplayPos >= m_Recording.size())
    {
        /* must do this before posting last */
        m_InReplay = false;
    }

    QStringList path = rec.objName().split("/", QString::KeepEmptyParts);
    if (path.size() > 0)
    {
        QList<QObject*> objects = m_Obj->findChildren<QObject*>(path.last());
        if (path.size() == 1)
            objects.append(m_Obj);

        foreach(QObject *obj, objects)
        {
            if (obj->metaObject()->className() == rec.clsName() && objectPath(obj) == rec.objName())
            {
                QEvent *posted = cloneEvent(rec.event());
                m_ReplayFilter.insert(posted);
                qApp->postEvent(obj, posted);
                break;
            }
        }
    }

    if (m_ReplayPos >= m_Recording.size())
    {
        emit replayDone();
        return;
    }

    int delta = m_Recording[m_ReplayPos].timeOffset() - m_Recording[m_ReplayPos - 1].timeOffset();
    m_Timer->setInterval(delta > 0 ? (delta * m_ReplaySpeedFactor) : 0);
    m_Timer->start();
}
