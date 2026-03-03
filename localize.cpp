#include "localize.hpp"
#include <clocale>
#include <libintl.h>

void localizeInit() {
    setlocale(LC_ALL, "");
    bindtextdomain("craftive", "/usr/share/locale");
    bind_textdomain_codeset("craftive", "UTF-8");
    textdomain("craftive");
}
