/******************************************************************************\
 * DividedRange: An evenly spaced collection of points or boxes
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef DIVIDED_RANGE_H
#define DIVIDED_RANGE_H

#include "../indicator/Measure.h"
#include "../dims/Quantity.h"

namespace openworld {
  class DividedRange {
  protected:
    double min;
    double max;
    double widths;
    Indicator indicator;

  public:
    DividedRange(Indicator indicator)
    : indicator(indicator) {
      this->min = this->max = this->widths = 0;
    }

    DividedRange(double min, double max, double widths, Indicator indicator)
      : indicator(indicator) {
      initialize(min, max, widths);
    }

    void initialize(double min, double max, double widths) {
      this->min = min;
      this->max = max;
      this->widths = widths;
      this->indicator = indicator;
    }

    Measure getMin() const {
      return Measure(min, indicator);
    }

    Measure getMax() const {
      return Measure(max, indicator);
    }

    Quantity getWidths() const {
      return Quantity(widths, indicator.getDimensions());
    }

    friend ostream& operator<<(ostream& out, const DividedRange& xx) {
      out << "[" << xx.min << ":" << xx.widths << ":" << xx.max << " " << xx.indicator << "]";
      return out;
    }

    int inRange(double check) {
      if (check >= min && check < max)
        return (int) ((check - min) / widths);
      return -1;
    }

    int inRange(Measure check) {
      if (check.getIndicator() != indicator)
        throw runtime_error("Indicator does not match");

      return inRange(check.getValue());
    }

    unsigned count() const {
      return (int) ((max - min) / widths);
    }

    DividedRange intersect(DividedRange dr) {
      if (widths != dr.widths || indicator != dr.indicator)
        throw runtime_error("Intersection with differently sampled range");

      return DividedRange(min > dr.min ? min : dr.min, max > dr.max ? dr.max : max, widths, indicator);
    }

    static time_t toTime(int year, int month = 1, int day = 1, int hour = 0, int min = 0, int sec = 0) {
      struct tm time;
      time.tm_sec = sec;
      time.tm_min = min;
      time.tm_hour = hour;
      time.tm_mday = day;
      time.tm_mon = month;
      time.tm_year = year;

      return mktime(&time);
    }

    static unsigned toTimespan(int day = 0, int hour = 0, int min = 0, int sec = 0) {
      return ((day * 24 + hour) * 60 + min) * 60 + sec;
    }

    static DividedRange withEnds(double minpt, double maxpt, double widths, Indicator indicator) {
      // check if we should add an extra point:
      if ((maxpt - minpt) / widths - (int) ((maxpt - minpt) / widths) > .9)
        maxpt += widths / 10;
      return DividedRange(minpt - widths/2, maxpt + widths/2, widths, indicator);
    }

    static DividedRange withMax(double min, double maxpt, double widths, Indicator indicator) {
      // check if we should add an extra point:
      if ((maxpt - min) / widths - (int) ((maxpt - min) / widths) > .9)
        maxpt += widths / 10;
      return DividedRange(min, maxpt + widths, widths, indicator);
    }

    // Operators
    
    bool operator==(const DividedRange& b) const {
      return (min == b.min && max == b.max && widths == b.widths && indicator == b.indicator);
    }

    bool operator!=(const DividedRange& b) const {
      return !(*this == b);
    }
  };
}

#endif
