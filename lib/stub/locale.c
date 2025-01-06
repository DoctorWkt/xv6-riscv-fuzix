#include <locale.h>

locale_t _current_locale(void) {
  return(LC_ALL);
}
