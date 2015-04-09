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

    void loadFile(const std::string&);
    void exportFile(const std::string&) const;
private:
    bool isComment(const std::string&);

    void parseHeader(std::ifstream&);
    void parseBody(std::ifstream&);

    void exportHeader(std::ofstream&, const std::string&) const;
    void exportBody(std::ofstream&) const;
};
#endif
