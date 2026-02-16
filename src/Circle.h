#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "GraphicsObjects.h"

namespace editor {
    class Circle : public GraphicsObject {
        public:
          Circle(double cx, double cy, double r, double width, QString stroke, QString fill);
          QString SvgConvert() const override;
          void Draw(QPainter* painter) override;
          bool containsPoint(const QPoint& point) const override;
          void moveBy(const QPoint& delta) override;
          void setRadius(double r) { r_ = r; }
          void setCenter(const QPointF& center) { cx_ = center.x(); cy_ = center.y(); }
          void setStrokeWidth(int width) override { stroke_width_ = width; }
          void setStrokeColor(const QString& color) override { stroke_color_ = color; }
          void setFillColor(const QString& color) override { fill_color_ = color; }
          int getStrokeWidth() const override { return stroke_width_; }
          QString getStrokeColor() const override { return stroke_color_; }
          QString getFillColor() const override { return fill_color_; }
          QRectF boundBox() const override { return QRectF(cx_ - r_, cy_ - r_, 2 * r_, 2 * r_); }
          std::unique_ptr<GraphicsObject> cloneShape() const override {
              return std::make_unique<Circle>(*this);
          }
        private:
          double cx_, cy_, r_; 
    };
}
#endif