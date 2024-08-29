#include <exception>
#include <iostream>
#include <fstream>
#include <string_view>
#include <ctime>
#include <chrono>
#include <cassert>

 
template<typename T>
inline void log_assert(T&& assertion, string_view log_msg)
{
    if (assertion == true)
    {
        auto current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        ifstream crash_dump;
        crash_dump.open("error_log.txt");


        if (!crash_dump)
        {
            ofstream MyFile("error_log.txt");
            MyFile << ctime(&current_time) << "---" << log_msg << endl;
        }
        else
        {
            ofstream MyFile("error_log.txt", ios_base::app);
            MyFile << ctime(&current_time) << "---" << log_msg << endl;
            MyFile.close();
        }
        assert(!assertion);
    }
}