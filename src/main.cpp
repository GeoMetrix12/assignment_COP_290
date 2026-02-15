#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>
#include "Canvas.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;

    // Create the central drawing area
    editor::Canvas* canvas = new editor::Canvas(&window);
    window.setCentralWidget(canvas);

    // --- 1. FILE MENU ---
    QMenu* fileMenu = window.menuBar()->addMenu("&File");
    
    fileMenu->addAction("&New", canvas, &editor::Canvas::clearCanvas, QKeySequence::New);
    
    fileMenu->addAction("&Open SVG", [&]() {
        QString fileName = QFileDialog::getOpenFileName(
            &window, 
            "Open Vector Graphics", 
            "", 
            "SVG Files (*.svg);;All Files (*)"
        );

        if (!fileName.isEmpty()) {
            canvas->loadFromFile(fileName);
            window.statusBar()->showMessage("Loaded: " + fileName, 3000);
        }
    }, QKeySequence::Open);

    auto performSaveAs = [&]() {
        QString currentPath = canvas->hasFile() ? canvas->getCurrentFile() : "untitled.svg";
        
        QString fileName = QFileDialog::getSaveFileName(
            &window, 
            "Save As", 
            currentPath, 
            "SVG Files (*.svg);;All Files (*)"
        );

        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".svg", Qt::CaseInsensitive)) {
                fileName += ".svg";
            }
            
            if (canvas->saveToFile(fileName)) {
                canvas->setCurrentFile(fileName); 
                window.statusBar()->showMessage("Saved to " + fileName, 3000);
            } else {
                window.statusBar()->showMessage("Failed to save file!", 3000);
            }
        }
    };

    fileMenu->addAction("&Save", [&]() {
        if (canvas->hasFile()) {
            if (canvas->saveToFile(canvas->getCurrentFile())) {
                window.statusBar()->showMessage("Saved.", 2000);
            } else {
                window.statusBar()->showMessage("Error saving file.", 2000);
            }
        } 
        else {
            performSaveAs();
        }
    }, QKeySequence::Save);

    fileMenu->addAction("Save &As...", performSaveAs, QKeySequence::SaveAs);

    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", &window, &QMainWindow::close, QKeySequence::Quit);

    // --- 2. EDIT MENU ---
    QMenu* editMenu = window.menuBar()->addMenu("&Edit");
    editMenu->addAction("&Undo", canvas, &editor::Canvas::undo, QKeySequence::Undo);
    editMenu->addAction("&Redo", canvas, &editor::Canvas::redo, QKeySequence::Redo);
    
    editMenu->addSeparator();
    
    editMenu->addAction("Cu&t", canvas, &editor::Canvas::cut, QKeySequence::Cut);
    editMenu->addAction("&Copy", canvas, &editor::Canvas::copy, QKeySequence::Copy);
    editMenu->addAction("&Paste", canvas, &editor::Canvas::paste, QKeySequence::Paste);

    // --- 3. TOOLS MENU ---
    QMenu* toolsMenu = window.menuBar()->addMenu("&Tools");

    toolsMenu->addAction("&Selection Tool", [=]() {
        canvas->setCurrentTool(editor::ToolType::SELECTION); 
    }, QKeySequence(Qt::Key_V));

    toolsMenu->addSeparator();

    // NEW: Freehand (Pencil) Tool
    toolsMenu->addAction("&Freehand Pencil", [=]() {
        canvas->setCurrentTool(editor::ToolType::FREEHAND);
    }, QKeySequence(Qt::Key_P)); // NEW

    // NEW: Text Tool
    toolsMenu->addAction("&Text Tool", [=]() {
        canvas->setCurrentTool(editor::ToolType::TEXT);
    }, QKeySequence(Qt::Key_T)); // NEW

    toolsMenu->addAction("Draw &Line", [=]() {
        canvas->setCurrentTool(editor::ToolType::LINE);
    }, QKeySequence(Qt::Key_L)); 

    toolsMenu->addSeparator();

    toolsMenu->addAction("Draw &Circle", [=]() {
        canvas->setCurrentTool(editor::ToolType::CIRCLE);
    }, QKeySequence(Qt::Key_C));

    toolsMenu->addAction("Draw &Rectangle", [=]() {
        canvas->setCurrentTool(editor::ToolType::RECTANGLE);
    }, QKeySequence(Qt::Key_R));

    toolsMenu->addAction("Draw R&ounded Rect", [=]() {
        canvas->setCurrentTool(editor::ToolType::ROUNDED_RECTANGLE);
    });

    toolsMenu->addAction("Draw &Hexagon", [=]() {
        canvas->setCurrentTool(editor::ToolType::HEXAGON);
    }, QKeySequence(Qt::Key_H));

    // --- 4. PROPERTIES MENU ---
    QMenu* propMenu = window.menuBar()->addMenu("&Properties");

    propMenu->addAction("Stroke &Width...", [&]() {
        bool ok;
        int width = QInputDialog::getInt(&window, "Stroke Width", "Width:", 2, 1, 20, 1, &ok);
        if (ok) canvas->setStrokeWidth(width);
    });

    propMenu->addAction("Stroke &Color...", [&]() {
        QColor color = QColorDialog::getColor(Qt::black, &window, "Select Stroke Color");
        if (color.isValid()) canvas->setStrokeColor(color);
    });

    propMenu->addAction("&Fill Color...", [&]() {
        QColor color = QColorDialog::getColor(Qt::white, &window, "Select Fill Color");
        if (color.isValid()) canvas->setFillColor(color);
    });

    // Final window setup
    window.setWindowTitle("Vector Graphics Editor - IIT Delhi COP290");
    window.resize(1000, 700);
    window.show();

    return app.exec();
}