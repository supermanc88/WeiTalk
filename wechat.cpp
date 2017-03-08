#include "wechat.h"
#include "ui_wechat.h"

#include <QVBoxLayout>
#include <groupmembermodel.h>

WeChat::WeChat(int groupId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeChat)
{
    ui->setupUi(this);
    this->groupId = groupId;

    memberCount = 0;
    g_layout = new QVBoxLayout;
    g_layout->setContentsMargins(1,0,7,0);
    g_layout->setSpacing(2);

    groupMemberList = new QList<group_member_t>;

    API.SelectGroupUserList(this->groupId, groupMemberList, this->memberCount);

}

WeChat::~WeChat()
{
    delete ui;
}

void WeChat::initGroupMemberList()
{
    QList<group_member_t>::iterator iter;

    for(iter = groupMemberList->begin(); iter != groupMemberList->end(); iter++)
    {
        GroupMemberModel * model = new GroupMemberModel;
        QString c_nickName = (*iter).nickname;
        QString c_userName = (*iter).username;

        //如果有昵称的话设置的是昵称，没有就设置用户名
        if(!c_nickName.isEmpty())
        {
            model->setNickName(c_nickName);
        }
        else
        {
            model->setNickName(c_userName);
        }

        int count = g_layout->count();
        if(count > 0)
        {
            g_layout->removeItem(g_layout->itemAt(count - 1));
        }
        g_layout->addWidget(model);
        g_layout->addStretch();
    }

    this->ui->scrollAreaWidgetContents->setLayout(g_layout);
}

QString WeChat::getGroupName() const
{
    return groupName;
}

void WeChat::setGroupName(const QString &value)
{
    groupName = value;

    this->ui->groupNameLabel->setText(groupName);
}
