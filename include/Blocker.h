#ifndef TRK_BLOCKER_HH
#define TRK_BLOCKER_HH

#include <string>

namespace trk  
{
    class DemuxAddress;;

    class Blocker
    {
        public:
            Blocker(const std::string& blk_name);
            ~Blocker();

            bool        set();
            bool        clear();

        private:
            std::string     blk_name_;
            int             blk_index_;
            int             blk_base_addr_;

            DemuxAddress*   demux_address_;
    };
}

#endif
