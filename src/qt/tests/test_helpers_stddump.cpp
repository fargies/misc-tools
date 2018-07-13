/*
** This document and/or file is SOMFY's property. All information it
** contains is strictly confidential. This document and/or file shall
** not be used, reproduced or passed on in any way, in full or in part
** without SOMFY's prior written approval. All rights reserved.
** Ce document et/ou fichier est la propritye SOMFY. Les informations
** quil contient sont strictement confidentielles. Toute reproduction,
** utilisation, transmission de ce document et/ou fichier, partielle ou
** intégrale, non autorisée préalablement par SOMFY par écrit est
** interdite. Tous droits réservés.
**
** Copyright © (2009-2012), Somfy SAS. All rights reserved.
** All reproduction, use or distribution of this software, in whole or
** in part, by any means, without Somfy SAS prior written approval, is
** strictly forbidden.
**
** test_helpers_stddump.cpp
**
**        Created on: Apr 22, 2013
**   Original Author: Sylvain Fargier <sylvain.fargier@somfy.com>
**
*/

#include <QDebug>
#include "test_helpers_stddump.hpp"

CPPUNIT_NS_BEGIN

template <typename T> std::string dump(const T &t)
{
    QString out;
    QDebug dbg(&out);

    dbg << t;
    return qPrintable(out);
}

CPPUNIT_TRAITS_OVERRIDE(QString)
{ return dump(t); }

CPPUNIT_TRAITS_OVERRIDE(QPoint)
{ return dump(t); }

CPPUNIT_TRAITS_OVERRIDE(QPointF)
{ return dump(t); }

CPPUNIT_TRAITS_OVERRIDE(Qt::KeyboardModifiers)
{ return dump(t); }

template <> bool assertion_traits<QPointF>::equal(const QPointF &a, const QPointF &b)
{
    return (qAbs(a.x() - b.x()) < 0.0001) && (qAbs(a.y() - b.y()) < 0.0001);
}

CPPUNIT_NS_END
