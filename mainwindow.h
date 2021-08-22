#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"common.h"
#include <QMainWindow>
#include<QString>
//#include"loadWidget/loadconfig.h"
#include <QComboBox>
#include <QLabel>
#include <QDateTimeEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include<QSpinBox>
#include<QLineEdit>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_actionopenxml_triggered();

    void on_actionopenjson_triggered();

    void on_actionopensqlite_triggered();


    void on_actionclearAll_triggered();

private:

    Ui::MainWindow *ui;




    //loadconfig *loadConfig=new loadconfig;
    //通过解析app-config.ini获取界面文件的路径和类型
    void readIni();

    //--------loadconfig
    void loadfile(const QString &filepath );
    vecWidgetArray* loadxmlconfig(const QString &filepath);
    vecWidgetArray* loadjsonconfig(const QString &filepath);
    vecWidgetArray* loadjSqliteconfig(const QString &filepath);
    //vecWidgetArray *m_xmlwidgetArray;
   // vecWidgetArray m_widgetArray;
     static vecWidgetArray m_widgetArray;


    //---------layout---------
    void addwidgets(vecWidgetArray *vecWidgetArray);
    void addsignalWidget(WidgetInfo *info);
    void DeleteWidget();

    //用于清理布局
    vecWidgetArray *m_deleteWidgetArray=NULL;
    QWidget *m_parent=NULL;


    //------路径---------
    QString m_xmlPath;
    QString m_jsonPath;
    QString m_sqlitepath;
};
#endif // MAINWINDOW_H
