#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include "weitalkserverapi.h"
#include <QGridLayout>

namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    enum FindType
    {
        FindUser,       //找人
        FindGroup,      //找群
    };

public:
    explicit SearchWidget(QWidget *parent = 0);
    ~SearchWidget();

public slots:
    void searchUserOrGroup();
    void searchUser(const QString & searchContent);
    void searchGroup(const QString & searchContent);

    //将数据显示到widget上，并且做出分页效果，每页最多显示六个
    void showOnWidget(int pageNum);

    void deleteAllSubWidget(); //清空gridlayout中所有的控件

    void showPagination();//显示分页功能

    void hidePagination();//隐藏分页功能按钮

    void nextPageClicked();//下一页点击相应

    void prePageClicked();//上一页点击相应

    void gotoGoalPage();//跳转到目标页面相应

    void findUserLabelClicked();//查找好友Label被点击

    void findGroupLabelClicked();//查找群组Label被点击

private:
    Ui::SearchWidget *ui;

    QList<user_info_t> userList;
    QList<group_info_t> groupList;

    WeiTalkServerAPI WTAPI;
//    QGridLayout * gridLayout;
    QList<QWidget *> userListTemp;

    int currentPageNum;  //当前页数
    int totalPageNum; //总页数


    FindType m_findType; //查找类型
};

#endif // SEARCHWIDGET_H
