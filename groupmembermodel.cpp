#include "groupmembermodel.h"
#include "ui_groupmembermodel.h"

GroupMemberModel::GroupMemberModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupMemberModel)
{
    ui->setupUi(this);
}

GroupMemberModel::~GroupMemberModel()
{
    delete ui;
}

void GroupMemberModel::setNickName(QString nickname)
{
    this->ui->nickname->setText(nickname);
}
