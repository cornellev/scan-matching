\page trimmed_icp Trimmed ICP
\par Usage
You can construct a new instance of Trimmed ICP with `icp::ICP::from_method("trimmed")`, with an additional optional parameter for configuration.
\par Description
Trimmed ICP is identical to \ref vanilla_icp with the addition of an overlap
rate parameter, which specifies the percentage of points between the two
point sets that have correspondences. When the overlap rate is 1, the
algorithm reduces to vanilla.

1.     **Matching Step**: match closest points
    
    Sources:  
    - https://arxiv.org/pdf/2206.06435.pdf
    - https://web.archive.org/web/20220615080318/https://www.cs.technion.ac.il/~cs236329/tutorials/ICP.pdf
    - https://en.wikipedia.org/wiki/Iterative_closest_point
    - https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf


2.     **Trimming Step**
    
    Matches are considered in increasing order of distance.
    
    Sources:  
    - https://ieeexplore.ieee.org/abstract/document/1047997


3.     **Transformation Step**: determine optimal transformation.
    
    The translation vector is determined by the displacement between
    the centroids of both point clouds. The rotation matrix is
    calculated via singular value decomposition.
    
    Sources:  
    - https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf



Read \ref icp_sources for a list of all resources used in this project.
This page was automatically generated from trimmed.cpp with icp_doc_builder.py.