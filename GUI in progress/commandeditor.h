#ifndef COMMANDEDITOR_H
#define COMMANDEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QGridLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <vector>
#include <QListWidget>

class CommandEditor : public QObject
{
public:
    CommandEditor(QString name, QListWidget *listWidget);
    void ConnectButtons();
    void setName(QString newName);
    QString getName();
    void setProjectName(QString name);

    QWidget *CommandEditorWidget;
    std::vector<QLineEdit*> *PointVec;
    QLineEdit *Command_Name;
    QComboBox *Line_Color, *Line_Style;
    QPushButton *Add_Command, *Add_Point, *Remove_Point;

private:
    void PopulateButtons(QGridLayout *ButtonHolder);
    void PopulateParameters(QFormLayout *ParameterHolder);
    void BuildEditor();
    void MakePoint();

    int pointCount;
    QFormLayout *ParameterHolder;
    QString name;
    QString projectName;
    QListWidget *widget;


public slots:
    void Add_Command_Clicked();
    void Add_Point_Clicked();
    void Remove_Point_Clicked();
};

#endif // COMMANDEDITOR_H
