#include <iostream>
#include <vector>
#include "SlidingWindow.h"

SlidingWindow::SlidingWindow(int max) {
    setMax(max);
}

void SlidingWindow::setWinSize(int wSize){
    // Invalid
    if (windowSize > windowMax) {
        std::cout << "Size cannot be greater than max window" << std::endl;
        return;
    }
    /*Expanding Window 
    for (int i = window.size(); i < wSize; ++i) {
        window.push_back(i);
        unacknowledgedNums.push_back(i % windowMax);
    }
    //Note: Constricting window check later
    */
    windowSize = wSize;
}

void SlidingWindow::setMax(int max){
    windowMax = max;
}
bool SlidingWindow::canAddNew() {
    if (windowSize < windowMax) {
        return true;
    }
    // Size and max are the same
    for (int i = 0; i < acknowledgedNums.size(); i++) {
        if (acknowledgedNums[i] == window[0]) {
            return true;
        }
    }
    return false;
}

int SlidingWindow::addNew() {
    if (canAddNew()) {
        int newSN = (lastack + 1) % windowMax;
        if (windowSize < windowMax) {
            window.push_back(newSN);
            unacknowledgedNums.push_back(newSN);
            windowSize += 1;
        } else {
            window.erase(window.begin());
            acknowledgedNums.erase(acknowledgedNums.begin());
            window.push_back(newSN);
            unacknowledgedNums.push_back(newSN);

        }
        
        return newSN;
    }
    return -1;
}

int SlidingWindow::nrSeqInWin() {
    return unacknowledgedNums.size();
}

void SlidingWindow::acknowledge(int sequence) {
    // Adds acknowledged sequence number to the acknowledged nums vector.
    acknowledgedNums.push_back(sequence);
    lastack = sequence;
    // Loop that removes sequence number from the unacknowledged frames if it exists there.
    for (int i = 0; i < unacknowledgedNums.size(); i++) {
        if (sequence == unacknowledgedNums[i]) {
            unacknowledgedNums.erase(unacknowledgedNums.begin() + (i - 1));
        }
    }
}
