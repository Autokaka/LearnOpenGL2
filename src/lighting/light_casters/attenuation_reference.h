/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/25.
 */

#include <vector>

class AttenuationReference final {
 public:
  static AttenuationReference GetProperModel(float distance) {
    if (distance <= references_.begin()->distance) {
      return *references_.begin();
    };

    for (auto it = references_.begin(); it < references_.end(); ++it) {
      if (it == references_.end() - 1) {
        break;
      }

      if (distance >= it->distance && distance < (it + 1)->distance) {
        const auto left = distance - it->distance;
        const auto right = (it + 1)->distance - distance;
        return left < right ? *it : *(it + 1);
      }
    }

    return *(references_.end() - 1);
  }

  AttenuationReference(float distance,
                       float constant,
                       float linear,
                       float quadratic)
      : distance(distance),
        constant(constant),
        linear(linear),
        quadratic(quadratic) {}

  const float distance;
  const float constant;
  const float linear;
  const float quadratic;

 private:
  static const std::vector<AttenuationReference> references_;
};

const std::vector<AttenuationReference> AttenuationReference::references_ = {
    {7, 1.0, 0.7, 1.8},        {13, 1.0, 0.35, 0.44},
    {20, 1.0, 0.22, 0.20},     {32, 1.0, 0.14, 0.07},
    {50, 1.0, 0.09, 0.032},    {65, 1.0, 0.07, 0.017},
    {100, 1.0, 0.045, 0.0075}, {160, 1.0, 0.027, 0.0028},
    {200, 1.0, 0.022, 0.0019}, {325, 1.0, 0.014, 0.0007},
    {600, 1.0, 0.007, 0.0002}, {3250, 1.0, 0.0014, 0.000007},
};
