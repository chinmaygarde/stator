// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#pragma once

#include <sstream>
#include <vector>

#include "types.h"

namespace epoxy {

class Sema {
 public:
  enum class Result {
    kSuccess,
    kError,
  };

  Sema();

  ~Sema();

  Sema(const Sema&) = delete;

  Sema& operator=(const Sema&) = delete;

  Result Perform(std::vector<Namespace> namespaces);

  void PrettyPrintErrors(std::ostream& stream);

  const std::vector<Namespace>& GetNamespaces() const;

 private:
  std::stringstream errors_;
  std::vector<Namespace> namespaces_;
};

}  // namespace epoxy
