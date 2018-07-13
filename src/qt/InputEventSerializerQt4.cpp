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

#include <QDebug>

#include "InputEventSerializer.hpp"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

template <typename T>
QDataStream &operator<<(QDataStream &ds, const T &t)
{
    switch (sizeof (T))
    {
    case 1:
        return ds << static_cast<quint8>(t);
    case 2:
        return ds << static_cast<quint16>(t);
    case 4:
        return ds << static_cast<quint32>(t);
    case 8:
        return ds << static_cast<quint64>(t);
    default:
        qWarning() << "failed to convert" << t;
        return ds;
        break;
    }
}

template <typename T>
QDataStream &operator>>(QDataStream &ds, T &t)
{
    switch (sizeof (T))
    {
    case 1:
        {
            quint8 val; ds >> val;
            t = static_cast<T>(val);
            return ds;
        }
    case 2:
        {
            quint16 val; ds >> val;
            t = static_cast<T>(val);
            return ds;
        }
    case 4:
        {
            quint32 val; ds >> val;
            t = static_cast<T>(val);
            return ds;
        }
    case 8:
        {
            quint64 val; ds >> val;
            t = static_cast<T>(val);
            return ds;
        }
    default:
        qWarning() << "failed to convert" << t;
        return ds;
        break;
    }
}

QDataStream& operator<<(QDataStream &ds, const QContextMenuEvent &ev)
{
    ds
        << ev.reason() << ev.pos() << ev.globalPos()
        << ev.modifiers();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, QContextMenuEvent* &ev)
{
    QContextMenuEvent::Reason reason;
    QPoint pos, globalPos;
    Qt::KeyboardModifiers modifiers;
    ds >> reason >> pos >> globalPos >> modifiers;
    ev = new QContextMenuEvent(reason, pos, globalPos, modifiers);
    return ds;
}

QDataStream& operator<<(QDataStream &ds, const QKeyEvent &ev)
{
    ds
        << ev.type() << ev.key() << ev.modifiers() << ev.text()
        << ev.isAutoRepeat() << ev.count();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, QKeyEvent* &ev)
{
    QEvent::Type type;
    int key, count;
    Qt::KeyboardModifiers modifiers;
    QString text;
    bool autorep;

    ds >> type >> key >> modifiers >> text >> autorep >> count;
    ev = new QKeyEvent(type, key, modifiers, text, autorep, count);
    return ds;
}

QDataStream& operator<<(QDataStream &ds, const QMouseEvent &ev)
{
    ds
        <<  ev.type() << ev.pos() << ev.globalPos() << ev.button()
        << ev.buttons() << ev.modifiers();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, QMouseEvent* &ev)
{
    QEvent::Type type;
    QPoint pos, globalPos;
    Qt::MouseButton button;
    Qt::MouseButtons buttons;
    Qt::KeyboardModifiers modifiers;

    ds >> type >> pos >> globalPos >> button >> buttons >> modifiers;
    ev = new QMouseEvent(type, pos, globalPos, button, buttons, modifiers);
    return ds;
}

QDataStream& operator<<(QDataStream &ds, const QWheelEvent &ev)
{
    ds
        << ev.type() << ev.pos() << ev.globalPos() << ev.delta()
        << ev.buttons() << ev.modifiers() << ev.orientation();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, QWheelEvent* &ev)
{
    QEvent::Type type;
    QPoint pos, globalPos;
    int delta;
    Qt::MouseButtons buttons;
    Qt::KeyboardModifiers modifiers;
    Qt::Orientation orientation;

    ds >> type >> pos >> globalPos >> delta >> buttons >> modifiers
        >> orientation;
    ev = new QWheelEvent(pos, globalPos, delta, buttons, modifiers,
        orientation);
    return ds;
}

#endif
