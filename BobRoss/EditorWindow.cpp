#pragma once

#include "EditorWindow.h"
#include "ui_EditorWindow.h"

//public methods below here
EditorWindow::EditorWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::setShapeToEdit(Shape s){
	this->toEdit = s;
}
//private methods below here
void EditorWindow::build() {
	printf("TODO: build editor\n");
}

//slots below here
void EditorWindow::updateSketchPadClicked() {
	printf("TODO: update sketch pad\n");
}
