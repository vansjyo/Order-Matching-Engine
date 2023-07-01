# Order-Matching-Engine
Implementing the Order Matching Engine

For this project, I have assumed that we are dealing with equities with high liquidity.
This implies that if we know the price of the best Buy/sell order, the next best buy/sell price point is within a few decimal points.
So the number of steps (0.01 decimal point) that we will likely have to move is bounded by a statistical constant. This is a reasonable
assumption for most assets traded in an exchange. Hence, I use the array data structure for the order book instead of a tree data structure.

Anyone is free to contrbute to this project via either suggestions (high level or low level) or corrections. I would love to collaborate actually.
I treat this as a fun learning project. 
