#include <sepch.h>

#include "TextBox.h"
#include "ShmingoApp.h"


std::string removeFirstChar(const std::string& input);

TextBox::TextBox(std::string text, vec2 position, ivec2 size, unsigned int fontSize, unsigned int lineSpacing) : position(position), size(size), lineSpacing(lineSpacing), fontSize(fontSize), text(text) {

}

DynamicTextBox::DynamicTextBox(std::string text, ivec2 position, ivec2 size, unsigned int fontSize, unsigned int lineSpacing) : TextBox(text, position, size, fontSize, lineSpacing){
    parseText(text);
}


void DynamicTextBox::updateDynamicText(){
    for(int i = 0; i < sections.size(); i++) {
        if(isSectionDynamic(sections[i])) {
			sections[i] = se_getApplicationInfo(sections[i].substr(1, sections[i].size() - 1));
		}
        //TODO update in vertex array and rasterize and all of that
	}
}

std::string DynamicTextBox::compileText(){
    std::string out;
    for (const auto& section : sections) {
        if (isSectionDynamic(section)) {
            out += se_application.getApplicationInfo(removeFirstChar(section));
        }
        else {
            out += section;
        }
    }
    return out;
}

std::string DynamicTextBox::getText(){
    return compileText();
}

void DynamicTextBox::parseText(std::string text){
    size_t pos = 0;
    size_t lastPos = 0;

    int spacesAmt = 0;
    size_t currentRealOffset = 0;
    size_t currentOffsetNoSpaces = 0;

    while (pos < text.size()) {
        while (text[pos] != '~' && pos < text.size()) {
            if (text[pos] == ' ') {
                spacesAmt++;
            }
            pos++;
        }

        // Collect characters until the next space or end of string
        size_t start = pos;

        while (pos < text.size() && text[pos] != ' ') {
            pos++;

        }

        // Add the collected substring to the result vector
        if (start - lastPos != 1 || text[lastPos] == '.') {
            sections.push_back(text.substr(lastPos, start - lastPos));
            sectionOffsets.push_back(currentRealOffset);
            sectionOffsetsNoSpaces.push_back(currentOffsetNoSpaces);
        }
        std::string key = text.substr(start, pos - start);

        if (pos == start) {
            break;
        }

        sections.push_back(text.substr(start, pos - start));
        sectionOffsets.push_back(start);
        sectionOffsetsNoSpaces.push_back(start - spacesAmt);

        currentRealOffset = pos;
        currentOffsetNoSpaces = pos - spacesAmt;

        lastPos = pos;
    }
}

bool DynamicTextBox::isSectionDynamic(std::string section){
    if (section[0] == '~') {
        return true;
    }
    return false;
}


std::string removeFirstChar(const std::string& input) {
    // Check if the string is not empty

    std::string out;
    if (input.empty()) {
        return "";
    }
    // Return the substring starting from the second character
    out = input.substr(1);
    return out;
}