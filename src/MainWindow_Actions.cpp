#include "MainWindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QStyle> 

void MainWindow::createActions() {
    actionNew = new QAction(style()->standardIcon(QStyle::SP_FileIcon), "&New", this);
    actionNew->setShortcut(QKeySequence::New);
    connect(actionNew, &QAction::triggered, canvas_, &editor::Canvas::clearCanvas);

    actionOpen = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton), "&Open...", this);
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);

    actionSave = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), "&Save", this);
    actionSave->setShortcut(QKeySequence::Save);
    connect(actionSave, &QAction::triggered, this, &MainWindow::saveFile);

    actionSaveAs = new QAction("Save &As...", this);
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);

    actionExit = new QAction("E&xit", this);
    actionExit->setShortcut(QKeySequence::Quit);
    connect(actionExit, &QAction::triggered, this, &QWidget::close);

    actionUndo = new QAction(style()->standardIcon(QStyle::SP_ArrowBack), "&Undo", this);
    actionUndo->setShortcut(QKeySequence::Undo);
    connect(actionUndo, &QAction::triggered, canvas_, &editor::Canvas::undo);

    actionRedo = new QAction(style()->standardIcon(QStyle::SP_ArrowForward), "&Redo", this);
    actionRedo->setShortcut(QKeySequence::Redo);
    connect(actionRedo, &QAction::triggered, canvas_, &editor::Canvas::redo);

    actionCut = new QAction("&Cut", this);
    actionCut->setShortcut(QKeySequence::Cut);
    connect(actionCut, &QAction::triggered, canvas_, &editor::Canvas::cut);

    actionCopy = new QAction("&Copy", this);
    actionCopy->setShortcut(QKeySequence::Copy);
    connect(actionCopy, &QAction::triggered, canvas_, &editor::Canvas::copy);

    actionPaste = new QAction("&Paste", this);
    actionPaste->setShortcut(QKeySequence::Paste);
    connect(actionPaste, &QAction::triggered, canvas_, &editor::Canvas::paste);

    toolGroup = new QActionGroup(this);

    auto createTool = [&](const QString& name, editor::ToolType type, const QString& key) {
        QAction* act = new QAction(name, this);
        act->setCheckable(true);
        if(!key.isEmpty()) act->setShortcut(QKeySequence(key));
        act->setData(QVariant::fromValue(static_cast<int>(type)));
        toolGroup->addAction(act);
        connect(act, &QAction::triggered, [this, type]() {
            canvas_->setCurrentTool(type);
            statusBar()->showMessage("Selected Tool: " + QString::number(static_cast<int>(type)));
        });
        return act;
    };

    actionSelect   = createTool("Select", editor::ToolType::SELECTION, "V");
    actionLine     = createTool("Line", editor::ToolType::LINE, "L");
    actionRect     = createTool("Rect", editor::ToolType::RECTANGLE, "R");
    actionRoundedRect = createTool("R-Rect", editor::ToolType::ROUNDED_RECTANGLE, "");
    actionCircle   = createTool("Circle", editor::ToolType::CIRCLE, "C");
    actionHexagon  = createTool("Hexagon", editor::ToolType::HEXAGON, "H");
    actionFreehand = createTool("Pencil", editor::ToolType::FREEHAND, "P");
    actionText     = createTool("Text", editor::ToolType::TEXT, "T");

    actionSelect->setChecked(true); 
}