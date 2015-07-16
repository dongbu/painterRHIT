#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "shapes.cpp"

#include <QObject>
#include <QMainWindow>
#include <vector>
#include <QWidget>

namespace Ui {
class EditorWindow;
}

class EditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();
	void setShapeToEdit(Shape s);

private:
    void build();

    Shape toEdit;
    Ui::EditorWindow *ui;


private slots:
    void updateSketchPadClicked();

signals:
    void updateSketchPad();
};

#endif // EDITORWINDOW_H

