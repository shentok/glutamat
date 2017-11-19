/*  Copyright (C) 2017  Bernhard Beschow <shentey@gmail.com>
    This file is part of Glutamat, a singleton definition detector plugin for Clang.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <clang/AST/ASTConsumer.h>

#include "SingletonAstVisitor.h"

namespace clang {
class CompilerInstance;
class DiagnosticsEngine;
}

// It runs the pseudo const analysis on the given translation unit.
class GlutamatAstConsumer : public clang::ASTConsumer
{
public:
    enum class Level {
        Evil,
        Bad,
        All
    };

    GlutamatAstConsumer(const clang::CompilerInstance &compiler, Level level);
    GlutamatAstConsumer(const GlutamatAstConsumer &) = delete;
    GlutamatAstConsumer &operator=(const GlutamatAstConsumer &) = delete;

    void HandleTranslationUnit(clang::ASTContext &astContext) override;

private:
    clang::DiagnosticsEngine &m_diagnosticsEngine;
    Level m_level;
};
