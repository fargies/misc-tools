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

#include "InputEventSerializer.hpp"

QDataStream& operator<<(QDataStream &ds, const QInputEvent *ev)
{
    if (dynamic_cast<const QContextMenuEvent*>(ev))
        ds << (qint32) EventClass::ContextMenuEvent
            << *static_cast<const QContextMenuEvent*>(ev);
    else if (dynamic_cast<const QKeyEvent*>(ev))
        ds << (qint32) EventClass::KeyEvent
            << *static_cast<const QKeyEvent*>(ev);
    else if (dynamic_cast<const QMouseEvent*>(ev))
        ds << (qint32) EventClass::MouseEvent
            << *static_cast<const QMouseEvent*>(ev);
    else if (dynamic_cast<const QWheelEvent*>(ev))
        ds << (qint32) EventClass::WheelEvent
            <<*static_cast<const QWheelEvent*>(ev);

    return ds;
}

QDataStream& operator>>(QDataStream &ds, QInputEvent*& ev)
{
    qint32 cls;
    ds >> cls;

    switch(cls)
    {
    case EventClass::ContextMenuEvent:
        {
            QContextMenuEvent *tmp;
            ds >> tmp;
            ev = tmp;
        }
        break;
    case EventClass::KeyEvent:
        {
            QKeyEvent *tmp;
            ds >> tmp;
            ev = tmp;
        }
        break;
    case EventClass::MouseEvent:
        {
            QMouseEvent *tmp;
            ds >> tmp;
            ev = tmp;
        }
        break;
    case EventClass::WheelEvent:
        {
            QWheelEvent *tmp;
            ds >> tmp;
            ev = tmp;
        }
        break;
    }
    return ds;
}
