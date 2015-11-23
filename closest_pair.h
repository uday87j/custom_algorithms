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
    extern void test_cp_methods();

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

    // DISTRIBUTION = uniform_real_distribution or uniform_int_distribution
    template <typename T = int32_t, typename DISTRIBUTION = std::uniform_int_distribution<> >
    struct gen_point_t  {

        gen_point_t(const T minf = T(), const T maxf = T())
            : gen(rd()),
            dis(minf, maxf) {}

        gen_point_t(const gen_point_t& g) {
            typename DISTRIBUTION::param_type p(g.dis.a(), g.dis.b());
            dis.param(p);                
        }

        void swap(gen_point_t& g) {
            typename DISTRIBUTION::param_type pg(g.dis.a(), g.dis.b());
            typename DISTRIBUTION::param_type pm(dis.a(), dis.b());
            dis.param(pg);
            g.dis.param(pm);
        }

        gen_point_t& operator = (const gen_point_t& g)    {
            auto t  = g;
            swap(t);
            return *this;
        }

        point_t<T> operator ()  ()    {
            return point_t<T>(dis(gen), dis(gen));
        }

        std::random_device rd;
        std::mt19937 gen;
        DISTRIBUTION dis;
    };

    typedef gen_point_t<float, std::uniform_real_distribution<> > gen_fpoint_t;
    typedef gen_point_t<> gen_ipoint_t;
}

#endif
