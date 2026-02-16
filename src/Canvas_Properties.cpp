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
    void Canvas::setStrokeWidth(int width){
        if(selected_shape_){
            int oldWidth = selected_shape_->getStrokeWidth();
            auto cmd = std::make_unique<PropertyChangeCommand>(
                selected_shape_, PropertyChangeCommand::PropertyType::STROKE_WIDTH, oldWidth, width);
            pushCommand(std::move(cmd));
        }
        current_stroke_width_ = width;
        update();
    }
    void Canvas::setStrokeColor(const QColor& color){
        if(selected_shape_){
            QString oldColor = selected_shape_->getStrokeColor();
            auto cmd = std::make_unique<PropertyChangeCommand>(
                selected_shape_, PropertyChangeCommand::PropertyType::STROKE_COLOR, oldColor, color.name());
            pushCommand(std::move(cmd));
        }
        current_color_ = color;
        update();
    }
    void Canvas::setFillColor(const QColor& color){
        if(selected_shape_){
            QString oldColor = selected_shape_->getFillColor();
            auto cmd = std::make_unique<PropertyChangeCommand>(
                selected_shape_, PropertyChangeCommand::PropertyType::FILL_COLOR, oldColor, color.name());
            pushCommand(std::move(cmd));
        }
        current_fill_color_ = color;
        update();
    }
    void Canvas::setFontSize(int size){
        if(selected_shape_){
            auto* text = dynamic_cast<Text*>(selected_shape_);
            if(text){
                int oldSize = text->getFontSize();
                auto cmd = std::make_unique<PropertyChangeCommand>(
                    selected_shape_, PropertyChangeCommand::PropertyType::FONT_SIZE, oldSize, size);
                pushCommand(std::move(cmd));
            }
        }
        current_font_size_ = size;
        update();
    }
}