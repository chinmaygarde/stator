// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#pragma once

#include "impeller/stator/epoxy/decls.h"
#include "lexer.h"

namespace epoxy {

class Scanner {
 public:
  explicit Scanner(const std::string& text);

  ~Scanner();

  Scanner(const Scanner&) = delete;

  Scanner& operator=(const Scanner&) = delete;

  bool IsValid() const;

  yyscan_t GetHandle();

 private:
  yyscan_t scanner_ = {};
  YY_BUFFER_STATE buffer_ = {};
  bool is_valid_ = false;
};

}  // namespace epoxy
