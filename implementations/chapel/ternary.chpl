/**
 * Ternary Music Bridge — Chapel Implementation
 * 
 * Chapel is designed for parallel computing. This reveals how
 * the ternary group operations can be distributed across cores,
 * nodes, or GPUs. The ternary math IS embarrassingly parallel.
 *
 * Forgemaster: Run with `chpl ternary.chpl -o ternary && ./ternary`
 * For parallel: `./ternary --numLocales=4` on a cluster
 */

use Random;

// Ternary type as an enum
enum Ternary { Opposition=-1, Sustain=0, Assertion=1 };

// Interval record with operator overloading
record Interval {
    var semitones: int;
    var name: string;
    
    // Conservation law as an operator — Chapel makes this natural
    operator+(a: Interval, b: Interval): Interval {
        if a.semitones + b.semitones == 0 then
            return new Interval(0, "unison");  // Conservation!
        else
            return new Interval(a.semitones + b.semitones, "composite");
    }
}

// Compile-time interval constants
const MAJOR_THIRD = new Interval(4, "major third up");
const UNISON = new Interval(0, "unison");
const MINOR_THIRD = new Interval(-4, "minor third down");

// Procedural conversion — can be run in parallel with forall
proc vectorToNotes(vector: [] Ternary, basePitch: uint(8) = 60): [] uint(8) {
    var notes: [0..vector.size] uint(8);
    notes[0] = basePitch;
    for i in 0..vector.size-1 {
        const interval = select(vector[i]) {
            when Ternary.Assertion do 4;
            when Ternary.Opposition do -4;
            otherwise 0;
        };
        var next = notes[i]:int + interval;
        next = max(0, min(127, next));
        notes[i+1] = next:uint(8);
    }
    return notes;
}

// Parallel symmetry analysis using forall
proc analyzeSymmetry(vector: [] Ternary): [] (string, int, int) {
    var groups: [0..vector.size/2] (string, int, int);
    var count = 0;
    forall i in 0..vector.size/2-1 with (ref count) {
        const left = vector[i];
        const right = vector[vector.size-1-i];
        if left == right {
          groups[count] = ("mirror", i, vector.size-1-i);
          count += 1;
        } else if left:int + right:int == 0 {
          groups[count] = ("conservation", i, vector.size-1-i);
          count += 1;
        }
    }
    return groups[0..count-1];
}

proc main() {
    // Prove conservation
    const proof = MAJOR_THIRD + MINOR_THIRD;
    writeln("Conservation: ", MAJOR_THIRD.name, " + ", MINOR_THIRD.name,
            " = ", proof.name, " (", proof.semitones, " semitones)");
    
    // Test with domain array
    var vector: [0..7] Ternary = [
        Ternary.Assertion, Ternary.Sustain, Ternary.Opposition, Ternary.Assertion,
        Ternary.Sustain, Ternary.Opposition, Ternary.Assertion, Ternary.Assertion
    ];
    
    var notes = vectorToNotes(vector);
    write("Notes: [");
    for n in notes do write(" ", n);
    writeln(" ]");
    
    // Parallel symmetry analysis
    var sym = analyzeSymmetry(vector);
    writeln("Symmetry groups: ", sym.size);
    for g in sym do writeln("  ", g[0], ": idx ", g[1], " ⇄ idx ", g[2]);
}
