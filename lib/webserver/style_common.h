#ifndef STYLE_COMMON_H
#define STYLE_COMMON_H

#include <Arduino.h>

class StyleCommon {
public:
    static String getCommonStyle();
    static String getCardStyle();
    static String getFormStyle();
    static String getButtonStyle();
};

#endif // STYLE_COMMON_H