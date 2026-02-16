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
    AddShapeCommand::AddShapeCommand(std::vector<std::unique_ptr<GraphicsObject>>& shapes, std::unique_ptr<GraphicsObject> shape)
        :   shapes_(shapes), 
            shape_(std::move(shape)) {
                pntr = shape_.get();
            }
    void AddShapeCommand::execute(){
        if(shape_){
            shapes_.push_back(std::move(shape_));
        }
    }
    void AddShapeCommand::undo(){
        auto it = std::find_if(shapes_.begin(), shapes_.end(), [this](const std::unique_ptr<GraphicsObject>& sh){
            return sh.get() == pntr;
        });
        if(it != shapes_.end()){
            shape_ = std::move(*it);
            shapes_.erase(it);
        }
        else{
            qDebug() << "Error: Shape not found in undo operation.";
        }
    }
    MoveShapeCommand::MoveShapeCommand(GraphicsObject* shape, const QPoint& delta)
        :   shape_(shape), 
            delta_(delta) {}
    void MoveShapeCommand::execute(){
        if(shape_){
            shape_->moveBy(delta_);
        }
    }
    void MoveShapeCommand::undo(){
        if(shape_){
            shape_->moveBy(-delta_);
        }
    }
    DeleteShapeCommand::DeleteShapeCommand(std::vector<std::unique_ptr<GraphicsObject>>& shapes, GraphicsObject* shape)
        :   shapes_(shapes), 
            pntr_(shape), index_(-1){}
    void DeleteShapeCommand::execute(){
        auto it = std::find_if(shapes_.begin(), shapes_.end(), [this](const std::unique_ptr<GraphicsObject>& sh){
            return sh.get() == pntr_;
        });
        if(it != shapes_.end()){
            removed_shape_ = std::move(*it);
            index_ = std::distance(shapes_.begin(), it);
            shapes_.erase(it);
        }
    }
    void DeleteShapeCommand::undo(){
        if(removed_shape_ && index_ >= 0 && index_ <= static_cast<int>(shapes_.size())){
            shapes_.insert(shapes_.begin() + index_, std::move(removed_shape_));
        }
    }
}