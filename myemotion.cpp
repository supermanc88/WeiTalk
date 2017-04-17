#include "myemotion.h"
#include "ui_myemotion.h"

#include <QTableWidgetItem>
#include <QLabel>
#include <QMovie>
#include <QSize>

#include <QDebug>

MyEmotion::MyEmotion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyEmotion)
{
    ui->setupUi(this);
}

MyEmotion::~MyEmotion()
{
    delete ui;
}

//添加表情
void MyEmotion::addEmotionItem(QString fileName)
{
    int row = m_emotionList.size() / this->ui->tableWidget->columnCount();
    qDebug()<<"columnCount: "<<this->ui->tableWidget->columnCount();
    int column = m_emotionList.size() % this->ui->tableWidget->columnCount();

    QTableWidgetItem * tableWidgetItem = new QTableWidgetItem;
    ui->tableWidget->setItem(row, column, tableWidgetItem);

    //添加gif表情
    QLabel * emotionIcon = new QLabel;
    emotionIcon->setMargin(4);

    QMovie * movie = new QMovie(fileName);
    movie->setScaledSize(QSize(24, 24));
    movie->start();
    emotionIcon->setMovie(movie);
    this->ui->tableWidget->setCellWidget(row, column, emotionIcon);

    m_emotionList.push_back(fileName);

    qDebug()<<"movie->loopCount: "<<movie->loopCount();

}

void MyEmotion::initEmotion()
{
    QString path = ":/faces/emotion/%1.gif";
    for(int i=0; i<24; i++)
    {
        qDebug()<<i;
        addEmotionItem(path.arg(i+1));
    }
}
