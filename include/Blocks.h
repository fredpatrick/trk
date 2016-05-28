#ifndef TRK_BLOCKS_HH
#define TRK_BLOCKS_HH

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace trk
{
    class Block;

    class Blocks
    {
        public:
            Blocks();
            ~Blocks();

            bool        blockit(const std::string& blk_name);
            bool        clearit(const std::string& blk_name);
            bool        enable_sensors(int sensor_fd, int& n_event);
        private:
            std::vector<std::string> blk_names_;
            std::map<std::string, Block*> blocks_;
    };
}

#endif
