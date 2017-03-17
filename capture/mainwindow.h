#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MD5.h"
#include <string>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void captureFinished(int type);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    string GenerateAuthCode(string strAuthCode);

    friend void CaptureNotice(int nType, int x, int y, int width, int height, const char* szInfo);

public slots:
    void btnClicked();
    void OnCaptureFinish(int type);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
