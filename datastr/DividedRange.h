/******************************************************************************\
 * DividedRange: An evenly spaced collection of points or boxes
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef DIVIDED_RANGE_H
#define DIVIDED_RANGE_H

#include "../measure/Measure.h"
#include "../measure/Units.h"
#include "../measure/Quantity.h"

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

    double getMinOPT() const {
      return min;
    }

    double getMaxOPT() const {
      return max;
    }

    Quantity getWidths() const {
      return Quantity(widths, indicator.getUnit());
    }

    Measure getCellMin(unsigned ii) const {
      return Measure(min + ii * widths, indicator);
    }

    Measure getCellMax(unsigned ii) const {
      return Measure(min + (ii + 1) * widths, indicator);
    }

    Measure getCellCenter(unsigned ii) const {
      return Measure(min + (ii + .5) * widths, indicator);
    }

    friend ostream& operator<<(ostream& out, const DividedRange& xx) {
      out << "[" << xx.min << ":" << xx.widths << ":" << xx.max << " " << xx.indicator << "]";
      return out;
    }

    int inRange(double check) const {
      if ((widths > 0 && check >= min && check < max) || (widths < 0 && check <= min && check > max))
        return (int) ((check - min) / widths);
      return -1;
    }

    int inRange(Measure check) const {
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

      if (widths > 0)
        return DividedRange(min > dr.min ? min : dr.min, max > dr.max ? dr.max : max, widths, indicator);
      else
        return DividedRange(min < dr.min ? min : dr.min, max < dr.max ? dr.max : max, widths, indicator);
    }

    static time_t toTime(int year, int month = 1, int day = 1, int hour = 0, int min = 0, int sec = 0) {
      struct tm time;
      time.tm_sec = sec;
      time.tm_min = min;
      time.tm_hour = hour;
      time.tm_mday = day;
      time.tm_mon = month - 1;
      time.tm_year = year - 1900;

      return mktime(&time);
    }

    static Quantity toTimespan(int day = 0, int hour = 0, int min = 0, int sec = 0) {
      return Quantity(((day * 24 + hour) * 60 + min) * 60 + sec, Units::s);
    }

    static DividedRange rounded(double minpt, double maxpt, double widths, Indicator indicator) {
      // check if we should add an extra point:
      if (abs((maxpt - minpt) / widths - (int) ((maxpt - minpt) / widths)) > .9)
        maxpt += widths / 10;

      return DividedRange(minpt, maxpt, widths, indicator);
    }

    static DividedRange withEnds(double minpt, double maxpt, double widths, Indicator indicator) {
      // check if we should add an extra point:
      if (abs((maxpt - minpt) / widths - (int) ((maxpt - minpt) / widths)) > .9)
        maxpt += widths / 10;
      return DividedRange(minpt - widths/2, maxpt + widths/2, widths, indicator);
    }

    static DividedRange withMax(double min, double maxpt, double widths, Indicator indicator) {
      // check if we should add an extra point:
      if (abs((maxpt - min) / widths - (int) ((maxpt - min) / widths)) > .9)
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

    // Serialization

    virtual ostream& streamInsert(ostream& os) const {
      return indicator.streamInsert(os) << min << " " << max << " " << widths << " ";
    }

    static DividedRange streamExtract(istream& in) {
      Indicator indicator = Indicator::streamExtract(in);
      double min, max, widths;
      in >> min >> max >> widths;

      return DividedRange(min, max, widths, indicator);
    }

    template<class T> friend class GeographicMap;
    template<class T> friend class ScaledGeographicMap;
  };
}

#endif
