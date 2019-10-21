# GA-based-Optimising-Relay-Network-Topology-
Optimising Relay Network Topology Using Generic Algorithm

The scheme of Relay wireless backpropagation presents a network optimisation model from centre to boundary. The center is connected by the DeNB base station via satellite transmission and serves as the connection and transmission of a series of RRN substations in its neighbourhood.

We implemented GA on the problem of RRN topology optimisation. This implementation was also optimised by multi-threads.

1. Optimisation target: a sequence of sites allocated whether base station or substation, which is represented as a sequence of binary numbers.
2. Value function: cost of constructing different types of stations and statellite communication expense.
3. Generic Algorithmm: initialisation, network connection (greedy algorithm), value calculation, selection, crossover, mutation.

A generic framework synthesizes a group of classic methods:
1. selection: Bolzmann selection and elite selection.
2. crossover: uniform and/or crossover.
3. mutation: mutation odds adjusted according to performance.
