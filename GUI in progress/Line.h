#ifndef LINE_H
#define LINE_H

#include "WorkSpace.h"
#include "ui_Line.h"

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
	QString check;
    explicit Line(QWidget *parent = 0);
    ~Line();

    void setName(QString newName);
	void setWorkSpace(WorkSpace *workSpace);
    void setCommandAdded(bool commandAdded);

	QWidget *CommandEditorWidget;
    bool commandAdded;
	void AddPoint(int x, int y);

private:
	void PopulateButtons(QGridLayout *ButtonHolder);
	void PopulateParameters(QFormLayout *ParameterHolder);
	void ConnectButtons();
	void BuildEditor();
    void removeExcessLines();

	Ui::Line *ui;
    int pointCount;
    QFormLayout *ParameterHolder;
	QString name;

	WorkSpace *workSpace;
	std::vector<QLineEdit*> *PointVec;
	QLineEdit *Command_Name;
	QComboBox *Line_Color, *Line_Style;
	QSpinBox *Line_Width;
	QPushButton *Add_Command, *Add_Point;

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
