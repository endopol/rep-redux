namespace boost {
  template <class VertexListGraph, class Order, class Degree, 
            class Marker, class BucketSorter>
  void 
  smallest_last_ordering(const VertexListGraph& G, Order order, 
                         Degree degree, Marker marker, 
                         BucketSorter& degree_buckets) {
    typedef typename graph_traits<VertexListGraph> GraphTraits;

    typedef typename GraphTraits::vertex_descriptor Vertex;
    //typedef typename GraphTraits::size_type size_type;
    typedef size_t size_type;

    const size_type num = num_vertices(G);
    
    typename GraphTraits::vertex_iterator v, vend;
    for (tie(v, vend) = vertices(G); v != vend; ++v) {
      put(marker, *v, num);
      put(degree, *v, out_degree(*v, G));
      degree_buckets.push(*v);
    }
 
    size_type minimum_degree = 1;
    size_type current_order = num - 1;
    
    while ( 1 ) {
      typedef typename BucketSorter::stack MDStack;
      MDStack minimum_degree_stack = degree_buckets[minimum_degree];
      while (minimum_degree_stack.empty())
        minimum_degree_stack = degree_buckets[++minimum_degree];
      
      Vertex node = minimum_degree_stack.top();
      put(order, current_order, node);
      
      if ( current_order == 0 ) //find all vertices
        break;
      
      minimum_degree_stack.pop();
      put(marker, node, 0); //node has been ordered.
      
      typename GraphTraits::adjacency_iterator v, vend;
      for (tie(v,vend) = adjacent_vertices(node, G); v != vend; ++v)
        
        if ( get(marker, *v) > current_order ) { //*v is unordered vertex
          put(marker, *v, current_order);   //mark the columns adjacent to node
          
          //It is possible minimum degree goes down
          //Here we keep tracking it.
          put(degree, *v, get(degree, *v) - 1); 
          minimum_degree = std::min(minimum_degree, get(degree, *v)); 
          
          //update the position of *v in the bucket sorter
          degree_buckets.update(*v);
        }

      current_order--;
    }
    //at this point, get(order, i) == vertex(i, g);
  }
} // namespace boost