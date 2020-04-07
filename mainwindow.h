#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "gameCore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    gameCore *game;
    QTimer *timer;
    QLabel *timeLabel;
    void handleGameState(gameCore *game);

private slots:
    void onStartGameClick();
    void onLevelChooseClick();
    void onQuitClick();
    void updateTimer();

};
#endif // MAINWINDOW_H
