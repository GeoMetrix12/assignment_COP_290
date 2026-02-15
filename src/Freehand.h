#ifndef FREEHAND_H_
#define FREEHAND_H_

#include <QPolygonF>
#include "GraphicsObjects.h"
#include "Constants.h"

namespace editor{
    class Freehand : public GraphicsObject{
        public:
            Freehand(const QPointF& start, int width, const QString& color);
            void Draw(QPainter* painter) override;
            bool containsPoint(const QPoint& point) const override;
            void moveBy(const QPoint& delta) override;
            QRectF boundBox() const override;
            void addPoint(const QPoint& point);
            QPolygonF getPath() const { return path_; }
            int getWidth() const { return width_; }
            QString getColor() const { return color_; }
            std::unique_ptr<GraphicsObject> cloneShape() const override;
            void setStrokeWidth(int width) override;
            void setStrokeColor(const QString& color) override;
            void setFillColor(const QString& color) override;
            void scalePath(double scaleFactor, HandleType handle);
            int getStrokeWidth() const override { return width_; }
            QString getStrokeColor() const override { return color_; }
            QString getFillColor() const override { return color_; }
            QString SvgConvert() const override;
        private:
            int width_;
            QPolygonF path_;
            QString color_;
    };
}

#endif