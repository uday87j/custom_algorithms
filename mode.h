#ifndef CA_MODE_H_
#define CA_MODE_H_

#include "ca_util.h"
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

namespace ca {

    extern void test_mode();

    template<typename T>
        struct mode_t   {

            std::vector<T> modes;
            
            std::ostream& print(std::ostream& os = std::cout) const   {
                std::copy(begin(modes), end(modes), std::ostream_iterator<T>(os, " "));
                return os;
            }
        };

    template <typename T>
        std::ostream& operator << (std::ostream& os, const mode_t<T>& m)    {
            m.print(os);
            return os;
        }

    template<typename FwdItr, typename T>
        mode_t<T> find_mode(FwdItr b, FwdItr e)  {
            typedef std::map<T, size_t> mmap_t;
            mmap_t mode_map;

            std::for_each (b, e,
                    [&](const T& k)    {
                        mode_map[k] = mode_map[k] + 1;
                    });

            typedef typename mmap_t::value_type mm_t;
            auto mode_itr = std::max_element(begin(mode_map), end(mode_map),
                    [](const mm_t& a, const mm_t& b)    {
                        return a.second < b.second;
                    });

            mode_t<T> ret_mode;

            if (mode_itr != mode_map.end()) {
                ret_mode.modes.push_back((*mode_itr).first);
                auto mode_count = ((*mode_itr).second);

                std::for_each(begin(mode_map), end(mode_map),
                        [&](const mm_t& m)    {
                            if ((m.second == mode_count) &&
                                (m.first != ret_mode.modes[0])) {
                                ret_mode.modes.push_back(m.first);
                            }
                        });
            }
                

            return ret_mode;
        }

}

#endif
