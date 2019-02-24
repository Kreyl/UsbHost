/*
 * main.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Kreyl
 */

#pragma once

struct StickSetup_t {
    Color_t Clr;
    uint8_t Vibro;
    StickSetup_t() : Clr({0,0,4}), Vibro(0) {}
    StickSetup_t& operator = (const StickSetup_t &Right) {
        Clr = Right.Clr;
        Vibro = Right.Vibro;
        return *this;
    }
};

extern StickSetup_t StickSetup[2];
