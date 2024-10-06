#include <iostream>
#include <chrono>

// g++ main.cpp -o main -std=c++20

std::timespec timespec_now() {
  std::timespec ts;
  std::timespec_get(&ts, TIME_UTC);
  return ts;
}

std::timespec timepoint2timespec(std::chrono::system_clock::time_point const& tp) {
  return std::timespec{
    std::chrono::system_clock::to_time_t( tp ),
    std::chrono::nanoseconds{
      tp - std::chrono::time_point_cast<std::chrono::seconds>(tp)
    }.count()
  };
}

std::chrono::system_clock::time_point
timespec2timepoint(std::timespec const& ts) {
  return std::chrono::system_clock::time_point{
    std::chrono::seconds{ ts.tv_sec } +
    std::chrono::nanoseconds{ ts.tv_nsec }
  };
}

std::string to_string(std::chrono::system_clock::time_point const& tp)
{
  std::time_t t { std::chrono::system_clock::to_time_t( tp ) };
  std::tm* tmPtr = std::gmtime( &t );

  char tmp[30]{}; // size for storing "yyyy-mm-dd hh:mm:ss.nnnnnnnnn\0"
  const char format[] = "%F %T.";
  std::strftime(tmp, std::size(tmp), format, tmPtr);

  std::chrono::nanoseconds ns {
    tp - std::chrono::time_point_cast<std::chrono::seconds>(tp)
  };

  return std::string(tmp) + std::to_string( ns.count() );
}

std::string to_string(std::timespec const& ts) {
  const std::time_t t { ts.tv_sec };
  const std::tm* tmPtr = std::gmtime( &t );

  char tmp[30]{}; // size for storing "yyyy-mm-dd hh:mm:ss.nnnnnnnnn\0"
  const char format[] = "%F %T.";
  std::strftime(tmp, std::size(tmp), format, tmPtr);

  return std::string(tmp) + std::to_string(ts.tv_nsec);
}

int main()
{
  auto tp = std::chrono::system_clock::now(); // get time_point
  auto ts = timepoint2timespec(tp); // check conversion tp -> ts

  // Check if std::operator<<() to_string(tp) and to_string(ts) are equal
  std::cout << tp << std::endl;
  std::cout << to_string( tp ) << std::endl;
  std::cout << to_string( ts ) << std::endl;

  // Check conversion ts -> tp
  tp = timespec2timepoint(ts);
  std::cout << to_string(tp) << std::endl;

  // Output must be the same for every print
}