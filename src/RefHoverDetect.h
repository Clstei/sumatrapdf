/* Copyright 2026 the SumatraPDF project authors (see AUTHORS file).
   License: GPLv3 */

// Pure-function region detectors used by RefHover to decide what slice of the
// destination page to render into the hover popup. Kept engine-independent so
// the heuristics can be unit-tested with synthetic glyph arrays (see
// src/utils/tests/RefHover_ut.cpp).
//
// All three functions take:
//   text     — per-glyph WCHAR array (engine->GetTextForPage's first out-ptr)
//   coords   — per-glyph Rect array, parallel to `text` (second out-ptr)
//   textLen  — glyph count
//   mediabox — page bounds in PDF user space
//   destX, destY — link's destination coordinates (PDF user space)
//
// Returned RectF is in PDF user space, clipped to mediabox.

// Landscape view: full page width strip anchored at destY, extending downward
// to the last text glyph or a recognised caption block. Fallback when no
// recognisable entry or equation is found.
RectF LandscapeBox(RectF mediabox, float destX, float destY, const WCHAR* text, const Rect* coords, int textLen);

// Equation cross-ref: tight one-line box around a "(N)" or "(N.M)" label
// sitting at the right column edge near destY. Returns empty rect when no
// equation label is detected.
RectF DetectEquationBox(const WCHAR* text, const Rect* coords, int textLen, RectF mediabox, float destX, float destY);

// Bibliography / glossary / abbreviation entry box. Tries bracket-style
// ("[Foo+09]"), hanging-indent author-year, and single-line description-list
// layouts. Falls back to LandscapeBox when the destination doesn't look like
// a list entry.
RectF DetectEntryBox(const WCHAR* text, const Rect* coords, int textLen, RectF mediabox, float destX, float destY);

// Plain-text citation detection (PDFs without hyperref). Detect a
// "(Surname et al., 2020)" / "Surname (2020)" pattern at pagePos (page
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
