#ifndef SOLID_H
#define SOLID_H
#include "ui_Solid.h"
#include "WorkSpace.h"


#include <QWidget>
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

namespace Ui {
class Solid;
}

class Solid : public QWidget
{
	friend class Line;
    Q_OBJECT

public:
    explicit Solid(QWidget *parent = 0);
    ~Solid();

	void ConnectButtons();
	void setName(QString newName);
	QString getName();
	void setCommandAdded(bool commandAdded);
	void add_Command_Externally(QString projectName);
	void set_Point_At(int index, int x, int y);
	void setWorkSpace(WorkSpace *workSpace);

	QWidget *CommandEditorWidget;
	std::vector<QLineEdit*> *PointVec;
	QLineEdit *Command_Name;
	QComboBox *Line_Color, *Fill_Style;

	QSpinBox *Line_Width;
	QPushButton *Add_Command;
	bool commandAdded;

private:
	Ui::Solid *ui;

	void PopulateButtons(QGridLayout *ButtonHolder);
	void PopulateParameters(QFormLayout *ParameterHolder);
	void BuildEditor();
	void MakePoint();
	void removeExcessLines();
	int pointCount;
	QFormLayout *ParameterHolder;
	QString name;
	WorkSpace *workSpace;

	public slots:
	void Add_Command_Clicked();
	void InfoChanged();
	void updateLineStyles(QString color, QString style, int width);

signals:
	void fileStatusChanged();
	void tell_Command_Added(int index);
	void tell_Command_Added();
	void signal_Info_Changed();
	void sendUpdateToDrawOn(Solid* editor);
};

#endif // SOLID_H
