#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QString>
#include <QPainter>
#include <QPoint>

namespace editor{
    class GraphicsObject{ 
        public:
            virtual QString SvgConvert() const = 0;
            virtual void Draw(QPainter* painter) = 0;
            virtual bool containsPoint(const QPoint& point) const = 0;
            // virtual bool isInMoveZone(const QPoint& point) const = 0;
            virtual void moveBy(const QPoint& delta) = 0;
            virtual void setCornerRadius(double radius) {}
            virtual QRectF boundBox() const = 0;
            virtual ~GraphicsObject() = default;
            virtual std::unique_ptr<GraphicsObject> cloneShape() const = 0;
            virtual void setStrokeWidth(int width) = 0;
            virtual void setStrokeColor(const QString& color) = 0;
            virtual void setFillColor(const QString& color) = 0;
            virtual int getStrokeWidth() const = 0;
            virtual QString getStrokeColor() const = 0;
            virtual QString getFillColor() const = 0;
        protected : 
            GraphicsObject(double width, QString stroke, QString fill);
            double stroke_width_;
            QString stroke_color_;
            QString fill_color_;
            bool is_changing_radius_ = false;
    };
}
#endif