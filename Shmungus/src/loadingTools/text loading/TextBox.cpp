#include <sepch.h>

#include "TextBox.h"
#include "ShmingoApp.h"


std::string removeFirstChar(const std::string& input);

TextBox::TextBox(std::string text, vec2 position, ivec2 size, GLuint fontSize, GLuint lineSpacing) : position(position), size(size), lineSpacing(lineSpacing), fontSize(fontSize), text(text) {

    setTextBufferSize(); //Sets the size of the text buffer
}

void TextBox::setTextBufferSize(){

    size_t size = text.size();

    int skipAmt = 0;

    if (text.substr(0, 3) == "§§§") { //Checks for deault color
		skipAmt += 4;
        defaultColor = se_application.getTextColor(text[3]);
		text = text.substr(4, text.size() - 3);
	}
    std::string::iterator c;

    for (c = text.begin(); c != text.end(); c++) {
        if (*c == ' ' || *c == '\n') {
            skipAmt++;
        }
        else if (*c == '§') {
            if (*(c + 1) == '§') {
                skipAmt += 3;
                c += 2;
            }
            else {
                skipAmt += 2;
                c++;
            }
        }
    }

    textBufferSize = size - skipAmt;
}

DynamicTextBox::DynamicTextBox(std::string text, vec2 position, vec2 size, GLuint fontSize, GLuint lineSpacing, uint8_t maxDynamicTextSize)
    : TextBox(text, position, size, fontSize, lineSpacing), maxDynamicTextSize(maxDynamicTextSize){

    parseText(); //Populates sections
    setAllOffsets(); //Sets offsets for each section
    setTextBufferSize(); //Sets the size of the text buffer again using the correct constructor

    for (int i = 0; i < sections.size(); i ++) {
        if (!isSectionDynamic(i)) {
			firstDynamicSectionIndex += getSectionBufferOffset(i);
		}
        else {
            break;
        }
    }
}


void DynamicTextBox::updateDynamicText(){
    for(int i = 0; i < sections.size(); i++) {
        if(isSectionDynamic(i)) {
			sections[i] = se_getApplicationInfo(sections[i].substr(1, sections[i].size() - 1));
		}
        //TODO update in vertex array and rasterize and all of that
	}
}

std::string DynamicTextBox::compileText(){
    std::string out;

    for (int i = 0; i < sections.size(); i++) {

        std::string section = sections[i];

        if (isSectionDynamic(i)) {
            out += se_application.getApplicationInfo(removeFirstChar(section));
        }
        else {
            out += section;
        }
    }
    return out;
}

std::string DynamicTextBox::compileTextAfterFirstDynamicSection(){
    std::string out;
    bool pastFirstDynamicSection = false;

    for (int i = 0; i < sections.size(); i++) {

        std::string section = sections[i];

        if (isSectionDynamic(i)) {
            out += se_application.getApplicationInfo(removeFirstChar(section));
            pastFirstDynamicSection = true;
        }
        else {
            if (pastFirstDynamicSection) {
				out += section;
			}
        }
    }
    return out;
}

std::string DynamicTextBox::compileSection(GLuint sectionIndex){
    if (isSectionDynamic(sectionIndex)) {
		return se_application.getApplicationInfo(removeFirstChar(sections[sectionIndex])); //If dynamic returns the dynamic text
	}
    return sections[sectionIndex]; //Default case returns the section
}

void DynamicTextBox::setAllOffsets(){

    size_t totalOffset = 0;

    sectionBufferOffsets.emplace_back(0);

    for (int i = 0; i < sections.size(); i++) {

        std::string sectionText = sections[i];

        size_t sectionSize = getSectionSize(i);
        totalOffset += sectionSize;

        sectionBufferOffsets.emplace_back(totalOffset);

    }
}



std::string DynamicTextBox::getText(){
    return compileText();
}

size_t DynamicTextBox::getSectionSize(GLuint index){

    std::string section = compileSection(index);

    size_t size = section.size();

    int skipAmt = 0;

    std::string::iterator c;

    if (isSectionDynamic(index)) {
        skipAmt++; //Account for ~ 
	}

    for (c = section.begin(); c != section.end(); c++) {
        if (*c == ' ' || *c == '\n') {
            skipAmt++;
        }
        else if (*c == '§') {
            if (*(c + 1) == '§') {
                skipAmt += 3;
                c += 2;
            }
            else {
                skipAmt += 2;
                c++;
            }
        }
    }

    return size - skipAmt;
}

uint8_t DynamicTextBox::getDynamicSectionDefaultColor(GLuint index){
    if (dynamicSectionColors.find(index) == dynamicSectionColors.end()) {
        return defaultColor;
    }
    else {
        return dynamicSectionColors[index];
    }
}

void DynamicTextBox::setTextBufferSize(){
    size_t totalSize = 0;

    for (int i = 0; i < sections.size(); i++) {
        if (isSectionDynamic(i)) {
            totalSize += maxDynamicTextSize + 1; //Setting to allocated size to properly reflect dynamic text box size in buffer (Dont know why we need the +1 but it is wrong without it)
            if (sections[i].substr(1, 2) == "§§") {
                dynamicSectionColors.emplace(std::make_pair(i,se_application.getTextColor(sections[i][3]))); //Emplace color code
                sections[i] = '~' + sections[i].substr(4, sections[i].size() - 4); //Remove color code from section
            }
        }
        else {
            totalSize += getSectionSize(i);
        }
    }
    textBufferSize = totalSize;
}

void DynamicTextBox::parseText(){

    size_t pos = 0;
    size_t lastPos = 0;

    while (pos < text.size()) {
        while (text[pos] != '~' && pos < text.size()) {
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
        }
        std::string key = text.substr(start, pos - start);

        if (pos == start) {
            break;
        }

        sections.push_back(text.substr(start, pos - start));

        lastPos = pos;
    }
}

bool DynamicTextBox::isSectionDynamic(GLuint index){
    std::string section = sections[index];
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