#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>

namespace Ui {
class StartWindow;
}

class StartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();

private:
    Ui::StartWindow *ui;
	int maxLength;
	
private slots:
	void updateExample();
	void loadClicked();
	void newClicked();
	void radioChanged();


};

#endif // STARTWINDOW_H
