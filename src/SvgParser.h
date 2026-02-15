#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <vector>
#include <memory>
#include <QString>
#include "GraphicsObjects.h"

namespace editor {
    class SvgParser{
        public :
            static std::vector<std::unique_ptr<GraphicsObject>> ParseSvg(const QString& filePath);
            static bool ModifyFile(const QString& filename, const std::vector<std::unique_ptr<GraphicsObject>>& shapes,
                                    int canvas_width = 800, int canvas_height = 600);
        private :
            static QString getValue (const QString& line, const QString& attribute_name);
    }; 
}
#endif