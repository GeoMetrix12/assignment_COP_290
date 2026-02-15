#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "GraphicsObjects.h"

namespace editor {
    class Rectangle : public GraphicsObject {
        public:
          Rectangle(double x, double y, double width, double height, double stroke_width, QString stroke, QString fill);
          QString SvgConvert() const override;
          void Draw(QPainter* painter) override;
          void moveBy(const QPoint& delta) override;
          bool containsPoint(const QPoint& point) const override;
          void setDimensions(double x, double y, double width, double height) { width_ = width; height_ = height; x_ = x; y_ = y; }
          double getX() const { return x_; }
          double getY() const { return y_; }
          double getWidth() const { return width_; } 
          double getHeight() const { return height_; }
          void setStrokeWidth(int width) override { stroke_width_ = width; }
          void setStrokeColor(const QString& color) override { stroke_color_ = color; }
          void setFillColor(const QString& color) override { fill_color_ = color; }
          int getStrokeWidth() const override { return stroke_width_; }
          QString getStrokeColor() const override { return stroke_color_; }
          QString getFillColor() const override { return fill_color_; }
          QRectF boundBox() const override { return QRectF(x_, y_, width_, height_); }
          std::unique_ptr<GraphicsObject> cloneShape() const override {
              return std::make_unique<Rectangle>(*this);
          }
        protected:
          double x_, y_, width_, height_; 
    };
}
#endif