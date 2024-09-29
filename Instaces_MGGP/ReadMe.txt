The benchmark set consists of 225 instances of the MGGPP.

The name of each instance is composed of four substrings stating the features of the instance:
1) "n" followed by the number of vertices (n = 100, 200, 300, 400, 500)
2) "pla" for the planar instances,
   "d03" for the sparse instances (random graphs with m = n(n-1)/3 edges)
   "d06" for the dense instances (random graphs with m = 2n(n-1)/3 edges)
3) "p" followed by 1 if p = round(ln(n))
   "p" followed by 2 if p = round(sqrt(n))
   "p" followed by 3 if p = round(n/ln(n))
4) "i" followed by an index i = 1, 2, 3, 4, 5 for instances of the same class

The files are in MathProg (i.e., AMPL) format, which should be self-explaining:
- p is the number of subgraphs
- V the set of vertices
- w the weight vector (the vertex indices have been permuted so that the weight values are nondecreasing)
- E the set of edges

Sets D0 and Y0 refer to logical conditions for ILP formulations we are currently experimenting on.
They are not of exploited by the heuristic algorithms.
