/* Copyright 2026 the SumatraPDF project authors (see AUTHORS file).
   License: GPLv3 */

// Pure-function plain-text citation detectors for PDFs without hyperref links.
// Engine-independent so the heuristics can be unit-tested with synthetic glyph
// arrays (see src/utils/tests/RefHover_ut.cpp).
//
// Both functions take the engine->GetTextForPage out-ptrs:
//   text     — per-glyph WCHAR array
//   coords   — per-glyph Rect array, parallel to `text`
//   textLen  — glyph count

// Detect a "(Surname et al., 2020)" / "Surname (2020)" pattern at pagePos (page
// coordinates). On success returns true and fills *surnameOut with a
// freshly-allocated UTF-8 surname (caller frees) and *yearOut.
bool DetectCitationInPageText(const WCHAR* text, const Rect* coords, int textLen, Point pagePos, char** surnameOut,
                              int* yearOut);

// Search a page's glyph arrays for a bibliography entry whose line starts
// with (or contains, near the line start) `surnameW` and whose entry text
// contains `year`. Returns true on hit and fills xOut/yOut with the entry's
// anchor (top-left of the matching line's first glyph).
bool FindSurnameInPageText(const WCHAR* text, const Rect* coords, int textLen, const WCHAR* surnameW, int surnameLen,
                           int year, float* xOut, float* yOut);
