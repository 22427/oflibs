PROCESSING_GRAPH: Processing Graph
================================================================================
This is a set of classes to build a processing graph.
The idea is, that processing data often requires different steps or operations.
In order to keep the opeartions interchangeable and reusable, you can use this
interface to join different operation nodes together to a processing graph.

Each nodes performes asyncronous to the other joints(spanws a thread),
so this system makes uses of all available cores.

API
--------------------------------------------------------------------------------

###Node
A Node is an Object, that processes data. Nodes are connected via Joints. A pair
of Joints represents a queue/dequeue interaface. Each node works assyncronous.

####TransformNode
A Transformer is like a Source and a Sink glued together. On one side
It is a sink for items of type A (left) and on the other side it is a Source of
items of type B. So a Transformer would, for example, read Images on the left
and write positions of detected features on the right.



