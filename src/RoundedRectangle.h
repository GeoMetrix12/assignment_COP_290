#ifndef ROUNDED_RECTANGLE_H_
#define ROUNDED_RECTANGLE_H_

#include "Rectangle.h"

namespace editor{
    class RoundedRectangle : public Rectangle{
        public :
            RoundedRectangle(double x, double y, double width, double height, double radius, double stroke_width, QString stroke, QString fill);
            QString SvgConvert() const override;
            void Draw(QPainter* painter) override;
            bool containsPoint(const QPoint& point) const override;
            double getRadius() const { return radius_; }
            void setCornerRadius(double radius) override { radius_ = radius; }
            QRectF boundBox() const override { return QRectF(x_, y_, width_, height_); }
            std::unique_ptr<GraphicsObject> cloneShape() const override {
                return std::make_unique<RoundedRectangle>(*this);
            }
        private :
            double radius_;
    };
}
#endif