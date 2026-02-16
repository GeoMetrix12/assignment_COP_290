#include "MainWindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QStyle> // For standard icons

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), canvas_(new editor::Canvas(this)) {
    
    setCentralWidget(canvas_);
    resize(1200, 800);
    setWindowTitle("Vector Graphics Editor - IIT Delhi COP290");

    createActions();
    createMenus();
    createToolbar();
    
    // Initialize Status Bar
    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow() {
    // Canvas is a child widget, so it deletes automatically
}

void MainWindow::createActions() {
    // --- FILE ACTIONS ---
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

    // --- EDIT ACTIONS ---
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

    // --- TOOL ACTIONS (Mutually Exclusive) ---
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

    // Note: You can load custom icons here later using QIcon(":/path/to/icon.png")
    actionSelect   = createTool("Select", editor::ToolType::SELECTION, "V");
    actionLine     = createTool("Line", editor::ToolType::LINE, "L");
    actionRect     = createTool("Rect", editor::ToolType::RECTANGLE, "R");
    actionRoundedRect = createTool("R-Rect", editor::ToolType::ROUNDED_RECTANGLE, "");
    actionCircle   = createTool("Circle", editor::ToolType::CIRCLE, "C");
    actionHexagon  = createTool("Hexagon", editor::ToolType::HEXAGON, "H");
    actionFreehand = createTool("Pencil", editor::ToolType::FREEHAND, "P");
    actionText     = createTool("Text", editor::ToolType::TEXT, "T");

    actionSelect->setChecked(true); // Default tool
}

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

void MainWindow::createToolbar() {
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(18, 18));

    // File
    toolbar->addAction(actionNew);
    toolbar->addAction(actionOpen);
    toolbar->addAction(actionSave);
    toolbar->addSeparator();

    // Undo/Redo
    toolbar->addAction(actionUndo);
    toolbar->addAction(actionRedo);
    toolbar->addSeparator();

    // Tools
    toolbar->addAction(actionSelect);
    toolbar->addSeparator();
    toolbar->addAction(actionLine);
    toolbar->addAction(actionRect);
    toolbar->addAction(actionRoundedRect);
    toolbar->addAction(actionCircle);
    toolbar->addAction(actionHexagon);
    toolbar->addAction(actionFreehand);
    toolbar->addAction(actionText);
    
    toolbar->addSeparator();

    // --- Properties Widgets ---
    
    // Stroke Width Spinbox
    toolbar->addWidget(new QLabel(" Width: ", this));
    strokeWidthSpinBox = new QSpinBox(this);
    strokeWidthSpinBox->setRange(1, 20);
    strokeWidthSpinBox->setValue(2);
    connect(strokeWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), canvas_, &editor::Canvas::setStrokeWidth);
    toolbar->addWidget(strokeWidthSpinBox);
    toolbar->addSeparator();

    // Stroke Color Button
    strokeColorBtn = new QToolButton(this);
    strokeColorBtn->setText("Stroke"); // If icon is missing, text will show
    strokeColorBtn->setStyleSheet("background-color: black; color: white; border: 1px solid gray; padding: 2px;");
    connect(strokeColorBtn, &QToolButton::clicked, this, &MainWindow::pickStrokeColor);
    toolbar->addWidget(strokeColorBtn);

    // Fill Color Button
    fillColorBtn = new QToolButton(this);
    fillColorBtn->setText(" Fill ");
    fillColorBtn->setStyleSheet("background-color: white; color: black; border: 1px solid gray; padding: 2px;");
    connect(fillColorBtn, &QToolButton::clicked, this, &MainWindow::pickFillColor);
    toolbar->addWidget(fillColorBtn);
}

// --- Implementation of Slots ---

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open SVG", "", "SVG Files (*.svg);;All Files (*)");
    if (!fileName.isEmpty()) {
        canvas_->loadFromFile(fileName);
        statusBar()->showMessage("Loaded: " + fileName, 3000);
    }
}

void MainWindow::saveFile() {
    if (canvas_->hasFile()) {
        if (canvas_->saveToFile(canvas_->getCurrentFile())) {
            statusBar()->showMessage("Saved.", 2000);
        } else {
            QMessageBox::warning(this, "Error", "Could not save file.");
        }
    } else {
        saveFileAs();
    }
}

void MainWindow::saveFileAs() {
    QString currentPath = canvas_->hasFile() ? canvas_->getCurrentFile() : "untitled.svg";
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", currentPath, "SVG Files (*.svg);;All Files (*)");
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".svg", Qt::CaseInsensitive)) fileName += ".svg";
        if (canvas_->saveToFile(fileName)) {
            canvas_->setCurrentFile(fileName);
            statusBar()->showMessage("Saved to " + fileName, 3000);
        }
    }
}

void MainWindow::pickStrokeColor() {
    QColor color = QColorDialog::getColor(Qt::black, this, "Select Stroke Color");
    if (color.isValid()) {
        canvas_->setStrokeColor(color);
        // Update button visual
        strokeColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(color.name()));
        // Ensure text is readable against background
        strokeColorBtn->setStyleSheet(strokeColorBtn->styleSheet() + 
            QString("color: %1;").arg(color.lightness() > 128 ? "black" : "white"));
    }
}

void MainWindow::pickFillColor() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Fill Color");
    if (color.isValid()) {
        canvas_->setFillColor(color);
        fillColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(color.name()));
        fillColorBtn->setStyleSheet(fillColorBtn->styleSheet() + 
            QString("color: %1;").arg(color.lightness() > 128 ? "black" : "white"));
    }
}