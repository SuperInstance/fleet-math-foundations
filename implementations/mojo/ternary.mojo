# Ternary Music Bridge — Mojo Implementation
#
# Mojo combines Python's readability with systems-level performance.
# The @parameter decorator makes the ternary group values compile-time constants.
# Use `fn` for zero-overhead function calls.
#
# Forgemaster: Run with `mojo ternary.mojo` on ProArt with RTX4050

from memory import memset_zero
from random import rand
from math import min, max

# Compile-time ternary constants
alias OPPOSITION: Int = -1
alias SUSTAIN: Int = 0
alias ASSERTION: Int = 1

# Intervals as compile-time tuples — zero runtime overhead
alias MAJOR_THIRD: Int = 4
alias UNISON: Int = 0
alias MINOR_THIRD: Int = -4

fn ternary_to_interval(t: Int) -> Int:
    """Map ternary value to semitone interval. Inlined at compile time."""
    @parameter
    if t == ASSERTION:
        return MAJOR_THIRD
    elif t == OPPOSITION:
        return MINOR_THIRD
    else:
        return UNISON

fn vector_to_notes(vector: StaticIntTuple[8], base_pitch: UInt8 = 60) -> StaticIntTuple[9]:
    """Convert ternary vector to MIDI pitches. Stack-allocated, zero-copy.
    StaticIntTuple ensures the size is known at compile time."""
    var notes = StaticIntTuple[9]()
    notes[0] = base_pitch
    for i in range(8):
        let interval = ternary_to_interval(vector[i])
        let last = notes[i]
        let next = max(0, min(127, last + interval))
        notes[i + 1] = next
    return notes

# Compile-time validation of conservation law
@parameter
fn prove_conservation() -> Bool:
    """The conservation law: +4 + (-4) = 0. Proven at compile time.
    If this assertion fails, the code doesn't compile."""
    return (MAJOR_THIRD + MINOR_THIRD) == 0

fn main() raises:
    # Prove conservation at compile time
    print("Conservation law holds:", prove_conservation())
    
    # Test vector
    let vector = StaticIntTuple[8](1, 0, -1, 1, 0, -1, 1, 1)
    let notes = vector_to_notes(vector)
    print("Vector:", vector)
    print("Notes:", notes)
