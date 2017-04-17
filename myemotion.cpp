#include "myemotion.h"
#include "ui_myemotion.h"

#include <QTableWidgetItem>
#include <QLabel>
#include <QMovie>
#include <QSize>

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
    int row = m_emotionList.size() / this->ui->tableWidget->rowCount();
    int column = m_emotionList.size() % this->ui->tableWidget->rowCount();

    QTableWidgetItem * tableWidgetItem = new QTableWidgetItem;
    ui->tableWidget->setItem(row, column, tableWidgetItem);

    //添加gif表情
    QLabel * emotionIcon = new QLabel;
    emotionIcon->setMargin(4);

    QMovie * movie = new QMovie;
    movie->setScaledSize(QSize(24, 24));
    movie->setFileName(fileName);
    movie->start();
    emotionIcon->setMovie(movie);
    this->ui->tableWidget->setCellWidget(row, column, emotionIcon);

    m_emotionList.push_back(fileName);

}

void MyEmotion::initEmotion()
{
    QString path = ":/faces/classic/%1.gif";
    for(int i=0; i<16; i++)
    {
        addEmotionItem(path.arg(i));
    }


}
