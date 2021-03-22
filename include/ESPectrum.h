#ifndef ESPectrum_h
#define ESPectrum_h

#include "hardpins.h"

// Declared vars
#ifdef COLOR_3B
#include "ESP32Lib/VGA/VGA3Bit.h"
#include "ESP32Lib/VGA/VGA3BitI.h"
#define VGA VGA3Bit
#endif

#ifdef COLOR_6B
#include "ESP32Lib/VGA/VGA6Bit.h"
#include "ESP32Lib/VGA/VGA6BitI.h"
#define VGA VGA6Bit
#endif

#ifdef COLOR_14B
#include "ESP32Lib/VGA/VGA14Bit.h"
#include "ESP32Lib/VGA/VGA14BitI.h"
#define VGA VGA14Bit
#endif

class ESPectrum
{
public:
    // arduino setup/loop
    static void setup();
    static void loop();

    // graphics
    static VGA vga;
    static uint8_t borderColor;
    static void precalcColors();
    static uint16_t zxColor(uint8_t color, uint8_t bright);
    static void waitForVideoTask();

    static void processKeyboard();

private:
    static void videoTask(void* unused);
};

class Mem
{
public:
    static uint8_t* rom0;
    static uint8_t* rom1;
    static uint8_t* rom2;
    static uint8_t* rom3;

    static uint8_t* rom[4];

    static uint8_t* ram0;
    static uint8_t* ram1;
    static uint8_t* ram2;
    static uint8_t* ram3;
    static uint8_t* ram4;
    static uint8_t* ram5;
    static uint8_t* ram6;
    static uint8_t* ram7;

    static uint8_t* ram[8];

    static volatile uint8_t bankLatch;
    static volatile uint8_t videoLatch;
    static volatile uint8_t romLatch;
    static volatile uint8_t pagingLock;
    static uint8_t modeSP3;
    static uint8_t romSP3;
    static uint8_t romInUse;
};

extern "C" inline uint8_t readbyte(uint16_t addr) {
    uint8_t page = addr >> 14;
    switch (page) {
    case 0:
        return Mem::rom[Mem::romInUse][addr];
    case 1:
        return Mem::ram5[addr - 0x4000];
    case 2:
        return Mem::ram2[addr - 0x8000];
    case 3:
        return Mem::ram[Mem::bankLatch][addr - 0xC000];
    }
}

extern "C" inline uint16_t readword(uint16_t addr) { return ((readbyte(addr + 1) << 8) | readbyte(addr)); }

extern "C" inline void writebyte(uint16_t addr, uint8_t data)
{
    uint8_t page = addr >> 14;
    switch (page) {
    case 0:
        return;
    case 1:
        Mem::ram5[addr - 0x4000] = data;
        break;
    case 2:
        Mem::ram2[addr - 0x8000] = data;
        break;
    case 3:
        Mem::ram[Mem::bankLatch][addr - 0xC000] = data;
        break;
    }
    return;
}

extern "C" inline void writeword(uint16_t addr, uint16_t data) {
    writebyte(addr, (uint8_t)data);
    writebyte(addr + 1, (uint8_t)(data >> 8));
}

class Ports
{
public:
    // keyboard ports read from PS2 keyboard
    static volatile uint8_t base[128];

    // keyboard ports read from Wiimote
    static volatile uint8_t wii[128];
};



#endif