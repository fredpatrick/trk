#include <iostream>
#include <string>
#include <bitset>

#include "filestore.h"
#include "debugcntl.h"
#include "trkutl.h"
#include "layoutconfig.h"
#include "jobclock.h"
#include "enablepcb.h"
#include "gpio.h"

using namespace trk;

int main(int argc, char* argv[]) {
    int debug_level = trk::debug_level(argc, argv);
    std::string homedir = "/home/fredpatrick/";
    std::string cfg_filnam = homedir + "wrk/cfg/layout_config.txt";
    std::string todts = JobClock::instance()->tod_timestamp();
    std::string dbg_filnam = homedir + "wrk/log/" + todts + "_trkDriver.txt";
    FileStore* fs = FileStore::instance();
    fs->cfgfil(cfg_filnam);
    fs->dbgfil(dbg_filnam);
    DebugCntl* dbg = DebugCntl::instance();
    dbg->level(debug_level);
    JobClock*      job_clock = JobClock::instance();
    EnablePCB*     pcb       = EnablePCB::instance();
    LayoutConfig* layout_config = LayoutConfig::instance();
    OutputGPIO* gpios[5];
    int gpio_num = layout_config->demux_address_gpio_num("a0");
    gpios[0] = new OutputGPIO(gpio_num);
    gpio_num  = layout_config->demux_address_gpio_num("a1");
    gpios[1] = new OutputGPIO(gpio_num);
    gpio_num  = layout_config->demux_address_gpio_num("a2");
    gpios[2] = new OutputGPIO(gpio_num);
    gpio_num  = layout_config->demux_address_gpio_num("a3");
    gpios[3] = new OutputGPIO(gpio_num);
    gpio_num  = layout_config->demux_address_gpio_num("a4");
    gpios[4] = new OutputGPIO(gpio_num);

    double t0 = job_clock->job_time();
    for ( int i = 0; i < 10; i++) {
        std::bitset<8 * sizeof(int)> b = 1;
        gpios[4]->set_value(1);
        gpios[0]->set_value( (int)b[0] );
        gpios[1]->set_value( (int)b[1] );
        gpios[2]->set_value( (int)b[2] );
        gpios[3]->set_value( (int)b[3] );
        if (gpios[4]->value() != 1 ) std::cout << "error on value read, 4" << std::endl;
        if (gpios[0]->value() != (int)b[0] ) std::cout << "error on value read, 0" << std::endl;
        if (gpios[1]->value() != (int)b[1] ) std::cout << "error on value read, 1" << std::endl;
        if (gpios[2]->value() != (int)b[2] ) std::cout << "error on value read, 2" << std::endl;
        if (gpios[3]->value() != (int)b[3] ) std::cout << "error on value read, 3" << std::endl;
        gpios[4]->set_value(0);
        if (gpios[4]->value() != 0 ) std::cout << "error on value read, 4" << std::endl;
    }
    int nt = 10 * 6;
    double t1 = job_clock->job_time();
    double dt = t1 - t0;
    std::cout << " deltat = " << dt << ", time/per write = " << dt/nt << std::endl;
    return 0;
}

