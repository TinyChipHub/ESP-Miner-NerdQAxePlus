#include "board.h"
#include "nerdoctaxeplus.h"

static const char* TAG="nerdoctaxe+";

NerdOctaxePlus::NerdOctaxePlus() : NerdQaxePlus() {
    m_deviceModel = "NerdOCTAXE+";
    m_miningAgent = m_deviceModel;
    m_asicCount = 8;
    m_numPhases = 3;
    m_imax = m_numPhases * 30;
    m_ifault = (float) (m_imax - 5);

    m_asicMaxDifficulty = 2048;
    m_asicMinDifficulty = 512;

    m_params.maxPin = 130.0;
    m_params.minPin = 70.0;
    m_params.maxVin = 13.0;
    m_params.minVin = 11.0;

#ifdef NERDOCTAXEPLUS
    m_theme = new ThemeNerdoctaxeplus();
#endif
}
