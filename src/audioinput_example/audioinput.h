/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QPixmap>
#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QFile>

#include "qtmultimedia/audio/qaudioinput.h"
#include "qtmultimedia/audio/qaudiooutput.h"
class BlockAnalyzer;
namespace Analyzer {
	class Base;
};

#include "3rdparty/analyzers/blockanalyzer.h"
#include "3rdparty/analyzers/baranalyzer.h"

class AudioInfo : public QIODevice
{
    Q_OBJECT
public:
    AudioInfo(QObject* parent, QAudioInput* device, Analyzer::Base *analyzer);
    ~AudioInfo();

    void start(QIODevice *output);
    void stop();

    int LinearMax();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

    QAudioInput*   input;

public slots:
	void setBufferMs(int);

private:
    int m_maxValue;
    QFile outputFile;
	Analyzer::Base *analyzer;
	Analyzer::Scope scope;
	QIODevice *outputDev;
	char *buffer;
	int bufferWritePos;
	int bufferLengthMs;

signals:
    void update();
};


class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);

    void setLevel(int value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int level;
    QPixmap pixmap;
};

class InputTest : public QMainWindow
{
    Q_OBJECT
public:
    InputTest();
    ~InputTest();

    QAudioDeviceInfo device;
    QAudioFormat   format;

    QAudioInput*   audioInput;
    AudioInfo*     audioinfo;
    QIODevice*     input;

    QAudioOutput*  audioOutput;
    QIODevice*     output;

    RenderArea*    canvas;

    bool           pullMode;

    QPushButton*   button;
    QPushButton*   button2;
    QComboBox*     deviceBox;

    char*          buffer;

private slots:
    void refreshDisplay();
    void status();
    void readMore();
    void toggleMode();
    void toggleSuspend();
    void state(QAudio::State s);
    void deviceChanged(int idx);
};

