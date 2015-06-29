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
    Q_OBJECT

public:
    CommandEditor(QListWidget *listWidget, int tabPos, QTabWidget* editorTabs);
    void ConnectButtons();
    void setName(QString newName);
    void setProjectName(QString name);
    QString getName();
    void setCommandAdded(bool commandAdded);
    void add_Command_Externally();
    void set_Point_At(int index, int x, int y);

    QWidget *CommandEditorWidget;
    std::vector<QLineEdit*> *PointVec;
    QLineEdit *Command_Name;
    QComboBox *Line_Color, *Line_Style;
    QPushButton *Add_Command, *Add_Point;
    int tabPosition;
    QTabWidget *editorTabs;
    bool commandAdded;

private:
    void PopulateButtons(QGridLayout *ButtonHolder);
    void PopulateParameters(QFormLayout *ParameterHolder);
    void BuildEditor();
    void MakePoint();
    void removeExcessLines();

    int pointCount;
    QFormLayout *ParameterHolder;
    QString name;
    QString projectName;
    QListWidget *widget;

public slots:
    void Add_Command_Clicked();
    void Add_Point_Clicked();
    void InfoChanged();

signals:
    void fileStatusChanged();
    void tell_Command_Added(int index);
	void tell_Command_Added();
    void signal_Info_Changed();
    void sendUpdateToDrawOn(CommandEditor* editor);
};

#endif // COMMANDEDITOR_H
