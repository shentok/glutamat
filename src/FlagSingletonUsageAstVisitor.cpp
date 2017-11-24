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

#include "FlagSingletonUsageAstVisitor.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>


FlagSingletonUsageAstVisitor::FlagSingletonUsageAstVisitor(clang::DiagnosticsEngine &diagnosticsEngine, const clang::SourceManager &sourceManager) :
    m_diagnosticsEngine(diagnosticsEngine),
    m_sourceManager(sourceManager)
{
}

bool FlagSingletonUsageAstVisitor::VisitCallExpr(const clang::CallExpr *call)
{
    const clang::FunctionDecl *function = call->getDirectCallee();
    if (function &&
        !m_sourceManager.isInSystemHeader(function->getLocation()) &&
        !m_sourceManager.isInSystemMacro(function->getLocation()) &&
        looksLikeSingletonAccessor(*function)) {
        const unsigned id = m_diagnosticsEngine.getCustomDiagID(clang::DiagnosticsEngine::Warning, "Usage of suspicious function or static method '%0' having no arguments but returning pointer or reference to writable object");
        const clang::DiagnosticBuilder builder = m_diagnosticsEngine.Report(call->getLocStart(), id);
        builder << function->getQualifiedNameAsString();
        builder.setForceEmit();
    }

    return true;
}

bool FlagSingletonUsageAstVisitor::looksLikeSingletonAccessor(const clang::FunctionDecl &function)
{
    auto method = clang::dyn_cast<const clang::CXXMethodDecl>(&function);

    return function.param_empty() &&
            (function.getReturnType().getTypePtr()->isPointerType() || function.getReturnType().getTypePtr()->isReferenceType()) &&
            !function.getReturnType()->getPointeeType().isConstQualified() &&
            (method == nullptr || (method->isStatic() && method->isUserProvided()));
}
