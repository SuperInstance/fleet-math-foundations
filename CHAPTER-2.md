# Chapter 2 — Cohomology and Emergence Detection

> *Emergence is a cohomology class. It appears exactly where local consistency does not imply global consistency.*

---

## 2.1 Simplicial Cohomology of Graphs

**Definition 2.1 (Cochain groups).** Let Γ = (V, E) be a graph. Define:

- **C⁰(Γ)** = {f: V → ℝ}, the space of 0-cochains (functions on vertices)
- **C¹(Γ)** = {ω: E → ℝ}, the space of 1-cochains (functions on edges)
- **C²(Γ)** = {θ: T → ℝ}, the space of 2-cochains (functions on triangles), if Γ is extended to its 2-skeleton

**Definition 2.2 (Coboundary operators).** The coboundary maps:

    d₀: C⁰ → C¹,   (d₀ f)(u, v) = f(v) − f(u)
    d₁: C¹ → C²,   (d₁ ω)(u, v, w) = ω(v, w) − ω(u, w) + ω(u, v)

**Definition 2.3 (Cohomology groups).** The cohomology groups are:

    H⁰(Γ) = ker(d₀)    (locally constant functions)
    H¹(Γ) = ker(d₁) / im(d₀)   (closed 1-forms modulo exact 1-forms)
    H²(Γ) = C² / im(d₁)   (when 2-skeleton is defined)

**Theorem 2.1 (Betti numbers of a graph).** For a finite graph Γ with |V| vertices, |E| edges, and C connected components:

    β₀ = dim H⁰(Γ) = C
    β₁ = dim H¹(Γ) = |E| − |V| + C
    β_k = 0 for k ≥ 2

*Proof.* H⁰ is the space of functions constant on each component, so dim = C. The Euler characteristic formula χ(Γ) = |V| − |E| = β₀ − β₁ gives β₁ = |E| − |V| + C. Since Γ is 1-dimensional, no higher cohomology. ∎

---

## 2.2 Emergence as Cohomology

**Definition 2.4 (Laman rigidity).** A graph Γ = (V, E) is **Laman-rigid** in ℝ² if:

    |E| ≥ 2|V| − 3

and this inequality holds for every subgraph. (For a connected graph, |V| ≥ 2.)

**Definition 2.5 (Emergence threshold).** The **emergence threshold** for a connected graph is:

    |E| > 2|V| − 3

**Theorem 2.2 (Cohomological emergence criterion).** For a connected graph Γ:

    β₁ > |V| − 2   ⇔   |E| > 2|V| − 3   ⇔   the graph is over-constrained

*Proof.* For a connected graph (C = 1):

    β₁ = |E| − |V| + 1
    β₁ > |V| − 2  ⇔  |E| − |V| + 1 > |V| − 2  ⇔  |E| > 2|V| − 3

The equivalence is algebraic. The "over-constrained" interpretation follows from Laman's theorem: a graph is generically rigid in ℝ² iff it has a spanning subgraph with exactly 2|V| − 3 edges satisfying the Laman inequality on all subgraphs. Any edge beyond this minimum creates over-constraint. ∎

**Definition 2.6 (Emergence severity).** For a connected graph:

    ε = β₁ / (|V| − 2) − 1

When ε > 0, the system has more constraints than needed for rigidity. These surplus constraints create **emergent degrees of freedom** — system-level behaviors not determined by local rules.

---

## 2.3 The Emergence Spectrum

**Definition 2.7 (Emergence classes).** The emergence severity ε partitions graphs into regimes:

| ε | Regime | Interpretation |
|---|--------|----------------|
| ε < 0 | Under-constrained | Too few constraints; system has internal degrees of freedom |
| ε = 0 | Critically constrained | Minimal rigidity; exactly as many constraints as needed |
| ε > 0 | Over-constrained | Surplus constraints produce emergent behavior |
| ε ≫ 1 | Highly emergent | Many overlapping constraints; complex emergent phenomena |

**Example 2.1 (Triangular lattice).** Consider a triangular lattice patch with |V| = n² vertices. The number of edges in a triangular lattice is approximately 3n² − O(n). For large n:

    β₁ ≈ (3n²) − n² + 1 = 2n² + 1
    ε = 2n²/(n² − 2) − 1 ≈ 1

A triangular lattice has ε ≈ 1 — it is substantially over-constrained. The surplus constraints manifest as the lattice's ability to transmit stress across long distances.

**Example 2.2 (Tree).** For a tree, |E| = |V| − 1, so β₁ = 0 and ε = −1. Trees are purely under-constrained. They have no redundant information — failure of any edge disconnects the system.

---

## 2.4 Worked Example: Protein Folding

**Setup.** Consider a protein with N amino acids (nodes). The backbone forms a path of N − 1 edges. Hydrogen bonds between non-adjacent amino acids add additional edges.

- **Random coil:** |E| ≈ N − 1 (just the backbone). β₁ = 0, ε = −1.
- **Partially folded:** Some hydrogen bonds form. H = number of hydrogen bonds. |E| = (N − 1) + H. β₁ = H.
- **Folding transition:** When β₁ crosses N − 2, i.e., when H > N − 1, the protein transitions to being over-constrained. This is the folding transition.

**Analysis.** The folding transition occurs when:

    β₁ = H > N − 2

This threshold is purely structural — it depends only on the bond graph, not on the chemical details. The emergence severity ε measures "how folded" the protein is:

    ε = H / (N − 2) − 1

| H | ε | State |
|---|----|-------|
| 0 | −1 | Unfolded |
| N/2 | ≈ −0.5 | Molten globule |
| N | ≈ 0.02 | Near-native |
| 2N | ≈ 1.02 | Native + crystal contacts |

**Observation.** The folding transition (H > N − 1) occurs surprisingly late — when there are more hydrogen bonds than backbone bonds. This explains why proteins require many non-local contacts to fold stably.

---

## 2.5 Real-Time Emergence Detection

**Definition 2.8 (Emergence event).** An **emergence event** occurs when the addition of a single edge e transforms an under-constrained or critically constrained graph into an over-constrained one.

**Algorithm 2.1 (Emergence Monitor).**

```
Input: Dynamic graph Γ(t) evolving over time
Output: Alerts on emergence events

1. Maintain |V|, |E| incrementally
2. On edge addition (u, v):
   a. Update |E| ← |E| + 1
   b. Compute new β₁ = |E| − |V| + C
   c. If β₁ == |V| − 2 — this edge just crossed the threshold
      d. Mark emergence event at time t
      e. Identify the fundamental cycle containing (u, v)
      f. The cycle is the "emergent motif" — the minimal structure causing emergence
3. Track ε(t) as a time series
```

**Theorem 2.3 (Motif detection).** Every emergence event involves exactly one fundamental cycle — the cycle closed by the over-constraining edge. This cycle is the **emergent motif** — the minimal subgraph whose surplus constraint produces emergence.

*Proof.* When β₁ crosses from |V| − 3 to |V| − 2, exactly one new independent cycle is created. This cycle is the fundamental cycle γ_e in T ∪ {e} for the newly added edge e. The over-constraint is localized to this cycle. ∎

---

## 2.6 Sheaf Cohomology (Generalization)

**Definition 2.9 (Cellular sheaf).** A **cellular sheaf** F on a graph Γ = (V, E) assigns:

- To each node v: a vector space F(v) (the **stalk** at v)
- To each edge e = (u, v): a linear map F(e): F(u) → F(v) (the **restriction map**)

The sheaf must satisfy: for any path u → v → w, F(v → w) ∘ F(u → v) = F(u → w) (this is enforced for the entire graph, not just edges; for edges, the condition only applies to 2-paths).

**Definition 2.10 (Sheaf cohomology).** Define:

- C⁰(F) = ⊕_{v∈V} F(v)
- C¹(F) = ⊕_{e∈E} F(e)
- δ₀: C⁰ → C¹ by (δ₀ s)(u,v) = F(u→v)(s_u) − s_v
- H⁰(F) = ker(δ₀)
- H¹(F) = ker(δ₁) / im(δ₀)

where δ₁ is the sheaf coboundary on 2-cochains (defined analogously to the scalar case).

**Theorem 2.4 (Sheaf consensus).** A distributed system has a globally consistent state (a global section) iff H⁰(F) has dimension at least 1. Local obstructions to consistency are measured by H¹(F).

*Proof.* A global section s ∈ C⁰(F) satisfies s_u = F(e)(s_v) for all edges — equivalently, δ₀(s) = 0. This is exactly H⁰(F). When edge constraints are locally satisfiable but no global assignment exists, the coboundary δ₀ applied to any candidate local assignment produces a non-zero 1-cochain that is a 1-cocycle (closed under δ₁) but not a coboundary — this is H¹(F). ∎

**Theorem 2.5 (Sheaf vs. ZHC).** When the sheaf is locally constant — all stalks are copies of the same group G and all restriction maps are left multiplication by g_e ∈ G — then:

    H⁰(F) ≅ {sections of the principal G-bundle}
    H¹(F) ≅ {obstructions detected by holonomy}

ZHC detects non-zero holonomy in the principal bundle; sheaf cohomology detects non-zero H¹ as the same obstruction in a more general setting.

---

## 2.7 Applications

**Anomaly detection in networks.** Monitor β₁ in real-time network traffic graphs. A sudden increase in β₁ (crossing the emergence threshold) signals anomalous behavior — an attack, a cascade, or a phase change.

**Community detection.** In social networks, communities are subgraphs with high internal edge density (over-constrained) but sparse external connections. The emergence severity ε within a subgraph measures its "community-ness."

**Phase transitions in complex systems.** The emergence severity ε acts as an order parameter for phase transitions. The critical point is ε = 0, where the system transitions from constrained-but-flexible to over-constrained.

**Regulatory networks.** In gene regulatory networks, ε > 0 corresponds to the regime where feedback loops create emergent dynamics (oscillations, bistability). The number of feedback circuits is exactly β₁.

---

## 2.8 Open Problems

**Open Problem 2.1 (Nonlinear sheaves).** Sheaf cohomology is linear. But many real constraints are nonlinear (e.g., Lie group-valued constraints in robotics). How do we define cohomology for nonlinear sheaves on graphs?

**Open Problem 2.2 (Emergence prediction).** Can we predict when a dynamic graph will cross the emergence threshold before it happens? Is there a "precursor" signal in the spectrum of the graph Laplacian?

**Open Problem 2.3 (Multi-scale emergence).** When does emergence at one scale (β₁ > |V| − 2 at the global level) imply emergence at other scales? Is there a renormalization group for emergence severity?
