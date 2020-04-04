#include <memory>
#include <vector>
#include <array>
#include <functional>
#include <random>



#ifndef __ALGO_HEADER__
#define __ALGO_HEADER__
#include "Node.h"
#include "Reversi2.h"

typedef std::unique_ptr<Node>* child_ptr;

child_ptr monte_carlo(const Reversi2&, std::function<child_ptr(Reversi2&)>);

child_ptr random_h(Reversi2&);
child_ptr mobility_h(Reversi2&);
child_ptr blocking_h(Reversi2&);
child_ptr capture_h(Reversi2&);
child_ptr corner_h(Reversi2&);
child_ptr stability_h(Reversi2&);


#endif
