#include <sepch.h>

#include "TextBox.h"
#include "ShmingoApp.h"




void DynamicTextBox::updateDynamicText(){
    for(int i = 0; i < sections.size(); i++) {
        if(isSectionDynamic(sections[i])) {
			sections[i] = se_getApplicationInfo(sections[i].substr(1, sections[i].size() - 1));
		}
        //TODO update in vertex array and rasterize and all of that
	}
}

void DynamicTextBox::parseText(std::string text){
    size_t pos = 0;
    size_t lastPos = 0;

    while (pos < text.size()) {
        // Find the next '~' symbol
        pos = text.find('~', pos);
        if (pos == std::string::npos) {
            break; // No more '~' found, exit the loop
        }

        // Collect characters until the next space or end of string
        size_t start = pos;

        while (pos < text.size() && text[pos] != ' ' && text[pos] != '.') {
            ++pos;
        }
        // Add the collected substring to the result vector
        if (start - lastPos != 1 || text[lastPos] == '.') {
            sections.push_back(text.substr(lastPos, start - lastPos));
        }
        std::string key = text.substr(start, pos - start);
        if (se_isValidInfoKey(key)) {
            sections.push_back(text.substr(start, pos - start));
        }
        else {
            sections.push_back("KEYNOTFOUND");
        }
        lastPos = pos;
    }
}

bool DynamicTextBox::isSectionDynamic(std::string section){
    if (section[0] == '~') {
        return true;
    }
    return false;
}

TextBox::TextBox(std::string text, vec2 position, ivec2 size, unsigned int fontSize, unsigned int lineSpacing) : position(position), size(size), lineSpacing(lineSpacing), fontSize(fontSize), text(text){

    //convertTextToLines(text);
}

void TextBox::convertTextToLines(std::string text){

    char currentLine[100];
    int currentIndex = 0;
    float currentLineWidth = 0; //Current width of line

    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++) {
        Shmingo::Character currentChar = se_application.getCharacterFontInfo(*c);

        int totalCharSpace =  (currentChar.Advance >> 6) * fontSize; //Total space taken up by a character
        if (currentLineWidth + totalCharSpace > size.x || c == text.end() - 1) { //Checks if line width is greater than text box width

            if (c == text.end() - 1) { //Adds before on last line
                currentLine[currentIndex] = *c; //Add character to line
                currentLineWidth += totalCharSpace; //Add space taken up by character to line width
                currentIndex++; //Increment index
            }

            currentLine[currentIndex + 1] = '\0'; // Add terminating char

            std::string out = currentLine; //Create c++ string

            out.resize(currentIndex + 0); //Resize line to only contain up to terminal

            lines.emplace_back(out); //Add full line to lines

            currentIndex = 0; //Reset current Index
            currentLineWidth = 0;
        }

        currentLine[currentIndex] = *c; //Add character to line
        currentLineWidth += totalCharSpace; //Add space taken up by character to line width
        currentIndex++; //Increment index

    }
}
 