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
            notifySelectionUpdate();
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
            notifySelectionUpdate();
            update();
            qDebug() << "Redo performed.";
        }
    }
}