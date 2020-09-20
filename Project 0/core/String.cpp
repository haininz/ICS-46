
// String.cpp
//
// ICS 46 Spring 2020
// Project #0: Getting to Know the ICS 46 VM
//
// Implement all of your String member functions in this file.
//
// Note that the entire standard library -- both the C Standard
// Library and the C++ Standard Library -- is off-limits for this
// task, as the goal is to exercise your low-level implementation
// skills (pointers, memory management, and so on).

#include "String.hpp"
#include "OutOfBoundsException.hpp"
String::String()
    :chars{nullptr}
{
}

String::String(const char* chars){
    unsigned int index = 0;
    while (chars[index] != '\0'){
        index ++;
    }
    this->chars = new char[index+1];
    for (unsigned int i = 0; i < index; i++){
        this->chars[i] = chars[i];
    }
    this->chars[index] = '\0';
}

String::String(const String& s){
    unsigned int index = 0;
    while (s.chars[index] != '\0'){
        index ++;
    }
    this->chars = new char[index+1];
    for (unsigned int i = 0; i < index; i++){
        this->chars[i] = s.chars[i];
    }
    this->chars[index] = '\0';
}

String::~String() noexcept{
    if (this->chars){
        delete[] this->chars;
    }
}

String& String::operator=(const String& s){
    if (this != &s){
        this->~String();
        unsigned int index = 0;
        while (s.chars[index] != '\0'){
            index ++;
        }
        this->chars = new char[index+1];
        for (unsigned int i = 0; i < index; i++){
            this->chars[i] = s.chars[i];
        }
        this->chars[index] = '\0';
    }
    return *this;
}

void String::append(const String& s){
    char* temp = new char[this->length()+s.length()+1];
    for (unsigned int i = 0; i < this->length(); i++){
        temp[i] = this->chars[i];
    }
    unsigned int count = 0;
    for (unsigned int j = this->length(); j < this->length()+s.length()+1; j++){
        temp[j] = s.chars[count];
        count ++;
    }
    this->~String();
    this->chars = temp;
}

char String::at(unsigned int index) const{
    if (index >= this->length()){
        throw OutOfBoundsException{};
    }
    return this->chars[index];
}

char& String::at(unsigned int index){
    if (index >= this->length()){
        throw OutOfBoundsException{};
    }
    char* answer = this->chars;
    for (int i = 0; i < index; i++){
        answer++;
    }
    return *answer;
}

void String::clear(){
    this->~String();
    this->chars = nullptr;
}

int String::compareTo(const String& s) const noexcept{
    int counter = 0;
    while (true){
        if (this->chars[counter] < s.chars[counter]){
            return -1;
        }
        else if (this->chars[counter] > s.chars[counter]){
            return 1;
        }
        else if (this->chars[counter] == '\0' && s.chars[counter] == '\0'){
            return 0;
        }
        counter ++;
    }
}

String String::concatenate(const String& s) const{
    String newString(*this);
    newString.append(s);
    return newString;
}

bool String::contains(const String& substring) const noexcept{
    if (this->length() < substring.length()){
        return false;
    }
    for (unsigned int i = 0; i < this->length()-substring.length(); i++){
        if (this->substring(i, i+substring.length()).equals(substring)){
            return true;
        }
    }
    return false;
}

bool String::equals(const String& s) const noexcept{
    if (this->compareTo(s) != 0){
        return false;
    }
    return true;
}

int String::find(const String& substring) const noexcept{
    if (this->contains(substring) == true){
        for (int i = 0; i < this->length()-substring.length(); i++){
            if (this->substring(i, i+substring.length()).equals(substring)){
                return i;
            }
        }
    }
    return -1;
}

bool String::isEmpty() const noexcept{
    if (this->length() == 0){
        return true;
    }
    return false;
}

unsigned int String::length() const noexcept{
    if (this->chars){
        unsigned int length = 0;
        while (this->chars[length] != '\0'){
            length ++;
        }
        return length;
    }
    return 0;
}

String String::substring(unsigned int startIndex, unsigned int endIndex) const{
    if (startIndex >= this->length() || endIndex >= this->length()){
        throw OutOfBoundsException{};
    }
    char* subChar = new char[endIndex-startIndex+1];
    unsigned int subChar_index = 0;
    for (unsigned int i = startIndex; i < endIndex; i++){
        subChar[subChar_index] = this->chars[i];
        subChar_index ++;
    }
    subChar[subChar_index] = '\0';
    const char* answer = subChar;
    String s{answer};
    delete[] answer;
    return s;
}

const char* String::toChars() const noexcept{
    if (this->length() == 0){
        return "";
    }
    return this->chars;
}