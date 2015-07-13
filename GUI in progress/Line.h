#ifndef LINE_H
#define LINE_H

#include "Robot.h"

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
#include <QSpinBox>

#include <QWidget>

namespace Ui {
class Line;
}

class Line : public QWidget
{
    Q_OBJECT

public:
    explicit Line(QWidget *parent = 0);
    ~Line();

    void ConnectButtons();
    void setName(QString newName);
    QString getName();
    void setCommandAdded(bool commandAdded);
	void add_Command_Externally(QString projectName);
    void set_Point_At(int index, int x, int y);
    void setList(QListWidget *list);
	void setProjectName(QString projectName);
	void setProjectLocation(QString newLocation);
	void setRobot(Robot *robot);

    QWidget *CommandEditorWidget;
    std::vector<QLineEdit*> *PointVec;
    QLineEdit *Command_Name;
    QComboBox *Line_Color, *Line_Style;
    QSpinBox *Line_Width;
    QPushButton *Add_Command, *Add_Point;
    bool commandAdded;

private:
    Ui::Line *ui;

    void PopulateButtons(QGridLayout *ButtonHolder);
    void PopulateParameters(QFormLayout *ParameterHolder);
    void BuildEditor();
    void MakePoint();
    void removeExcessLines();

    int pointCount;
    QListWidget *list;
    QFormLayout *ParameterHolder;
    QString name, projectName, projectLocation;
	Robot *robot;

public slots:
void Add_Command_Clicked();
    void Add_Point_Clicked();
    void InfoChanged();
    void updateLineStyles(QString color, QString style, int width);

signals:
    void fileStatusChanged();
    void tell_Command_Added(int index);
    void tell_Command_Added();
    void signal_Info_Changed();
    void sendUpdateToDrawOn(Line* editor);
};

#endif // COMMANDEDITOR_H
