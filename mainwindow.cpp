#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSettings>
#include<QTextCodec>
#include<QApplication>
#include<QDebug>
#include<QMessageBox>
#include<QtXml/QDomDocument>
#include<QtXml/QDomElement>
#include<QSettings>
#include<QMessageBox>
#include<QFile>
#include<QIODevice>
#include<QJsonDocument>
#include<QJsonArray>
#include<QDebug>
#include<QJsonParseError>
#include<QJsonObject>
#include<QSqlDatabase>
#include<QSqlQuery>

vecWidgetArray MainWindow::m_widgetArray;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);




    readIni();



}

MainWindow::~MainWindow()
{



    delete ui;
}


//---------------------------读取ini文件------------------------------------
void MainWindow::readIni()
{
    //获取当前路径
   // QString appPath = qApp->applicationDirPath();
   // appPath += "\\app-config.ini";


   QSettings *configRead=new QSettings("D:/src/test/simulatepui/app-config.ini",QSettings::IniFormat);
   configRead->setIniCodec(QTextCodec::codecForName("utf-8"));


    m_xmlPath=configRead->value("filepath/xmlpath").toString();
    m_jsonPath=configRead->value("filepath/jsonpath").toString();
    m_sqlitepath=configRead->value("filepath/sqlitepath").toString();
    qDebug()<<"读取xml文件的路径"+m_xmlPath;
    qDebug()<<"读取json文件的路径"+m_jsonPath;
    qDebug()<<"读取sqlite文件的路径"+m_sqlitepath;
}


//---------------------------加载 ini文件路径并开始解析------------------------------------
void MainWindow::loadfile(const QString &filepath)
{


    qDebug()<<"开始加载文件"+filepath;
    if(filepath.isEmpty())
    {return;}

    if(filepath.compare("D:/src/test/simulatepui/puixml02.xml")==0)
    {
        //返回的是一个控件界面数组 需要我们进行创建
        //把控件数组给到专门做布局的类 然后通过那个类实现

        qDebug()<<"loadxmlconfig"+filepath;

        addwidgets(loadxmlconfig(filepath));


    }
    else if(filepath.compare("D:/src/test/simulatepui/puijson.json")==0)
    {
        qDebug()<<"loadjsonconfig"+filepath;
        addwidgets(loadjsonconfig(filepath));
    }
    else if(filepath.compare("D:/src/test/simulatepui/data.db")==0){
        qDebug()<<"loadsqliteconfig"+filepath;
        addwidgets(loadjSqliteconfig(filepath));

    }

    qDebug()<<"loadfile 加载完成";
}


//---------------------------xml解析------------------------------------
vecWidgetArray *MainWindow::loadxmlconfig(const QString &filepath)
{
    qDebug()<<"函数返回数组中 loadxmlconfig"+filepath;
   //判断传入路径是否出错
   if(filepath.isEmpty())
   {
       qDebug()<<"文件未传入";
       QMessageBox::warning(NULL,"警告","文件未传入");
       return NULL;

   }
   //文件传入
   QFile file(filepath);
   if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
   {
       qDebug()<<"文件不能打开";
       QMessageBox::warning(NULL,"警告","文件打开错误");
       file.close();
       return NULL;

   }
   //将文件传华为DOM格式
   QDomDocument document;

   //判断是否转换
   if(!document.setContent(&file)){
       qDebug()<<document.toString();
       QMessageBox::warning(NULL,"警告","未发现QDomcument文件");
       file.close();
       return NULL;

   }
   if(document.isNull())
   {
       qDebug()<<"DOM文件未接受";
       return NULL;
       file.close();
   }
   file.close();

   //获取根节点
   QDomElement root=document.documentElement();
   //根节点的信息  本例中有
   //class="MainWindow"> class="QLabel"> class="QPushButton">  class="QRadioButton"> class="QCheckBox">
   //class="QComboBox"  class="QLineEdit"  class="QSpinBox"  class="QTimeEdit"  class="QDateEdit"

   //在这些节点中 主要有以下属性 name  rect
   //其中rect还包括 x y heigth width
   QDomNode node=root.firstChild();
   qDebug()<<node.nodeName();
   while(!node.isNull()) {
       WidgetInfo *info=new WidgetInfo;
       if(info==NULL){
           return NULL;
       }
         QDomElement e = node.toElement();
         if(!e.isNull()) {
            info->type=e.attributeNode("class").value();
            QDomNode childnode=e.firstChild();
            qDebug()<<childnode.nodeName();

                QDomElement childele=childnode.toElement();
                //通过属性取出大小 位置 名称
                info->posx=childele.attributeNode("x").value().toInt();
                qDebug()<<info->posx;
                info->posy=childele.attributeNode("y").value().toInt();
                qDebug()<<info->posy;
                info->width=childele.attributeNode("width").value().toInt();
                qDebug()<<info->width;
                info->heigth=childele.attributeNode("heigth").value().toInt();
                qDebug()<<info->heigth;
                info->name=childele.attributeNode("name").value();
         }
         MainWindow::m_widgetArray.push_back(info);
         node = node.nextSibling();
     }

   //将获取的xml文件中所有的布局和特征 返回m_widgetArray中
   qDebug()<<"xmlconfig已经结束开始返回";
   return &MainWindow::m_widgetArray;

}
//---------------------------json解析------------------------------------
vecWidgetArray *MainWindow::loadjsonconfig(const QString &filepath)
{
    qDebug()<<"解析文件： loadjson"+filepath;
   //判断传入路径是否出错
   if(filepath.isEmpty())
   {
       qDebug()<<"文件未传入";
       QMessageBox::warning(NULL,"警告","文件未传入");
       return NULL;

   }


    //读取文件
    QFile file;
    file.setFileName(filepath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox::warning(NULL,"warning","打开失败");
        file.close();
        return NULL;
    }


    qDebug()<<"开始进行QJsonDocument读取";
    //开始利用json数据类型读取
    // QJsonArray 是[{}] 而object是{} 所以数组包含对象
    QJsonParseError error;
    QJsonDocument p_document=QJsonDocument::fromJson(file.readAll(),&error);
    if(error.error!=QJsonParseError::NoError){qDebug()<<"解析错误！";return NULL;}

    if(error.error==QJsonParseError::NoError&&!(p_document.isNull())){
        if(p_document.isObject()){

            QJsonObject jsonObject=p_document.object();
            if(jsonObject.contains("widget")&&jsonObject.value("widget").isArray()){


                QJsonArray jsonarray=jsonObject.value("widget").toArray();
                for(int j=0;j<jsonarray.size();j++){
                    WidgetInfo *info=new WidgetInfo;
                    if(info==NULL){
                        return NULL;
                    }

                    if(jsonarray[j].isObject()){

                        QJsonObject jsonObjectwidget=jsonarray[j].toObject();

                        info->type=jsonObjectwidget.value("class").toString();
                        qDebug()<<info->type;

                        info->posx=jsonObjectwidget.value("x").toInt();
                        qDebug()<<info->posx;

                        info->posy=jsonObjectwidget.value("y").toInt();
                        qDebug()<<info->posy;

                        info->width=jsonObjectwidget.value("width").toInt();
                        qDebug()<<info->width;

                        info->heigth=jsonObjectwidget.value("heigth").toInt();
                        qDebug()<<info->heigth;

                        info->name=jsonObjectwidget.value("name").toString();
                        qDebug()<<info->name;
                    }

                    MainWindow::m_widgetArray.push_back(info);
                }//遍历数组并载入

            }//判断数组是否是WIdget 并且它是数组

        }//判断是否是对象

        }//判断是否打开
    qDebug()<<"json文件解析完毕";
    file.close();
    return &MainWindow::m_widgetArray;
}

//---------------------------sqlite解析------------------------------------
vecWidgetArray *MainWindow::loadjSqliteconfig(const QString &filepath)
{
    qDebug()<<"解析文件： loadjson"+filepath;
   //判断传入路径是否出错
   if(filepath.isEmpty())
   {
       qDebug()<<"文件未传入";
       QMessageBox::warning(NULL,"警告","文件未传入");
       return NULL;

   }
   QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
   database.setDatabaseName(filepath);

   if(database.open()){

       QSqlQuery sql_query;
       QString strselect="select *from puisqlite";
       QString strselectparm="select *from paraminfo";

       //开始查询
       sql_query.prepare(strselect);
       if(!sql_query.exec()){
           QMessageBox::warning(NULL,"waring","查询出错");
           database.close();
           return NULL;
           qDebug()<<sql_query.lastQuery();

       }
       else{

           int count;
           while (sql_query.next()) {

               WidgetInfo *info=new WidgetInfo;
               if(info==NULL){
                  qDebug()<<"info错误"; return NULL;}
               count=0;
               info->type=sql_query.value("class").toString();
               qDebug()<<info->type;

                info->posx=sql_query.value("x").toInt();
                 qDebug()<<info->posx;

                 info->posy=sql_query.value("y").toInt();
                 qDebug()<<info->posy;

                 info->heigth=sql_query.value("heigth").toInt();
                 qDebug()<<info->heigth;

                 info->width=sql_query.value("width").toInt();
                 qDebug()<<info->width;

                 info->name=sql_query.value("name").toString();
                 qDebug()<<info->name;

                 MainWindow::m_widgetArray.push_back(info);
           }//查询数据库

       }//else
   }//数据库打开


    qDebug()<<"sqlite解析完成";
   database.close();
    return  &MainWindow::m_widgetArray;

}

//---------------------------通用 添加布局------------------------------------
void MainWindow::addwidgets(vecWidgetArray *vecWidgetArray)
{
    qDebug()<<"addWidgets";
    if(vecWidgetArray==NULL)
    {
        qDebug()<<"界面数组是空的";
        return;
    }
    qDebug()<<"赋值m_WidgetArray";



    //此时控件是有信息的
    m_deleteWidgetArray=vecWidgetArray;
    //清除之前的控件
    //如果直接加载另外一个文件，会覆盖之前的布局，因此需要进行删除
   // DeleteWidget();

    qDebug()<<"开始创建迭代器";

    for(unsigned int i=0;i<vecWidgetArray->size();i++)
    {
        qDebug()<<"开始创建Widget";
        WidgetInfo *sInfo=vecWidgetArray->at(i);
        addsignalWidget(sInfo);
    }


}
//---------------------------通用 添加单控件布局------------------------------------
void MainWindow::addsignalWidget(WidgetInfo *info)
{

    qDebug()<<"进入到创建单个对象";
    qDebug()<<info->type;

    if(info==NULL){
        return ;
    }
    //控件布局指针 用于创建对象
    QWidget *pointerWidget=NULL;
    //开始实例化对象
    if(info->type.compare("TYPE_QLABEL")==0){
        qDebug()<<"创建label";
         pointerWidget=new QLabel(info->name,this);
         pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
         info->objpointer=pointerWidget;
         pointerWidget->show();
         return;
    }
    else if(info->type.compare("TYPE_CHECKBOX")==0){
         qDebug()<<"创建checkbox";
         pointerWidget=new QCheckBox(info->name,this);
         pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
         info->objpointer=pointerWidget;
         pointerWidget->show();
         return;
    }
    else if(info->type.compare("TYPE_SPQINBOX")==0){
         qDebug()<<"创建spinbox";
        pointerWidget=new QSpinBox(this);
        pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
        info->objpointer=pointerWidget;
        pointerWidget->show();
        return;
   }
    else if(info->type.compare("TYPE_QCOMBOBOX")==0){
         qDebug()<<"创建combobox";
         pointerWidget=new QComboBox(this);
         pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
         info->objpointer=pointerWidget;
         pointerWidget->show();
         return;
    }
    else if(info->type.compare("TYPE_PUSHBOTTON")==0){
         qDebug()<<"创建pushbutton";
         pointerWidget=new QPushButton(info->name,this);
         pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
         info->objpointer=pointerWidget;
         pointerWidget->show();
         return;
    }
    else if(info->type.compare("TYPE_RADIOBUTTON")==0){
         qDebug()<<"创建radiobutton";
        pointerWidget=new QRadioButton(info->name,this);
        pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
         info->objpointer=pointerWidget;
        pointerWidget->show();
         return;
    }
    else if(info->type.compare("TYPE_QLINEEDIT")==0){
         qDebug()<<"创建lineedit";
          pointerWidget=new QLineEdit(info->name,this);
          pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
          info->objpointer=pointerWidget;
          pointerWidget->show();
          return;
    }
    else if(info->type.compare("TYPE_QDATEEDIT")==0){
         qDebug()<<"创建dateedit";
          pointerWidget=new QDateEdit(this);
          pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
          info->objpointer=pointerWidget;
          pointerWidget->show();
         return;
    }
    else if(info->type.compare("TYPE_QTIMEEDIT")==0){
         qDebug()<<"创建timeedit";
         pointerWidget=new QTimeEdit(this);
         pointerWidget->setGeometry(info->posx,info->posy,info->width,info->heigth);
         info->objpointer=pointerWidget;
         pointerWidget->show();
         return;
    }

}
//---------------------------删除------------------------------------
void MainWindow::DeleteWidget()
{
    qDebug()<<"清除控件";
    if(!m_deleteWidgetArray){
        qDebug()<<"删除控件对象不存在";
        return;
    }
    WidgetInfo *info=NULL;
    vecWidgetArray::iterator it;
    for(it=m_deleteWidgetArray->begin();it!=m_deleteWidgetArray->end();it++){
        info=*it;
        if(info){

            if(info->objpointer){

                delete info->objpointer;
                info->objpointer=NULL;
            }

        }


    }
      m_deleteWidgetArray->clear();
      m_deleteWidgetArray=NULL;


}


//---------------------------点击事件------------------------------------
void MainWindow::on_actionopenxml_triggered()
{
    DeleteWidget();
   //能够获取到
    qDebug()<<"点击启动"+m_xmlPath;
    loadfile(m_xmlPath);

}
//---------------------------点击时间------------------------------------
void MainWindow::on_actionopenjson_triggered()
{
    DeleteWidget();
    qDebug()<<"点击启动"+m_jsonPath;
    loadfile(m_jsonPath);
}

void MainWindow::on_actionopensqlite_triggered()
{
    DeleteWidget();

    qDebug()<<"点击启动"+m_sqlitepath;
    loadfile(m_sqlitepath);

}



void MainWindow::on_actionclearAll_triggered()
{
     DeleteWidget();

}
