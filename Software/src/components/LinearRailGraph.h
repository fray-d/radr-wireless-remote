#pragma once

#ifndef LINEARRAILGRAPH_H
#define LINEARRAILGRAPH_H

#include <Arduino.h>

#include <Adafruit_MCP23X17.h>
#include <Adafruit_ST77xx.h>
#include <constants.h>
#include <memory>
#include <vector>

#include "DisplayObject.h"
#include "esp_log.h"
#include "services/display.h"

class LinearRailGraph : public DisplayObject {
  private:
    float *minValue;
    float *maxValue;
    int lastMinValue;
    int lastMaxValue;
    std::unique_ptr<GFXcanvas16> canvas;

  public:
    struct Props {
        float *minValue;
        float *maxValue;
        int16_t x = -1;
        int16_t y = -1;
        int16_t w = 90;
        int16_t h = 30;
    };
    explicit LinearRailGraph(const Props &props)
        : DisplayObject((props.x == -1) ? (DISPLAY_WIDTH / 2 - props.w / 2) : props.x,
                        (props.y == -1) ? (DISPLAY_HEIGHT / 2 - props.h / 2) : props.y, props.w, props.h),
          minValue(props.minValue),
          maxValue(props.maxValue),
          canvas(std::make_unique<GFXcanvas16>(props.w, props.h)) {}

    bool shouldDraw() override {
        if (minValue != nullptr && constrain((int)roundf(*minValue), 0, 100) != lastMinValue) {
            return true;
        }
        if (maxValue != nullptr && constrain((int)roundf(*maxValue), 0, 100) != lastMaxValue) {
            return true;
        }
        return false;
    }

    void draw() override {
        if (xSemaphoreTake(displayMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
            int minPos = 0;
            int maxPos = 0;
            if (minValue != nullptr && maxValue != nullptr && width > 0 and height > 0) {
                int usableWidth = width - height;
                lastMinValue = (int)roundf(*minValue);
                minPos = usableWidth * roundf(*minValue) / 100.0f;
                lastMaxValue = (int)roundf(*maxValue);
                maxPos = usableWidth * roundf(*maxValue) / 100.0f;
            }
            int rounding = min(height, width) / 2;
            int thickness = maxPos - minPos + height;
            thickness = max(thickness, int(height));

            canvas->fillRect(0, 0, width, height, ST77XX_BLACK);
            canvas->fillRoundRect(minPos, 1, thickness, height - 2, rounding - 1, COLOR_WHITE);
            canvas->drawRoundRect(0, 0, width, height, rounding, COLOR_WHITE);
            tft.drawRGBBitmap(x, y, canvas->getBuffer(), canvas->width(), canvas->height());
        }
        xSemaphoreGive(displayMutex);
    }
};

// Initialize static member
#endif