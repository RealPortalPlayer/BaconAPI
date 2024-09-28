// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/MessageBox.h"
#include "BaconAPI/OperatingSystem.h"
#include "BaconAPI/Logger.h"
#include "BaconAPI/Debugging/StaticAssert.h"
#include "BaconAPI/String.h"

#ifdef BA_X11_FOUND
#   include <X11/Xlib.h>
#   include <X11/Xutil.h>
#   include <string.h>
#   include <X11/xpm.h>

#   ifdef BA_FREETYPE_FOUND
#       include <X11/Xft/Xft.h>
#   endif

#   include "BaconAPI/Math/Bitwise.h"
#   include "Error.xpm"
#   include "Question.xpm"
#   include "Warning.xpm"
#   include "Information.xpm"
typedef struct {
    const char* text;
    BA_MessageBox_Result result;
    size_t textLength;
} BA_MessageBox_ButtonObject;

#   define BA_MESSAGEBOX_BUTTON_WIDTH 88
#   define BA_MESSAGEBOX_BUTTON_HEIGHT 26
#   define BA_MESSAGEBOX_BUTTON_BOTTOM_PADDING 9
#   define BA_MESSAGEBOX_BUTTON_BOTTOM_PADDING_NO_IMAGE_NO_LINES_ADDITION 2
#   define BA_MESSAGEBOX_BUTTON3_RIGHT_PADDING 8
#   define BA_MESSAGEBOX_BUTTON3_RIGHT_PADDING_MAX_WIDTH_AND_IMAGE_OR_HAS_SECOND_BUTTON 1
#   define BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING 8
#   define BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING_HAS_FIRST_BUTTON_ADDITION 2
#   define BA_MESSAGEBOX_BUTTON1_RIGHT_PADDING 8
#   define BA_MESSAGEBOX_BUTTON_BACKGROUND_HEIGHT 49
#   define BA_MESSAGEBOX_MAX_CHARACTERS 53
#   define BA_MESSAGEBOX_MAX_CHARACTERS_LOADED_IMAGE 47
#   define BA_MESSAGEBOX_TOP_PADDING_BASE 29
#   define BA_MESSAGEBOX_TOP_PADDING_IMAGE_SINGLE_OR_NO_LINE_ADDITION 8
#   define BA_MESSAGEBOX_BOTTOM_PADDING_BASE 23
#   define BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_SINGLE_LINE_ADDITION 13
#   define BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_TWO_LINES_ADDITION 6
#   define BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_THREE_ADDITION 4
#   define BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_ZERO_LINES_ADDITION 24
#   define BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_ONE_LINE_ADDITION 15
#   define BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_TWO_OR_MORE_LINES_ADDITION 8
#   define BA_MESSAGEBOX_BOTTOM_PADDING_SINGLE_OR_MORE_LINES_ADDITION 6
#   define BA_MESSAGEBOX_LEFT_PADDING_BASE 16
#   define BA_MESSAGEBOX_RIGHT_PADDING_BASE 125
#   define BA_MESSAGEBOX_RIGHT_PADDING_EMPTY_BOX 26
#   define BA_MESSAGEBOX_RIGHT_PADDING_IMAGE_SUBTRACTION 53
#   define BA_MESSAGEBOX_MAX_WIDTH_WITHOUT_IMAGE 474
#   define BA_MESSAGEBOX_MAX_WIDTH_WITH_IMAGE_ADDITION 6
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

BA_MessageBox_Result BA_MessageBox_Open(BA_MessageBox_Flags flags, const char* title, const char* text, ...) {
    char* formattedText = BA_String_Copy(text);
    {
        va_list arguments;

        va_start(arguments, text);
        
        formattedText = BA_String_FormatPremadeList(formattedText, arguments);
        
        va_end(arguments);
    }

    BA_MessageBox_Result result = BA_MESSAGEBOX_RESULT_OK;
    
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   ifdef BA_X11_FOUND
    // OPTIMIZE: This is a mess. Try to optimize some fat away
    // TODO: Icon?
    // TODO: Using tab to select button
    // TODO: Default button
    Display* display = XOpenDisplay(NULL);
    BA_Boolean loadedImage = BA_BOOLEAN_FALSE;

    if (display == NULL) {
        BA_LOGGER_ERROR("Failed to open display for MessageBox\n");
        free(formattedText);
        return result;
    }

    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 100, 100, 1, BlackPixel(display, screen), WhitePixel(display, screen));
    BA_DynamicArray lines;

    Pixmap pixmap;
    Pixmap shapeMask;
    XpmAttributes attributes;

    attributes.valuemask = XpmExactColors;

#define BA_MESSAGEBOX_LOAD_IMAGE_FIRST(bit, image) \
if (BA_BITWISE_IS_BIT_SET(flags, bit)) {           \
    XpmCreatePixmapFromData(display, window, image, &pixmap, &shapeMask, &attributes); \
    loadedImage = BA_BOOLEAN_TRUE;                 \
}

#define BA_MESSAGEBOX_LOAD_IMAGE_ADDITIONAL(bit, image) else BA_MESSAGEBOX_LOAD_IMAGE_FIRST(bit, image)

    BA_MESSAGEBOX_LOAD_IMAGE_FIRST(BA_MESSAGEBOX_FLAG_ICON_INFORMATION, information_xpm)
    BA_MESSAGEBOX_LOAD_IMAGE_ADDITIONAL(BA_MESSAGEBOX_FLAG_ICON_WARNING, warning_xpm)
    BA_MESSAGEBOX_LOAD_IMAGE_ADDITIONAL(BA_MESSAGEBOX_FLAG_ICON_QUESTION, question_xpm)
    BA_MESSAGEBOX_LOAD_IMAGE_ADDITIONAL(BA_MESSAGEBOX_FLAG_ICON_ERROR, error_xpm)
    
    GC imageGC;
    size_t fontHeight;

    if (loadedImage) {
        XGCValues values;

        values.clip_mask = shapeMask;
        values.clip_x_origin = 25;
        values.clip_y_origin = 26;

        imageGC = XCreateGC(display, window, GCClipMask + GCClipXOrigin + GCClipYOrigin, &values);
    }
    
    {
        BA_DynamicArray* unparsedLines = BA_String_SplitCharacter(formattedText, '\n');
        int maxCharacters = loadedImage ? BA_MESSAGEBOX_MAX_CHARACTERS_LOADED_IMAGE : BA_MESSAGEBOX_MAX_CHARACTERS;

        BA_DynamicArray_Create(&lines, unparsedLines->size);

        for (int i = 0; i < unparsedLines->used; i++) {
            char* target = unparsedLines->internalArray[i];
            ssize_t lineLength = strlen(target);

            while (lineLength > 0) {
                size_t toDraw = lineLength < maxCharacters ? lineLength : maxCharacters;
                char* newString = calloc(sizeof(char) * (toDraw + 1), 1);

                strncpy(newString, target, toDraw);
                BA_DynamicArray_AddElementToLast(&lines, newString);

                lineLength -= toDraw;
                target += toDraw;
            }
        }

        for (int i = 0; i < unparsedLines->used; i++)
            free(unparsedLines->internalArray[i]);

        free(unparsedLines->internalArray);
        free(unparsedLines);
    }
    
    XStoreName(display, window, title);

#define BA_MESSAGEBOX_CREATE_FOREGROUND_GC(variable, color) \
do {                                                        \
    XGCValues values;                                       \
    values.foreground = color;                              \
    variable = XCreateGC(display, window, GCForeground, &values); \
} while (BA_BOOLEAN_FALSE)

#ifdef BA_FREETYPE_FOUND
    XftFont* font = XftFontOpen(display, screen, XFT_FAMILY, XftTypeString, "Segoe UI Regular",
                                                 XFT_SIZE, FcTypeDouble, 9.0,
                                                 NULL);
    XftDraw* xftDraw = NULL;
    
    if (font != NULL) {
        xftDraw = XftDrawCreate(display, window, DefaultVisual(display, screen), DefaultColormap(display, screen));
        fontHeight = font->height;
    } else
        BA_LOGGER_WARN("Tahoma font not found. Using fallback fixed\n");
#endif

    GC textGC;
    
#ifdef BA_FREETYPE_FOUND
    if (font == NULL)
#endif
    {
        XGCValues values;

        values.font = XLoadFont(display, "7x13");
        values.foreground = BlackPixel(display, screen);
        textGC = XCreateGC(display, window, GCFont + GCForeground, &values);
    }
        
    GC buttonBackgroundGC;

    BA_MESSAGEBOX_CREATE_FOREGROUND_GC(buttonBackgroundGC, 0xF0F0F0);

    GC buttonOuterShineGC;

    BA_MESSAGEBOX_CREATE_FOREGROUND_GC(buttonOuterShineGC, 0xFFFFFF);

    GC buttonOuterShadowGC;

    BA_MESSAGEBOX_CREATE_FOREGROUND_GC(buttonOuterShadowGC, 0x696969);

    GC buttonInnerShineGC;

    BA_MESSAGEBOX_CREATE_FOREGROUND_GC(buttonInnerShineGC, 0xE3E3E3);

    GC buttonInnerShadowGC;

    BA_MESSAGEBOX_CREATE_FOREGROUND_GC(buttonInnerShadowGC, 0xA0A0A0);

    GC buttonPressedOuterGC;

    BA_MESSAGEBOX_CREATE_FOREGROUND_GC(buttonPressedOuterGC, 0x646464);

    GC buttonPressedInnerGC;

    BA_MESSAGEBOX_CREATE_FOREGROUND_GC(buttonPressedInnerGC, 0xA0A0A0);

    XFontStruct* fontStructure;
    
#ifdef BA_FREETYPE_FOUND
    if (font == NULL)
#endif
    {
        fontStructure = XLoadQueryFont(display, "-*-fixed-medium-r-*-*-13-*-*-*-*-*-*-*");

        if (fontStructure == NULL) {
            BA_LOGGER_ERROR("Failed loading font: fixed\n");
            goto destroy;
        }

        XSetFont(display, textGC, fontStructure->fid);

        fontHeight = fontStructure->ascent + fontStructure->descent;
    }

    BA_MessageBox_ButtonObject button1 = {0};
    BA_MessageBox_ButtonObject button2 = {0};
    BA_MessageBox_ButtonObject button3 = {0};
    
    switch (flags & (1 << sizeof(int)) - 1) {
        default:
        case BA_MESSAGEBOX_FLAG_BUTTON_OK:
            button3.text = "OK";
            button3.result = BA_MESSAGEBOX_RESULT_OK;
            break;

        case BA_MESSAGEBOX_FLAG_BUTTON_OK_CANCEL:
            button2.text = "OK";
            button2.result = BA_MESSAGEBOX_RESULT_OK;
            button3.text = "Cancel";
            button3.result = BA_MESSAGEBOX_RESULT_CANCEL;
            break;

        case BA_MESSAGEBOX_FLAG_BUTTON_ABORT_RETRY_IGNORE:
            button1.text = "Abort";
            button1.result = BA_MESSAGEBOX_RESULT_ABORT;
            button2.text = "Retry";
            button2.result = BA_MESSAGEBOX_RESULT_RETRY;
            button3.text = "Ignore";
            button3.result = BA_MESSAGEBOX_RESULT_IGNORE;
            break;

        case BA_MESSAGEBOX_FLAG_BUTTON_YES_NO_CANCEL:
            button1.text = "Yes";
            button1.result = BA_MESSAGEBOX_RESULT_YES;
            button2.text = "No";
            button2.result = BA_MESSAGEBOX_RESULT_NO;
            button3.text = "Cancel";
            button3.result = BA_MESSAGEBOX_RESULT_CANCEL;
            break;

        case BA_MESSAGEBOX_FLAG_BUTTON_YES_NO:
            button2.text = "Yes";
            button2.result = BA_MESSAGEBOX_RESULT_YES;
            button3.text = "No";
            button3.result = BA_MESSAGEBOX_RESULT_NO;
            break;

        case BA_MESSAGEBOX_FLAG_BUTTON_RETRY_CANCEL:
            button2.text = "Retry";
            button2.result = BA_MESSAGEBOX_RESULT_RETRY;
            button3.text = "Cancel";
            button3.result = BA_MESSAGEBOX_RESULT_CANCEL;
            break;

        case BA_MESSAGEBOX_FLAG_BUTTON_CANCEL_TRY_AGAIN_CONTINUE:
            button1.text = "Cancel";
            button1.result = BA_MESSAGEBOX_RESULT_CANCEL;
            button2.text = "Try Again";
            button2.result = BA_MESSAGEBOX_RESULT_TRY_AGAIN;
            button3.text = "Continue";
            button3.result = BA_MESSAGEBOX_RESULT_CONTINUE;
            break;
    }

    if (button1.text != NULL)
        button1.textLength = strlen(button1.text);

    if (button2.text != NULL)
        button2.textLength = strlen(button2.text);

    button3.textLength = strlen(button3.text);

    uint32_t width = 0;
    uint32_t topPadding = BA_MESSAGEBOX_TOP_PADDING_BASE + (loadedImage && lines.used <= 1 ? BA_MESSAGEBOX_TOP_PADDING_IMAGE_SINGLE_OR_NO_LINE_ADDITION : 0);
    uint32_t height = topPadding + (lines.used >= 1 ? 9 * lines.used + 6 * (lines.used - 1) : 0) + BA_MESSAGEBOX_BOTTOM_PADDING_BASE + BA_MESSAGEBOX_BUTTON_BACKGROUND_HEIGHT;

    if (loadedImage) {
        if (lines.used == 0)
            height += BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_ZERO_LINES_ADDITION;
        else if (lines.used == 1)
            height += BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_ONE_LINE_ADDITION;
        else if (lines.used >= 2)
            height += BA_MESSAGEBOX_BOTTOM_PADDING_IMAGE_TWO_OR_MORE_LINES_ADDITION;
    } else if (lines.used != 0)
        height += BA_MESSAGEBOX_BOTTOM_PADDING_SINGLE_OR_MORE_LINES_ADDITION;

#define BA_MESSAGEBOX_GET_TEXT_WIDTH_CLASSIC(variable, text, textLength) variable = XTextWidth(fontStructure, text, textLength)

#ifdef BA_FREETYPE_FOUND
#   define BA_MESSAGEBOX_GET_TEXT_WIDTH(variable, text, textLength) \
do {                                                                \
    size_t lineLength = textLength;                                 \
    if (font == NULL)                                               \
        BA_MESSAGEBOX_GET_TEXT_WIDTH_CLASSIC(variable, text, lineLength); \
    else {                                                          \
        XGlyphInfo information;                                     \
        XftTextExtentsUtf8(display, font, text, lineLength, &information); \
        variable = information.width;                               \
    }                                                               \
} while (BA_BOOLEAN_FALSE)
#else
#   define BA_MESSAGEBOX_GET_TEXT_WIDTH(variable, text, textLength) BA_MESSAGEBOX_GET_TEXT_WIDTH_CLASSIC(variable, text, textLength)
#endif
    
    {
        size_t biggestLineLength = 0;
    
        for (int i = 0; i < lines.used; i++) {
            size_t length = strlen(lines.internalArray[i]);
            size_t calculatedLength;

            BA_MESSAGEBOX_GET_TEXT_WIDTH(calculatedLength, lines.internalArray[i], length);

            if (width >= calculatedLength)
                continue;

            width = calculatedLength;
            biggestLineLength = length;
        }

        width += BA_MESSAGEBOX_LEFT_PADDING_BASE;

    
        if (biggestLineLength == 0)
            width += BA_MESSAGEBOX_RIGHT_PADDING_EMPTY_BOX;
    }

    if (button1.text != NULL)
        width += BA_MESSAGEBOX_BUTTON_WIDTH + BA_MESSAGEBOX_BUTTON1_RIGHT_PADDING;

    if (button2.text != NULL)
        width += BA_MESSAGEBOX_BUTTON_WIDTH + BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING + (button1.text != NULL ? BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING_HAS_FIRST_BUTTON_ADDITION : 0);

    width += BA_MESSAGEBOX_BUTTON_WIDTH + BA_MESSAGEBOX_BUTTON3_RIGHT_PADDING + (button2.text != NULL ? BA_MESSAGEBOX_BUTTON3_RIGHT_PADDING_MAX_WIDTH_AND_IMAGE_OR_HAS_SECOND_BUTTON : 0);

    if (loadedImage && width )
    
    {
        int maxWidth = BA_MESSAGEBOX_MAX_WIDTH_WITHOUT_IMAGE;

        if (loadedImage)
            maxWidth += BA_MESSAGEBOX_MAX_WIDTH_WITH_IMAGE_ADDITION;

        if (width > maxWidth)
            width = maxWidth;
    }
    
    {
        XSizeHints hints;

        hints.flags = PSize | PMinSize | PMaxSize;
        hints.min_width = width;
        hints.max_width = width;
        hints.min_height = height;
        hints.max_height = height;

        XSetWMNormalHints(display, window, &hints);
    }
    
    XSelectInput(display, window, ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

    {
        Atom deleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);

        XSetWMProtocols(display, window, &deleteWindow, 1);
    }

    XMapWindow(display, window);

    size_t pressedX = 0;
    size_t pressedY = 0;
    int pressedButton = 0;

#ifdef BA_FREETYPE_FOUND
    XRenderColor renderColor;
    XftColor color = {0};

    if (font != NULL) {
        renderColor.red = 0;
        renderColor.green = 0;
        renderColor.blue = 0;
        renderColor.alpha = 65535;

        XftColorAllocValue(display, DefaultVisual(display, screen), DefaultColormap(display, screen), &renderColor, &color);
    }
#endif
    
#define BA_MESSAGEBOX_CLASSIC_RENDER_TEXT(display, drawable, x, y, string, length) XDrawString(display, drawable, textGC, x, y, string, length)
    
#ifdef BA_FREETYPE_FOUND
#   define BA_MESSAGEBOX_RENDER_TEXT(display, drawable, x, y, string, length) \
do {                                                                          \
    if (font != NULL)                                                         \
        XftDrawString8(xftDraw, &color, font, x, y, string, length);          \
    else                                                                      \
        BA_MESSAGEBOX_CLASSIC_RENDER_TEXT(display, drawable, x, y, string, length); \
} while (BA_BOOLEAN_FALSE)
#else
#   define BA_MESSAGEBOX_RENDER_TEXT(display, drawable, x, y, string, length) BA_MESSAGEBOX_CLASSIC_RENDER_TEXT(display, drawable, x, y, string, length)
#endif
    
    while (BA_BOOLEAN_TRUE) {
        XEvent event;
        
        XNextEvent(display, &event);
        
        if (event.type == Expose) {
            if (loadedImage)
                XCopyArea(display, pixmap, window, imageGC, 0, 0, attributes.width, attributes.height, 25, 26);
            
            int y = 9 + topPadding;
            
            for (int i = 0; i < lines.used; i++) {
                char* target = lines.internalArray[i];
            
                BA_MESSAGEBOX_RENDER_TEXT(display, window, loadedImage ? 65 : 12, y, target, strlen(target));
            
                y += fontHeight + 2;
            }

            XFillRectangle(display, window, buttonBackgroundGC, 0, height - BA_MESSAGEBOX_BUTTON_BACKGROUND_HEIGHT, width, BA_MESSAGEBOX_BUTTON_BACKGROUND_HEIGHT);

#define BA_MESSAGEBOX_GET_BUTTON_X_Y(padding1, padding2) \
size_t buttonX = width - (BA_MESSAGEBOX_BUTTON3_RIGHT_PADDING + ((loadedImage && width == BA_MESSAGEBOX_MAX_WIDTH_WITHOUT_IMAGE + BA_MESSAGEBOX_MAX_WIDTH_WITH_IMAGE_ADDITION) || button2.text != NULL ? BA_MESSAGEBOX_BUTTON3_RIGHT_PADDING_MAX_WIDTH_AND_IMAGE_OR_HAS_SECOND_BUTTON : 0)) - BA_MESSAGEBOX_BUTTON_WIDTH - (padding2 != 0 ? (padding2 + (button1.text != NULL ? BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING_HAS_FIRST_BUTTON_ADDITION : 0) + BA_MESSAGEBOX_BUTTON_WIDTH) : 0) - (padding1 != 0 ? padding1 + BA_MESSAGEBOX_BUTTON_WIDTH : 0); \
size_t buttonY = height - (BA_MESSAGEBOX_BUTTON_BOTTOM_PADDING + (!loadedImage && lines.used != 0 ? BA_MESSAGEBOX_BUTTON_BOTTOM_PADDING_NO_IMAGE_NO_LINES_ADDITION : 0)) - BA_MESSAGEBOX_BUTTON_HEIGHT

#define BA_MESSAGEBOX_DRAW_BORDER(outerShine, outerShadow, innerShine, innerShadow) \
do {                                                                                \
    XDrawLine(display, window, outerShine, buttonX, buttonY, buttonX + BA_MESSAGEBOX_BUTTON_WIDTH - 1, buttonY); \
    XDrawLine(display, window, outerShine, buttonX, buttonY + 1, buttonX, buttonY + 1 + BA_MESSAGEBOX_BUTTON_HEIGHT - 2); \
    XDrawLine(display, window, outerShadow, buttonX + 1, buttonY + BA_MESSAGEBOX_BUTTON_HEIGHT - 1, buttonX + 1 + BA_MESSAGEBOX_BUTTON_WIDTH - 2, buttonY + BA_MESSAGEBOX_BUTTON_HEIGHT - 1); \
    XDrawLine(display, window, outerShadow, buttonX + BA_MESSAGEBOX_BUTTON_WIDTH - 1, buttonY + 1, buttonX + BA_MESSAGEBOX_BUTTON_WIDTH - 1, buttonY + 1 + BA_MESSAGEBOX_BUTTON_HEIGHT - 3); \
    XDrawLine(display, window, innerShine, buttonX + 1, buttonY + 1, buttonX + 1 + BA_MESSAGEBOX_BUTTON_WIDTH - 3, buttonY + 1); \
    XDrawLine(display, window, innerShine, buttonX + 1, buttonY + 2, buttonX + 1, buttonY + 2 + BA_MESSAGEBOX_BUTTON_HEIGHT - 4); \
    XDrawLine(display, window, innerShadow, buttonX + 2, buttonY + BA_MESSAGEBOX_BUTTON_HEIGHT - 2, buttonX + 2 + BA_MESSAGEBOX_BUTTON_WIDTH - 4, buttonY + BA_MESSAGEBOX_BUTTON_HEIGHT - 2); \
    XDrawLine(display, window, innerShadow, buttonX + BA_MESSAGEBOX_BUTTON_WIDTH - 2, buttonY + 2, buttonX + BA_MESSAGEBOX_BUTTON_WIDTH - 2, buttonY + 2 + BA_MESSAGEBOX_BUTTON_HEIGHT - 5); \
} while (BA_BOOLEAN_FALSE)

#define BA_MESSAGEBOX_DRAW_BUTTON(button, padding1, padding2) \
do {                                                          \
    BA_MESSAGEBOX_GET_BUTTON_X_Y(padding1, padding2);         \
    BA_MESSAGEBOX_DRAW_BORDER(buttonOuterShineGC, buttonOuterShadowGC, buttonInnerShineGC, buttonInnerShadowGC); \
    size_t textWidth;                                         \
    BA_MESSAGEBOX_GET_TEXT_WIDTH(textWidth, button.text, button.textLength); \
    BA_MESSAGEBOX_RENDER_TEXT(display, window, buttonX + BA_MESSAGEBOX_BUTTON_WIDTH / 2 - textWidth / 2 + 1, buttonY + fontHeight + 1, button.text, button.textLength); \
} while (BA_BOOLEAN_FALSE)
            
            if (button1.text != NULL)
                BA_MESSAGEBOX_DRAW_BUTTON(button1, BA_MESSAGEBOX_BUTTON1_RIGHT_PADDING, BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING);

            if (button2.text != NULL)
                BA_MESSAGEBOX_DRAW_BUTTON(button2, 0, BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING);
            
            BA_MESSAGEBOX_DRAW_BUTTON(button3, 0, 0);
            continue;
        }

        if (event.type == ClientMessage) {
            result = BA_MESSAGEBOX_RESULT_CANCEL;
            break;
        }

#define BA_MESSAGEBOX_IS_INSIDE_BUTTON(x1, y1) (event.xmotion.x >= x1 && event.xmotion.x <= x1 + BA_MESSAGEBOX_BUTTON_WIDTH && event.xmotion.y >= y1 && event.xmotion.y <= y1 + BA_MESSAGEBOX_BUTTON_HEIGHT)
        
#define BA_MESSAGEBOX_CHECK_BUTTON(padding1, padding2) \
if (BA_MESSAGEBOX_IS_INSIDE_BUTTON(buttonX, buttonY))
        
#define BA_MESSAGEBOX_SET_PRESSED_BUTTON(id, padding1, padding2) \
do {                                                             \
        BA_MESSAGEBOX_GET_BUTTON_X_Y(padding1, padding2);        \
        BA_MESSAGEBOX_CHECK_BUTTON(padding1, padding2) {         \
            pressedButton = id;                                  \
            pressedX = buttonX;                                  \
            pressedY = buttonY;                                  \
            BA_MESSAGEBOX_DRAW_BORDER(buttonPressedOuterGC, buttonPressedOuterGC, buttonPressedInnerGC, buttonPressedInnerGC); \
        }                                                        \
} while (BA_BOOLEAN_FALSE)
        
        if (event.type == ButtonPress) {
            if (button1.text != NULL)
                BA_MESSAGEBOX_SET_PRESSED_BUTTON(1, BA_MESSAGEBOX_BUTTON1_RIGHT_PADDING, BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING);
        
            if (button2.text != NULL)
                BA_MESSAGEBOX_SET_PRESSED_BUTTON(2, 0, BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING);
            
            BA_MESSAGEBOX_SET_PRESSED_BUTTON(3, 0, 0);
            continue;
        }

#define BA_MESSAGEBOX_PRESS_BUTTON(id, padding1, padding2) \
{                                                          \
    BA_MESSAGEBOX_GET_BUTTON_X_Y(padding1, padding2);      \
    BA_MESSAGEBOX_CHECK_BUTTON(padding1, padding2) {       \
        if (pressedButton == id) {                         \
            result = button ## id.result;                  \
            break;                                         \
        }                                                  \
    }                                                      \
} (void) NULL
        
        if (event.type == ButtonRelease) {
            BA_MESSAGEBOX_PRESS_BUTTON(1, BA_MESSAGEBOX_BUTTON1_RIGHT_PADDING, BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING);
            BA_MESSAGEBOX_PRESS_BUTTON(2, 0, BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING);
            BA_MESSAGEBOX_PRESS_BUTTON(3, 0, 0);
            
            pressedButton = 0;
            continue;
        }

#define BA_MESSAGEBOX_UPDATE_BORDER(id, padding1, padding2) \
do {                                                        \
    BA_MESSAGEBOX_GET_BUTTON_X_Y(padding1, padding2);       \
    if (BA_MESSAGEBOX_IS_INSIDE_BUTTON(pressedX, pressedY) && pressedButton == id) \
        BA_MESSAGEBOX_DRAW_BORDER(buttonPressedOuterGC, buttonPressedOuterGC, buttonPressedInnerGC, buttonPressedInnerGC); \
    else                                                    \
        BA_MESSAGEBOX_DRAW_BORDER(buttonOuterShineGC, buttonOuterShadowGC, buttonInnerShineGC, buttonInnerShadowGC); \
} while (BA_BOOLEAN_FALSE)
        
        if (event.type == MotionNotify) {
            if (button1.text != NULL)
                BA_MESSAGEBOX_UPDATE_BORDER(1, BA_MESSAGEBOX_BUTTON1_RIGHT_PADDING, BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING);
            
            if (button2.text != NULL)
                BA_MESSAGEBOX_UPDATE_BORDER(2, 0, BA_MESSAGEBOX_BUTTON2_RIGHT_PADDING);
            
            BA_MESSAGEBOX_UPDATE_BORDER(3, 0, 0);
        }
    }
    
    destroy:
    if (loadedImage) {
        XFreePixmap(display, pixmap);
        XFreePixmap(display, shapeMask);
        XpmFreeAttributes(&attributes);
        XFreeGC(display, imageGC);
    }

#ifdef BA_FREETYPE_FOUND
    if (font != NULL) {
        XftColorFree(display, DefaultVisual(display, screen), DefaultColormap(display, screen), &color);
        XftFontClose(display, font);
        XftDrawDestroy(xftDraw);
    } else
#endif
    {
        XFreeGC(display, textGC);
    }
    
    XFreeGC(display, buttonBackgroundGC);
    XFreeGC(display, buttonOuterShineGC);
    XFreeGC(display, buttonOuterShadowGC);
    XFreeGC(display, buttonInnerShineGC);
    XFreeGC(display, buttonInnerShadowGC);
    XFreeGC(display, buttonPressedOuterGC);
    XFreeGC(display, buttonPressedInnerGC);
    
    for (int i = 0; i < lines.used; i++)
        free(lines.internalArray[i]);

    free(lines.internalArray);
    XFlush(display);
    XCloseDisplay(display);
#   else
    BA_LOGGER_WARN("X11 is disabled on this build. Sorry");
#   endif
#elif BA_OPERATINGSYSTEM_WINDOWS
    result = MessageBox(NULL, formattedText, title, flags);
#endif

    free(formattedText);
    return result;
}
