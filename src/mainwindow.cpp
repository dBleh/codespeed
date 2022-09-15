#include "mainwindow.h"
#include "qplaintextedit.h"
#include "QToolBar.h"
#include <QVBoxLayout>
#include "qlabel.h"
#include "typingzone.h"
#include "styledtextzone.h"
#include <QElapsedTimer>
#include "timer.h"

MainWindow::MainWindow()
{
    auto *ToolBar = new QToolBar;
    addToolBar(ToolBar);

    auto layout = new QVBoxLayout();

    QWidget* wid = new QWidget();
    
    auto typingZone = new TypingZone(wid);
    auto styledTextZone = new StyledTextZone(wid, *typingZone);
    Timer* timer = new Timer(this);
  
    QString text("ABCDEFGH");
    QObject::connect(typingZone, &TypingZone::textChanged, styledTextZone, &StyledTextZone::onUserTyped);
    QObject::connect(styledTextZone, &StyledTextZone::onNewText, typingZone, &TypingZone::onNewText);
    
    layout->addWidget(timer);
    layout->addWidget(styledTextZone);
    layout->addWidget(typingZone);
    wid->setLayout(layout);
    styledTextZone->setText(text);
    QObject::connect(typingZone, &TypingZone::textChanged, timer, &Timer::startTimer);
    setCentralWidget(wid);
}
 