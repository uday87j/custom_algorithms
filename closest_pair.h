#ifndef CLOSEST_PAIR_H_
#define CLOSEST_PAIR_H_

#include <array>
#include <iostream>
#include <cstdint>
#include <utility>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <cassert>

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

        gen_point_t(const T minf = T(), const T maxf = T(), std::uint32_t step = 100)
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

    template <typename T>
        pair_points_t<T> closest_pair(const std::vector<point_t<T> >& points)   {
            // Base cases
            if (points.empty()) {
                assert(0);
                return pair_points_t<T>();
            }
            else if (points.size() == 1) {
                return pair_points_t<T>(points[0], point_t<T>());
            }
            else if (points.size() == 2)    {
                return pair_points_t<T>(points[0], points[1]);
            }

            // All points are on same x co-ordinate
            bool all_pts_same_x = true;
            for (auto i = 1; i < points.size(); ++i)    {
                if (points[i].x != points[0].x) {
                    all_pts_same_x = false;
                    break;
                }
            }

            auto points_y    = points;
            std::sort(begin(points_y), end(points_y), &y_less_than<T>);
            
            pair_points_t<T> cp(points_y[0], points_y[1]);

            if (all_pts_same_x) {
                auto dmin_y = distance(points_y[0], points_y[1]);
                for (auto i = 1; i < points_y.size() - 1; ++i)  {
                    auto d = distance(points_y[i], points_y[i+1]);
                    if (d < dmin_y) {
                        cp.p1   = points_y[i];
                        cp.p2   = points_y[i+1];
                    }
                }
                return cp;
            }
            

            // Other cases
            auto points_x    = points;
            std::sort(begin(points_x), end(points_x), &x_less_than<T>);

            auto x_mid   = ((points_x.front().x + points_x.back().x) / 2);

            std::cout << "\nInput sorted on x: \n";    print_sequence_container(points_x);
            std::cout << "\nx_mid: " << x_mid << std::endl;

            // Partition into Left
            decltype(points_x) points_x_1;
            std::copy_if(begin(points_x), end(points_x), std::back_inserter(points_x_1),
                    [x_mid](const point_t<T>& a)    {
                        return a.x <= x_mid;
                    });
            std::cout << "\n points_x_1: \n";    print_sequence_container(points_x_1);

            // Partition into Right
            decltype(points_x) points_x_2;
            std::copy_if(begin(points_x), end(points_x), std::back_inserter(points_x_2),
                    [x_mid](const point_t<T>& a)    {
                        return a.x > x_mid;
                    });
            std::cout << "\n points_x_2: \n";    print_sequence_container(points_x_2);

            auto cp_left    = closest_pair(points_x_1);
            auto cp_right   = closest_pair(points_x_2);

            auto dmin_left  = distance(cp_left.p1, cp_left.p2);
            auto dmin_right = distance(cp_right.p1, cp_right.p2);
            auto dmin       = std::min(dmin_left, dmin_right);

            if (dmin_left < dmin_right) {
                cp  = cp_left;
            }
            else    {
                cp  = cp_right;
            }

            decltype(points_x) points_x_1_dmin;
            std::copy_if(begin(points_x_1), end(points_x_1), std::back_inserter(points_x_1_dmin),
                    [&](const point_t<T>& a)    {
                        //return a.x <= dmin;
                        assert((x_mid - a.x) >= 0);
                        return (x_mid - a.x) <= dmin;
                    });
            std::cout << "\n points_x_1_dmin: \n";    print_sequence_container(points_x_1_dmin);
            decltype(points_x) points_x_2_dmin;
            std::copy_if(begin(points_x_2), end(points_x_2), std::back_inserter(points_x_2_dmin),
                    [&](const point_t<T>& a)    {
                        assert((a.x - x_mid) >= 0);
                        return (a.x - x_mid) <= dmin;
                    });
            std::cout << "\n points_x_2_dmin: \n";    print_sequence_container(points_x_2_dmin);

            //TODO: Optimize Split pair
            //for (auto& pl : points_x_1_dmin) {
            //    for (auto& pr : points_x_2_dmin)    {
            //        auto d_split    = distance(pl, pr);
            //        if (d_split < dmin) {
            //            std::cout << "\nSplit pair is closer: " << pair_points_t<T>(pl, pr) << std::endl;;
            //            dmin    = d_split;
            //            cp.p1   = pl;
            //            cp.p2   = pr;
            //        }
            //    }
            //}
            
            // Optimized version
            std::sort(begin(points_x_2_dmin), end(points_x_2_dmin), &y_less_than<T>);
            for (auto& pl : points_x_1_dmin)    {
                auto pair_count = 0;
                for (auto& pr : points_x_2_dmin)    {
                    if ((pr.y - pl.y) <= dmin)    {
                        ++pair_count;
                        if (pair_count == 6)    {   // These many points are enough to consider ClosestPair in 2D (Fact)
                            break;
                        }
                        auto dmin_pair  = distance(pl, pr);
                        if (dmin_pair < dmin)   {
                            dmin = dmin_pair;
                            cp.p1 = pl;
                            cp.p2 = pr;
                        }
                    }
                }
            }
            
            return cp;
        }
}

#endif
