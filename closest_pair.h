#ifndef CLOSEST_PAIR_H_
#define CLOSEST_PAIR_H_

#include <array>
#include <iostream>
#include <cstdint>
#include <utility>
#include <vector>
#include <random>
#include <algorithm>

using std::int32_t;

namespace ca    {

    extern void test_closest_pair();

    // 'T' should have well define '<' operator
    template <typename T>
    struct point_t  {
        point_t(T px = T(), T py = T())
            : x(px),
            y(py)   {}

        std::ostream& print(std::ostream& os) const   {
            os << "(" << x << ", " << y << ")";
            return os;
        }
        
        T x;
        T y;
    };

    template <typename T>
        std::ostream& operator << (std::ostream& os, const point_t<T>& p)    {
            p.print(os);
            return os;
        }

    template <typename T>
        bool x_less_than(const point_t<T>& p1, const point_t<T>& p2)  {
            return p1.x < p2.x;
        }

    template <typename T>
        bool y_less_than(const point_t<T>& p1, const point_t<T>& p2)  {
            return p1.y < p2.y;  
        }

    typedef point_t<int32_t> ipoint_t;
    typedef point_t<float> fpoint_t;
    typedef point_t<double> dpoint_t;

    struct gen_fpoint_t  {

        gen_fpoint_t(float minf = 0.0, float maxf = 1.0)
            : gen(rd()),
            dis(minf, maxf) {}

        gen_fpoint_t(const gen_fpoint_t& g) {
            std::uniform_real_distribution<>::param_type p(g.dis.a(), g.dis.b());
            dis.param(p);                
        }

        void swap(gen_fpoint_t& g) {
            std::uniform_real_distribution<>::param_type pg(g.dis.a(), g.dis.b());
            std::uniform_real_distribution<>::param_type pm(dis.a(), dis.b());
            dis.param(pg);
            g.dis.param(pm);
        }

        gen_fpoint_t& operator = (const gen_fpoint_t& g)    {
            auto t  = g;
            swap(t);
            return *this;
        }

        fpoint_t operator ()  ()    {
            return fpoint_t(dis(gen), dis(gen));
        }

        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dis;
    };
}

#endif
