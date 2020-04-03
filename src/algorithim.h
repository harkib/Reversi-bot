#include <memory>
#include <vector>
#include <array>
#include <functional>
#include <random>



#ifndef __ALGO_HEADER__
#define __ALGO_HEADER__
#include "Node.h"
#include "Reversi2.h"


Reversi2::action_t monte_carlo(const Reversi2&, std::function<Reversi2::action_t(Reversi2&)>);

Reversi2::action_t random_h(Reversi2&);
Reversi2::action_t mobility_h(Reversi2&);
Reversi2::action_t blocking_h(Reversi2&);
Reversi2::action_t capture_h(Reversi2&);
Reversi2::action_t corner_h(Reversi2&);
Reversi2::action_t stability_h(Reversi2&);


#endif
