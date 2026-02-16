#include "MainWindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QStyle> 

void MainWindow::createToolbar() {
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(18, 18));

    toolbar->addAction(actionNew);
    toolbar->addAction(actionOpen);
    toolbar->addAction(actionSave);
    toolbar->addSeparator();

    toolbar->addAction(actionUndo);
    toolbar->addAction(actionRedo);
    toolbar->addSeparator();

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

    toolbar->addWidget(new QLabel(" Width: ", this));
    strokeWidthSpinBox = new QSpinBox(this);
    strokeWidthSpinBox->setRange(1, 20);
    strokeWidthSpinBox->setValue(2);
    connect(strokeWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), canvas_, &editor::Canvas::setStrokeWidth);
    toolbar->addWidget(strokeWidthSpinBox);
    toolbar->addSeparator();

    toolbar->addWidget(new QLabel(" Font: ", this));
    fontSizeSpinBox = new QSpinBox(this);
    fontSizeSpinBox->setRange(6, 100); 
    fontSizeSpinBox->setValue(12);  
    connect(fontSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), canvas_, &editor::Canvas::setFontSize);
    toolbar->addWidget(fontSizeSpinBox);
    toolbar->addSeparator();
    
    strokeColorBtn = new QToolButton(this);
    strokeColorBtn->setText("Stroke"); 
    strokeColorBtn->setStyleSheet("background-color: black; color: white; border: 1px solid gray; padding: 2px;");
    connect(strokeColorBtn, &QToolButton::clicked, this, &MainWindow::pickStrokeColor);
    toolbar->addWidget(strokeColorBtn);

    fillColorBtn = new QToolButton(this);
    fillColorBtn->setText(" Fill ");
    fillColorBtn->setStyleSheet("background-color: white; color: black; border: 1px solid gray; padding: 2px;");
    connect(fillColorBtn, &QToolButton::clicked, this, &MainWindow::pickFillColor);
    toolbar->addWidget(fillColorBtn);

    connect(canvas_, &editor::Canvas::selectionChanged, [this](editor::GraphicsObject* shape) {
        strokeWidthSpinBox->blockSignals(true);
        fontSizeSpinBox->blockSignals(true);

        if (!shape) {
            strokeWidthSpinBox->setEnabled(true);
            fontSizeSpinBox->setEnabled(true);
            strokeColorBtn->setEnabled(true);
            fillColorBtn->setEnabled(true);

            QColor sColor = canvas_->getCurrentStrokeColor();
            QColor fColor = canvas_->getCurrentFillColor();
            int sWidth = canvas_->getCurrentStrokeWidth();
            int fontSize = canvas_->getCurrentFontSize();

            strokeColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(sColor.name()));
            strokeColorBtn->setStyleSheet(strokeColorBtn->styleSheet() + 
                QString("color: %1;").arg(sColor.lightness() > 128 ? "black" : "white"));

            fillColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(fColor.name()));
            fillColorBtn->setStyleSheet(fillColorBtn->styleSheet() + 
                QString("color: %1;").arg(fColor.lightness() > 128 ? "black" : "white"));

            strokeWidthSpinBox->setValue(sWidth);
            fontSizeSpinBox->setValue(fontSize);
        } 
        else{
            QColor sColor(shape->getStrokeColor());
            QColor fColor(shape->getFillColor());
            auto *text = dynamic_cast<editor::Text*>(shape);
            if (sColor.isValid()) {
                strokeColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(sColor.name()));
                strokeColorBtn->setStyleSheet(strokeColorBtn->styleSheet() + 
                    QString("color: %1;").arg(sColor.lightness() > 128 ? "black" : "white"));
            }
            if (fColor.isValid()) {
                fillColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(fColor.name()));
                fillColorBtn->setStyleSheet(fillColorBtn->styleSheet() + 
                    QString("color: %1;").arg(fColor.lightness() > 128 ? "black" : "white"));
            }
            if(text){
                fontSizeSpinBox->setEnabled(true);
                fontSizeSpinBox->setValue(text->getFontSize());
                strokeWidthSpinBox->setEnabled(false);
                fillColorBtn->setEnabled(false);
                fillColorBtn->setStyleSheet("background-color: lightgray; color: gray; border: 1px solid gray;");
                strokeColorBtn->setEnabled(true);
            }
            else{
                strokeWidthSpinBox->setEnabled(true);
                strokeWidthSpinBox->setValue(shape->getStrokeWidth());
                fontSizeSpinBox->setEnabled(false); 
                strokeColorBtn->setEnabled(true);
                fillColorBtn->setEnabled(true);
            }
        }
        strokeWidthSpinBox->blockSignals(false);
        fontSizeSpinBox->blockSignals(false);
    });
}