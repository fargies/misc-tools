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
** TestRecorder.hpp
**
**        Created on: Apr 06, 2013
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __TEST_RECORDER_HH__
#define __TEST_RECORDER_HH__

#include <QString>
#include <QObject>

class InputEventRecorder;

/**
 * @brief QEvent recorder automated tests
 * @param name     test name
 * @param observee Qt object to observe
 * @param parent   parent QObject
 */
class TestRecorder : public QObject
{
    Q_OBJECT
public:
    TestRecorder(
            const QString &name,
            QObject *observee,
            QObject *parent = nullptr);
    ~TestRecorder();

    void run(const QString &msg, float speed = 0.1);

    /**
     * @brief stop TestRecorder on last window close event
     */
    void stopOnLastWin();

    /**
     * for test-recorder to record a new sequence
     */
    static void setForced(bool value = true);

    QString getDataFileName() const;

public slots:
    void stop();

protected:
    bool askForUpdate(const QString &msg) const;
    bool askToSave() const;

protected:
    InputEventRecorder *m_recorder;
    QString m_dataName;
    bool m_update;
};

#endif
