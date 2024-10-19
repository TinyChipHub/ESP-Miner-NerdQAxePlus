#include "board.h"
#include "nerdoctaxeplus.h"

static const char* TAG="nerdoctaxe+";

NerdOctaxePlus::NerdOctaxePlus() : NerdQaxePlus() {
    m_deviceModel = "NerdOCTAXE+";
    m_asicCount = 8;
    m_numPhases = 3;

    m_asicMaxDifficulty = 2048;
    m_asicMinDifficulty = 512;

    m_maxPin = 130.0;
    m_minPin = 70.0;
    m_maxVin = 13.0;
    m_minVin = 11.0;

    m_asicJobIntervalMs = 750;

#ifdef NERDOCTAXEPLUS
    m_theme = new ThemeNerdoctaxeplus();
#endif
}
