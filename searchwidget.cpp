#include "searchwidget.h"
#include "ui_searchwidget.h"

#include "userorgroupitem.h"
#include <string>
#include <QTextCodec>

using std::string;


SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);  //不显示标题栏

    setAttribute(Qt::WA_QuitOnClose, false);

    //默认查找类型为finduser
    m_findType = FindUser;

    hidePagination();

    currentPageNum = 1;  //默认第一页
    totalPageNum = 1;

    connect(this->ui->goSearchButton, SIGNAL(clicked(bool)), this, SLOT(searchUserOrGroup()));

    //关联最小化和关闭按钮
    connect(this->ui->closeLabel, SIGNAL(clicked()), this, SLOT(close()));
    connect(this->ui->miniLabel, SIGNAL(clicked()), this, SLOT(showMinimized()));

    //关联翻页按钮点击
    connect(this->ui->prePageLabel, SIGNAL(clicked()), this, SLOT(prePageClicked()));
    connect(this->ui->nextPageLabel, SIGNAL(clicked()), this, SLOT(nextPageClicked()));

    //关联前进到目标页面按钮
    connect(this->ui->goPageNumButton, SIGNAL(clicked(bool)), this, SLOT(gotoGoalPage()));

    //关联查找用户还是查找群的按钮
    connect(this->ui->findUserLabel, SIGNAL(clicked()), this, SLOT(findUserLabelClicked()));
    connect(this->ui->findGroupLabel, SIGNAL(clicked()), this, SLOT(findGroupLabelClicked()));

}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::searchUserOrGroup()
{
    //先判断现在是搜索用户还是群组
    QString searchContent = this->ui->lineEdit->text();
    if(searchContent.isEmpty())
    {
        //如果搜索框为空的话，直接返回
        return;
    }
    qDebug()<<"查找内容"<<searchContent;

    //判断搜索用户还是搜索群组
    if(m_findType == FindUser)
    {
        searchUser(searchContent);
    }
    else if (m_findType == FindGroup)
    {
        searchGroup(searchContent);
    }

}

void SearchWidget::searchUser(const QString &searchContent)
{
    userList.clear();
    currentPageNum = 1;
    totalPageNum = 1;

    deleteAllSubWidget();
    hidePagination(); //隐藏分页

    WTAPI.SelectUserText(searchContent, &userList);

    totalPageNum = ceil(userList.size() / 6.0);//向上取整

    qDebug()<<"总共页数:"<<totalPageNum;

    if(totalPageNum == 1 || totalPageNum == 0)
    {
        showOnWidget(1); //默认显示1页
    }
    else
    {
        showPagination(); //如果页数大于一页，就显示分页
        showOnWidget(1);
    }
}

void SearchWidget::searchGroup(const QString &searchContent)
{
    groupList.clear();
    currentPageNum = 1;
    totalPageNum = 1;

    deleteAllSubWidget();
    hidePagination(); //隐藏分页

    WTAPI.SelectGroupText(searchContent, &groupList);

    totalPageNum = ceil(groupList.size() / 6.0);//向上取整

    if(groupList.size() == 0)
    {

    }
    else
    {
        if(totalPageNum == 1 || totalPageNum == 0)
        {
            showOnWidget(1); //默认显示1页
        }
        else
        {
            showPagination(); //如果页数大于一页，就显示分页
            showOnWidget(1);
        }
    }

}

void SearchWidget::showOnWidget(int pageNum)
{
    int listStart = (pageNum - 1) * 6;
    int listEnd = pageNum * 6;

//    deleteAllSubWidget();

    if(m_findType == FindUser)
    {
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
    else if (m_findType == FindGroup)
    {
        int i=0;
        for(; listStart<groupList.size() && listStart<listEnd; listStart++)
        {
            int j= i%3;

            UserOrGroupItem * item1 = new UserOrGroupItem;
            item1->changeGroupSomething();

            userListTemp.append(item1);

            item1->setNameLabelText(groupList.at(listStart).acGroupName);
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


}

void SearchWidget::deleteAllSubWidget()
{
    if(userListTemp.size() != 0)
    {
        qDebug()<<"删除控件";
        QWidget * item;

        int i=0;

        foreach (item, userListTemp)
        {
            this->ui->gridLayout->removeWidget(item);
            delete item;
            qDebug()<<i++;
        }

        userListTemp.clear();

        this->ui->widget_3->update();
    }
}

void SearchWidget::showPagination()
{
    this->ui->prePageLabel->show();
    this->ui->nextPageLabel->show();
    this->ui->goalPageLineEdit->show();
    this->ui->goPageNumButton->show();

    //由于QT对中文格式化还是有点问题，故这里不用 当前第X页，共X页
    QString pageText = QString("%1 / %2");
    this->ui->currentPageLabel->setText(pageText.arg(currentPageNum).arg(totalPageNum));
}

void SearchWidget::hidePagination()
{
    this->ui->prePageLabel->hide();
    this->ui->nextPageLabel->hide();
    this->ui->goalPageLineEdit->hide();
    this->ui->goPageNumButton->hide();
}

void SearchWidget::nextPageClicked()
{

    deleteAllSubWidget();

    int nextPageNum = currentPageNum + 1;
    currentPageNum++;

    if(nextPageNum > totalPageNum)
    {
        currentPageNum = totalPageNum;
        showOnWidget(totalPageNum);
    }
    else
    {
        showOnWidget(nextPageNum);
    }

    QString pageText = QString("%1 / %2");
    this->ui->currentPageLabel->setText(pageText.arg(currentPageNum).arg(totalPageNum));
}

void SearchWidget::prePageClicked()
{

    deleteAllSubWidget();

    int prePageNum = currentPageNum - 1;
    currentPageNum--;

    if(prePageNum <= 1)
    {
        currentPageNum = 1;
        showOnWidget(1);
    }
    else
    {
        showOnWidget(prePageNum);
    }

    QString pageText = QString("%1 / %2");
    this->ui->currentPageLabel->setText(pageText.arg(currentPageNum).arg(totalPageNum));
}

void SearchWidget::gotoGoalPage()
{

    deleteAllSubWidget();

    int goalPageNum = this->ui->goalPageLineEdit->text().toInt();
    if(goalPageNum < 1 && goalPageNum > totalPageNum)
    {
        showOnWidget(1);
        currentPageNum = 1;
    }
    else
    {
        showOnWidget(goalPageNum);
        currentPageNum = goalPageNum;
    }

    QString pageText = QString("%1 / %2");
    this->ui->currentPageLabel->setText(pageText.arg(currentPageNum).arg(totalPageNum));
}

void SearchWidget::findUserLabelClicked()
{
    m_findType = FindUser;
    this->ui->findUserLabel->setStyleSheet(QLatin1String("color:white;\n"
                                                         "font-weight:bold;\n"
                                                         "font-size:16px;\n"
                                                         "\n"
                                                         ""));
    this->ui->findGroupLabel->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
    deleteAllSubWidget();
    hidePagination();
    currentPageNum = 1;
    totalPageNum = 1;
    QString pageText = QString("%1 / %2");
    this->ui->currentPageLabel->setText(pageText.arg(currentPageNum).arg(totalPageNum));
}

void SearchWidget::findGroupLabelClicked()
{
    m_findType = FindGroup;

    this->ui->findUserLabel->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
    this->ui->findGroupLabel->setStyleSheet(QLatin1String("color:white;\n"
                                                         "font-weight:bold;\n"
                                                         "font-size:16px;\n"
                                                         "\n"
                                                         ""));
    deleteAllSubWidget();
    hidePagination();
    currentPageNum = 1;
    totalPageNum = 1;
    QString pageText = QString("%1 / %2");
    this->ui->currentPageLabel->setText(pageText.arg(currentPageNum).arg(totalPageNum));
}

