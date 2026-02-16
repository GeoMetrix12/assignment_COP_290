#include "Commands.h"
#include "GraphicsObjects.h"
#include "Line.h"
#include "Rectangle.h"
#include "Freehand.h"
#include "Text.h"
#include "RoundedRectangle.h"
#include "Hexagon.h"
#include "Circle.h"
#include <algorithm>
#include <QDebug>

namespace editor{
    ResizeCommand::ResizeCommand(GraphicsObject* shape, const QRectF& originalBox, const QRectF& newBox)
        :   shape_(shape), 
            is_line_(false),
            original_box_(originalBox), 
            new_box_(newBox) {}
    ResizeCommand::ResizeCommand(GraphicsObject* shape, const QPointF& originalP1, const QPointF& originalP2, const QPointF& newP1, const QPointF& newP2)
        :   shape_(shape), 
            is_line_(true),
            original_p1_(originalP1), 
            original_p2_(originalP2), 
            new_p1_(newP1), 
            new_p2_(newP2) {}
    void ResizeCommand::execute(){
        applyState(true);
    }
    void ResizeCommand::undo(){
        applyState(false);
    }
    void ResizeCommand::applyState(bool forward){
        if(!shape_) return;
        if(is_line_){
            auto* line = dynamic_cast<Line*>(shape_);
            if(line){
                line->setStartPoint(forward ? new_p1_ : original_p1_);
                line->setEndPoint(forward ? new_p2_ : original_p2_);
            }
        }
        else{
            QRectF targetBox = forward ? new_box_ : original_box_;
            auto* rect = dynamic_cast<Rectangle*>(shape_);
            auto* freehand = dynamic_cast<Freehand*>(shape_);
            auto* text = dynamic_cast<Text*>(shape_);
            auto* roundRect = dynamic_cast<RoundedRectangle*>(shape_);
            auto* hexagon = dynamic_cast<Hexagon*>(shape_);
            auto* circle = dynamic_cast<Circle*>(shape_);
            if(rect){
                rect->setDimensions(targetBox.x(), targetBox.y(), targetBox.width(), targetBox.height());
            }
            else if(roundRect){
                roundRect->setDimensions(targetBox.x(), targetBox.y(), targetBox.width(), targetBox.height());
            }
            else if(circle){
                double newRadius = targetBox.height() / 2.0;
                circle->setRadius(newRadius);
                circle->setCenter(targetBox.center());
            }
            else if(hexagon){
                double newRadius = targetBox.height() / 2.0;
                hexagon->setRadius(newRadius);
                hexagon->setCenter(targetBox.center());
            }
            else if(freehand){
                QRectF currentBox = freehand->boundBox();
                if(currentBox.width() > 0 && currentBox.height() > 0){
                    double scaleX = targetBox.width() / currentBox.width();
                    double scaleY = targetBox.height() / currentBox.height();
                    QPointF delta = targetBox.topLeft() - currentBox.topLeft();
                    freehand->moveBy(delta.toPoint());
                    freehand->scalePath(std::min(scaleX, scaleY), HandleType::HANDLE_BOTTOM_RIGHT);
                }
            }
            else if(text){
                QPointF delta = targetBox.topLeft() - text->boundBox().topLeft();
                text->moveBy(delta.toPoint());
            }
        }
    }
}