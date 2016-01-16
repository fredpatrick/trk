

#include "switcher.h"
#include <iostream>
#include <bitset>

using namespace std;

trk::Switcher::
Switcher(GPIO** gpios)
{
    gpio_a0 = gpios[0];
    gpio_a1 = gpios[1];
    gpio_a2 = gpios[2];
    gpio_a3 = gpios[3];
    gpio_a4 = gpios[4];
}

trk::Switcher::
~Switcher()
{
}

void
trk::Switcher::
flip_switch(int switch_index,
            int pos)
{
    int line_index = switch_index * 2  + pos;
    std::bitset<8 * sizeof(int)> b = line_index;

    cout << "switch_index = " << switch_index << ",position = " <<
                  pos << endl;
    cout << "                   line_index = " << line_index << endl;
    cout << b[0] << " " << b[1] << " " << b[2] << " " << b[3] << endl;
    gpio_a0->value( GPIO_VALUE((int)b[0]) );
    gpio_a1->value( GPIO_VALUE((int)b[1]) );
    gpio_a2->value( GPIO_VALUE((int)b[2]) );
    gpio_a3->value( GPIO_VALUE((int)b[3]) );
    gpio_a4->value( GPIO_VALUE((int)b[4]) );
    cout << "Switch values set" << endl;
    cout << gpio_a0->value() << endl;
    cout << gpio_a1->value() << endl;
    cout << gpio_a2->value() << endl;
    cout << gpio_a3->value() << endl;
    cout << gpio_a4->value() << endl;

    usleep (500000);
    cout << "waking up" << endl;
    gpio_a4->value( HIGH);
    cout << gpio_a0->value() << endl;
    cout << gpio_a1->value() << endl;
    cout << gpio_a2->value() << endl;
    cout << gpio_a3->value() << endl;
    cout << gpio_a4->value() << endl;
    return;
}

void
trk::Switcher::
reset()
{
    cout << "trk::Switcher::reset" << endl;
    gpio_a0->value( LOW );
    gpio_a1->value( LOW );
    gpio_a2->value( LOW );
    gpio_a3->value( LOW );
    gpio_a4->value( HIGH);
}

