#include "CommandWindow.h"

int RGB_COL = 0;
int NAME_COL = 1;
int BREAK_COL = 2;
int SIM_COL = 3;
int PAINT_COL = 4;
int POS_COL = 5;

/**
 * @brief constructor
 * @param ss
 * @param parent
 */
CommandWindow::CommandWindow(Shapes *ss, QWidget *parent) :
QMainWindow(parent),
ui(new Ui::CommandWindow)
{
	ui->setupUi(this);
	QRect r = QApplication::desktop()->availableGeometry();
	this->move(0, r.bottom() - height() - 50);

	shapes = ss;

	timer = new QTimer(this);
	commandsFinished = 0;
	secondCount = 0;
	minuteCount = 0;
	minPrep = false;

	ui->tableWidget->setColumnCount(6);
	ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(launchRightClick(QPoint)));
	connect(ui->tableWidget, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(cellChange(int, int, int, int)));

	connect(ui->MoveUp, SIGNAL(clicked()), this, SLOT(moveUpClicked()));
	connect(ui->MoveDown, SIGNAL(clicked()), this, SLOT(moveDownClicked()));
	connect(ui->DeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommandClicked()));

	connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(drawingStarted()));
	connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(drawingPaused()));
	connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(drawingCleared()));
	connect(ui->actionForward, SIGNAL(triggered()), this, SLOT(drawingPaused()));
	connect(ui->actionBackward, SIGNAL(triggered()), this, SLOT(drawingPaused()));
	connect(this->timer, SIGNAL(timeout()), this, SLOT(showTime()));

	//building color changing dialog
	colorForm = new QDialog();
	buildColorChangeDialog(colorForm);

	// setting up misc. modes
	modeBox = new QComboBox();
	delayTimeLabel = new QLabel("delay (ms): ");
	QStringList modes;
	modes << "Simulate Magic Marker" << "Simulate Real Brush" << "Non-touch robot motion" << "Paint w/o feedback" << "Paint w/ feedback";
	modeBox->addItems(modes);
	ui->toolBar->insertWidget(ui->actionPlay, modeBox);
	connect(modeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMode()));

	delayTime = new QSpinBox();
	//setupDelayTime(delayTime);
	setupDelayTime();
	ui->toolBar->insertSeparator(ui->actionPlay);
	connect(delayTime, SIGNAL(valueChanged(int)), this, SLOT(updateMode()));

	connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(disableModeSetting()));
	connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(enableModeSetting()));

	populate();
	this->hide();
}
/**
 * @brief destructor
 */
CommandWindow::~CommandWindow()
{
	delete ui;
}
/**
/ @brief Sets up DelayTime
*/
//void CommandWindow::setupDelayTime(QSpinBox *DelayTime)
void CommandWindow::setupDelayTime()
{
	delayTime->setFixedWidth(40);
	delayTime->setMinimum(0);
	delayTime->setMaximum(3000);
	delayTime->setSingleStep(10);
	delayTime->setValue(50);
	ui->toolBar->insertSeparator(ui->actionPlay);
	delayTimeLabel_action = ui->toolBar->insertWidget(ui->actionPlay, delayTimeLabel);
	delayTime_action = ui->toolBar->insertWidget(ui->actionPlay, delayTime);
}

/**
/ @ brief Sets up the color change dialog.

*/
void CommandWindow::buildColorChangeDialog(QDialog *colorForm)
{
	colorUi.setupUi(colorForm);
	colorForm->setWindowTitle("Color Options");
	colorUi.tableWidget->setRowCount(0);
	colorUi.tableWidget->setColumnCount(6);
	colorUi.tableWidget->insertRow(ui->tableWidget->rowCount());
	colorUi.tableWidget->setHorizontalHeaderItem(RGB_COL, new QTableWidgetItem("RGB"));
	colorUi.tableWidget->setHorizontalHeaderItem(BREAK_COL, new QTableWidgetItem("Break"));
	colorUi.tableWidget->setHorizontalHeaderItem(NAME_COL, new QTableWidgetItem("Type"));
	colorUi.tableWidget->setHorizontalHeaderItem(SIM_COL, new QTableWidgetItem("Simulated"));
	colorUi.tableWidget->setHorizontalHeaderItem(PAINT_COL, new QTableWidgetItem("Painted"));
	colorUi.tableWidget->setHorizontalHeaderItem(POS_COL, new QTableWidgetItem("pos"));
	colorUi.tableWidget->resizeColumnsToContents();
	colorUi.ChangeAll->setChecked(true);
	connect(colorUi.ConfirmButton, SIGNAL(pressed()), this, SLOT(colorChangeConfirmed()));
}
/**
 * @brief move command up in window (and vec)
 */
void CommandWindow::moveUpClicked() {
	int currentIndex = ui->tableWidget->currentRow();
	if (currentIndex <= 0) return;
	if (currentIndex > 0){
		shapes->swap(currentIndex, currentIndex - 1);
		populate();
	}
	ui->tableWidget->setCurrentCell(currentIndex - 1, NAME_COL);
}
/**
 * @brief move command down in window (and vec)
 */
void CommandWindow::moveDownClicked() {
	if (ui->tableWidget->rowCount() <= 0 || ui->tableWidget->currentRow() == ui->tableWidget->rowCount() - 1) return;

	int currentIndex = ui->tableWidget->currentRow();
	if (currentIndex < ui->tableWidget->rowCount()){
		shapes->swap(ui->tableWidget->currentRow(), ui->tableWidget->currentRow() + 1);
		populate();
	}
	ui->tableWidget->setCurrentCell(currentIndex + 1, NAME_COL);

}
/**
 * @brief remove command from window (and vec)
 */
void CommandWindow::deleteCommandClicked() {
	if (ui->tableWidget->rowCount() <= 0) return;
	int currentIndex = ui->tableWidget->currentRow();
	shapes->removeShapeAt(currentIndex);
	populate();
	emit modifiedCommand();
}

/**
 * @brief populate list with commands.
 */
void CommandWindow::populate(){
	//reseting
	this->show();
	ui->tableWidget->clear();
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setHorizontalHeaderItem(RGB_COL, new QTableWidgetItem("RGB"));
	ui->tableWidget->setHorizontalHeaderItem(BREAK_COL, new QTableWidgetItem("Break"));
	ui->tableWidget->setHorizontalHeaderItem(SIM_COL, new QTableWidgetItem("Simulated"));
	ui->tableWidget->setHorizontalHeaderItem(PAINT_COL, new QTableWidgetItem("Painted"));
	ui->tableWidget->setHorizontalHeaderItem(NAME_COL, new QTableWidgetItem("Type"));
	ui->tableWidget->setHorizontalHeaderItem(POS_COL, new QTableWidgetItem("pos"));

	//refilling the list with shape data
	std::vector<cv::Scalar> existingColors;
	for (int i = 0; i < shapes->length(); i++){
		QString name = QString::fromStdString(shapes->at(i)->type);

		//cell creation
		ui->tableWidget->insertRow(ui->tableWidget->rowCount());
		ui->tableWidget->setItem(i, RGB_COL, new QTableWidgetItem()); //rgb
		ui->tableWidget->setItem(i, NAME_COL, new QTableWidgetItem(name)); //command type
		if (shapes->at(i)->isBreakPoint){ //beakpoint
			ui->tableWidget->setItem(i, BREAK_COL, new QTableWidgetItem("yes"));
		}
		else { ui->tableWidget->setItem(i, BREAK_COL, new QTableWidgetItem("no")); }
		if (shapes->at(i)->hasPainted) { //paint col
			ui->tableWidget->setItem(i, PAINT_COL, new QTableWidgetItem("yes"));
		}
		else {
			ui->tableWidget->setItem(i, PAINT_COL, new QTableWidgetItem("no"));
		}
		if (shapes->at(i)->hasSimulated) { //sim col
			ui->tableWidget->setItem(i, SIM_COL, new QTableWidgetItem("yes"));
		}
		else {
			ui->tableWidget->setItem(i, SIM_COL, new QTableWidgetItem("no"));
		}
		cv::Scalar penCol = shapes->at(i)->getPenColor(); //color col
		ui->tableWidget->item(i, RGB_COL)->setBackgroundColor(QColor(penCol[0], penCol[1], penCol[2]));

		bool alreadyExistingColor = 0;
		for (int j = 0; j < existingColors.size(); j++) {
			if (penCol[0] == existingColors.at(j)[0] && penCol[1] == existingColors.at(j)[1] && penCol[2] == existingColors.at(j)[2]) {
				alreadyExistingColor = 1;
				shapes->at(i)->pos = 'A' + j;
				break;
			}
		}
		if (!alreadyExistingColor) {
			shapes->at(i)->pos = 'A' + existingColors.size();
			existingColors.push_back(penCol);
		}
		ui->tableWidget->setItem(i, POS_COL, new QTableWidgetItem(QString(shapes->at(i)->pos)));

		//centering
		ui->tableWidget->item(i, BREAK_COL)->setTextAlignment(Qt::AlignCenter);
		ui->tableWidget->item(i, NAME_COL)->setTextAlignment(Qt::AlignCenter);
		ui->tableWidget->item(i, SIM_COL)->setTextAlignment(Qt::AlignCenter);
		ui->tableWidget->item(i, PAINT_COL)->setTextAlignment(Qt::AlignCenter);
		ui->tableWidget->item(i, POS_COL)->setTextAlignment(Qt::AlignCenter);

		//resizing
		ui->tableWidget->resizeColumnsToContents();
	}

	//reseting run stats
	ui->CommandsRun->setText(QString::number(commandsFinished) + QString("/") + QString::number(shapes->length()) + QString(" run"));
	ui->CommandsRun->setAlignment(Qt::AlignCenter);
}
/**
 * @brief recieve right click
 * @param pos
 */
void CommandWindow::launchRightClick(QPoint pos) {
	ui->actionPause->trigger();
	QMenu *menu = new QMenu(ui->tableWidget);
	int col = ui->tableWidget->currentColumn();

	if (col == RGB_COL) { //color menu
		menu->addAction(new QAction("Change Color", ui->tableWidget));
	}
	else if (col == BREAK_COL) { //breakpoint menu
		menu->addAction(new QAction("Set Breakpoint", ui->tableWidget));
	}
	else if (col == NAME_COL) { //run menu
		menu->addAction(new QAction("Run from here", ui->tableWidget));
		menu->addAction(new QAction("Run this command", ui->tableWidget));
	}
	menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(menuSort(QAction*)));
}
/**
 * @brief sort menu from right click.
 * @param a
 */
void CommandWindow::menuSort(QAction *a) {
	if (a->text() == "Run from here") emit runFrom(ui->tableWidget->currentRow());
	else if (a->text() == "Run this command") emit runOnly(ui->tableWidget->currentRow());
	else if (a->text() == "Set Breakpoint") {
		int index = ui->tableWidget->currentRow();
		if (shapes->at(index)->isBreakPoint) {
			ui->tableWidget->setItem(index, BREAK_COL, new QTableWidgetItem("no")); //turn off
			shapes->at(index)->toggleBreakPoint(false);
		}
		else {
			ui->tableWidget->setItem(index, BREAK_COL, new QTableWidgetItem("yes")); //turn on
			shapes->at(index)->toggleBreakPoint(true);
		}
	}
	else if (a->text() == "Change Color") {
		int curRow = ui->tableWidget->currentRow();
		QString name = QString::fromStdString(shapes->at(curRow)->type);
		colorUi.tableWidget->setItem(0, NAME_COL, new QTableWidgetItem(name));

		cv::Scalar penCol = shapes->at(curRow)->getPenColor();
		QTableWidgetItem *rgb = new QTableWidgetItem();
		rgb->setBackgroundColor(QColor(penCol[0], penCol[1], penCol[2]));
		colorUi.tableWidget->setItem(0, RGB_COL, rgb);

		if (shapes->at(curRow)->isBreakPoint) {
			colorUi.tableWidget->setItem(0, BREAK_COL, new QTableWidgetItem("yes"));
		}
		else {
			colorUi.tableWidget->setItem(0, BREAK_COL, new QTableWidgetItem("no"));
		}

		if (shapes->at(curRow)->hasPainted) { //paint col
			colorUi.tableWidget->setItem(0, PAINT_COL, new QTableWidgetItem("yes"));
		}
		else {
			colorUi.tableWidget->setItem(0, PAINT_COL, new QTableWidgetItem("no"));
		}

		if (shapes->at(curRow)->hasSimulated) { //sim col
			colorUi.tableWidget->setItem(0, SIM_COL, new QTableWidgetItem("yes"));
		}
		else {
			colorUi.tableWidget->setItem(0, SIM_COL, new QTableWidgetItem("no"));
		}

		colorUi.tableWidget->item(0, BREAK_COL)->setTextAlignment(Qt::AlignCenter);
		colorUi.tableWidget->item(0, SIM_COL)->setTextAlignment(Qt::AlignCenter);
		colorUi.tableWidget->item(0, PAINT_COL)->setTextAlignment(Qt::AlignCenter);
		colorUi.tableWidget->item(0, NAME_COL)->setTextAlignment(Qt::AlignCenter);

		colorUi.comboBox->setCurrentText("current color");
		colorUi.tableWidget->resizeColumnsToContents();
		colorForm->show();
	}
}

/**
 * @brief toggle identifier for already run commands.
 * @param index
 * @param runToggle
 */
void CommandWindow::updateCommandList(int index, QString runToggle){
	if (index >= ui->tableWidget->rowCount()){
		index = ui->tableWidget->rowCount() - 1;
	}
	if (runToggle == "finishedSim"){
		ui->tableWidget->setItem(index, SIM_COL, new QTableWidgetItem("yes"));
		shapes->at(index)->hasSimulated = true;
		ui->tableWidget->item(index, SIM_COL)->setTextAlignment(Qt::AlignCenter);
		ui->tableWidget->item(index, PAINT_COL)->setTextAlignment(Qt::AlignCenter);

		if (index == shapes->length() - 1) { this->timer->stop(); }
		if (this->commandsFinished >= shapes->length()) { return; }
		this->commandsFinished++;
		ui->CommandsRun->setText(QString::number(commandsFinished) + QString("/") + QString::number(shapes->length()) + QString(" run"));
		ui->CommandsRun->setAlignment(Qt::AlignCenter);
	}
	else if (runToggle == "finishedBot"){
		ui->tableWidget->setItem(index, SIM_COL, new QTableWidgetItem("yes"));
		ui->tableWidget->setItem(index, PAINT_COL, new QTableWidgetItem("yes"));
		shapes->at(index)->hasSimulated = true;
		shapes->at(index)->hasPainted = true;
		ui->tableWidget->item(index, SIM_COL)->setTextAlignment(Qt::AlignCenter);
		ui->tableWidget->item(index, PAINT_COL)->setTextAlignment(Qt::AlignCenter);

		if (index == shapes->length() - 1) { this->timer->stop(); }
		if (this->commandsFinished >= shapes->length()) { return; }
		this->commandsFinished++;
		ui->CommandsRun->setText(QString::number(commandsFinished) + QString("/") + QString::number(shapes->length()) + QString(" run"));
		ui->CommandsRun->setAlignment(Qt::AlignCenter);
	}
	else if (runToggle == "runningSim") {
		ui->tableWidget->setItem(index, SIM_COL, new QTableWidgetItem("running"));

		ui->tableWidget->scrollToItem(ui->tableWidget->item(index, RGB_COL), QAbstractItemView::PositionAtCenter);
		emit highlightShape(index);
	}
	else if (runToggle == "runningBot") {
		ui->tableWidget->setItem(index, SIM_COL, new QTableWidgetItem("running"));
		ui->tableWidget->setItem(index, PAINT_COL, new QTableWidgetItem("running"));

		ui->tableWidget->scrollToItem(ui->tableWidget->item(index, RGB_COL), QAbstractItemView::PositionAtCenter);
		emit highlightShape(index);
	}
	else if (runToggle == "clear") {
		ui->tableWidget->setItem(index, SIM_COL, new QTableWidgetItem("no"));
		ui->tableWidget->setItem(index, PAINT_COL, new QTableWidgetItem("no"));
		shapes->at(index)->hasSimulated = false;
		shapes->at(index)->hasPainted = false;
	}

	//centering
	ui->tableWidget->item(index, SIM_COL)->setTextAlignment(Qt::AlignCenter);
	ui->tableWidget->item(index, PAINT_COL)->setTextAlignment(Qt::AlignCenter);
}

void CommandWindow::drawingStarted() {
	if (shapes->length() == 0) { return; }
	timer->start();
	startTime = QTime::currentTime();
}

void CommandWindow::drawingPaused() {
	if (!this->timer->isActive()) {
		return;
	}

	this->timer->stop();
	secondCount = QTime::currentTime().second() - startTime.second() + secondCount;
	if (secondCount < 0) { secondCount = 60 + secondCount; }
}

void CommandWindow::drawingCleared() {
	this->timer->stop();
	ui->TimeEllapsed->setText(QString::number(0) + QString(":") + QString::number(0) + QString(" elapsed"));
	ui->TimeEllapsed->setAlignment(Qt::AlignCenter);
	secondCount = 0;
	minuteCount = 0;
	minPrep = false;
}

void CommandWindow::showTime() {
	int sec = QTime::currentTime().second() - startTime.second() + secondCount;
	if (sec < 0) { sec = 60 + sec; }

	//updating minute logic
	if (sec == 1) { minPrep = true; }
	if (sec == 0 && minPrep) {
		minPrep = false;
		minuteCount++;
	}

	ui->TimeEllapsed->setText(QString::number(minuteCount) + QString(":") + QString::number(sec) + QString(" elapsed"));
	ui->TimeEllapsed->setAlignment(Qt::AlignCenter);
}

void CommandWindow::cellChange(int curRow, int curCol, int prevRow, int prevCol) { emit highlightShape(curRow); }

std::vector<int> getQColor(QString col) {
	std::string col2 = col.toUtf8().constData();
	return paint_util::getColor(col2);
}

void CommandWindow::colorChangeConfirmed() {
	colorForm->hide();
	std::vector<int> NewRGB = getQColor(colorUi.comboBox->currentText());
	cv::Scalar OldRGB = shapes->at(ui->tableWidget->currentRow())->getPenColor();
	if (NewRGB[0] == -1) return; //no change detected.

	if (colorUi.ChangeAll->isChecked()) {
		for (int i = 0; i < shapes->length(); i++) {
			cv::Scalar checkRGB = shapes->at(i)->getPenColor();
			if (checkRGB[0] == OldRGB[0] && checkRGB[1] == OldRGB[1] && checkRGB[2] == OldRGB[2]) {
				shapes->at(i)->setPenColor(NewRGB[0], NewRGB[1], NewRGB[2]);
			}
		}
	}
	else {
		shapes->at(ui->tableWidget->currentRow())->setPenColor(NewRGB[0], NewRGB[1], NewRGB[2]);
	}

	populate();
}

void CommandWindow::updateMode() {
	if (modeBox->currentText() == "Simulate Idealized Brush") {
		updateModeHelper(false, false, true, true);
	}
	else if (modeBox->currentText() == "Simulate Real Brush") {
		updateModeHelper(false, false, true, true);
	}
	else if (modeBox->currentText() == "Non-touch robot motion") {
		updateModeHelper(true, true, false, false);
	}
	else if (modeBox->currentText() == "Paint w/o feedback") {
		updateModeHelper(true, true, false, false);
	}
	else if (modeBox->currentText() == "Paint w/ feedback") {
		updateModeHelper(true, true, false, false);
	}

	emit modeUpdated(modeBox->currentText(), delayTime->text().toInt());
}
void CommandWindow::updateModeHelper(bool setForward,
	bool setBackward, bool timeLabelAction, bool timeAction)
{
	ui->actionBackward->setDisabled(setForward);
	ui->actionForward->setDisabled(setBackward);
	delayTimeLabel_action->setVisible(timeLabelAction);
	delayTime_action->setVisible(timeAction);
}
void CommandWindow::enableModeSetting() {
	this->modeBox->setDisabled(false);
	this->delayTime->setDisabled(false);
	this->delayTimeLabel->setDisabled(false);
}

void CommandWindow::disableModeSetting() {
	this->modeBox->setDisabled(true);
	this->delayTime->setDisabled(true);
	this->delayTimeLabel->setDisabled(true);
}