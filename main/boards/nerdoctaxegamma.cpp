#include "board.h"
#include "nerdoctaxegamma.h"
#include "nerdqaxeplus2.h"

static const char* TAG="nerdqaxeplus2";

NerdOctaxeGamma::NerdOctaxeGamma() : NerdQaxePlus2() {
    m_deviceModel = "NerdOCTAXE-γ";
    m_miningAgent = m_deviceModel;
    m_asicModel = "BM1370";
    m_asicCount = 8;
    m_numPhases = 4;
    m_imax = 180;
    m_ifault = 140.0;

    // use m_asicVoltage for init
    m_initVoltage = 0.0;

    m_params.maxPin = 200.0;
    m_params.minPin = 100.0;
    m_params.maxVin = 13.0;
    m_params.minVin = 11.0;

    m_asicMaxDifficulty = 4096;
    m_asicMinDifficulty = 1024;

#ifdef NERDOCTAXEGAMMA
    m_theme = new ThemeNerdoctaxegamma();
#endif
}
