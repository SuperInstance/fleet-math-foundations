# Chapter 5 — Tropical Geometry and Constraint Scheduling

> *Feasibility is tropical holonomy: a schedule exists iff every cycle has non-positive travel time.*

---

## 5.1 The Tropical Semiring

**Definition 5.1 (Tropical semiring).** The **(max, +) tropical semiring** 𝕋 = (ℝ ∪ {−∞}, ⊕, ⊗) is defined by:

    a ⊕ b = max(a, b)
    a ⊗ b = a + b

with identity elements: −∞ for ⊕, 0 for ⊗.

**Definition 5.2 (Tropical matrix).** An n × n tropical matrix A has entries a_{ij} ∈ 𝕋. Matrix multiplication is:

    (A ⊗ B)_{ij} = ⊕_{k} a_{ik} ⊗ b_{kj} = max_k (a_{ik} + b_{kj})

---

## 5.2 Tropical Linear Inequalities

**Definition 5.3 (Tropical constraint system).** A tropical constraint system consists of variables x₁, ..., x_n ∈ ℝ and constraints of the form:

    x_j ≥ a_{ji} + x_i

for some set of pairs (i, j), where a_{ji} ∈ ℝ ∪ {−∞}.

In matrix form:

    x_j ≥ max_i (a_{ji} + x_i) = (A ⊗ x)_j

or equivalently, x ≥ A ⊗ x (componentwise).

**Theorem 5.1 (Karp-Sargent theorem, 1980).** A system of tropical linear inequalities x ≥ A ⊗ x has a solution x ∈ ℝ^n iff the maximum cycle mean of the weighted digraph of A is ≤ 0.

*Proof sketch.* Consider the digraph with edge weight a_{ij} from node i to node j. For a cycle γ = (i₀, i₁, ..., i_k = i₀), iterating the inequality gives:

    x_{i₀} ≥ a_{i₀i₁} + a_{i₁i₂} + ... + a_{i_{k-1}i₀} + x_{i₀}

Canceling x_{i₀}: the sum of edge weights around γ must be ≤ 0. If all cycles satisfy this, a solution exists and can be found by longest paths from any starting point (using the Bellman-Ford algorithm on the negated weights). ∎

---

## 5.3 Tropical Holonomy

**Definition 5.4 (Tropical holonomy).** For a weighted directed graph (V, E, w) with w: E → ℝ, the **tropical holonomy** around a cycle γ = (v₀, v₁, ..., v_k = v₀) is:

    Hol_{trop}(γ) = w(v₀, v₁) + w(v₁, v₂) + ... + w(v_{k-1}, v₀)

**Theorem 5.2 (Tropical ZHC).** A tropical constraint system x ≥ A ⊗ x has a solution iff for every cycle γ:

    Hol_{trop}(γ) ≤ 0

*Proof.* Direct translation of Theorem 5.1: the maximum cycle mean ≤ 0 iff all cycle sums are ≤ 0. ∎

**Corollary 5.3 (Non-positive tropical holonomy = feasibility).** The tropical analog of zero holonomy is non-positive cycle sum. "Zero" in the tropical sense means "the cycle introduces no positive feedback" — any positive cycle sum creates infeasibility.

---

## 5.4 The ZHC Connection on the Tropical Semiring

**Definition 5.5 (Tropical principal bundle).** When the structure group G is (ℝ, +) (the additive group of reals), a principal G-bundle over a graph has:

- Edge elements w_e ∈ ℝ (real-valued edge weights)
- Holonomy around cycle γ: sum of w_e along γ
- Zero holonomy condition: sum = 0 (additive group identity)

**Theorem 5.4 (Tropical ZHC reduces to cycle time analysis).** When G = (ℝ, +):

- Zero holonomy: cycle sums are zero
- Non-zero holonomy: cycle sums are non-zero
- Positive holonomy (sum > 0): infeasible schedule (timing contradiction)
- Negative holonomy (sum < 0): feasible with slack

*Proof.* Zero holonomy Hol(γ) = 0 means w₀₁ + ... + w_{k-1,0} = 0. For a timing constraint system with edge weights = required durations, a cycle sum > 0 means the constraints are contradictory — you'd need to be at one place at two different times. A cycle sum < 0 means there's slack in the system. ∎

---

## 5.5 Application: Scheduling

**Problem 5.1 (Project scheduling).** Given |V| tasks with durations d_i and precedence constraints (task i must precede task j by at least Δ_{ij} time units), find the earliest start times.

**Algorithm 5.1 (Tropical Schedule Solver).**

```
Input: Tasks 1..n with durations d_i, precedences (i → j, Δ_{ij})
Output: Earliest start times s_i, or INFEASIBLE

1. Build weighted graph with edge (i → j) weight = d_i + Δ_{ij}
2. Compute tropical holonomy (cycle sum) for all cycles
3. If any cycle has positive holonomy → INFEASIBLE
4. Else compute longest paths from a dummy source node (start of time):
   a. s_i = longest path length from source to node i
   b. This is the earliest feasible start time
5. Return {s_i}
```

**Theorem 5.5 (Complexity).** Tropical scheduling runs in O(|V|·|E|) time using the Bellman-Ford algorithm.

*Proof.* Step 3 (detecting positive cycles) is exactly the negative cycle detection problem in Bellman-Ford (negate weights, detect negative cycles). Step 4 (longest paths) is equivalent to shortest paths with negated weights in a DAG after removing cycles (which have non-positive sums, so removing them doesn't affect longest path distances). ∎

**Example 5.1 (Three-task project).**

| Task | Duration | Precedence | Lag |
|------|----------|------------|-----|
| A | 5 days | — | — |
| B | 3 days | A → B | 1 day |
| C | 7 days | A → C | 0 days |
| | | B → C | 2 days |

Graph edges: A→B (weight 5+1=6), A→C (weight 5+0=5), B→C (weight 3+2=5)

Cycle A→B→C→A: Hol = 6 + 5 + (−5) = 6 — wait, direction matters. Actually, the cycle is A→B (6), B→C (5), and we need C→A which doesn't exist. So no cycle, system is feasible. Earliest start: s_A = 0, s_B = 6, s_C = max(5, 6+5) = 11.

---

## 5.6 FLUX-C as a Tropical Algebra Evaluator

**Observation.** The FLUX-C constraint system is a tropical algebra evaluator on the fleet.

- Each agent's local clock is a tropical variable x_v
- Communication delays between agents are edge weights w_{uv}
- The constraint x_v ≥ w_{uv} + x_u expresses that agent v cannot act until agent u's message arrives
- FLUX-C computes the transitive closure of these constraints — i.e., it computes longest paths in the tropical sense

**Theorem 5.6 (FLUX-C = longest tropical path).** The FLUX-C constraint propagation algorithm computes the earliest feasible global schedule by evaluating:

    x_v = max_{u ∈ pred(v)} (w_{uv} + x_u)

which is exactly tropical matrix multiplication: x ← A ⊗ x.

*Proof.* The FLUX-C update rule is the tropical matrix-vector product. Iterating this to a fixed point computes the solution to x = A ⊗ x, which is the longest-path problem in the tropical semiring. ∎

---

## 5.7 Application: Just-in-Time Manufacturing

**Problem 5.2 (Production line balancing).** A production line has V stations, each with processing time d_i. Work-in-progress moves between stations with transport time t_{ij}. Maximizing throughput requires preventing station starvation.

Model: x_i = time when station i starts processing. Edge weights include both processing and transport times. The tropical holonomy of the production cycle gives the minimum cycle time — the reciprocal of maximum throughput.

**Theorem 5.7 (Maximum throughput).** The maximum throughput of a production line is the reciprocal of the maximum cycle mean:

    Throughput_max = 1 / max_{cycle γ} (Hol_{trop}(γ) / |γ|)

where |γ| is the number of edges in the cycle.

*Proof.* The maximum cycle mean (Karp's algorithm) gives the bottleneck of the system. The throughput cannot exceed this bottleneck. ∎

---

## 5.8 Open Problems

**Open Problem 5.1 (Min-max tropical holonomy).** Given edge weights a_{ij} (lower bounds) and b_{ij} (upper bounds), when does there exist an assignment of edge weights w_{ij} ∈ [a_{ij}, b_{ij}] such that Hol_{trop}(γ) ≤ 0 for all cycles? This is the feasibility of interval-constrained scheduling.

**Open Problem 5.2 (Tropical sheaf cohomology).** The tropical semiring's lack of additive inverses means standard sheaf cohomology does not apply. Is there a tropical analog of H¹ that detects schedule infeasibility more subtly than positive cycle sums?

**Open Problem 5.3 (Probabilistic tropical scheduling).** When edge weights are random variables (stochastic durations), what is the probability that a schedule exists? How does the distribution of the maximum cycle mean evolve as the schedule progresses?
