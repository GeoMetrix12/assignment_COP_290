#include "Commands.h"
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
}