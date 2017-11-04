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

#include <clang/AST/RecursiveASTVisitor.h>

namespace clang {
class SourceManager;
class DiagnosticsEngine;
class Decl;
class FunctionDecl;
}

class SingletonAstVisitor : public clang::RecursiveASTVisitor<SingletonAstVisitor>
{
public:
    SingletonAstVisitor(clang::DiagnosticsEngine &diagnosticsEngine, const clang::SourceManager &sourceManager);

    bool VisitFunctionDecl(const clang::FunctionDecl *function);

    bool VisitCallExpr(const clang::CallExpr *call);

private:
    static bool looksLikeSingletonAccessor(const clang::FunctionDecl &function);

    clang::DiagnosticsEngine &m_diagnosticsEngine;
    const clang::SourceManager &m_sourceManager;
};
