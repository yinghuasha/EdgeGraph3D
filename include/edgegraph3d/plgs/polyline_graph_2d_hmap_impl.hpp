/*
 ***********************************************************************
 *
 * 							  EdgeGraph3D
 *
 *         Copyright (C) 2018 Andrea Bignoli (andrea.bignoli@gmail.com)
 *                         All rights reserved
 *
 ***********************************************************************
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 *
*/


#ifndef INCLUDE_EDGEGRAPH3D_PLGS_POLYLINE_GRAPH_2D_HMAP_IMPL_HPP_
#define INCLUDE_EDGEGRAPH3D_PLGS_POLYLINE_GRAPH_2D_HMAP_IMPL_HPP_

#include <boost/serialization/base_object.hpp>
#include <stddef.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "polyline_graph_2d.hpp"
#include "edge_graph_3d_utilities.hpp"
#include "global_defines.hpp"
#include "global_switches.hpp"
#include "global_switches.hpp"
#include "serialization_utilities.hpp"
#include "glm.hpp"

namespace boost {
namespace serialization {
class access;
} /* namespace serialization */
} /* namespace boost */

using namespace std;

struct KeyFuncs
{
    size_t operator()(const glm::vec2& k)const
    {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
    }

    bool operator()(const glm::vec2& a, const glm::vec2& b)const
    {
            return a.x == b.x && a.y == b.y;
    }
};

typedef unordered_map<glm::vec2,ulong,KeyFuncs,KeyFuncs> pointmaptype;

class PolyLineGraph2DHMapImpl: public PolyLineGraph2D {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        // serialize base class information
        ar & boost::serialization::base_object<PolyLineGraph2D>(*this);
        ar & point_map;
    }


	bool is_duplicate(const polyline &pl);
	void internal_add_polyline(const polyline &pl);
	void invalidate_node(ulong node_id);
	void remove_invalid_polylines();
	void remove_short_polylines_out_from_hubs();
	void prolong_polyline_extreme_and_intersect(const ulong polyline_id, ulong extreme_id, float max_dist);
	void prolong_polyline_extremes_and_intersect(const ulong polyline_id, float max_dist);
	void split_loop(ulong pl_id);
	void split_loops();
public:
	PolyLineGraph2DHMapImpl();
	~PolyLineGraph2DHMapImpl();
	ulong get_node_id(const glm::vec2 &p_coords);
	// Returns ID of new split node
	ulong split_polyline(const PolyLineGraph2D::plg_point &plgp);
	void add_polyline(const vector<glm::vec2> &polyline_coords);
	void add_direct_connection(const ulong start, const ulong end);

	void prolong_extremes_and_intersect(float max_dist);
	void connect_close_extremes();
	void optimize();
	void remove_2connection_nodes();
	void remove_degenerate_loops();
	void connect_close_extremes_following_direction();
	pointmaptype point_map;
	PolyLineGraph2DHMapImpl(const vector<polyline> &polylines, const vector<vector<ulong>> &connections, const vector<bool> &visited_nodes, const ulong &real_nodes_amount, const ulong &nodes_amount, const ulong &next_node_id, const vector<glm::vec2> &nodes_coords, const pointmaptype &point_map);
};

void serialize_plg(const PolyLineGraph2DHMapImpl &plg, const string plg_path);
PolyLineGraph2DHMapImpl deserialize_plg(const string plg_path);

#endif /* INCLUDE_EDGEGRAPH3D_PLGS_POLYLINE_GRAPH_2D_HMAP_IMPL_HPP_ */
