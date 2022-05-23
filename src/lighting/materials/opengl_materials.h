/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/23.
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "base/macros.h"

class Materials final {
 public:
  class Material final {
   public:
    glm::vec3 ambient{0.0f};
    glm::vec3 diffuse{0.0f};
    glm::vec3 specular{0.0f};
    float shininess = 32.0f;
  };

  static constexpr Material kEmerald{{0.0215, 0.1745, 0.0215},
                                     {0.07568, 0.61424, 0.07568},
                                     {0.633, 0.727811, 0.633},
                                     32};
  static constexpr Material kJade{{0.135, 0.2225, 0.1575},
                                  {0.54, 0.89, 0.63},
                                  {0.316228, 0.316228, 0.316228},
                                  32};
  static constexpr Material kObsidian{{0.05375, 0.05, 0.06625},
                                      {0.18275, 0.17, 0.22525},
                                      {0.332741, 0.328634, 0.346435},
                                      32};
  static constexpr Material kPearl{{0.25, 0.20725, 0.20725},
                                   {1, 0.829, 0.829},
                                   {0.296648, 0.296648, 0.296648},
                                   32};
  static constexpr Material kRuby{{0.1745, 0.01175, 0.01175},
                                  {0.61424, 0.04136, 0.04136},
                                  {0.727811, 0.626959, 0.626959},
                                  32};
  static constexpr Material kTurquoise{{0.1, 0.18725, 0.1745},
                                       {0.396, 0.74151, 0.69102},
                                       {0.297254, 0.30829, 0.306678},
                                       32};
  static constexpr Material kBrass{{0.329412, 0.223529, 0.027451},
                                   {0.780392, 0.568627, 0.113725},
                                   {0.992157, 0.941176, 0.807843},
                                   32};
  static constexpr Material kBronze{{0.2125, 0.1275, 0.054},
                                    {0.714, 0.4284, 0.18144},
                                    {0.393548, 0.271906, 0.166721},
                                    32};
  static constexpr Material kChrome{{0.25, 0.25, 0.25},
                                    {0.4, 0.4, 0.4},
                                    {0.774597, 0.774597, 0.774597},
                                    32};
  static constexpr Material kCopper{{0.19125, 0.0735, 0.0225},
                                    {0.7038, 0.27048, 0.0828},
                                    {0.256777, 0.137622, 0.086014},
                                    32};
  static constexpr Material kGold{{0.24725, 0.1995, 0.0745},
                                  {0.75164, 0.60648, 0.22648},
                                  {0.628281, 0.555802, 0.366065},
                                  32};
  static constexpr Material kSilver{{0.19225, 0.19225, 0.19225},
                                    {0.50754, 0.50754, 0.50754},
                                    {0.508273, 0.508273, 0.508273},
                                    32};
  static constexpr Material kBlackPlastic{{0.0, 0.0, 0.0},
                                          {0.01, 0.01, 0.01},
                                          {0.50, 0.50, 0.50},
                                          32};
  static constexpr Material kCyanPlastic{{0.0, 0.1, 0.06},
                                         {0.0, 0.50980392, 0.50980392},
                                         {0.50196078, 0.50196078, 0.50196078},
                                         32};
  static constexpr Material kGreenPlastic{{0.0, 0.0, 0.0},
                                          {0.1, 0.35, 0.1},
                                          {0.45, 0.55, 0.45},
                                          32};
  static constexpr Material kRedPlastic{{0.0, 0.0, 0.0},
                                        {0.5, 0.0, 0.0},
                                        {0.7, 0.6, 0.6},
                                        32};
  static constexpr Material kWhitePlastic{{0.0, 0.0, 0.0},
                                          {0.55, 0.55, 0.55},
                                          {0.70, 0.70, 0.70},
                                          32};
  static constexpr Material kYellowPlastic{{0.0, 0.0, 0.0},
                                           {0.5, 0.5, 0.0},
                                           {0.60, 0.60, 0.50},
                                           32};
  static constexpr Material kBlackRubber{{0.02, 0.02, 0.02},
                                         {0.01, 0.01, 0.01},
                                         {0.4, 0.4, 0.4},
                                         32};
  static constexpr Material kCyanRubber{{0.0, 0.05, 0.05},
                                        {0.4, 0.5, 0.5},
                                        {0.04, 0.7, 0.7},
                                        32};
  static constexpr Material kGreenRubber{{0.0, 0.05, 0.0},
                                         {0.4, 0.5, 0.4},
                                         {0.04, 0.7, 0.04},
                                         32};
  static constexpr Material kRedRubber{{0.05, 0.0, 0.0},
                                       {0.5, 0.4, 0.4},
                                       {0.7, 0.04, 0.04},
                                       32};
  static constexpr Material kWhiteRubber{{0.05, 0.05, 0.05},
                                         {0.5, 0.5, 0.5},
                                         {0.7, 0.7, 0.7},
                                         32};
  static constexpr Material kYellowRubber{{0.05, 0.05, 0.0},
                                          {0.5, 0.5, 0.4},
                                          {0.7, 0.7, 0.04},
                                          32};

  static const std::vector<Material> ToVector() {
    return {
        kEmerald,      kJade,          kObsidian,     kPearl,
        kRuby,         kTurquoise,     kBrass,        kBronze,
        kChrome,       kCopper,        kGold,         kSilver,
        kBlackPlastic, kCyanPlastic,   kGreenPlastic, kRedPlastic,
        kWhitePlastic, kYellowPlastic, kBlackRubber,  kCyanRubber,
        kGreenRubber,  kRedRubber,     kWhiteRubber,  kYellowRubber,
    };
  }

 private:
  Materials() = default;
  DISALLOW_COPY_ASSIGN_AND_MOVE(Materials);
};
