#ifndef TRK_BLOCKERS_HH
#define TRK_BLOCKERS_HH

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace trk
{
    class Blocker;

    class Blockers
    {
        public:
            Blockers();
            ~Blockers();

            bool        blockit(const std::string& blk_name);
            bool        clearit(const std::string& blk_name);
        private:
            std::vector<std::string> blk_names_;
            std::map<std::string, Blocker*> blockers_;
    };
}

#endif
