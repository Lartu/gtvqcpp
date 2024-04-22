#include <iostream>
#include <map>
#include <stack>
#include <string>
#include "gtvq_utils.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_PNG_Image.h>

enum WidgetType
{
    wtWINDOW,
    wtBUTTON,
    wtLABEL,
    wtIMAGE,
    single_line_text_area,
    multi_line_text_area,
    radio_button,
    checkbox,
    progress_bar,
    scrollbar_v,
    scrollhbar_h,
    drop_down,
    scroll_list
};

class Widget
{
public:
    Fl_Widget *widget;
    vector<gtvqString> string_data;
    WidgetType type;
};

map<gtvqString, Widget> widgets;

Widget &newWidget(gtvqString &id, WidgetType type)
{
    Widget newWidget;
    newWidget.type = type;
    widgets[id] = newWidget;
    return widgets[id];
}

Widget &getWidget(gtvqString &id)
{
    if (widgets.count(id) == 0)
    {
        gtvq_error("Widget with ID " + id + " not registered.");
    }
    return widgets[id];
}

void buttonPressCallback(Fl_Widget *button, void *body_data_pointer)
{
    gtvqString body_data = *((gtvqString *)body_data_pointer);
    gtvq_execute_code(body_data);
}

gtvqString window_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    Widget &w = newWidget(widget_id, wtWINDOW);
    w.string_data.push_back("untitled window");
    Fl_Window *window = new Fl_Window(320, 240, toCString(w.string_data[0]));
    window->end();
    w.widget = window;
    return widget_id;
}

gtvqString button_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    Widget &w = newWidget(widget_id, wtBUTTON);
    w.string_data.push_back("press me"); // Text
    w.string_data.push_back("");         // Callback
    Fl_Button *widget = new Fl_Button(0, 0, 160, 40, toCString(w.string_data[0]));
    w.widget = widget;
    return widget_id;
}

gtvqString show_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    getWidget(widget_id).widget->show();
    return widget_id;
}

gtvqString run_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString window_id = parameters[0];
    Widget &window = getWidget(window_id);
    if (window.type != wtWINDOW)
    {
        gtvq_error(command + " expects a window.");
    }
    getWidget(window_id).widget->show();
    return to_string(Fl::run());
}

gtvqString locate_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 3, 3, parameters.size());
    gtvqString widget_id = parameters[0];
    int x = toInteger(parameters[1]);
    int y = toInteger(parameters[2]);
    getWidget(widget_id).widget->position(x, y);
    return widget_id;
}

gtvqString size_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 3, 3, parameters.size());
    gtvqString widget_id = parameters[0];
    int w = toInteger(parameters[1]);
    int h = toInteger(parameters[2]);
    Widget &widget = getWidget(widget_id);
    widget.widget->size(w, h);
    if (widget.type == wtIMAGE)
    {
        widget.widget->redraw();
    }
    return widget_id;
}

gtvqString text_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    gtvqString widget_id = parameters[0];
    gtvqString value = parameters[1];
    Widget &widget = getWidget(widget_id);
    if (false)
    {
    }
    else
    {
        widget.string_data[0] = value;
        widget.widget->label(toCString(widget.string_data[0]));
    }

    return widget_id;
}

gtvqString text_get_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    gtvqString widget_id = parameters[0];
    gtvqString value = parameters[1];
    Widget &widget = getWidget(widget_id);
    if (false)
    {
    }
    else
    {
        widget.string_data[0] = value;
        return widget.widget->label();
    }

    return "";
}

gtvqString window_add_child_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    Widget &window = getWidget(parameters[0]);
    Widget &widget = getWidget(parameters[1]);
    if (window.type != wtWINDOW)
    {
        gtvq_error(command + " can only be used with window widgets.");
    }
    ((Fl_Window *)window.widget)->add(widget.widget);
    return parameters[0]; // Widget id
}

gtvqString window_redraw_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    Widget &window = getWidget(parameters[0]);
    if (window.type != wtWINDOW)
    {
        gtvq_error(command + " can only be used with window widgets.");
    }
    ((Fl_Window *)window.widget)->redraw();
    return parameters[0]; // Widget id
}

gtvqString button_onpress_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    Widget &widget = getWidget(parameters[0]);
    if (widget.type != wtBUTTON)
    {
        gtvq_error(command + " can only be used with button widgets.");
    }
    widget.string_data[1] = parameters[1];
    ((Fl_Button *)widget.widget)->callback(buttonPressCallback, &(widget.string_data[1]));
    return parameters[0]; // Widget id
}

gtvqString message_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 2, parameters.size());
    gtvqString title = "";
    gtvqString body = parameters[0];
    if (parameters.size() >= 2)
    {
        title = parameters[1];
    }
    fl_message_title(toCString(title));
    fl_message("%s", toCString(body));
    return "";
}

gtvqString alert_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 2, parameters.size());
    gtvqString title = "";
    gtvqString body = parameters[0];
    if (parameters.size() >= 2)
    {
        title = parameters[1];
    }
    fl_message_title(toCString(title));
    fl_alert("%s", toCString(body));
    return "";
}

gtvqString screen_width_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    return toString(Fl::w());
}

gtvqString screen_height_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    return toString(Fl::h());
}

gtvqString size_get_w_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    Widget &widget = getWidget(widget_id);
    return toString(widget.widget->w());
}

gtvqString size_get_h_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    Widget &widget = getWidget(widget_id);
    return toString(widget.widget->h());
}

gtvqString loc_get_x_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    Widget &widget = getWidget(widget_id);
    return toString(widget.widget->x());
}

gtvqString loc_get_y_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    Widget &widget = getWidget(widget_id);
    return toString(widget.widget->y());
}

gtvqString label_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    Widget &w = newWidget(widget_id, wtLABEL);
    w.string_data.push_back(widget_id); // Text
    Fl_Box *widget = new Fl_Box(0, 0, 160, 40, toCString(w.string_data[0]));
    widget->align(FL_ALIGN_WRAP | FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    widget->labelfont(FL_HELVETICA);
    w.widget = widget;
    return widget_id;
}

gtvqString label_align_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 3, parameters.size());
    Widget &widget = getWidget(parameters[0]);
    if (widget.type != wtLABEL)
    {
        gtvq_error(command + " can only be used with label widgets.");
    }
    gtvqString hAlign = parameters[1];
    gtvqString_toUpper(hAlign);
    gtvqString vAlign = "MIDDLE";
    if (parameters.size() >= 3)
    {
        vAlign = parameters[2];
        gtvqString_toUpper(vAlign);
    }
    Fl_Align alignment = FL_ALIGN_WRAP | FL_ALIGN_INSIDE;
    if (hAlign == "LEFT" && vAlign == "MIDDLE")
    {
        alignment |= FL_ALIGN_LEFT;
    }
    else if (hAlign == "CENTER" && vAlign == "MIDDLE")
    {
        alignment |= FL_ALIGN_CENTER;
    }
    else if (hAlign == "RIGHT" && vAlign == "MIDDLE")
    {
        alignment |= FL_ALIGN_RIGHT;
    }
    else if (hAlign == "LEFT" && vAlign == "TOP")
    {
        alignment |= FL_ALIGN_TOP_LEFT;
    }
    else if (hAlign == "CENTER" && vAlign == "TOP")
    {
        alignment |= FL_ALIGN_TOP;
    }
    else if (hAlign == "RIGHT" && vAlign == "TOP")
    {
        alignment |= FL_ALIGN_TOP_RIGHT;
    }
    else if (hAlign == "LEFT" && vAlign == "BOTTOM")
    {
        alignment |= FL_ALIGN_BOTTOM_LEFT;
    }
    else if (hAlign == "CENTER" && vAlign == "BOTTOM")
    {
        alignment |= FL_ALIGN_BOTTOM;
    }
    else if (hAlign == "RIGHT" && vAlign == "BOTTOM")
    {
        alignment |= FL_ALIGN_BOTTOM_RIGHT;
    }
    else
    {
        gtvq_error("Unknown alignment " + hAlign + ", " + vAlign + ".");
    }
    ((Fl_Box *)widget.widget)->align(alignment);
    return parameters[0]; // Widget id
}

gtvqString label_style_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, parameters.size());
    Widget &widget = getWidget(parameters[0]);
    if (widget.type != wtLABEL)
    {
        gtvq_error(command + " can only be used with label widgets.");
    }
    Fl_Font style = 0;
    // Look for fonts
    for (size_t i = 1; i < parameters.size(); ++i)
    {
        gtvqString par = parameters[i];
        gtvqString_toUpper(par);
        if (par == "TIMES")
        {
            style |= FL_TIMES;
        }
        else if (par == "COURIER")
        {
            style |= FL_COURIER;
        }
        else if (par == "MONOSPACE")
        {
            style |= FL_SCREEN;
        }
        else if (par == "HELVETICA")
        {
            style |= FL_HELVETICA;
        }
        else if (par == "SYMBOL")
        {
            style |= FL_SYMBOL;
        }
        else if (par == "ZAPF")
        {
            style |= FL_ZAPF_DINGBATS;
        }
    }
    // Lok for styles
    for (size_t i = 1; i < parameters.size(); ++i)
    {
        gtvqString par = parameters[i];
        gtvqString_toUpper(par);
        if (par == "BOLD")
        {
            style |= FL_BOLD;
        }
        else if (par == "ITALIC")
        {
            style |= FL_ITALIC;
        }
    }
    ((Fl_Box *)widget.widget)->labelfont(style);
    return parameters[0]; // Widget id
}

class ImageBox : public Fl_Box
{
private:
    Fl_Image *original; // Original image to resize from
    Fl_Image *resized;  // Holds the resized image

public:
    // Constructor
    ImageBox(int X, int Y, int W, int H, const char *L = 0) : Fl_Box(X, Y, W, H, L), original(nullptr), resized(nullptr)
    {
    }

    // Set the image and store the original
    void image(Fl_Image *img)
    {
        original = img; // Store original image
        resize(x(), y(), w(), h());
    }

    // Override the resize method to adjust the image size
    void resize(int X, int Y, int W, int H)
    {
        Fl_Box::resize(X, Y, W, H);
        if (original)
        {
            delete resized;                 // Delete the old resized image
            resized = original->copy(W, H); // Create a new resized image
            Fl_Box::image(resized);
            redraw();
            if (window() != NULL)
            {
                window()->redraw();
            }
        }
    }

    ~ImageBox()
    {
        delete resized; // Clean up the resized image
    }
};

gtvqString image_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 1, 1, parameters.size());
    gtvqString widget_id = parameters[0];
    Widget &w = newWidget(widget_id, wtIMAGE);
    w.string_data.push_back(""); // Image Path
    ImageBox *widget = new ImageBox(0, 0, 100, 100, toCString(w.string_data[0]));
    w.widget = widget;
    return widget_id;
}

gtvqString image_set_handler(gtvqString &command, vector<gtvqString> &parameters)
{
    gtvqCheckArguments(command, 2, 2, parameters.size());
    gtvqString image_path = parameters[1];
    Widget &widget = getWidget(parameters[0]);
    if (widget.type != wtIMAGE)
    {
        gtvq_error(command + " can only be used with image widgets.");
    }
    widget.string_data[0] = image_path;
    Fl_PNG_Image icon(toCString(widget.string_data[0]));
    ImageBox *widgetPointer = (ImageBox *)widget.widget;
    widgetPointer->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);
    widgetPointer->image(icon.copy(icon.w(), icon.h()));
    widgetPointer->label("");
    widgetPointer->redraw();
    return parameters[0]; // Widget id
}

void UI_Setup()
{
    Fl::scheme("gtk+"); // base, plastic, gtk+ or gleam
}

extern "C" void gtvq_link_handlers(GTVQ_PARAMETERS)
{
    // Set up
    gtvqLibSetup(GTVQ_PAR_CALL);
    UI_Setup();

    // Add Handlers
    // -- Generic Commands --
    gtvq_link_command_handler("ui::run", run_handler);
    gtvq_link_command_handler("ui::message", message_handler);
    gtvq_link_command_handler("ui::alert", alert_handler);
    gtvq_link_command_handler("ui::screen.w", screen_width_handler);
    gtvq_link_command_handler("ui::screen.h", screen_height_handler);
    // -- FL_Widget Properties --
    gtvq_link_command_handler("ui::show", show_handler);
    gtvq_link_command_handler("ui::loc.set", locate_handler);
    gtvq_link_command_handler("ui::loc.get.x", loc_get_x_handler);
    gtvq_link_command_handler("ui::loc.get.y", loc_get_y_handler);
    gtvq_link_command_handler("ui::size.set", size_handler);
    gtvq_link_command_handler("ui::size.get.w", size_get_w_handler);
    gtvq_link_command_handler("ui::size.get.h", size_get_h_handler);
    gtvq_link_command_handler("ui::text.set", text_handler);
    gtvq_link_command_handler("ui::text.get", text_get_handler);
    // -- Widget Specific Properties --
    // Window
    gtvq_link_command_handler("ui::window", window_handler);
    gtvq_link_command_handler("ui::window.add", window_add_child_handler);
    gtvq_link_command_handler("ui::window.redraw", window_redraw_handler);
    // Button
    gtvq_link_command_handler("ui::button", button_handler);
    gtvq_link_command_handler("ui::button.onpress", button_onpress_handler);
    // Button
    gtvq_link_command_handler("ui::image", image_handler);
    gtvq_link_command_handler("ui::image.set", image_set_handler);
    // Button
    gtvq_link_command_handler("ui::label", label_handler);
    gtvq_link_command_handler("ui::label.align", label_align_handler);
    gtvq_link_command_handler("ui::label.style", label_style_handler);
}