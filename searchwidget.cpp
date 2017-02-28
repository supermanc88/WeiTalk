#include "searchwidget.h"
#include "ui_searchwidget.h"

#include "userorgroupitem.h"


SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);  //不显示标题栏

//    UserOrGroupItem * item1 = new UserOrGroupItem;
//    UserOrGroupItem * item2 = new UserOrGroupItem;
//    UserOrGroupItem * item3 = new UserOrGroupItem;
//    UserOrGroupItem * item4 = new UserOrGroupItem;
//    UserOrGroupItem * item5 = new UserOrGroupItem;
//    UserOrGroupItem * item6 = new UserOrGroupItem;

//    this->ui->gridLayout->addWidget(item1, 0, 0, 1, 1);
//    this->ui->gridLayout->addWidget(item2, 0, 1, 1, 1);
//    this->ui->gridLayout->addWidget(item3, 0, 2, 1, 1);
//    this->ui->gridLayout->addWidget(item4, 1, 0, 1, 1);
//    this->ui->gridLayout->addWidget(item5, 1, 1, 1, 1);
//    this->ui->gridLayout->addWidget(item6, 1, 2, 1, 1);

    connect(this->ui->goSearchButton, SIGNAL(clicked(bool)), this, SLOT(searchUserOrGroup()));

}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::searchUserOrGroup()
{
    //先判断现在是搜索用户还是群组
    //目前默认搜索群组

    QString searchContent = this->ui->lineEdit->text();
    if(searchContent.isEmpty())
    {
        //如果搜索框为空的话，直接返回
        return;
    }
    qDebug()<<"查找内容"<<searchContent;
    searchUser(searchContent);
}

void SearchWidget::searchUser(const QString &searchContent)
{
    userList.clear();

    deleteAllSubWidget();

    WTAPI.SelectUserText(searchContent, &userList);
    showOnWidget(1); //默认显示1页

    this->ui->widget_3->update();
}

void SearchWidget::searchGroup(const QString &searchContent)
{

}

void SearchWidget::showOnWidget(int pageNum)
{
    int listStart = (pageNum - 1) * 6;
    int listEnd = pageNum * 6;

    int i=0;
    for(; listStart<userList.size() && listStart<listEnd; listStart++)
    {
        int j= i%3;

        UserOrGroupItem * item1 = new UserOrGroupItem;

        userListTemp.append(item1);

        item1->setNameLabelText(userList.at(listStart).acUserName);
        if(i >= 3)
        {
            this->ui->gridLayout->addWidget(item1, 1, j, 1, 1);
        }
        else
        {
            this->ui->gridLayout->addWidget(item1, 0, j, 1, 1);
        }

        i++; //控制行
    }

}

void SearchWidget::deleteAllSubWidget()
{
    if(userListTemp.size() != 0)
    {
        QWidget * item;
        foreach (item, userListTemp)
        {
            qDebug()<<"删除控件";
            this->ui->gridLayout->removeWidget(item);
            delete item;
        }
    }
}

