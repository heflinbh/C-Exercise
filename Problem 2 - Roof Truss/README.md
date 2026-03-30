# Problem 2 - Roof Truss

## Overview

This solution solves a roof truss cutting problem in C++.

Given a truss span and roof angle, the system calculates the constraints for a roof truss and generates an "optimal" cutting plan to minimize waste when cutting pieces from standard stock lengths.

## Classes

### `BenCustomTrussConstraintProvider`

This class implements the `ITrussConstraintProvider` interface and converts a `TrussInput` (given by the user) into full `TrussConstraints`.

- It calculates diagonal member lengths using trigonometry based on the roof angle. 
- Knowing that, it defines bay zones for both bottom and diagonal members. 
- And knowing THAT, it uses hardcoded values to calculate join zone ranges.

### `CuttingPlanner`

Takes a list of available stock lengths and generates a cutting plan.
For now, it only implements a greedy algorithm, but the structure allows for other algorithms to be added in the future.

- It instantiates a `GreedySingleMemberSolver` for each member of the truss (bottom, left diagonal, right diagonal).
- It solves each member independently and combines the results into a `CutPlan`.

### `GreedySingleMemberSolver`

Implements the `ISingleMemberSolver` interface and generates a cutting plan for a single truss member.

- It validates stock lengths and uses a greedy approach to select pieces.
- It finds the furthest "legal" endpoint within join zones for each stock piece, minimizing waste.
- It tracks which stock lengths are used and how many times each is used.

## Areas for Improvement
Well, there are a lot.

- Truss systems with more than three members are unsupported.
- Truss systems with non-isosceles triangular geometry are unsupported.
- Hopefully easier to implement: variable join zones, or different number of join zones
- Dynamically calculated join zones? Sounds intense. 
- Input validation could be improved, especially when you consider different measurement systems (feet vs meters, degress vs radians). 
- Alternate algorithms should be implemented and tested before this reaches "production". Greedy is very unlikely to be to best solution.
- We could reverse this: instead of starting with a truss and an infinate supply of stock lengths, we could start with a truss and a finite list of stock lengths, and then determine what type of truss we could build.
- A cost calculator could be added to this system.
- The `CutPlan` results could be exported via our XML writer from Problem 1. That would have been nifty!
