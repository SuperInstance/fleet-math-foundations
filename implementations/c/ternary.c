/**
 * Ternary Music Bridge — C Implementation
 * 
 * C has no objects, no generics, no closures. Just structs and functions.
 * This forces the mathematical structure to be completely explicit.
 * The ternary group, the interval mapping, the conservation law —
 * every operation is a plain function you can see in its entirety.
 */

#include <stdio.h>
#include <stdlib.h>

/* The three ternary values as an enum — the only abstraction we get */
typedef enum { OPPOSITION = -1, SUSTAIN = 0, ASSERTION = 1 } Ternary;

/* A musical interval in semitones. Plain struct, no methods. */
typedef struct {
    int semitones;
    const char* name;
} Interval;

/* The three fundamental intervals — the group's generators */
Interval MAJOR_THIRD = { 4, "major third up" };
Interval UNISON = { 0, "unison" };
Interval MINOR_THIRD = { -4, "minor third down" };

/* Map ternary value to interval. Pure function. */
Interval ternary_to_interval(Ternary t) {
    switch (t) {
        case ASSERTION:  return MAJOR_THIRD;
        case SUSTAIN:    return UNISON;
        case OPPOSITION: return MINOR_THIRD;
    }
    return UNISON;
}

/* Conservation law: +4 + (-4) = 0. Proved in C. */
Interval compose_intervals(Interval a, Interval b) {
    int sum = a.semitones + b.semitones;
    if (sum == 0) return UNISON;  /* Conservation! */
    return (Interval){ .semitones = sum, .name = "composite" };
}

/* Convert ternary vector to MIDI pitches. No allocations needed
 * for vectors up to 128 — stack-allocated because C respects stack. */
void vector_to_notes(const Ternary* vector, int len, unsigned char base_pitch,
                     unsigned char* notes_out) {
    notes_out[0] = base_pitch;
    for (int i = 0; i < len; i++) {
        Interval interval = ternary_to_interval(vector[i]);
        int last = (int)notes_out[i];
        int next = last + interval.semitones;
        if (next < 0) next = 0;
        if (next > 127) next = 127;
        notes_out[i + 1] = (unsigned char)next;
    }
}

/* Symmetry group types */
typedef enum { MIRROR, CONSERVATION } SymmetryType;
typedef struct {
    SymmetryType type;
    int left_idx;
    int right_idx;
    int left_val;
    int right_val;
} SymmetryGroup;

/* Find symmetry in a ternary vector — allocated on the heap
 * because the number of symmetry groups is unknown at compile time. */
SymmetryGroup* analyze_symmetry(const Ternary* vector, int len, int* count_out) {
    int max_groups = len / 2;
    SymmetryGroup* groups = (SymmetryGroup*)malloc(max_groups * sizeof(SymmetryGroup));
    if (!groups) { *count_out = 0; return NULL; }
    
    int count = 0;
    for (int i = 0; i < len / 2; i++) {
        Ternary left = vector[i];
        Ternary right = vector[len - 1 - i];
        if (left == right) {
            groups[count++] = (SymmetryGroup){
                .type = MIRROR, .left_idx = i, .right_idx = len - 1 - i,
                .left_val = left, .right_val = right
            };
        } else if ((int)left + (int)right == 0) {
            groups[count++] = (SymmetryGroup){
                .type = CONSERVATION, .left_idx = i, .right_idx = len - 1 - i,
                .left_val = left, .right_val = right
            };
        }
    }
    *count_out = count;
    return groups;
}

int main() {
    /* Test: the conservation law */
    Interval composed = compose_intervals(MAJOR_THIRD, MINOR_THIRD);
    printf("Conservation: %s + %s = %s (%d semitones)\n",
           MAJOR_THIRD.name, MINOR_THIRD.name,
           composed.name, composed.semitones);
    
    /* Test: vector to notes */
    Ternary vector[] = { ASSERTION, SUSTAIN, OPPOSITION, ASSERTION,
                         SUSTAIN, OPPOSITION, ASSERTION, ASSERTION };
    int len = sizeof(vector) / sizeof(vector[0]);
    unsigned char notes[9] = {0};
    vector_to_notes(vector, len, 60, notes);
    
    printf("\nVector: [");
    for (int i = 0; i < len; i++) printf("%d%s", (int)vector[i], i < len-1 ? ", " : "");
    printf("]\nNotes: [");
    for (int i = 0; i <= len; i++) printf("%d%s", (int)notes[i], i < len ? ", " : "");
    printf("]\n");
    
    /* Test: symmetry */
    int sym_count = 0;
    SymmetryGroup* sym = analyze_symmetry(vector, len, &sym_count);
    printf("Symmetry groups: %d\n", sym_count);
    for (int i = 0; i < sym_count; i++) {
        printf("  %s: idx %d ⇄ idx %d\n",
               sym[i].type == MIRROR ? "mirror" : "conservation",
               sym[i].left_idx, sym[i].right_idx);
    }
    free(sym);
    
    return 0;
}
