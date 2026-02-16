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

namespace editor{
    Canvas::Canvas(QWidget* parent) : QWidget(parent){
        // last_pos_ = QPoint(0, 0);
        // drag_last_pos_ = QPoint(0, 0);
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
    }
    void Canvas::addShape(std::unique_ptr<GraphicsObject> shape) {
        auto cmd = std::make_unique<AddShapeCommand>(shapes_, std::move(shape));
        pushCommand(std::move(cmd));
        update();
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
    void Canvas::loadFromFile(const QString& filename){
        selected_shape_ = nullptr;
        is_dragging_ = false;
        is_changing_radius_ = false;
        auto new_shapes = editor::SvgParser::ParseSvg(filename);
        if(new_shapes.empty()){
            qDebug() << "Failed to load shapes from file: " << filename;
        }
        else{
            qDebug() << "Loaded " << new_shapes.size() << " shapes from file: " << filename;
        }
        shapes_ = std::move(new_shapes);
        current_file_ = filename;
        update();
    }
    bool Canvas::saveToFile(const QString& filename){
        return editor::SvgParser::ModifyFile(filename, shapes_, width(), height());
    }
    void Canvas::clearCanvas(){
        shapes_.clear();
        update();
    }
    void Canvas::copy(){
        if(selected_shape_){
            clipboard_shape_ = selected_shape_->cloneShape();
        }
    }
    void Canvas::paste(){
        if(clipboard_shape_){
            auto new_shape = clipboard_shape_->cloneShape();
            new_shape->moveBy(QPoint(20, 20));
            addShape(std::move(new_shape));
        }
    }
    void Canvas::cut(){
        if(selected_shape_){
            copy();
            auto cmd = std::make_unique<DeleteShapeCommand>(shapes_, selected_shape_);
            pushCommand(std::move(cmd));
            selected_shape_ = nullptr;
            is_changing_radius_ = false;
            is_dragging_ = false;
            update();
        }
    }
    void Canvas::pushCommand(std::unique_ptr<Command> cmd){
        cmd->execute();
        undo_stack_.push(std::move(cmd));
        while(!redo_stack_.empty()){
            redo_stack_.pop();
        }
    }
    void Canvas::undo(){
        if(!undo_stack_.empty()){
            auto cmd = std::move(undo_stack_.top());
            undo_stack_.pop();
            cmd->undo();
            redo_stack_.push(std::move(cmd));
            selected_shape_ = nullptr;
            update();
            qDebug() << "Undo performed.";
        }
    }
    void Canvas::redo(){
        if(!redo_stack_.empty()){
            auto cmd = std::move(redo_stack_.top());
            redo_stack_.pop();
            cmd->execute();
            undo_stack_.push(std::move(cmd));
            selected_shape_ = nullptr;
            update();
            qDebug() << "Redo performed.";
        }
    }
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
                addShape(std::make_unique<Text>(last_pos_.x(), last_pos_.y(), text, 12, current_color_.name()));
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
        update();
    }
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
    void Canvas::mouseReleaseEvent(QMouseEvent* event){
        if(temp_shape_ && current_tool_ != ToolType::SELECTION){
            auto cmd = std::make_unique<AddShapeCommand>(shapes_, std::move(temp_shape_));
            pushCommand(std::move(cmd));
            temp_shape_ = nullptr;
            qDebug() << "Shape added to canvas.";
        }
        else if(selected_shape_ && is_dragging_ && current_handle_ != HandleType::HANDLE_NONE){
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