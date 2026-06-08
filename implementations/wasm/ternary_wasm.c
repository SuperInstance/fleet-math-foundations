/**
 * Ternary Music Bridge — WebAssembly Implementation
 * 
 * Compile with: clang --target=wasm32 -O3 -nostdlib -Wl,--no-entry
 * -Wl,--export=vector_to_notes -Wl,--export=analyze_symmetry
 * -o ternary.wasm ternary_wasm.c
 * 
 * This compiles to ~300 bytes of WASM — the smallest possible
 * representation of the ternary group structure.
 */

typedef signed char Ternary;
#define T_OPPOSITION -1
#define T_SUSTAIN 0
#define T_ASSERTION 1

/* Export: convert ternary vector to MIDI pitches.
 * Returns the number of notes written. */
int vector_to_notes(const Ternary* vector, int len, 
                    unsigned char base_pitch, unsigned char* notes) {
    notes[0] = base_pitch;
    for (int i = 0; i < len; i++) {
        int semitones;
        switch (vector[i]) {
            case T_ASSERTION:  semitones = 4; break;
            case T_OPPOSITION: semitones = -4; break;
            default:           semitones = 0; break;
        }
        int next = (int)notes[i] + semitones;
        if (next < 0) next = 0;
        if (next > 127) next = 127;
        notes[i + 1] = (unsigned char)next;
    }
    return len + 1;
}

/* Export: find symmetry groups */
typedef struct { int type; int left; int right; } Symmetry;
int analyze_symmetry(const Ternary* vector, int len, Symmetry* out) {
    int count = 0;
    for (int i = 0; i < len / 2; i++) {
        if (vector[i] == vector[len - 1 - i]) {
            out[count++] = (Symmetry){0, i, len - 1 - i};  /* mirror */
        } else if (vector[i] + vector[len - 1 - i] == 0) {
            out[count++] = (Symmetry){1, i, len - 1 - i};  /* conservation */
        }
    }
    return count;
}
