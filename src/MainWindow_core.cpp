#include "MainWindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QStyle> 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), canvas_(new editor::Canvas(this)) {
    setCentralWidget(canvas_);
    resize(1200, 800);
    setWindowTitle("Vector Graphics Editor - IIT Delhi COP290");
    createActions();
    createMenus();
    createToolbar();  
    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow() {}

void MainWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(actionNew);
    fileMenu->addAction(actionOpen);
    fileMenu->addAction(actionSave);
    fileMenu->addAction(actionSaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(actionExit);

    QMenu* editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(actionUndo);
    editMenu->addAction(actionRedo);
    editMenu->addSeparator();
    editMenu->addAction(actionCut);
    editMenu->addAction(actionCopy);
    editMenu->addAction(actionPaste);

    QMenu* toolMenu = menuBar()->addMenu("&Tools");
    toolMenu->addAction(actionSelect);
    toolMenu->addSeparator();
    toolMenu->addAction(actionLine);
    toolMenu->addAction(actionRect);
    toolMenu->addAction(actionRoundedRect);
    toolMenu->addAction(actionCircle);
    toolMenu->addAction(actionHexagon);
    toolMenu->addAction(actionFreehand);
    toolMenu->addAction(actionText);
}