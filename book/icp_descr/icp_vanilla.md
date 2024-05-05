\page vanilla_icp Vanilla ICP

The vanilla algorithm for ICP will match the point-cloud centers exactly and
then iterate until an optimal rotation has been found.

1.     **Matching Step**: match closest points.
    
    Sources:  
    - https://arxiv.org/pdf/2206.06435.pdf
    - https://web.archive.org/web/20220615080318/https://www.cs.technion.ac.il/~cs236329/tutorials/ICP.pdf
    - https://en.wikipedia.org/wiki/Iterative_closest_point
    - https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf


2.     **Transformation Step**: determine optimal transformation.
    
    The translation vector is determined by the displacement between
    the centroids of both point clouds. The rotation matrix is
    calculated via singular value decomposition.
    
    Sources:  
    - https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf



Read \ref icp_sources for a list of all resources used in this project.
This page was automatically generated from vanilla.cpp with icp_doc_builder.py.