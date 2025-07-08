/**
 * @file displayTemplateStandard.h
 *
 * @brief Standard display template
 *
 */

#pragma once

/**
 * @brief Send data to display
 */
inline void printScreen() {

    // Show fullscreen brew timer:
    if (displayFullscreenBrewTimer()) {
        // Display was updated, end here
        return;
    }

    // Show fullscreen manual flush timer:
    if (displayFullscreenManualFlushTimer()) {
        // Display was updated, end here
        return;
    }

    // Show fullscreen hot water timer:
    if (displayFullscreenHotWaterTimer()) {
        // Display was updated, end here
        return;
    }

    // Print the machine state
    if (displayMachineState()) {
        // Display was updated, end here
        return;
    }

    // If no specific machine state was printed, print default:

    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_profont11_tf); // set font

    displayStatusbar();

    u8g2->setCursor(34, 16);
    u8g2->print(langstring_current_temp);
    u8g2->setCursor(84, 16);
    u8g2->print(temperature, 1);
    u8g2->setCursor(115, 16);
    u8g2->print(static_cast<char>(176));
    u8g2->print("C");
    u8g2->setCursor(34, 26);
    u8g2->print(langstring_set_temp);
    u8g2->setCursor(84, 26);
    u8g2->print(setpoint, 1);
    u8g2->setCursor(115, 26);
    u8g2->print(static_cast<char>(176));
    u8g2->print("C");

    displayThermometerOutline(4, 62);

    // Draw current temp in thermometer
    if (fabs(temperature - setpoint) < 0.3) {
        if (isrCounter < 500) {
            drawTemperaturebar(8, 30);
        }
    }
    else {
        drawTemperaturebar(8, 30);
    }

    // Brew and flush time
    if (config.get<bool>("hardware.switches.brew.enabled")) {
        // Show flush time
        if (machineState == kManualFlush) {
            u8g2->setDrawColor(0);
            u8g2->drawBox(34, 37, 100, 10);
            u8g2->setDrawColor(1);
            displayBrewTime(34, 36, langstring_manual_flush, currBrewTime);
        }
        else {
            if (shouldDisplayBrewTimer()) {
                if (config.get<bool>("brew.by_time") && config.get<int>("brew.mode") == 1) {
                    displayBrewTime(34, 36, langstring_brew, currBrewTime, totalTargetBrewTime);
                }
                else {
                    displayBrewTime(34, 36, langstring_brew, currBrewTime);
                }
            }
        }
    }

    // PID values over heat bar
    u8g2->setCursor(38, 47);

    u8g2->print(bPID.GetKp(), 0);
    u8g2->print("|");

    if (bPID.GetKi() != 0) {
        u8g2->print(bPID.GetKp() / bPID.GetKi(), 0);
    }
    else {
        u8g2->print("0");
    }

    u8g2->print("|");
    u8g2->print(bPID.GetKd() / bPID.GetKp(), 0);
    u8g2->setCursor(96, 47);

    if (pidOutput < 99) {
        u8g2->print(pidOutput / 10, 1);
    }
    else {
        u8g2->print(pidOutput / 10, 0);
    }

    u8g2->print("%");

    // Show heater output in %
    displayProgressbar(pidOutput / 10, 30, 60, 98);

    displayBufferReady = true;
}
