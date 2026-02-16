#ifndef CANVAS_H_
#define CANVAS_H_

#include <QWidget>
#include <vector>
#include <memory>
#include <stack>
#include "GraphicsObjects.h"
#include "Commands.h"
#include "Constants.h"

namespace editor {
    class Canvas : public QWidget{
        Q_OBJECT 
        public :
            explicit Canvas(QWidget* parent = nullptr);
            void addShape(std::unique_ptr<GraphicsObject> shape);
            void loadFromFile (const QString& filename);
            bool saveToFile (const QString& filename);
            void clearCanvas();
            void setCurrentTool(ToolType tool){current_tool_ = tool;}
            void undo();
            void redo();
            void copy();
            void paste();
            void cut();
            void setStrokeWidth(int width);
            void setStrokeColor(const QColor& color);
            void setFillColor(const QColor& color);
            bool hasFile() const {return !current_file_.isEmpty();}
            QString getCurrentFile() const { return current_file_; }
            void setCurrentFile(const QString& filename) { current_file_ = filename; }
        protected :
            void paintEvent(QPaintEvent* event) override;
            void mousePressEvent(QMouseEvent* event) override;
            void mouseMoveEvent(QMouseEvent* event) override;
            void mouseReleaseEvent(QMouseEvent* event) override;
        private :
            std::vector<std::unique_ptr<GraphicsObject>> shapes_;
            ToolType current_tool_ = ToolType::NONE;
            QPoint last_pos_;
            QPoint drag_last_pos_;
            GraphicsObject* selected_shape_ = nullptr;
            bool is_dragging_ = false;
            bool is_changing_radius_ = false;
            std::unique_ptr<GraphicsObject> clipboard_shape_ = nullptr;
            void pushCommand(std::unique_ptr<Command> cmd);
            std::stack<std::unique_ptr<Command>> undo_stack_;
            std::stack<std::unique_ptr<Command>> redo_stack_;
            std::unique_ptr<GraphicsObject> temp_shape_ = nullptr;
            QPoint drag_start_pos_;
            int current_stroke_width_;
            QColor current_color_;
            QColor current_fill_color_;
            QString current_file_;
            HandleType current_handle_ = HandleType::HANDLE_NONE;
            double original_radius_ = 0.0;
            QRectF original_box_;
            QPointF original_p1_, original_p2_;
            int original_font_size_;
    };
}
#endif