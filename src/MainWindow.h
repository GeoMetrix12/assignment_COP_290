#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QLabel>
#include <QSpinBox>
#include <QToolButton>
#include "Canvas.h" // Ensure this path is correct

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File Operations
    void openFile();
    void saveFile();
    void saveFileAs();
    
    // Property Helpers
    void pickStrokeColor();
    void pickFillColor();

private:
    void createActions();   // Initialize QActions
    void createMenus();     // Add actions to Menu Bar
    void createToolbar();   // Add actions to Tool Bar
    void updatePropertyUI(int width, int fontSize, const QColor& stroke, const QColor& fill);
    editor::Canvas* canvas_;
    
    // -- Actions --
    // File
    QAction* actionNew;
    QAction* actionOpen;
    QAction* actionSave;
    QAction* actionSaveAs;
    QAction* actionExit;

    // Edit
    QAction* actionUndo;
    QAction* actionRedo;
    QAction* actionCut;
    QAction* actionCopy;
    QAction* actionPaste;

    // Tools (Checkable)
    QActionGroup* toolGroup;
    QAction* actionSelect;
    QAction* actionLine;
    QAction* actionRect;
    QAction* actionRoundedRect;
    QAction* actionCircle;
    QAction* actionHexagon;
    QAction* actionFreehand;
    QAction* actionText;

    // Toolbar Widgets
    QSpinBox* strokeWidthSpinBox;
    QSpinBox* fontSizeSpinBox;
    QToolButton* strokeColorBtn;
    QToolButton* fillColorBtn;
};

#endif // MAINWINDOW_H