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
** test_helpers_stddump.hpp
**
**        Created on: Apr 22, 2013
**   Original Author: Sylvain Fargier <sylvain.fargier@somfy.com>
**
*/

#ifndef __TEST_HELPERS_STDDUMP_HPP__
#define __TEST_HELPERS_STDDUMP_HPP__

#include <cppunit/TestAssert.h>
#include <string>

#include <QString>
#include <QPoint>
#include <QPointF>
#include <QEvent>

CPPUNIT_NS_BEGIN

#define CPPUNIT_TRAITS_OVERRIDE(TYPE) \
    template <> std::string assertion_traits<TYPE>::toString(const TYPE &t)

CPPUNIT_TRAITS_OVERRIDE(QString);
CPPUNIT_TRAITS_OVERRIDE(Qt::KeyboardModifiers);
CPPUNIT_TRAITS_OVERRIDE(QPoint);
CPPUNIT_TRAITS_OVERRIDE(QPointF);

template <> bool assertion_traits<QPointF>::equal(const QPointF &a, const QPointF &b);

CPPUNIT_NS_END

#endif // TEST_HELPERS_STDDUMP_HPP
