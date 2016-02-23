#ifndef KMEANS_H_
#define KMEANS_H_

#include "point.h"
#include "ca_util.h"

#include <vector>

namespace ca    {

    extern void test_kmeans();

    template <typename POINT, size_t NCLUS>
        std::vector<size_t> kmeans(std::vector<POINT>& pts) {

            std::vector<size_t> clusters(pts.size(), 0);

            std::vector<POINT> centroids(NCLUS);

            // Randomly initialize centroids
            genT<true> igen(0, pts.size() - 1);
            for (auto& c : centroids)   {
                c   = pts[igen()];
            }
            //print_container<decltype(begin(centroids)), POINT>(begin(centroids), end(centroids));
            
            bool dont_stop = true;

            while (dont_stop)   {

            //for (auto i = 0; i < 2; ++i)    {

                // Assign clusters to each point
                
                std::transform(begin(pts), end(pts), begin(clusters),
                        [&](const POINT& pt)   {
                        
                        auto dmin   = std::numeric_limits<float>::max();
                        auto cnum = 0;

                        for (auto citr = centroids.begin(); citr != centroids.end(); ++citr)    {
                            
                            auto pdist = ca::distance(pt, *citr);

                            if (pdist < dmin) {
                                dmin = pdist;
                                cnum = std::distance(begin(centroids), citr);
                            }
                        }

                        return cnum;
                        });

                //std::cout << "\nClusters: ";
                //print_container<decltype(begin(clusters)), size_t>(begin(clusters), end(clusters));

                // Update centroids
                
                dont_stop = false;  // Changed _only_ if a centroid changes
                
                auto target_cluster = 0;
                for (auto ct = centroids.begin(); ct != centroids.end(); ++ct)  {

                        auto num_pts_clus   = 0;
                        
                        auto mean_x = 0.0f;
                        auto mean_y = 0.0f;

                        //std::cout << "\nFor cluster: " << target_cluster;
                        for (auto citr = begin(clusters); citr != end(clusters); ++citr)  {
                            
                            if (*citr == target_cluster)    {

                                //std::cout << "\nConsider " << (pts[std::distance(begin(clusters), citr)]);
                                
                                mean_x += std::pow(pts[std::distance(begin(clusters), citr)].x, 1);

                                mean_y += std::pow(pts[std::distance(begin(clusters), citr)].y, 1);

                                ++num_pts_clus;
                            }
                        }

                        auto mx = ((mean_x) / num_pts_clus);
                        auto my = ((mean_y) / num_pts_clus);

                        if ((mx != (*ct).x) && (my != (*ct).y)) { dont_stop = true; }
                        
                        (*ct).x = mx;
                        (*ct).y = my;

                        ++target_cluster;
                }
                //std::cout << "\nCentroids:\n";
                //print_container<decltype(begin(centroids)), fpoint_t>(begin(centroids), end(centroids));
            }

            std::cout << "\nCentroids:\n";
            print_container<decltype(begin(centroids)), fpoint_t>(begin(centroids), end(centroids));

            return clusters;
        }
}

#endif
