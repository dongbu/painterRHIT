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
#include <QSpinBox>

#include <QWidget>

namespace Ui {
class CommandEditor;
}

class CommandEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CommandEditor(QWidget *parent = 0);
    ~CommandEditor();

    void ConnectButtons();
    void setName(QString newName);
    QString getName();
    void setCommandAdded(bool commandAdded);
	void add_Command_Externally(QString projectName);
    void set_Point_At(int index, int x, int y);
    void setList(QListWidget *list);

    QWidget *CommandEditorWidget;
    std::vector<QLineEdit*> *PointVec;
    QLineEdit *Command_Name;
    QComboBox *Line_Color, *Line_Style;
    QSpinBox *Line_Width;
    QPushButton *Add_Command, *Add_Point;
    bool commandAdded;

private:
    Ui::CommandEditor *ui;

    void PopulateButtons(QGridLayout *ButtonHolder);
    void PopulateParameters(QFormLayout *ParameterHolder);
    void BuildEditor();
    void MakePoint();
    void removeExcessLines();

    int pointCount;
    QListWidget *list;
    QFormLayout *ParameterHolder;
    QString name;

public slots:
void Add_Command_Clicked(QString projectName);
    void Add_Point_Clicked();
    void InfoChanged();
    void updateLineStyles(QString color, QString style, int width);

signals:
    void fileStatusChanged();
    void tell_Command_Added(int index);
    void tell_Command_Added();
    void signal_Info_Changed();
    void sendUpdateToDrawOn(CommandEditor* editor);
};

#endif // COMMANDEDITOR_H
