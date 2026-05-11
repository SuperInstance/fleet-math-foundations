# Chapter 4 — Sheaf Theory and Distributed Consistency

> *A distributed system has global consistency iff its constraint sheaf has non-zero global sections.*

---

## 4.1 Cellular Sheaves on Graphs

**Definition 4.1 (Cellular sheaf).** Let Γ = (V, E) be a finite graph. A **cellular sheaf F** on Γ consists of:

- For each vertex v ∈ V, a vector space F(v) over a field K (the **stalk** at v)
- For each edge e = (u, v) ∈ E, a linear map F(e): F(u) → F(v) (the **restriction map**)

No composition condition is required for individual edges — but for any path, the composition of maps should be well-defined. For a general graph, we extend by requiring that for every 2-step path u → v → w:

    F(v → w) ∘ F(u → v) = F(u → w)

**Definition 4.2 (Sheaf of sections).** A **global section** of F is a choice s_v ∈ F(v) for each vertex v such that for every edge (u, v):

    F(u → v)(s_u) = s_v

The space of global sections is denoted Γ(F) or H⁰(F).

**Definition 4.3 (Sheaf of local sections).** A **local section** over a subset U ⊆ V is a choice s_v for v ∈ U satisfying the edge constraints for all edges with both endpoints in U.

---

## 4.2 Sheaf Cohomology

**Definition 4.4 (Cochain complex).** Define:

    C⁰(F) = ⊕_{v∈V} F(v)          (assignments to vertices)
    C¹(F) = ⊕_{e∈E} F(e)          (assignments to edges)
    C²(F) = ⊕_{t∈T} F(t)          (assignments to triangles), where T is the set of triangles

The coboundary maps:

    δ₀: C⁰ → C¹,   (δ₀ s)(u,v) = F(u→v)(s_u) − s_v
    δ₁: C¹ → C²,   (δ₁ ω)(u,v,w) = F(v→w)(ω(u,v)) − ω(u,w) + ω(v,w)

**Theorem 4.1 (δ² = 0).** For any cellular sheaf F, δ₁ ∘ δ₀ = 0.

*Proof.* Compute δ₁(δ₀(s))(u,v,w):

    δ₁(δ₀(s))(u,v,w) = F(v→w)(δ₀(s)(u,v)) − δ₀(s)(u,w) + δ₀(s)(v,w)
    = F(v→w)(F(u→v)(s_u) − s_v) − (F(u→w)(s_u) − s_w) + (F(v→w)(s_v) − s_w)
    = F(v→w)F(u→v)(s_u) − F(v→w)(s_v) − F(u→w)(s_u) + s_w + F(v→w)(s_v) − s_w
    = F(u→w)(s_u) − F(u→w)(s_u) + s_w − s_w = 0

where we used the compatibility condition F(v→w)F(u→v) = F(u→w). ∎

**Definition 4.5 (Sheaf cohomology groups).**

    H⁰(F) = ker(δ₀)        — global sections
    H¹(F) = ker(δ₁) / im(δ₀)  — obstruction classes
    H²(F) = C² / im(δ₁)    — higher obstructions

---

## 4.3 Global Consistency = Non-Zero H⁰

**Theorem 4.2 (Consensus = global section).** A distributed system has a globally consistent state iff H⁰(F) ≠ 0 (i.e., there exists a non-zero global section of the constraint sheaf).

*Proof.* Direct from definitions: H⁰(F) = ker(δ₀) = {s ∈ C⁰ : F(u→v)(s_u) = s_v ∀(u,v)}. This is exactly a consistent global state. Non-zero H⁰ means at least one such state exists. ∎

**Theorem 4.3 (Dimension of solution space).** When global consensus exists, the space of consensus states has dimension dim(H⁰(F)). In particular, the degree of freedom of consensus is precisely the dimension of the first sheaf cohomology group.

---

## 4.4 Local vs. Global Consistency

**Definition 4.6 (Locally consistent).** A system is **locally consistent** if for every edge (u,v), the constraint F(u→v) is satisfiable — i.e., there exist x ∈ F(u) and y ∈ F(v) such that F(u→v)(x) = y.

**Definition 4.7 (Globally consistent).** A system is **globally consistent** if there exists a global section.

**Theorem 4.4 (Obstruction = H¹).** If a system is locally consistent but not globally consistent, this is detected by H¹(F) ≠ 0.

*Proof.* Local consistency means that for each edge, we can find a local assignment satisfying that edge's constraint. This gives a 1-cochain ω ∈ C¹(F) such that (δ₀ ω)(u,v) = 0 (each edge constraint is satisfied by some local pair). However, these local choices may not patch together globally. The obstruction to patching is δ₁(ω). If δ₁(ω) ≠ 0, the system is not globally consistent. If δ₁(ω) = 0 but ω ∉ im(δ₀), then H¹(F) ≠ 0. ∎

**Example 4.1 (Simple obstruction).** Consider a triangle graph with stalks ℝ and restriction maps:

    F(1→2) = id,   F(2→3) = id,   F(1→3) = 2·id

Local consistency: each edge is individually satisfiable (the map is onto). Global consistency: we need x₁, x₂, x₃ such that x₁ = x₂, x₂ = x₃, and 2x₁ = x₃. This implies x₁ = x₃ = 2x₁, so x₁ = 0. The only global section is the zero section. H¹(F) = ℝ (one-dimensional obstruction).

---

## 4.5 Comparison to ZHC

**Theorem 4.5 (Sheaf cohomology generalizes ZHC).** Let F be the sheaf where:

- F(v) = ℝ^n for all v (same stalk at every vertex)
- F(u→v)(x) = g_{uv}·x where g_{uv} ∈ GL(n, ℝ)

Then:

- H⁰(F) ≠ 0 iff ZHC consensus is possible (zero holonomy)
- H¹(F) measures the space of obstructions (non-zero holonomy classes)

*Proof.* The sheaf condition F(v→w)F(u→v) = F(u→w) is equivalent to g_{vw}g_{uv} = g_{uw} for all paths — this is exactly the flat connection condition (zero holonomy). When the connection is not flat, the holonomy around each cycle is an isomorphism of the stalk, and these isomorphisms generate H¹(F) as the space of cocycles modulo coboundaries. ∎

**Remark.** The sheaf-theoretic formulation reveals something ZHC does not: **the existence of global consensus depends on the entire constraint structure, not just individual cycles.** A set of edges can be locally satisfiable (each edge has a consistent map) but globally impossible — and H¹ detects this.

---

## 4.6 Distributed Databases and ACID

**Application: Sharded databases with consistency constraints.**

Consider a distributed key-value store sharded across V nodes. Each shard v holds a subset of the data modeled as F(v) = ℝ^{k_v}. Consistency constraints between shards are linear maps F(u→v): what one shard believes about shared keys must agree with another.

- **No consistency (no constraints):** The sheaf F has only stalks and no edges. H⁰(F) = ⊕ F(v) (all assignments are valid).
- **Eventual consistency:** Constraints are soft — F(u→v) maps may be multi-valued. The sheaf is not well-defined (not a function).
- **Strong consistency:** All constraints are enforced. The system is globally consistent iff H⁰(F) ≠ 0.

**Theorem 4.6 (CAP theorem in sheaf language).** In a distributed database:

- **Consistency** (C) = H⁰(F) contains the true state
- **Availability** (A) = each F(v) is locally accessible
- **Partition tolerance** (P) = the graph Γ may be disconnected

The CAP theorem says: in a partition (Γ disconnected), you cannot guarantee both consistency and availability. In sheaf terms: when Γ is disconnected, a global section on each component does not extend to a global section on the whole graph unless the stalks and maps between components are consistent — which requires communication across the partition.

---

## 4.7 Application: Sensor Networks

**Problem 4.1.** A sensor network has V sensors measuring overlapping spatial regions. Each sensor v measures a linear transformation of the underlying field. Edge constraints relate nearby sensors: F(u→v) maps sensor u's measurement space to sensor v's.

- If H⁰(F) ≠ 0, there exists a globally consistent interpretation of all sensor readings
- If H¹(F) ≠ 0, sensors are detecting conflicting underlying fields — which may indicate multiple signal sources or sensor malfunction

**Theorem 4.7 (Source separation).** In a sensor network with linear constraints, the number of independent signal sources is dim(H¹(F)).

*Proof.* Each obstruction class in H¹(F) corresponds to an independent conflict that cannot be resolved by local adjustments. These conflicts correspond to distinct underlying sources or distinct anomalies. ∎

---

## 4.8 Open Problems

**Open Problem 4.1 (Computational complexity).** Computing H⁰(F) for a cellular sheaf on a graph is straightforward (Gaussian elimination). Computing H¹(F) requires solving linear systems of size O(|E|·dim(F)). What is the complexity for general sheaves? Is there a polynomial-time algorithm for H¹(F)?

**Open Problem 4.2 (Nonlinear sheaves).** All of sheaf theory is linear. Real constraint systems (robot kinematics, chemical reaction networks) are nonlinear. Is there a nonlinear extension of cellular sheaves that captures these constraints while preserving the obstruction-detection property of H¹?

**Open Problem 4.3 (Dynamic sheaves).** When the graph evolves (nodes join/leave, edges appear/disappear), the sheaf cohomology changes. Can we compute incremental updates to H⁰ and H¹ in O(poly log |V|) time?

**Open Problem 4.4 (Approximate global sections).** When H⁰ = 0 (no exact global section), what is the closest approximation? This is the **maximum satisfiable subset** problem: find the largest subgraph on which a global section exists.
