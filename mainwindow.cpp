#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

const int blockSize = 20;
const int offsetX = 5;
const int offsetY = 5;
const int spaceY = 70;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/icon.jpg"));
    setWindowTitle("MineSweeper");
    timeLabel = new QLabel(this);
    connect(ui->actionStart,SIGNAL(triggered(bool)),this,SLOT(onStartGameClick()));
    connect(ui->actionBasic,SIGNAL(triggered(bool)),this,SLOT(onLevelChooseClick()));
    connect(ui->actionMedium,SIGNAL(triggered(bool)),this,SLOT(onLevelChooseClick()));
    connect(ui->actionHard,SIGNAL(triggered(bool)),this,SLOT(onLevelChooseClick()));
    connect(ui->actionQuit,SIGNAL(triggered(bool)),this,SLOT(onQuitClick()));

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTimer()));

    game = new gameCore();
    game->createGame();
    setFixedSize(game->Column*blockSize+offsetX*2, game->Row*blockSize+offsetY*2+spaceY);
    timeLabel->setGeometry(game->Column * blockSize + offsetX * 2 - 120, spaceY / 2, 80, 20);
    timeLabel->setText("Time: " + QString::number(game->Time) + "s");
    timer->start(1000);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap bmpCell(":/res/blocks.bmp");
    QPixmap bmpFace(":/res/faces.bmp");
    QPixmap bmpFrame(":/res/frame.bmp");
    QPixmap bmpNumber(":/res/timenumber.bmp");

    switch(game->gamestate) {
        case OVER:
            painter.drawPixmap((game->Column*blockSize + offsetX*2)/2-12,spaceY/2,bmpFace,0*24,0,24,24);
            break;
        case PLAYING:
            painter.drawPixmap((game->Column*blockSize + offsetX*2)/2-12,spaceY/2,bmpFace,1*24,0,24,24);
            break;
        case WIN:
            painter.drawPixmap((game->Column*blockSize + offsetX*2)/2-12,spaceY/2,bmpFace,2*24,0,24,24);
            break;
        default:
            painter.drawPixmap((game->Column*blockSize + offsetX*2)/2-12,spaceY/2,bmpFace,1*24,0,24,24);
            break;
    }

     int n = game->curMineNum;
     int posX = (game->Column * blockSize + offsetX * 2) / 2 - 50; // ���һλ���ֵĺ�����
     if(n <= 0) // �������Ϊ0���߼���0���£���������
     {
           painter.drawPixmap(posX, spaceY / 2, bmpNumber, n * 20, 0, 20, 28); // 20�����ֵĿ�28�Ǹ�
     }
     while(n > 0) // ����Ƕ�λ��
     {
         painter.drawPixmap(posX - 20, spaceY / 2, bmpNumber, n % 10 * 20, 0, 20, 28); // ÿ�δӺ������һλ
         n /= 10;
         posX -= 20;
     }

    for (int i = 0; i < game->Row; i++) {
        for (int j = 0; j < game -> Column; j++) {
            switch(game->Map[i][j].curState) {
                case UNDIG:
                    painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY , bmpCell, blockSize * 10, 0, blockSize, blockSize);
                    break;
                case DIGGED:
                    painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpCell, blockSize * game->Map[i][j].value, 0, blockSize, blockSize);
                    break;
                case MARKED:
                    painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpCell, blockSize * 11, 0, blockSize, blockSize);
                    break;
                case BOMB:
                    painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpCell, blockSize * 9, 0, blockSize, blockSize);
                    break;
                case WRONGMARKED:
                    if(game->gamestate == PLAYING || game->gamestate == FAUSE) {
                        painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpCell, blockSize * 11, 0, blockSize, blockSize);
                    }
                    else if(game->gamestate == OVER) {
                        painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpCell, blockSize * 12, 0, blockSize, blockSize);
                    }
                    break;
            }
        }
    }

    handleGameState(game);
}

void MainWindow::handleGameState(gameCore *game)
{
    if(game->gamestate == OVER)
    {
        timer->stop();
        qDebug() << "you lose!";
    }
    else if(game->gamestate == WIN)
    {
        timer->stop();
        qDebug() << "you win!";
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->y() < spaceY + offsetY)
    {
        int x = event->x();
        int y = event->y();
        // ��ʱ�ж��Ƿ���Ц��
        if(x >= (game->Column * blockSize + offsetX * 2) / 2 - 12
        && x <= (game->Column * blockSize + offsetX * 2) / 2 + 12
        && y >= spaceY / 2
        && y <= spaceY / 2 + 24)
        {
            game->restartGame(); // ����
            timer->start(1000);
            timeLabel->setText("Time: " + QString::number(game->Time) + " s");
            update();
        }
    }
    else if(game->gamestate != OVER && game->gamestate != WIN)
    {
        // ��Ϸû���ûӮ�Ž��ܵ��
        // ��ʱ�жϵ�������ĸ�����
        // ��õ������
        int px = event->x() - offsetX;
        int py = event->y() - offsetY - spaceY;
        // ����ɷ�������
        int row = py / blockSize;
        int col = px / blockSize;
        // ���ݲ�ͬ�����Ӧ
        switch(event->button())
        {
            case Qt::LeftButton:
                game->digMine(row, col);
                update(); // ÿ�ε����Ҫ�ػ�
                break;
            case Qt::RightButton:
                game->markMine(row, col);
                update();
                break;
            default:
                break;
        }
    }

}

void MainWindow::onStartGameClick()
{
    qDebug()<<"game started";
    game->restartGame(); // ���¿�ʼ
    timeLabel->setText("Time: " + QString::number(game->Time) + " s");
    timer->start(1000);
    update();
}

void MainWindow::onLevelChooseClick()
{
    QAction *actionSender = (QAction *)dynamic_cast<QAction *>(sender());
    if(actionSender == ui->actionBasic)
    {
        qDebug() << "basic";
        // ��������Ϸģ��
        game->createGame(8, 10, 15, BASIC);
    }
    else if(actionSender == ui->actionMedium)
    {
        qDebug() << "medium";
        game->createGame(15, 20, 50, MEDIUM);
    }
    else if(actionSender == ui->actionHard)
    {
        qDebug() << "hard";
        game->createGame(20, 30, 100, HARD);
    }
    // ���¼�ʱ
    timer->start(1000);
    // ��ˢ��UI�����ڴ�С�ı��ǿ���ػ�
    timeLabel->setText("Time: " + QString::number(game->Time) + " s");
    timeLabel->setGeometry(game->Column * blockSize + offsetX * 2 - 80, spaceY / 2, 80, 20);
    setFixedSize(game->Column * blockSize + offsetX * 2, game->Row * blockSize + offsetY * 2 + spaceY);
}

void MainWindow::updateTimer()
{
    // ��ʱ����ʱ
    game->Time++;
    timeLabel->setText("Time: " + QString::number(game->Time) + " s");
    qDebug() << game->Time;
}

void MainWindow::onQuitClick()
{
    QCoreApplication::quit();
}

MainWindow::~MainWindow()
{
    delete game;
    game = nullptr;
    delete ui;
}

