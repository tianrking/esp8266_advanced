#ifndef HTML_COMMON_H
#define HTML_COMMON_H

#include <Arduino.h>

class HtmlCommon {
public:
    static String getHeader(const String& title);
    static String getFooter();
    static String getNavBar(const String& currentPage);
    static String getCommonScript();
};

#endif // HTML_COMMON_H