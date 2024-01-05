//
// Created by Bram Nijenkamp on 05-01-2024.
//

#include "system.h"

namespace log {
    static bool print_ansi_coloring = true;

    void print_enable_ansi_coloring(bool enabled) {
        print_ansi_coloring = enabled;
    }

    static const char* c_debug = "[ DEBUG ] ";
    static const char* c_print = "          ";
    static const char* c_log = "[  LOG  ] ";
    static const char* c_success = "[SUCCESS] ";
    static const char* c_warning = "[WARNING] ";
    static const char* c_error = "[ ERROR ] ";

#if !defined(NDEBUG)
    void print_debug(const char* color, const std::string& str) {
        std::cout << c_debug;
        if (print_ansi_coloring) {
            std::cout << color << str << ::terminal_coloring::reset << std::endl;
        } else {
            std::cout << str << std::endl;
        }
    }
#endif

    void print(const std::string& str) {
        if (print_ansi_coloring) {
            std::cout << ::terminal_coloring::foreground_bold_white << str << ::terminal_coloring::reset << std::flush;
        } else {
            std::cout << str << std::flush;
        }
    }

    void println(const std::string& str) {
        if (print_ansi_coloring) {
            std::cout << ::terminal_coloring::foreground_bold_white << str << ::terminal_coloring::reset << std::endl;
        } else {
            std::cout << str << std::endl;
        }
    }

    void print_log(const std::string& str) {
        std::cout << c_log;
        if (print_ansi_coloring) {
            std::cout << ::terminal_coloring::foreground_bold_white << str << ::terminal_coloring::reset << std::endl;
        } else {
            std::cout << str << std::endl;
        }
    }

    void print_success(const std::string& str) {
        std::cout << c_success;
        if (print_ansi_coloring) {
            std::cout << ::terminal_coloring::foreground_bold_green << str << ::terminal_coloring::reset << std::endl;
        } else {
            std::cout << str << std::endl;
        }
    }

    void print_warn(const std::string& str) {
        std::cout << c_warning;
        if (print_ansi_coloring) {
            std::cout << ::terminal_coloring::foreground_bold_yellow << str << ::terminal_coloring::reset << std::endl;
        } else {
            std::cout << str << std::endl;
        }
    }

    void print_error(const std::string& str) {
        std::cout << c_error;
        if (print_ansi_coloring) {
            std::cout << ::terminal_coloring::foreground_bold_red << str << ::terminal_coloring::reset << std::endl;
        } else {
            std::cout << str << std::endl;
        }
    }

}  // namespace log