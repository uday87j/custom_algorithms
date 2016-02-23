#ifndef POINT_H_
#define POINT_H_

#include <utility>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <limits>
#include <cassert>
#include <iostream>

using std::int32_t;

namespace ca    {

    // 'T' should have well define '<' operator
    template <typename T>
    struct point_t  {
        point_t(T px = T(), T py = T())
            : x(px),
            y(py)   {}

        bool operator == (const point_t& p) {
            return (x == p.x) && (y == p.y);
        }

        std::ostream& print(std::ostream& os) const   {
            os << "(" << x << ", " << y << ")";
            return os;
        }

        std::ostream& fprint(std::ostream& os) const   {
            os << x << " " << y;
            return os;
        }
        
        T x;
        T y;
    };

    template <typename T>
        std::ostream& operator << (std::ostream& os, const point_t<T>& p)    {
            if (os == std::cout)    {
                p.print(os);
            }
            else    {
                p.fprint(os);
            }
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
    template <typename T = int32_t, typename DISTRIBUTION = std::uniform_int_distribution<>, bool unique = false>
    struct gen_point_t  {

        gen_point_t(const T minf = T(), const T maxf = std::numeric_limits<T>::max(), std::uint32_t step = 100)
            : gen(/*rd()*/),
            dis(minf, maxf),
            vec_(step),
            start_step_(step)  {
                for (auto i = 0; i < start_step_; ++i)   {
                    dis(gen);   // While away these pseudo-random numbers
                    //auto p  = point_t<T>(dis(gen), dis(gen));
                    //std::cout << "\nGenerating a point: " << p << std::endl;
                    //vec_.push_back(p);
                }
            }

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
            auto p  = point_t<T>(dis(gen), dis(gen));
            //std::cout << "\nGenerating a point: " << p << std::endl;
            if (unique) {
                //std::cout << "\nI'm unique";
                for (auto& a : vec_)    {
                    if (a == p) {
                        return this->operator()();
                    }
                }
            }

            vec_.push_back(p);
            
            return vec_.back();
        }

        std::random_device rd;
        std::mt19937 gen;
        DISTRIBUTION dis;
        std::vector<point_t<T> > vec_;
        std::uint32_t start_step_;
    };

    typedef gen_point_t<float, std::uniform_real_distribution<> > gen_fpoint_t;
    typedef gen_point_t<int32_t,std::uniform_int_distribution<>, true> gen_ipoint_t;

    template <typename T>
        struct pair_points_t    {
            pair_points_t(const point_t<T>& a = point_t<T>(), const point_t<T>& b = point_t<T>())
                : p1(a),
                p2(b)   {}

            std::ostream& print(std::ostream& os) const   {
                os << "(" << p1 << "), (" << p2 << ")";
                return os;
            }

            std::ostream& fprint(std::ostream& os) const   {
                //os << "(" << p1 << "), (" << p2 << ")"; // TODO:Format according to gnuplot
                os << p1 << std::endl << p2;
                return os;
            }

            point_t<T> p1;
            point_t<T> p2;
        };

    template <typename T>
        std::ostream& operator << (std::ostream& os, const pair_points_t<T>& p)    {
            if (os == std::cout)    {
                p.print(os);
            }
            else    {
                p.fprint(os);
            }
            return os;
        }

    template <typename T = int32_t, typename RET = float>
        RET distance(const point_t<T>& p1, const point_t<T>& p2)  {
            RET d = std::sqrt(std::pow(p2.y - p1.y, 2) + std::pow(p2.x - p1.x, 2));                
            return d;
        }

    //template <typename CONTAINER>
    //    std::ostream& operator << (std::ostream& os, const CONTAINER& c) {
    //        for (auto& p : c)   {
    //            os << p << ", ";
    //        }
    //        return os;
    //    }
}
#endif
