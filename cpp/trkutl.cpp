
#include <iostream>
#include "trkutl.h"

std::string
trk::get_yesno( const std::string& question ) 
{
    std::string yesno;
    bool got_ans = false;
    while ( !got_ans ) {
        std::cout << "trkDriver: " << question << "(yes/no):" << std::endl;
        std::cin >> yesno;
        if ( yesno == "yes" || yesno == "no" ) {
            got_ans = true;
        } else {
            got_ans = false;
        }
    }
    return yesno;
}

trk::SWKey
trk::get_switch ()
{
    SWKey key;
    bool got_ans = false;
    while ( !got_ans ) {
        std::cout << "get_switch:enter sw_num (-1:5), sw_direc (THRU/OUT): " << std::endl;
        std::cin >> key.num;
        if ( std::cin.fail() ) {
            std::cout << "Bad read on sw_num" << std::endl;
            std::cin.clear();
            std::cin.ignore(100, '\n');
            continue;
        }
        std::cout << key.num << std::endl;
        if ( key.num >= -1 && key.num <= 5 ) {
            if ( key.num == -1 ) {
                got_ans = true;
            } else {
                std::cin >> key.swd;
                if ( key.swd == THRU || key.swd == OUT ) {
                    std::cout << "get_switch, switch key = " << key  << std::endl;
                    got_ans = true;
                } else {
                    got_ans = false;
                }
            }
        }
    }
    return key;
}

std::istream& 
operator>>( std::istream& is, trk::SW_DIRECTION& idirec)
{
    std::string tmp;
    if ( is >> tmp ) {
        if ( tmp == "THRU") {
            idirec = trk::THRU;
        } else if ( tmp == "OUT" ) {
            idirec = trk::OUT;
        } else {
            idirec = trk::NOVAL;
        }
    }
    return is;
}

std::ostream&
operator<<( std::ostream& ostrm, const trk::SW_DIRECTION& idirec)
{
    if ( idirec == trk::THRU )      ostrm << "THRU";
    else if ( idirec == trk::OUT )   ostrm << "OUT";
    else if ( idirec == trk::NOVAL)  ostrm << "NOVAL";
    return ostrm;
}

std::istream& 
operator>>( std::istream& is, trk::TrackState& trkstate)
{
    std::string tmp;
    if ( is >> tmp ) {
        if ( tmp == "LowerLoopCW") {
            trkstate = trk::LowerLoopCW;
        } else if ( tmp == "LowerLoopCCW" ) {
            trkstate = trk::LowerLoopCCW;
        } else if ( tmp == "UpperLoopCW" ) {
            trkstate = trk::UpperLoopCW;
        } else if ( tmp == "UpperLoopCCW" ) {
            trkstate = trk::UpperLoopCCW;
        }
    }
    return is;
}

std::ostream&
operator<<( std::ostream& ostrm, const trk::TrackState& trkstate)
{
    if (      trkstate == trk::LowerLoopCW  )   ostrm << "LowerLoopCW";
    else if ( trkstate == trk::LowerLoopCCW )   ostrm << "LowerLoopCCW";
    else if ( trkstate == trk::UpperLoopCW  )   ostrm << "UpperLoopCW";
    else if ( trkstate == trk::UpperLoopCCW )   ostrm << "UpperLoopCCW";
    return ostrm;
}

std::ostream&
operator<<( std::ostream& ostrm, const trk::SWKey& key)
{
    ostrm << "trk::SWKey: " << key.num << '\t' << key.swd;
    return ostrm;
}
