#ifndef KMEANS_H_
#define KMEANS_H_

#include <iostream>
#include <cstdint>
#include <algorithm>
#include <random>

#include "ca_util.h"
#include "point.h"

namespace ca    {

    template <typename POINT>
        struct cluster_t    {
            std::vector<POINT*> pts;
        };

    template <typename POINT>
        std::ostream& operator << (std::ostream& os, const cluster_t<POINT>& c) {
            for (auto p : c.pts)   {
                os << (*p) << ", ";
            }
            return os;
        }

    template <typename POINT, size_t num_clus>
        std::vector<size_t> kmeans(std::vector<POINT>& pts) {

            std::vector<size_t> clusters(num_clus, 0);

            std::vector<POINT> centroids(num_clus);
            genT<> igen(0, num_clus);

            for (auto& c : centroids)    {
                c = pts[igen()]; // Randomly initialize centroids
            }

            // Assign each point to a cluster
            std::transform(begin(pts), end(pts), begin(clusters),
                    [&](const POINT& p)  {

                    auto d  = std::numeric_limits<float>::max();
                    auto c_num = ~0u;

                    for (auto c = begin(centroids); c != end(centroids); ++c)   {
                        auto cd = distance(p, *c);
                        if (cd < d)    {
                            d = cd;
                            c_num   = std::distance(begin(centroids), c);
                        }
                    }

                    return c_num;
                    });

            return clusters;
        }


    template <typename POINT>
        std::vector<cluster_t<POINT> >
        kmeans_v2(std::vector<POINT>& pts, int32_t num_clus) {
            
            assert(num_clus > 0);

            std::vector<cluster_t<POINT> > clusters(num_clus);
            
            for (auto p = pts.begin(); p != pts.end(); ++p) {
                clusters[0].pts.push_back(&(*p));
            }

            if (num_clus <= 0)  {
                return clusters;
            }

            std::vector<POINT> centroids(num_clus);
            genT<> igen(0, num_clus - 1);

            for (auto& c : centroids)    {
                c = pts[igen()]; // Randomly initialize centroids
            }

            // Until the centroid do _NOT_ change
            // 1. Assign each point to a cluster based n nearest centroid
            // 2. Update centrids based on mean distance of its cluster members
            
            // For now, let us run 10 times
            auto iter = 0;

            while (iter < 10)   {

                auto clus_id = 0;
                for (auto cl = clusters.begin(); cl != clusters.end(); ++cl, ++clus_id)  {

                    auto pts = (*cl).pts;

                    for (auto p = pts.begin(); p != pts.end(); ++p)  {

                        auto dist = std::numeric_limits<float>::max();
                    
                        for (auto c = centroids.begin(); c != centroids.end(); ++c)   {
                            
                            auto d = distance(**p, *c);
                            
                            if (d < dist)   {
                                dist = d;
                                clus_id = std::distance(centroids.begin(), c);
                            }
                        }
                        
                        // Move point to new (can be old) cluster
                        (*cl).pts.erase((*cl).pts.begin() + std::distance(pts.begin(), p));
                        clusters[clus_id].pts.push_back((*p));

                    }
                }

                ++iter;
            }

            return clusters;

        }
}


#endif
