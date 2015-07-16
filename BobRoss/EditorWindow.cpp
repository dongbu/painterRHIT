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

//private methods below here
void EditorWindow::build() {

}

//slots below here
void EditorWindow::updateSketchPadClicked() {

}
