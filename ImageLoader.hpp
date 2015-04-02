#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <string>

#include "Util/FlexGrid.hpp"
#include "Util/Optional.hpp"

class ImageLoader {
    std::string header;
    int greyScale;

    int rowCount;
    int colCount;
    FlexGrid<int> iData;
public:
    ImageLoader();

    FlexGrid<int> getGrid() const;
    void setGrid(const FlexGrid<int>&);

    bool loadFile(const std::string&);
    bool exportFile(const std::string&) const;
private:
    bool isComment(const std::string&);

    bool parseHeader(std::ifstream&);
    bool parseBody(std::ifstream&);

    bool exportHeader(std::ofstream&, const std::string&) const;
    bool exportBody(std::ofstream&) const;
};
#endif
