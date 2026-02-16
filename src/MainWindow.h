#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QLabel>
#include <QSpinBox>
#include <QToolButton>
#include "Canvas.h" 

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    
    void pickStrokeColor();
    void pickFillColor();

private:
    void createActions();   
    void createMenus();   
    void createToolbar(); 
    void updatePropertyUI(int width, int fontSize, const QColor& stroke, const QColor& fill);
    editor::Canvas* canvas_;

    QAction* actionNew;
    QAction* actionOpen;
    QAction* actionSave;
    QAction* actionSaveAs;
    QAction* actionExit;

    QAction* actionUndo;
    QAction* actionRedo;
    QAction* actionCut;
    QAction* actionCopy;
    QAction* actionPaste;

    QActionGroup* toolGroup;
    QAction* actionSelect;
    QAction* actionLine;
    QAction* actionRect;
    QAction* actionRoundedRect;
    QAction* actionCircle;
    QAction* actionHexagon;
    QAction* actionFreehand;
    QAction* actionText;

    QSpinBox* strokeWidthSpinBox;
    QSpinBox* fontSizeSpinBox;
    QToolButton* strokeColorBtn;
    QToolButton* fillColorBtn;
};

#endif 