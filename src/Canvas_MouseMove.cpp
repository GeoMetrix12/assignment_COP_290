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
    void Canvas::mouseMoveEvent(QMouseEvent* event){
        QPoint current_pos = event->pos();
        if(is_dragging_ && current_handle_ != HandleType::HANDLE_NONE && selected_shape_){
            auto* rect = dynamic_cast<Rectangle*>(selected_shape_);
            auto* freehand = dynamic_cast<Freehand*>(selected_shape_);
            auto* text = dynamic_cast<Text*>(selected_shape_);
            auto* line = dynamic_cast<Line*>(selected_shape_);
            double scale = 1.0;
            if(line){
                if(current_handle_ == HandleType::HANDLE_TOP_LEFT){
                    line->setStartPoint(current_pos);
                }
                else if(current_handle_ == HandleType::HANDLE_BOTTOM_RIGHT){
                    line->setEndPoint(current_pos);
                }
                update();
                return;
            }
            else if(freehand || text){
                double current_width = 0.0;
                if(current_handle_ == HandleType::HANDLE_TOP_RIGHT || current_handle_ == HandleType::HANDLE_BOTTOM_RIGHT){
                    current_width = std::abs(current_pos.x() - original_box_.left());
                }
                else{
                    current_width = std::abs(current_pos.x() - original_box_.right());
                }
                double ratio = current_width / original_box_.width();
                if(ratio > 0.1){
                    if(freehand){
                        QRectF box = selected_shape_->boundBox();
                        double scale = (current_pos.x() - box.left()) / box.width();
                        freehand->scalePath(scale, current_handle_);
                    }
                    else if(text){
                        int newFontSize = static_cast<int>(original_font_size_ * ratio);
                        text->setFontSize(newFontSize);
                    }
                }
            }
            else if(rect){
                QRectF box = rect->boundBox();
                double x = box.x();
                double y = box.y();
                double width = box.width();
                double height = box.height();
                if(current_handle_ == HandleType::HANDLE_BOTTOM_RIGHT){
                    width = current_pos.x() - x;
                    height = current_pos.y() - y;
                }
                else if(current_handle_ == HandleType::HANDLE_TOP_LEFT){
                    width += x - current_pos.x();
                    height += y - current_pos.y();
                    x = current_pos.x();
                    y = current_pos.y();                    
                }
                else if(current_handle_ == HandleType::HANDLE_TOP_RIGHT){
                    width = current_pos.x() - x;
                    height += y - current_pos.y();
                    y = current_pos.y();
                }
                else if(current_handle_ == HandleType::HANDLE_BOTTOM_LEFT){
                    width += x - current_pos.x();
                    height = current_pos.y() - y;
                    x = current_pos.x();
                }
                if(width > 5 && height > 5){
                    rect->setDimensions(x, y, width, height);
                }
            }
            else if(current_handle_ == HandleType::HANDLE_RADIAL){
                double r = QLineF(current_pos, QPointF(selected_shape_->boundBox().center())).length();
                auto* circle = dynamic_cast<Circle*>(selected_shape_);
                auto* hexagon = dynamic_cast<Hexagon*>(selected_shape_);
                if(circle){
                    circle->setRadius(r);
                }
                else if(hexagon){
                    hexagon->setRadius(r);
                }
            }
            notifySelectionUpdate();
            update();
            return;
        }
        if(is_dragging_ && current_tool_ != ToolType::SELECTION){
            if(current_tool_ == ToolType::LINE){
                auto* line = dynamic_cast<Line*>(temp_shape_.get());
                if(line){
                    line->setEndPoint(current_pos);
                }
            }
            else if(current_tool_ == ToolType::FREEHAND){
                auto* freehand = dynamic_cast<Freehand*>(temp_shape_.get());
                if(freehand){
                    freehand->addPoint(current_pos);
                }
            }
            else{
                double x = std::min((double)drag_start_pos_.x(), (double)current_pos.x());
                double y = std::min((double)drag_start_pos_.y(), (double)current_pos.y());
                double width = std::abs(current_pos.x() - drag_start_pos_.x());
                double height = std::abs(current_pos.y() - drag_start_pos_.y());
                if(current_tool_ == ToolType::RECTANGLE){
                    temp_shape_ = std::make_unique<Rectangle>(x, y, width, height, current_stroke_width_, current_color_.name(), current_fill_color_.name());
                }
                else if(current_tool_ == ToolType::HEXAGON){
                    double r = std::max(width, height)/2.0;
                    temp_shape_ = std::make_unique<Hexagon>(x + width/2.0, y + height/2.0, r, current_stroke_width_, current_color_.name(), current_fill_color_.name());
                }
                else if(current_tool_ == ToolType::ROUNDED_RECTANGLE){
                    temp_shape_ = std::make_unique<RoundedRectangle>(x, y, width, height, 20.0, current_stroke_width_, current_color_.name(), current_fill_color_.name());
                }
                else if(current_tool_ == ToolType::CIRCLE){
                    double r = std::max(width, height)/2.0;
                    temp_shape_ = std::make_unique<Circle>(x + r, y + r, r, current_stroke_width_, current_color_.name(), current_fill_color_.name());
                }
            }
        }
        else if(is_changing_radius_ && selected_shape_){
            auto *roundRect = dynamic_cast<RoundedRectangle*>(selected_shape_);
            if(roundRect){
                double dx = event->x() - roundRect->getX();
                double dy = event->y() - roundRect->getY();
                double new_radius = (dx + dy) / 2.0;
                if(new_radius < 0) new_radius = 0;
                if(new_radius > std::min(roundRect->getWidth(), roundRect->getHeight()) / 2.0) {
                    new_radius = std::min(roundRect->getWidth(), roundRect->getHeight()) / 2.0;
                }
                roundRect->setCornerRadius(new_radius);
            }
        }
        else if(is_dragging_ && selected_shape_ && current_tool_ == ToolType::SELECTION){
            QPoint delta = current_pos - last_pos_;
            selected_shape_->moveBy(delta);
            last_pos_ = current_pos;
        }
        update();
    }
}