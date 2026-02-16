#include "MainWindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QStyle> 

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
        strokeColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(color.name()));
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

void MainWindow::updatePropertyUI(int width, int fontSize, const QColor& stroke, const QColor& fill) {
    if(strokeWidthSpinBox) strokeWidthSpinBox->blockSignals(true);
    if(fontSizeSpinBox) fontSizeSpinBox->blockSignals(true);
    
    if(strokeWidthSpinBox) strokeWidthSpinBox->setValue(width);
    if(fontSizeSpinBox) fontSizeSpinBox->setValue(fontSize);
    
    if(strokeColorBtn) {
        strokeColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(stroke.name()));
        strokeColorBtn->setStyleSheet(strokeColorBtn->styleSheet() + 
            QString("color: %1;").arg(stroke.lightness() > 128 ? "black" : "white"));
    }
    if(fillColorBtn) {
        fillColorBtn->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(fill.name()));
        fillColorBtn->setStyleSheet(fillColorBtn->styleSheet() + 
            QString("color: %1;").arg(fill.lightness() > 128 ? "black" : "white"));
    }

    if(strokeWidthSpinBox) strokeWidthSpinBox->blockSignals(false);
    if(fontSizeSpinBox) fontSizeSpinBox->blockSignals(false);
}