#include <pebble.h>

#define BUF_LEN 40

#define LC_BG  0
#define LC_BY  1
#define LC_CN  2
#define LC_HR  3
#define LC_CZ  4
#define LC_DK  5
#define LC_NL  6
#define LC_EN  7
#define LC_EE  8
#define LC_FI  9
#define LC_FR 10
#define LC_DE 11
#define LC_GR 12
#define LC_HU 13
#define LC_IS 14
#define LC_IT 15
#define LC_JP 16
#define LC_LT 17
#define LC_NO 18
#define LC_PL 19
#define LC_PT 20
#define LC_RO 21
#define LC_RU 22
#define LC_YU 23
#define LC_SK 24
#define LC_SL 25
#define LC_ES 26
#define LC_SE 27
#define LC_TR 28
#define LC_UA 29
#define NUM_LOCALES 30

void update_date_display();

extern struct tm *pebble_time;

static TextLayer *s_date_layer;
int s_locale = LC_EN;

static char DAY[NUM_LOCALES][7][6] = {
  {"Нд ", "Пн ", "Вт ", "Ср ", "Чт ", "Пт ", "Сб "}, // LC_BG Bulgarian
  {"нд ", "пн ", "аў ", "ср ", "чц ", "пт ", "сб "}, // LC_BY Byelorussian
  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}, // LC_CN Chinese
  {"Ne", "Po", "Ut", "Sr", "Če", "Pe", "Su"},        // LC_HR Croatian
  {"ne", "po", "út", "st", "čt", "pá", "so"},        // LC_CZ Czech
  {"Søn", "Man", "Tir", "Ons", "Tor", "Fre", "Lør"}, // LC_DK Danish
  {"zo", "ma", "di", "wo", "do", "vr", "za"},        // LC_NL Dutch
  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}, // LC_EN English
  {"P", "E", "T", "K", "N", "R", "L"},               // LC_EE Estonian
  {"Su", "Ma", "Ti", "Ke", "To", "Pe", "La"},        // LC_FI Finnish
  {"Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam"}, // LC_FR French
  {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"},        // LC_DE German
  {"Κυρ", "Δευ", "Τρι", "Τετ", "Πεμ", "Παρ", "Σαβ"}, // LC_GR Greek
  {"Vas", "Hét", "Ked", "Sze", "Csü", "Pén", "Szo"}, // LC_HU Hungarian
  {"sun", "mán", "þri", "mið", "fim", "fös", "lau"}, // LC_IS Icelandic
  {"Dom", "Lun", "Mar", "Mer", "Gio", "Ven", "Sab"}, // LC_IT Italian
  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}, // LC_JP Japanese
  {"Sk", "Pr", "An", "Tr", "Kt", "Pn", "Št"},        // LC_LT Lithuanian
  {"søn", "man", "tir", "ons", "tor", "fre", "lør"}, // LC_NO Norwegian
  {"ndz", "pon", "wto", "śro", "czw", "ptk", "sob"}, // LC_PL Polish
  {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sáb"}, // LC_PT Portuguese
  {"Dum", "Lun", "Mar", "Mie", "Joi", "Vin", "Sâm"}, // LC_RO Romanian
  {"вс", "пн", "вт", "ср", "чт", "пт", "сб"},        // LC_RU Russian
  {"нед", "пон", "уто", "сре", "чет", "пет", "суб"}, // LC_YU Serbian
  {"ne ", "po ", "ut ", "st ", "št ", "pi ", "so "}, // LC_SK Slovak
  {"ned", "pon", "tor", "sre", "čet", "pet", "sob"}, // LC_SL Slovenian
  {"dom", "lun", "mar", "mié", "jue", "vie", "sáb"}, // LC_ES Spanish
  {"Sön", "Mån", "Tis", "Ons", "Tor", "Fre", "Lör"}, // LC_SE Swedish
  {"Paz", "Pts", "Sal", "Çar", "Per", "Cum", "Cts"}, // LC_TR Turkish
  {"нд", "пн", "вт", "ср", "чт", "пт", "сб"}         // LC_UA Ukranian
};
static char MONTH[NUM_LOCALES][12][8] = {
  {"Яну", "Фев", "Мар", "Апр", "Май", "Юни", "Юли", "Авг", "Сеп", "Окт", "Нов", "Дек"},
  {"сту", "лют", "сак", "кра", "тра", "чэр", "ліп", "жні", "вер", "кас", "ліс", "сне"},
  {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"},
  {"Sij", "Vel", "Ožu", "Tra", "Svi", "Lip", "Srp", "Kol", "Ruj", "Lis", "Stu", "Pro"},
  {"led", "úno", "bře", "dub", "kvě", "črv", "čvc", "srp", "zář", "říj", "lis", "pro"},
  {"Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec"},
  {"jan", "feb", "mrt", "apr", "mei", "jun", "jul", "aug", "sep", "okt", "nov", "dec"},
  {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"},
  {"jaan", "veebr", "märts", "apr", "mai", "juuni", "juuli", "aug", "sept", "okt", "nov", "dets"},
  {"Tam", "Hel", "Maa", "Huh", "Tou", "Kes", "Hei", "Elo", "Syy", "Lok", "Mar", "Jou"},
  {"jan", "fév", "mar", "avr", "mai", "jui", "jul", "aoû", "sep", "oct", "nov", "déc"},
  {"Jan", "Feb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez"},
  {"Ιαν", "Φεβ", "Μαρ", "Απρ", "Μαϊ", "Ιον", "Ιολ", "Αυγ", "Σεπ", "Οκτ", "Νοε", "Δεκ"},
  {"Jan", "Feb", "Már", "Ápr", "Máj", "Jún", "Júl", "Aug", "Sze", "Okt", "Nov", "Dec"},
  {"jan", "feb", "mar", "apr", "maí", "jún", "júl", "ágú", "sep", "okt", "nóv", "des"},
  {"Gen", "Feb", "Mar", "Apr", "Mag", "Giu", "Lug", "Ago", "Set", "Ott", "Nov", "Dic"},
  {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"},
  {"Sau", "Vas", "Kov", "Bal", "Geg", "Bir", "Lie", "Rgp", "Rgs", "Spa", "Lap", "Grd"},
  {"jan", "feb", "mar", "apr", "mai", "jun", "jul", "aug", "sep", "okt", "nov", "des"},
  {"sty", "lut", "mar", "kwi", "maj", "cze", "lip", "sie", "wrz", "paź", "lis", "gru"},
  {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"},
  {"Ian", "Feb", "Mar", "Apr", "Mai", "Iun", "Iul", "Aug", "Sep", "Oct", "Noi", "Dec"},
  {"янв", "фев", "мар", "апр", "май", "июн", "июл", "авг", "сен", "окт", "ноя", "дек"},
  {"јан", "феб", "мар", "апр", "мај", "јун", "јул", "авг", "сеп", "окт", "нов", "дец"},
  {"jan", "feb", "mar", "apr", "máj", "jún", "júl", "aug", "sep", "okt", "nov", "dec"},
  {"jan", "feb", "mar", "apr", "maj", "jun", "jul", "avg", "sep", "okt", "nov", "dec"},
  {"ene", "feb", "mar", "abr", "may", "jun", "jul", "ago", "sep", "oct", "nov", "dic"},
  {"Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec"},
  {"Oca", "Şub", "Mar", "Nis", "May", "Haz", "Tem", "Ağu", "Eyl", "Eki", "Kas", "Ara"},
  {"січ", "лют", "бер", "кві", "тра", "чер", "лип", "сер", "вер", "жов", "лис", "гру"}
};

void setup_date(Layer *root) {
  s_date_layer = text_layer_create(GRect(0, 58, 144, 28));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_date_layer));
}

void teardown_date() {
  text_layer_destroy(s_date_layer);
}

void update_date_display() {
  // Create a long-lived buffer
  static char day_date_buffer[BUF_LEN];

  char *wday = DAY[s_locale][pebble_time->tm_wday];
  int mday = pebble_time->tm_mday;
  char *mon = MONTH[s_locale][pebble_time->tm_mon];
  snprintf(day_date_buffer, sizeof(day_date_buffer), "%s %d %s", wday, mday, mon);
  text_layer_set_text(s_date_layer, day_date_buffer);
}
