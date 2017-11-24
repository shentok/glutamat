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

#include "FlagSingletonOwnUsageAstVisitor.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Type.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>


class FlagSingletonOwnUsageAstVisitor::Foo : public clang::RecursiveASTVisitor<FlagSingletonOwnUsageAstVisitor::Foo>
{
public:
    Foo(clang::DiagnosticsEngine &diagnosticsEngine, const clang::CXXRecordDecl *cxxRecordDecl);

    bool VisitCallExpr(const clang::CallExpr *callExpr);

private:
    clang::DiagnosticsEngine &m_diagnosticsEngine;
    const clang::CXXRecordDecl *const m_cxxRecordDecl;
};

FlagSingletonOwnUsageAstVisitor::Foo::Foo(clang::DiagnosticsEngine &diagnosticsEngine, const clang::CXXRecordDecl *cxxRecordDecl) :
    m_diagnosticsEngine(diagnosticsEngine),
    m_cxxRecordDecl(cxxRecordDecl)
{
}

bool FlagSingletonOwnUsageAstVisitor::Foo::VisitCallExpr(const clang::CallExpr *callExpr)
{
    const clang::FunctionDecl *function = callExpr->getDirectCallee();
    auto method = clang::dyn_cast_or_null<const clang::CXXMethodDecl>(function);

    if (method &&
        method->getParent()->getCanonicalDecl() == m_cxxRecordDecl &&
        looksLikeSingletonAccessor(*function) &&
        method->getReturnType()->getPointeeType()->getAsCXXRecordDecl() == m_cxxRecordDecl) {
        const unsigned id = m_diagnosticsEngine.getCustomDiagID(clang::DiagnosticsEngine::Warning, "Getting pointer or reference to singleton '%0' in its non-static method does not make sense");
        const clang::DiagnosticBuilder builder = m_diagnosticsEngine.Report(callExpr->getLocStart(), id);
        builder << method->getParent()->getQualifiedNameAsString();
        builder.setForceEmit();
    }

    return true;
}

FlagSingletonOwnUsageAstVisitor::FlagSingletonOwnUsageAstVisitor(clang::DiagnosticsEngine &diagnosticsEngine, const clang::SourceManager &sourceManager) :
    m_diagnosticsEngine(diagnosticsEngine),
    m_sourceManager(sourceManager)
{
}

bool FlagSingletonOwnUsageAstVisitor::VisitCXXMethodDecl(clang::CXXMethodDecl *method)
{
    if (!method->isStatic() &&
        !m_sourceManager.isInSystemHeader(method->getLocation()) &&
        !m_sourceManager.isInSystemMacro(method->getLocation()))
    {
        const auto cxxRecordDecl = method->getParent();
        const auto canonicalCxxRecordDecl = cxxRecordDecl->getCanonicalDecl();

        Foo visitor(m_diagnosticsEngine, canonicalCxxRecordDecl);
        visitor.TraverseDecl(method);
    }

    return true;
}

bool FlagSingletonOwnUsageAstVisitor::looksLikeSingletonAccessor(const clang::FunctionDecl &function)
{
    auto method = clang::dyn_cast<const clang::CXXMethodDecl>(&function);

    return function.param_empty() &&
            (function.getReturnType().getTypePtr()->isPointerType() || function.getReturnType().getTypePtr()->isReferenceType()) &&
            !function.getReturnType()->getPointeeType().isConstQualified() &&
            (method == nullptr || (method->isStatic() && method->isUserProvided()));
}
