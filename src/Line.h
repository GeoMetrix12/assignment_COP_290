#ifndef LINE_H_
#define LINE_H_
#include "GraphicsObjects.h"
#include "Constants.h"  

namespace editor{
    class Line : public GraphicsObject{
        public:
            Line(const QPointF& start, const QPointF& end, double width, const QString& stroke);
            void Draw(QPainter* painter) override;
            bool containsPoint(const QPoint& point) const override;
            void moveBy(const QPoint& delta) override;
            QRectF boundBox() const override;
            std::unique_ptr<GraphicsObject> cloneShape() const override;
            void setStrokeWidth(int width) override;
            void setStrokeColor(const QString& color) override;
            void setFillColor(const QString& color) override {}
            void setEndPoint(const QPointF& end) {p2_ = end;}
            void setStartPoint(const QPointF& start) {p1_ = start;}
            QPointF getP1() const { return p1_; }
            QPointF getP2() const { return p2_; }
            int getStrokeWidth() const override { return stroke_width_; }
            QString getStrokeColor() const override { return stroke_color_; }
            QString getFillColor() const override { return "none"; }
            QString SvgConvert() const override;
        private:
            QPointF p1_, p2_;
    };
}

#endif