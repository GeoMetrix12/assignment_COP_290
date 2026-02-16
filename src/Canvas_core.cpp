#include "Canvas.h"
#include "SvgParser.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Hexagon.h"
#include "RoundedRectangle.h"
#include "Text.h"
#include "Line.h"
#include "Commands.h"
#include "Freehand.h"
#include <QInputDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

namespace editor{
    Canvas::Canvas(QWidget* parent) : QWidget(parent){
        setFocusPolicy(Qt::StrongFocus);
        QPalette pal = palette();
        pal.setColor(QPalette::Base, Qt::darkGray);
        setPalette(pal);  
        setAutoFillBackground(true);
        setBackgroundRole(QPalette::Base);
        current_tool_ = ToolType::SELECTION;
        current_stroke_width_ = 2;
        current_color_ = Qt::black;
        current_fill_color_ = Qt::white;
        selected_shape_ = nullptr;
        temp_shape_ = nullptr;
        current_file_ = "";
        current_font_size_ = 12;
    }
    void Canvas::addShape(std::unique_ptr<GraphicsObject> shape) {
        auto cmd = std::make_unique<AddShapeCommand>(shapes_, std::move(shape));
        pushCommand(std::move(cmd));
        update();
    }
    void Canvas::notifySelectionUpdate(){
        emit selectionChanged(selected_shape_);
    }
    void Canvas::paintEvent(QPaintEvent* event){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);  
        for (const auto& shape : shapes_) {
            shape -> Draw(&painter);
        }
        if(selected_shape_){
            painter.setPen(QPen(Qt::magenta, 1, Qt::DashLine));
            painter.setBrush(Qt::NoBrush);
            QRectF boundingBox = selected_shape_->boundBox();
            painter.drawRect(boundingBox);
            painter.setBrush(Qt::black);
            painter.setPen(Qt::white);
            auto *rect = dynamic_cast<Rectangle*>(selected_shape_);
            auto *freehand = dynamic_cast<Freehand*>(selected_shape_);
            auto* text = dynamic_cast<Text*>(selected_shape_);
            auto* line = dynamic_cast<Line*>(selected_shape_);
            int s = 8;
            if(line){
                QRectF box = line->boundBox();
                painter.drawRect(line->getP1().x() - s/2, line->getP1().y() - s/2, s, s);
                painter.drawRect(line->getP2().x() - s/2, line->getP2().y() - s/2, s, s);
            }
            else if(text){
                painter.drawRect(boundingBox.right() - s/2, boundingBox.top() - s/2, s, s);
            }
            else if(rect || freehand){
                painter.drawRect(boundingBox.left() - s/2, boundingBox.top() - s/2, s, s);
                painter.drawRect(boundingBox.right() - s/2, boundingBox.top() - s/2, s, s);
                painter.drawRect(boundingBox.left() - s/2, boundingBox.bottom() - s/2, s, s);
                painter.drawRect(boundingBox.right() - s/2, boundingBox.bottom() - s/2, s, s);
            }
            else{
                painter.drawEllipse(QPointF(boundingBox.right(), boundingBox.center().y()), 4, 4);
            }
        }
        if(temp_shape_){
            painter.save();
            painter.setOpacity(0.5);
            temp_shape_->Draw(&painter);
            painter.restore();
        }
    }
}