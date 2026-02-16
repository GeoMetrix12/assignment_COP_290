#ifndef COMMAND_H_
#define COMMAND_H_

#include "GraphicsObjects.h"
#include "RoundedRectangle.h"
#include "Text.h"
#include <QVariant>

namespace editor{
    class Command{
        public:
            virtual void execute() = 0;
            virtual void undo() = 0;
            virtual ~Command() = default;
    };
    class PropertyChangeCommand : public editor::Command{
        public: 
            enum class PropertyType{
                STROKE_WIDTH, STROKE_COLOR, FILL_COLOR, CORNER_RADIUS, FONT_SIZE
            };
            PropertyChangeCommand(GraphicsObject* shape, PropertyType type, const QVariant& oldValue, const QVariant& newValue)
                : shape_(shape), type_(type), oldValue_(oldValue), newValue_(newValue) {}
            void execute(){
                applyChange(newValue_);
            }
            void undo(){
                applyChange(oldValue_);
            }
        private:
            void applyChange(const QVariant& val){
                if(type_ == PropertyType::STROKE_WIDTH){
                    shape_->setStrokeWidth(val.toInt());
                }
                else if(type_ == PropertyType::STROKE_COLOR){
                    shape_->setStrokeColor(val.toString());
                }
                else if(type_ == PropertyType::FILL_COLOR){
                    shape_->setFillColor(val.toString());
                }
                else if(type_ == PropertyType::CORNER_RADIUS){
                    auto* roundedRect = dynamic_cast<RoundedRectangle*>(shape_);
                    if(roundedRect){
                        roundedRect->setCornerRadius(val.toDouble());
                    }
                }
                else if(type_ == PropertyType::FONT_SIZE){
                    auto* text = dynamic_cast<Text*>(shape_);
                    if(text){
                        text->setFontSize(val.toInt());
                    }
                }
            }
            GraphicsObject* shape_;
            PropertyType type_;
            QVariant oldValue_;
            QVariant newValue_;
    };
}
#endif
