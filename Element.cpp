#include "Element.h"

Element::Element(size_t ledCount, CRGB *buffer) {
    _buffer = buffer;
    _ledCount = ledCount;

    _currentChildrenLedIndex = 0;
}

size_t Element::getLedCount() {
    return _ledCount;
}

void Element::setBuffer(CRGB *buffer) {
    _buffer = buffer;

    // update every child buffer
    _currentChildrenLedIndex = 0;
    for (std::list<Element*>::iterator it = _children.begin(); it != _children.end(); it++) {
        Element *elem = *it;
        elem->setBuffer(&_buffer[_currentChildrenLedIndex]);
        _currentChildrenLedIndex += elem->getLedCount();
    }
}

CRGB* Element::getBuffer() {
    return _buffer;
}

bool Element::addChild(Element *child) {
    if (_currentChildrenLedIndex + child->getLedCount() > _ledCount) {
        return false;
    }

    if (_buffer) {
        child->setBuffer(&_buffer[_currentChildrenLedIndex]);
    }
    _children.push_back(child);

    _currentChildrenLedIndex += child->getLedCount();

    return true;
}

void Element::removeChild(Element *child) {
    _children.remove(child);
}

void Element::clearChildren() {
    _children.clear();
}

element_iterator Element::childrenBegin() {
    return _children.begin();
}

element_iterator Element::childrenEnd() {
    return _children.end();
}

void Element::setColor(CRGB color) {
    for (int i = 0; i < getLedCount(); i++) {
        _buffer[i] = color;
    }
}

void Element::setColorAt(size_t index, CRGB color) {
    _buffer[index] = color;
}

void Element::update() {
    // Place to update own effects
    updateChildren();
}

void Element::updateChildren() {
    for (element_iterator it = _children.begin(); it != _children.end(); it++) {
        Element *elem = *it;
        elem->update();
    }
}