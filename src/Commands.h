#ifndef COMMANDS_H_
#define COMMANDS_H_
#include "Command.h"
#include "GraphicsObjects.h"
#include <vector>
#include <memory>
#include <QPoint>

namespace editor{
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
}

#endif 