#include <sepch.h>

#include "TextBox.h"
#include "ShmingoApp.h"


std::string removeFirstChar(const std::string& input);

TextBox::TextBox(std::string text, vec2 position, vec2 size, GLuint fontSize, GLuint lineSpacing, Shmingo::TextAlignment textAlignment) 
    : position(position), size(size), lineSpacing(lineSpacing), fontSize(fontSize), text(text), textAlignment(textAlignment) {
    setTextBufferSize(); //Sets the size of the text buffer

    setLineCharOffset(0,0); //Sets the offset of the first line to 0
}

void TextBox::setLineCharOffset(size_t lineIndex, size_t offset){
    if (lineIndex < charOffsetsOfLines.size()) {
		charOffsetsOfLines[lineIndex] = offset;
	}
    else {
        charOffsetsOfLines.emplace_back(offset);
	}
}

void TextBox::setTextBufferSize(){

    size_t size = text.size();

    if (text.find('Ø') != std::string::npos) {
        se_error("Invalid character Ø detected in text box!");
        text.erase(std::remove(text.begin(), text.end(), 'Ø'), text.end());
    }

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

DynamicTextBox::DynamicTextBox(std::string text, vec2 position, vec2 size, unsigned int fontSize, unsigned int lineSpacing, uint8_t maxDynamicTextSize, Shmingo::TextAlignment alignment)
    : TextBox(text, position, size, fontSize, lineSpacing, alignment), maxDynamicTextSize(maxDynamicTextSize){

    parseText(); //Populates sections
    setAllOffsets(); //Sets offsets for each section
    setTextBufferSize(); //Sets the size of the text buffer again using the correct constructor

    if (isSectionDynamic(1)) {
		firstDynamicSectionIndex = 1;
	}

    for (std::string section : sections) {
        se_log("Section: " + section);
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

        out += compileSection(i);
    }

    return out;
}

std::string DynamicTextBox::compileSection(size_t sectionIndex){

    std::string text = sections[sectionIndex];
    if (isSectionDynamic(sectionIndex)) {
        if (text.substr(1, 2) == "§§") {
			std::string info = se_application.getApplicationInfo(text.substr(4, text.size() - 4));

            size_t neededBlanks = maxDynamicTextSize - info.size(); //Get amount of padding needed

            totalSkipAmount += neededBlanks;

            return text.substr(1, 3) + info + std::string(neededBlanks, 'Ø');
		}
        else {

            std::string info = se_application.getApplicationInfo(text);

            size_t neededBlanks = maxDynamicTextSize - info.size(); //Get amount of padding needed

            totalSkipAmount += neededBlanks;

            return se_application.getApplicationInfo(text.substr(1, text.size() - 1));
        }
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


void DynamicTextBox::setTextBufferSize(){
    size_t totalSize = 0;

    for (int i = 0; i < sections.size(); i++) {
        if (isSectionDynamic(i)) {
            totalSize += maxDynamicTextSize + 1; //Setting to allocated size to properly reflect dynamic text box size in buffer (Dont know why we need the +1 but it is wrong without it)
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

        while (pos < text.size()) {
            if (text[pos] != ' ' && text[pos] != '\n' && text[pos] != ',') {
                pos++;
            }
            else {
                break;
            }
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