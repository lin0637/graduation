#include "ElectricalAutomation.h"
#include "ui_ElectricalAutomation.h"

ElectricalAutomation::ElectricalAutomation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ElectricalAutomation)
{
    ui->setupUi(this);

    //设置界面背景
    QPalette p;
    p.setBrush(QPalette::Background,QBrush(QPixmap(":/loginbk1.jpg").scaled(this->size())));
    this->setPalette(p);

    //设置inputedit提示显示
    ui->inputline->setPlaceholderText("请输入编号或者书名");

    //创建model实现与数据库的交互
    model = new QSqlTableModel(this);
    model->setTable("electricalautomation");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0,Qt::Horizontal,tr("编号"));
    model->setHeaderData(1,Qt::Horizontal,tr("书名"));
    model->setHeaderData(2,Qt::Horizontal,tr("数量"));
    model->setHeaderData(3,Qt::Horizontal,tr("链接地址"));
    model->removeColumns(4,4);

    //创建_model实现查询功能
    _model=new QSqlTableModel(this);
    _model->setTable("electricalautomation");
    _model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    _model->setHeaderData(0,Qt::Horizontal,"编号");
    _model->setHeaderData(1,Qt::Horizontal,"书名");
    _model->setHeaderData(2,Qt::Horizontal,"数量");
    _model->setHeaderData(3,Qt::Horizontal,"链接地址");
    _model->removeColumns(4,4);

    //创建buttongroup
    buttongroup=new QButtonGroup(this);
    buttongroup->addButton(ui->num_radio,0);
    buttongroup->addButton(ui->name_radio,1);

    //设置view不可修改
    ui->electricalautomationview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->electricalautomationview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

ElectricalAutomation::~ElectricalAutomation()
{
    delete ui;

}

void ElectricalAutomation::comeStudentManage(QString num)
{
    model->select();
    if(model->data(model->index(0,0)).toString().isEmpty())
    {
        QMessageBox::information(this,"提示","该专业暂无教学资料",QMessageBox::Yes);
//        emit EmitToTeacherManageToChangeStack();
        this->hide();
        return;
    }
    ui->electricalautomationview->setModel(model);
}

void ElectricalAutomation::on_returnbtn_clicked()
{
    this->hide();
    emit EmitToStuManage();
}

void ElectricalAutomation::on_searchbtn_clicked()
{
    if(ui->inputline->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","查询输入框不能为空!");
        return;
    }

    if(buttongroup->checkedId()==0)
    {
        QString id = ui->inputline->text();
        _model->setFilter(QObject::tr("id ='%1'").arg(id));
        _model->select();
        if(_model->data(model->index(0,0)).toString().isEmpty())
        {
            QMessageBox::warning(this,"警告","请去确认编号是否正确");
            ui->electricalautomationview->setModel(model);
            ui->inputline->clear();
            return;
        }
        ui->electricalautomationview->setModel(_model);
    }
    else if(buttongroup->checkedId()==1)
    {
        _model->setFilter("name='"+ui->inputline->text()+"'");
        _model->select();
        if(_model->data(model->index(0,0)).toString().isEmpty())
        {
            QMessageBox::warning(this,"警告","请去确认书名是否正确");
            ui->electricalautomationview->setModel(model);
            ui->inputline->clear();
            return;
        }
        ui->electricalautomationview->setModel(_model);
    }
    else
    {
        QMessageBox::warning(this,"警告","请选择查询方式");
    }
}
