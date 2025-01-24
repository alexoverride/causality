Test Case 1: Sequential Actions with Declined Reactions
U1 declares a Shoot action (A1).
U1 declines to react to A1.
U2 declines to react to A1.
U3 declines to react to A1.
U1 declares a second Shoot action (A2).
U1 declines to react to A2.
U2 declines to react to A2.
U3 declines to react to A2.
A1 resolves (damage applied).
A2 resolves (damage applied).
U1’s turn ends.

Test Case 2: Nested Reactions
U1 declares a Shoot action (A1).
U2 declares a Block reaction (A2 → A1).
U1 declines to react to A1.
U3 declines to react to A1.
U3 declares a Boost reaction (A3 → A2).
U1 declines to react to A2.
U2 declines to react to A2.
A3 resolves (Boost effect applied).
A2 resolves (Block reduces damage).
A1 resolves (reduced damage applied).

Test Case 3: Parallel Reactions
U1 declares a Shoot action (A1).
U2 declares a Block reaction (A2 → A1).
U3 declares a Block reaction (A3 → A1).
U1 declines to react to A1.
A2 resolves (Block reduces damage).
A3 resolves (Block further reduces damage).
A1 resolves (reduced damage applied).

Test Case 4: Self-Reaction
U1 declares a Shoot action (A1).
U1 declares a Boost reaction (A2 → A1).
U2 declines to react to A1.
U3 declines to react to A1.
A2 resolves (Boost effect applied).
A1 resolves (damage applied).





Current bugs:
declining should halt reaction submissions, but instead if a unit takes an action, then declines a reaction, they actually gain infinite reactions.
declining a reaction should allow switching to other units who have reactions, instead it prevents switching.
