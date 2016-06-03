#ifndef TRK_TRKUTL_HH
#define TRK_TRKUTL_HH

#include <iostream>
#include <string>

namespace trk
{

enum SW_DIRECTION {THRU, OUT, NOVAL };

enum TrackState { LowerLoopCW, LowerLoopCCW,
                  UpperLoopCW, UpperLoopCCW };

struct SWKey {
    int             num; 
    SW_DIRECTION    swd;

    bool
    operator< (const SWKey& k2) const
    {
        int a = num * 2 + static_cast<int>(swd);;
        int b = k2.num * 2 + static_cast<int>(k2.swd);;

        if ( a < b) return true;
        else        return false;
    }

    bool
    operator== (const SWKey& k2) const
    {
        if ( num == k2.num && swd == k2.swd ) return true;
        else                                  return false;
    }
};

std::string get_yesno(const std::string& question);

SWKey      get_switch();

}

std::istream&
operator>>( std::istream& is, trk::SW_DIRECTION& idirec);

std::ostream&
operator<<( std::ostream& ostrm, const trk::SW_DIRECTION& idirec);

std::istream&
operator>>( std::istream& is, trk::TrackState& idirec);

std::ostream&
operator<<( std::ostream& ostrm, const trk::TrackState& idirec);

std::ostream&
operator<<( std::ostream& ostrm, const trk::SWKey& kwy);

#endif
