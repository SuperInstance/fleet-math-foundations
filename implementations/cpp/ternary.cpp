/**
 * Ternary Music Bridge — C++ Implementation
 * 
 * Uses templates and constexpr to compute the interval group
 * at COMPILE TIME. The conservation law is enforced by the type system
 * before the program ever runs.
 */

#include <iostream>
#include <array>
#include <string>
#include <vector>

// Compile-time ternary values as a type parameter
template<int T>
struct TernaryValue {
    static_assert(T == -1 || T == 0 || T == 1, "Ternary must be -1, 0, or +1");
    static constexpr int value = T;
};

// Compile-time interval computation
struct Interval {
    int semitones;
    const char* name;
    
    constexpr Interval(int s, const char* n) : semitones(s), name(n) {}
    
    // Conservation law enforced at compile time
    friend constexpr Interval operator+(Interval a, Interval b) {
        return (a.semitones + b.semitones == 0) 
            ? Interval(0, "unison")  // Conservation!
            : Interval(a.semitones + b.semitones, "composite");
    }
};

// Compile-time interval constants
constexpr Interval MAJOR_THIRD(4, "major third up");
constexpr Interval UNISON(0, "unison");
constexpr Interval MINOR_THIRD(-4, "minor third down");

// Constexpr ternary-to-interval mapping
constexpr Interval ternary_to_interval(int t) {
    if (t == 1) return MAJOR_THIRD;
    if (t == -1) return MINOR_THIRD;
    return UNISON;
}

// Compile-time proof of conservation law
static_assert((MAJOR_THIRD + MINOR_THIRD).semitones == 0, 
              "Conservation law violated: major third + minor third must equal unison");
static_assert((MINOR_THIRD + MAJOR_THIRD).semitones == 0,
              "Conservation law violated: minor third + major third must equal unison");

// Template-based vector transformation — type-safe at compile time
template<size_t N>
std::array<unsigned char, N + 1> vector_to_notes(const std::array<int, N>& vector, 
                                                  unsigned char base_pitch = 60) {
    std::array<unsigned char, N + 1> notes{};
    notes[0] = base_pitch;
    for (size_t i = 0; i < N; i++) {
        Interval interval = ternary_to_interval(vector[i]);
        int last = notes[i];
        int next = last + interval.semitones;
        if (next < 0) next = 0;
        if (next > 127) next = 127;
        notes[i + 1] = (unsigned char)next;
    }
    return notes;
}

int main() {
    // Prove conservation at runtime too
    Interval proof = MAJOR_THIRD + MINOR_THIRD;
    std::cout << "Conservation (compile-time proven): "
              << MAJOR_THIRD.name << " + " << MINOR_THIRD.name
              << " = " << proof.name << " (" << proof.semitones << " semitones)"
              << std::endl;
    
    // Template with automatic size deduction
    std::array vector = {1, 0, -1, 1, 0, -1, 1, 1};
    auto notes = vector_to_notes(vector);
    
    std::cout << "\nVector: [";
    for (size_t i = 0; i < vector.size(); i++)
        std::cout << (i ? ", " : "") << vector[i];
    std::cout << "]\nNotes: [";
    for (size_t i = 0; i < notes.size(); i++)
        std::cout << (i ? ", " : "") << (int)notes[i];
    std::cout << "]" << std::endl;
    
    return 0;
}
