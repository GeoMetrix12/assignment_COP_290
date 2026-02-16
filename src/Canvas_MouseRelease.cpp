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
    void Canvas::mouseReleaseEvent(QMouseEvent* event){
        if(temp_shape_ && current_tool_ != ToolType::SELECTION){
            auto cmd = std::make_unique<AddShapeCommand>(shapes_, std::move(temp_shape_));
            pushCommand(std::move(cmd));
            temp_shape_ = nullptr;
            qDebug() << "Shape added to canvas.";
        }
        else if(selected_shape_ && is_dragging_ && current_handle_ != HandleType::HANDLE_NONE){
            auto* text = dynamic_cast<Text*>(selected_shape_);
            if(text){
                int currentSize = text->getFontSize();
                if(currentSize != original_font_size_){
                    auto cmd = std::make_unique<PropertyChangeCommand>(
                        selected_shape_, PropertyChangeCommand::PropertyType::FONT_SIZE, original_font_size_, currentSize);
                    undo_stack_.push(std::move(cmd));
                    while(!redo_stack_.empty()){
                        redo_stack_.pop();
                    }
                }
            }
            else{
                std::unique_ptr<ResizeCommand> cmd;
                bool changed = false;
                auto *line = dynamic_cast<Line*>(selected_shape_);
                if(line){
                    if(line->getP1() != original_p1_ || line->getP2() != original_p2_){
                        changed = true;
                        cmd = std::make_unique<ResizeCommand>(selected_shape_, original_p1_, original_p2_, line->getP1(), line->getP2());
                    }
                }
                else{
                    QRectF newBox = selected_shape_->boundBox();
                    if(newBox != original_box_){
                        changed = true;
                        cmd = std::make_unique<ResizeCommand>(selected_shape_, original_box_, newBox);
                    }
                }
                if(changed && cmd){
                    undo_stack_.push(std::move(cmd));
                    while(!redo_stack_.empty()){
                        redo_stack_.pop();
                    }
                }
            }
            notifySelectionUpdate();
        }
        else if(is_changing_radius_ && selected_shape_){
            auto* roundRect = dynamic_cast<RoundedRectangle*>(selected_shape_);
            if(roundRect){
                double new_radius = roundRect->getRadius();
                if(new_radius < 0) new_radius = 0;
                if(new_radius > std::min(roundRect->getWidth(), roundRect->getHeight()) / 2.0) {
                    new_radius = std::min(roundRect->getWidth(), roundRect->getHeight()) / 2.0;
                }
                if(std::abs(original_radius_ - new_radius) > 0.1){
                    auto cmd = std::make_unique<PropertyChangeCommand>(
                        selected_shape_, PropertyChangeCommand::PropertyType::CORNER_RADIUS, original_radius_, new_radius);
                    undo_stack_.push(std::move(cmd));
                    while(!redo_stack_.empty()){
                        redo_stack_.pop();
                    }
                }
            }
        }
        else if(selected_shape_ && is_dragging_ && current_tool_ == ToolType::SELECTION){
            QPoint total_delta = event->pos() - drag_start_pos_;
            if(total_delta.manhattanLength() > 2){
                selected_shape_->moveBy(-total_delta);
                auto cmd = std::make_unique<MoveShapeCommand>(selected_shape_, total_delta);
                pushCommand(std::move(cmd));
                qDebug() << "Shape moved by (" << total_delta.x() << ", " << total_delta.y() << ").";
            }
        }
        current_handle_ = HandleType::HANDLE_NONE;
        is_dragging_ = false;
        is_changing_radius_ = false;
        update();
    }
}