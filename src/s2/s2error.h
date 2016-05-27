// Copyright 2013 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS-IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// Author: ericv@google.com (Eric Veach)
//
// S2Error is a simple class consisting of an error code and a human-readable
// error message.

#ifndef S2_S2ERROR_H_
#define S2_S2ERROR_H_

#include <cstdarg>
#include <ostream>
#include <string>

#include "s2/base/port.h"

// This class is intended to be copied by value as desired.  It uses
// the default copy constructor and assignment operator.
class S2Error {
 public:
  enum Code {
    // Error codes that apply to more than one type of geometry:
    OK = 0,                  // No error.
    NO_ERROR = OK,           // DEPRECATED synonym for OK.
    NOT_UNIT_LENGTH = 1,     // Vertex is not unit length.
    DUPLICATE_VERTICES = 2,  // There are two identical vertices.
    ANTIPODAL_VERTICES = 3,  // There are two antipodal vertices.

    // Error codes that only apply to certain geometric objects:
    LOOP_NOT_ENOUGH_VERTICES = 100,  // Loop with fewer than 3 vertices.
    LOOP_SELF_INTERSECTION = 101,    // Loop has a self-intersection.

    POLYGON_LOOPS_SHARE_EDGE = 200,  // Two polygon loops share an edge.
    POLYGON_LOOPS_CROSS = 201,       // Two polygon loops cross.
    POLYGON_EMPTY_LOOP = 202,        // Polygon has an empty loop.
    POLYGON_EXCESS_FULL_LOOP = 203,  // Non-full polygon has a full loop.

    // InitOriented() was called and detected inconsistent loop orientations.
    POLYGON_INCONSISTENT_LOOP_ORIENTATIONS = 204,

    // Loop depths don't correspond to any valid nesting hierarchy.
    POLYGON_INVALID_LOOP_DEPTH = 205,

    // Actual polygon nesting does not correspond to the nesting hierarchy
    // encoded by the loop depths.
    POLYGON_INVALID_LOOP_NESTING = 206,

    // The S2Builder snap function moved a vertex by more than the specified
    // snap radius.
    BUILDER_SNAP_RADIUS_TOO_SMALL = 300,

    // S2Builder expected all edges to have siblings (as specified by
    // S2Builder::GraphOptions::SiblingPairs::REQUIRE), but some were missing.
    BUILDER_MISSING_EXPECTED_SIBLING_EDGES = 301,

    // S2Builder found an unexpected degenerate edge.  For example,
    // Graph::GetLeftTurnMap() does not support degenerate edges.
    BUILDER_UNEXPECTED_DEGENERATE_EDGE = 302,

    // S2Builder found a vertex with (indegree != outdegree), which means
    // that the given edges cannot be assembled into loops.
    BUILDER_EDGES_DO_NOT_FORM_LOOPS = 303,

    // The edges provided to S2Builder cannot be assembled into a polyline.
    BUILDER_EDGES_DO_NOT_FORM_POLYLINE = 304,
  };
  S2Error() : code_(OK), text_() {}

  // Set the error to the given code and printf-style message.  Note that you
  // can prepend text to an existing error by calling Init() more than once:
  //
  //   error->Init(error->code(), "Loop %d: %s", j, error->text().c_str());
  void Init(Code code, const char* format, ...) PRINTF_ATTRIBUTE(3, 4);

  bool ok() const { return code_ == OK; }
  Code code() const { return code_; }
  string text() const { return text_; }

 private:
  Code code_;
  string text_;
};

inline std::ostream& operator<<(std::ostream& os, S2Error const& error) {
  return os << error.text();
}

#endif  // S2_S2ERROR_H_
