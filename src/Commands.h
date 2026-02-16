#ifndef COMMANDS_H_
#define COMMANDS_H_
#include "Command.h"
#include <vector>
#include <memory>
#include <QPoint>

namespace editor{
    class GraphicsObject; 
    class Line;
    class Rectangle;
    class Freehand;
    class Text;
    class AddShapeCommand : public Command{
        public:
            AddShapeCommand(std::vector<std::unique_ptr<GraphicsObject>>& shapes, std::unique_ptr<GraphicsObject> shape);
            void execute() override;
            void undo() override;
        private:
            std::vector<std::unique_ptr<GraphicsObject>>& shapes_;
            std::unique_ptr<GraphicsObject> shape_;
            GraphicsObject* pntr;
    };
    class MoveShapeCommand : public Command{
        public:
            MoveShapeCommand(GraphicsObject* shape, const QPoint& delta);
            void execute() override;
            void undo() override;
        private:
            GraphicsObject* shape_;
            QPoint delta_;
    };
    class ResizeCommand : public Command{
        public:
            ResizeCommand(GraphicsObject* shape, const QRectF& originalBox, const QRectF& newBox);
            ResizeCommand(GraphicsObject* shape, const QPointF& originalP1, const QPointF& originalP2, const QPointF& newP1, const QPointF& newP2);
            void execute() override;
            void undo() override;
        private:
            void applyState(bool forward);
            bool is_line_;
            GraphicsObject* shape_;
            QRectF original_box_;
            QRectF new_box_;
            QPointF original_p1_;
            QPointF original_p2_;
            QPointF new_p1_;
            QPointF new_p2_;
    };
    class DeleteShapeCommand : public Command{
        public:
            DeleteShapeCommand(std::vector<std::unique_ptr<GraphicsObject>>& shapes, GraphicsObject* shape);
            void execute() override;
            void undo() override;
        private:
            std::vector<std::unique_ptr<GraphicsObject>>& shapes_;
            std::unique_ptr<GraphicsObject> removed_shape_;
            GraphicsObject* pntr_;
            int index_;
    };
}

#endif 