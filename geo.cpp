#include <vector>
#include <iostream>
#include <memory>
#include <utility>
#include <cmath>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <type_traits>

namespace geo {

class dimension_smaller_than_zero : public std::logic_error {
public:
  using std::logic_error::logic_error;
};

class shape {
public:
  virtual double get_circumference() = 0;
  virtual ~shape(){};
};

class circle : public shape {
  double r_;

public:
  circle(const double r) : r_(r) {
    if (r < 0)
      throw dimension_smaller_than_zero(
          "A circle must have a radius of at least 0.");
  }

  double get_circumference() override { return 8.L * std::atan(1) * r_; }
};

class square : public shape {
  double side_;

public:
  square(const double side) : side_(side) {
    if (side < 0)
      throw dimension_smaller_than_zero(
          "A square must have a length of at least 0.");
  }

  double get_circumference() override { return 4.L * side_; }
};

class equilateral_triangle : public shape {
  double side_;

public:
  equilateral_triangle(const double side) : side_(side) {
    if (side < 0)
      throw dimension_smaller_than_zero(
          "An equilateral_triangle must have a length of at least 0.");
  }
  double get_circumference() override { return 3.L * side_; }
};

class scene {
  std::vector<std::shared_ptr<shape>> shapes_;

public:
  template <class T, typename std::enable_if<
                         std::is_base_of<shape, T>::value>::type * = nullptr>
  void add_shape(std::shared_ptr<T> s) {
    shapes_.push_back(std::static_pointer_cast<shape>(s));
  }

  double get_circumference() {
    return std::accumulate(shapes_.begin(), shapes_.end(), 0.0L,
                           [](const double a, const std::shared_ptr<shape> b) {
                             return a + b->get_circumference();
                           });
    // This is really just shorthand for:
    //
    // double sum{};
    // for (auto shape: shapes_)
    // {
    //   sum += shape->get_circumference();
    // }
    // return sum;
  }
};
}

namespace {
void test_geo() {
  std::cerr << "Circumference of unity circle: "
            << geo::circle(1).get_circumference() << std::endl;
  std::cerr << "Circumference of unity square: "
            << geo::square(1).get_circumference() << std::endl;
  std::cerr << "Circumference of unity equilateral triangle: "
            << geo::equilateral_triangle(1).get_circumference() << std::endl;

  try {
    std::cerr << "Creating circle with radius -1" << std::endl;
    geo::circle(-1.0);
  } catch (const geo::dimension_smaller_than_zero &e) {
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }

  try {
    std::cerr << "Creating square with with length -1" << std::endl;
    geo::square(-1.0);
  } catch (const geo::dimension_smaller_than_zero &e) {
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }

  try {
    std::cerr << "Creating equilateral triangle with with length -1"
              << std::endl;
    geo::equilateral_triangle(-1.0);
  } catch (const geo::dimension_smaller_than_zero &e) {
    std::cerr << "Caught exception: " << e.what() << std::endl;
  }

  {
    geo::scene s;
    s.add_shape(std::make_shared<geo::circle>(1));
    s.add_shape(std::make_shared<geo::square>(1));
    std::cout << "Circumference of all unity shapes: " << s.get_circumference()
              << std::endl;
  }

  {
    geo::scene s;
    std::cout << "Circumference of the empty scene: " << s.get_circumference()
              << std::endl;
  }
}
}

int main() {
  try {
    test_geo();
  } catch (const geo::dimension_smaller_than_zero &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "A non-std::exception was thrown, but not caught."
              << std::endl;
  }
  return 0;
}

