# Vanilla ICP

> Automatically generated from vanilla.cpp

## Matching Step: match closest points.


**Sources**  
- https://arxiv.org/pdf/2206.06435.pdf
- https://web.archive.org/web/20220615080318/https://www.cs.technion.ac.il/~cs236329/tutorials/ICP.pdf
- https://en.wikipedia.org/wiki/Iterative_closest_point
- https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf

## Transformation Step: determine optimal transformation.

The translation vector is determined by the displacement between
the centroids of both point clouds. The rotation matrix is
calculated via singular value decomposition.


**Sources**  
- https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf
