#ifndef SOFTWARE_SETTINGPERCENTS_H
#define SOFTWARE_SETTINGPERCENTS_H

enum class StrokePatterns {
    SimpleStroke,
    TeasingPounding,
    RoboStroke,
    HalfnHalf,
    Deeper,
    StopNGo,
    Insist,
};

struct SettingPercents {
    float speed = 0.0;
    float min = 50.0;
    float sensation = 50.0;
    float max = 10.0;  // default to 10% depth
    StrokePatterns pattern = StrokePatterns::SimpleStroke;
};

#endif  // SOFTWARE_SETTINGPERCENTS_H
