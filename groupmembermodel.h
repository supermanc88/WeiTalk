#ifndef GROUPMEMBERMODEL_H
#define GROUPMEMBERMODEL_H

#include <QWidget>

namespace Ui {
class GroupMemberModel;
}

class GroupMemberModel : public QWidget
{
    Q_OBJECT

public:
    explicit GroupMemberModel(QWidget *parent = 0);
    ~GroupMemberModel();

    //设置在群组中用的昵称
    void setNickName(QString nickname);

private:
    Ui::GroupMemberModel *ui;
};

#endif // GROUPMEMBERMODEL_H
