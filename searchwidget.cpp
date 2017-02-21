#include "searchwidget.h"
#include "ui_searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);  //不显示标题栏
}

SearchWidget::~SearchWidget()
{
    delete ui;
}
