#ifndef TRK_BLOCKS_HH
#define TRK_BLOCKS_HH

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "trkutl.h"

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

            int         n_block() const;
            std::string blk_name(int i) const;
            BLK_STATE   get_state(int i) const;
        private:
            std::vector<std::string>    blk_names_;
            std::map<std::string, int > block_indexes_;
            std::vector<Block*>         blocks_;
    };

std::ostream&
operator<<( std::ostream& ostrm, const trk::Blocks& blocks);

}

#endif
