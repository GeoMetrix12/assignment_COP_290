#ifndef TEXT_H_
#define TEXT_H_
#include "GraphicsObjects.h"
#include <QPainter>
#include <QFontMetrics>

namespace editor{
    class Text : public GraphicsObject{
        public:
            Text(double x, double y, const QString& content, int fontSize, const QString& color);
            void Draw(QPainter* painter) override;
            QRectF boundBox() const override;
            bool containsPoint(const QPoint& point) const override;
            void moveBy(const QPoint& delta) override;
            std::unique_ptr<GraphicsObject> cloneShape() const override;
            void setStrokeWidth(int width) override;
            void setStrokeColor(const QString& color) override;
            void setFillColor(const QString& color) override;
            double getX() const { return x_; }
            double getY() const { return y_; }
            QString getContent() const { return content_; }
            int getFontSize() const { return font_size_; }
            QString getColor() const { return color_; }
            QString SvgConvert() const override;
            int getStrokeWidth() const override { return font_size_; }
            QString getStrokeColor() const override { return color_; }
            QString getFillColor() const override { return color_; }
            void setFontSize(int size) { font_size_ = size; };
        private:
            double x_, y_;
            QString content_;
            int font_size_;
            QString color_;    
    };
}

#endif