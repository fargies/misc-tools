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
** Created on: 2018-07-13T11:36:05+02:00
**     Author: Sylvain Fargier <fargie_s> <fargier.sylvain@free.fr>
**
*/

#ifndef __TST_TEST_RECORDER_HPP__
#define __TST_TEST_RECORDER_HPP__

#include <QObject>
#include <QWidget>

class AutoRecorder : public QObject
{
    Q_OBJECT
public:
    AutoRecorder(QWidget *subject, bool doSave, QObject *parent = nullptr);

public slots:
    void onAskForUpdate();
    void onRecording();
    void onValidate();

protected:
    QWidget *m_subject;
    bool m_doSave;
};

#endif  // __TST_TEST_RECORDER_HPP__
