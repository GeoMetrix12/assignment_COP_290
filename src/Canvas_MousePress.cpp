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
    void Canvas::mousePressEvent(QMouseEvent* event){
        last_pos_ = event->pos();
        drag_start_pos_ = event->pos();
        drag_last_pos_ = event->pos();
        QPoint pos = event->pos();
        current_handle_ = HandleType::HANDLE_NONE;
        if(selected_shape_ && current_tool_ == ToolType::SELECTION){
            QRectF box = selected_shape_->boundBox();
            QPoint pos = event->pos();
            int tolerance = 10;
            auto* rect = dynamic_cast<Rectangle*>(selected_shape_);
            auto* freehand = dynamic_cast<Freehand*>(selected_shape_);
            auto* text = dynamic_cast<Text*>(selected_shape_);
            auto* line = dynamic_cast<Line*>(selected_shape_);
            if(line){
                QRectF box = line->boundBox();
                if(QLineF(pos, line->getP1()).length() < tolerance){
                    current_handle_ = HandleType::HANDLE_TOP_LEFT;
                }
                else if(QLineF(pos, line->getP2()).length() < tolerance){
                    current_handle_ = HandleType::HANDLE_BOTTOM_RIGHT;
                }
            }
            else if(text){
                if(QLineF(pos, box.topRight()).length() < tolerance){
                    current_handle_ = HandleType::HANDLE_TOP_RIGHT;
                }
            }
            else if(rect || freehand){
                if(QLineF(pos, QPointF(box.left(), box.top())).length() < tolerance){
                    current_handle_ = HandleType::HANDLE_TOP_LEFT;
                }
                else if(QLineF(pos, QPointF(box.right(), box.top())).length() < tolerance){
                    current_handle_ = HandleType::HANDLE_TOP_RIGHT;
                }
                else if(QLineF(pos, QPointF(box.left(), box.bottom())).length() < tolerance){
                    current_handle_ = HandleType::HANDLE_BOTTOM_LEFT;
                }
                else if(QLineF(pos, QPointF(box.right(), box.bottom())).length() < tolerance){
                    current_handle_ = HandleType::HANDLE_BOTTOM_RIGHT;
                }
            }
            else{
                QPointF radialPos (box.right(), box.top() + box.height()/2);
                if(QLineF(pos, radialPos).length() < tolerance){
                    current_handle_ = HandleType::HANDLE_RADIAL;
                }
            }
            if(current_handle_ != HandleType::HANDLE_NONE){
                original_box_ = selected_shape_->boundBox();
                if(text){
                    original_font_size_ = text->getFontSize();
                }
                else if(line){
                    original_p1_ = line->getP1();
                    original_p2_ = line->getP2();
                }
                is_dragging_ = true;
                return;
            }
        }
        if(current_tool_ == ToolType::TEXT){
            bool check;
            QString text = QInputDialog::getText(this, "Text Tool", "Enter text:", QLineEdit::Normal, "", &check);
            if(check && !text.isEmpty()){
                addShape(std::make_unique<Text>(last_pos_.x(), last_pos_.y(), text, current_font_size_, current_color_.name()));
            }
            return;
        }
        else if(current_tool_ == ToolType::FREEHAND){
            temp_shape_ = std::make_unique<Freehand>(pos, current_stroke_width_, current_color_.name());
            is_dragging_ = true;
        }
        else if(current_tool_ == ToolType::LINE){
            temp_shape_ = std::make_unique<Line>(pos, pos, current_stroke_width_, current_color_.name());
            is_dragging_ = true;
        }
        else if(current_tool_ == ToolType::SELECTION){
            if(selected_shape_){
                auto* roundRect = dynamic_cast<RoundedRectangle*>(selected_shape_);
                if(roundRect){
                    double handleX = roundRect->getX() + roundRect->getRadius();
                    double handleY = roundRect->getY() + roundRect->getRadius();
                    double dist = std::sqrt(std::pow(event->pos().x() - handleX, 2) + std::pow(event->pos().y() - handleY, 2));
                    if(dist < 15){
                        original_radius_ = roundRect->getRadius();
                        is_changing_radius_ = true;
                        is_dragging_ = false;
                        return;
                    }
                } 
            }
            GraphicsObject* clicked = nullptr;
            for (int i = shapes_.size() - 1; i >= 0; --i){
                if(shapes_[i]->containsPoint(last_pos_)){
                    clicked = shapes_[i].get();
                    break;
                }
            }
            if(clicked){
                selected_shape_ = clicked;
                is_dragging_ = true;
            }
            else{
                selected_shape_ = nullptr;
                is_dragging_ = false;
            }
        }
        else{
            selected_shape_ = nullptr;
            is_dragging_ = true;
        }
        notifySelectionUpdate();
        update();
    }
}