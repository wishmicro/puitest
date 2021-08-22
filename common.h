#ifndef COMMON_H
#define COMMON_H
#include<QString>
#include<QWidget>
#include<vector>

using namespace std;
//控件类型
//enum Widgettype{
//    TYPE_PUSHBOTTON,
//    TYPE_RADIOBUTTON,
//    TYPE_QLABEL,
//    TYPE_CHECKBOX,
//    TYPE_QCOMBOBOX,
//    TYPE_QLINEEDIT,
//    TYPE_SPQINBOX,
//    TYPE_QTIMEEDIT,
//    TYPE_QDATEEDIT,

//};
//存储控件结构
struct WidgetInfo{
    WidgetInfo(){
        posx=0;
        posy=0;
        width=0;
        heigth=0;
        name="";
        objpointer=NULL;
        type="";

    };
    ~WidgetInfo(){

//        if(objpointer){
//            delete objpointer;
//            objpointer=NULL;
//        }
    };
//控件信息
    int posx;
    int posy;
    int width;
    int heigth;
    QString name;

    //控件对象
    QWidget *objpointer;

    QString type;








};

//存储界面数组
typedef vector<WidgetInfo*>vecWidgetArray;




#endif // COMMON_H
