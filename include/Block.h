#ifndef TRK_BLOCK_HH
#define TRK_BLOCK_HH

#include <string>

namespace trk  
{
    class DemuxAddress;;
    class InputGPIO;
    class BlockSensor;


    class Block
    {
        public:
            Block(const std::string& blk_name);
            ~Block();

            bool        enable_sensor( int sensor_fd, int& n_event);
            bool        set();
            bool        clear();
            int         state();
            std::string blk_name();

        private:
            std::string     blk_name_;
            int             blk_index_;
            int             blk_base_addr_;
            InputGPIO*      gpio_;
            BlockSensor*    blk_sensor_;

            DemuxAddress*   demux_address_;
    };
}

#endif
