#include <iostream>
#include <ctime>

using namespace std; //ba estedafe azin in line dige niaz nist std bezanim aval har line

int main() {
    //tarikh mored nazar karbar ro migirim
    int year, month, day;
    cout << "Enter the date (YYYY MM DD): ";
    cin >> year >> month >> day;

    // zaman hal ro migire
    time_t now = time(nullptr);
    tm current_time = *localtime(&now);

    // sal mabda va  hadeagahl mah
    tm user_date = {0};
    user_date.tm_year = year - 1900; // Years since 1900
    user_date.tm_mon = month - 1;    // Months are 0-based
    user_date.tm_mday = day;

    // ekhtelaf zaman ro hesab mikone
    time_t user_time = mktime(&user_date);
    time_t time_diff = now - user_time;

    // tabdil sanie be rooz,mah,sal
    const int SECONDS_PER_DAY = 24 * 60 * 60; //24 saat dar rooz har daghighe 60 sanie va har saat 60 daghighe
    const int SECONDS_PER_MONTH = 30 * SECONDS_PER_DAY; //tedad mah=30
    const int SECONDS_PER_YEAR = 365 * SECONDS_PER_DAY;//tedad rooz=365

    int days_passed = time_diff / SECONDS_PER_DAY;
    int months_passed = time_diff / SECONDS_PER_MONTH;
    int years_passed = time_diff / SECONDS_PER_YEAR;
    int hours_passed = time_diff / 3600; //

    // natije
    cout << "Time difference:\n";
    cout << "Days: " << days_passed << "\n";
    cout << "Months: " << months_passed << "\n";
    cout << "Years: " << years_passed << "\n";
    cout << "Hours: " << hours_passed << "\n";
    cout << "Seconds: " << time_diff << "\n";

    return 0;
}
