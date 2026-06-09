#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>

#define CRAFTIVE_DEF_TR                                                            \
  public:                                                                          \
    static inline QString tr(const char *s, const char *c = nullptr, int n = -1) { \
        return QCoreApplication::tr(s, c, n);                                      \
    }

void localizeInit();