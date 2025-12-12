#include "rtc.h"
#include "../../terminal.h"  // ← правильный путь

/* Read a value from RTC register */
static unsigned char rtc_read(unsigned char reg) {
    outb(0x70, reg);
    return inb(0x71);
}

/* Convert BCD to binary */
static int bcd_to_bin(unsigned char bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

/* Get current time from RTC */
void rtc_get_time(int* year, int* month, int* day, int* hour, int* minute, int* second) {
    *second = rtc_read(0x00);
    *minute = rtc_read(0x02);
    *hour   = rtc_read(0x04);
    *day    = rtc_read(0x07);
    *month  = rtc_read(0x08);
    *year   = rtc_read(0x09);

    // Convert from BCD if needed (check if RTC is in BCD mode)
    // For simplicity, assume BCD mode
    *second = bcd_to_bin(*second);
    *minute = bcd_to_bin(*minute);
    *hour   = bcd_to_bin(*hour);
    *day    = bcd_to_bin(*day);
    *month  = bcd_to_bin(*month);
    *year   = bcd_to_bin(*year);

    // Adjust year (RTC gives 2-digit year)
    if (*year < 70) {
        *year += 2000;
    } else {
        *year += 1900;
    }
}
