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

#include "NonStaticPrivateMethodsAstVisitor.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/Specifiers.h>


NonStaticPrivateMethodsAstVisitor::NonStaticPrivateMethodsAstVisitor(clang::DiagnosticsEngine &diagnosticsEngine) :
    m_diagnosticsEngine(diagnosticsEngine)
{
}

bool NonStaticPrivateMethodsAstVisitor::VisitCXXMethodDecl(const clang::CXXMethodDecl *method)
{
    if (method->isThisDeclarationADefinition() &&
        looksLikeSingletonAccessor(*method)) {
        const unsigned id = m_diagnosticsEngine.getCustomDiagID(clang::DiagnosticsEngine::Warning, "Definition of suspicious non-static private method '%0'");
        const clang::DiagnosticBuilder builder = m_diagnosticsEngine.Report(method->getLocStart(), id);
        builder << method->getNameAsString();
        builder.setForceEmit();
    }

    return true;
}

bool NonStaticPrivateMethodsAstVisitor::VisitCallExpr(const clang::CallExpr *call)
{
    const clang::CXXMethodDecl *method = clang::dyn_cast<const clang::CXXMethodDecl>(call->getCallee());
    if (method && looksLikeSingletonAccessor(*method)) {
        const unsigned id = m_diagnosticsEngine.getCustomDiagID(clang::DiagnosticsEngine::Warning, "Usage of suspicious function or static method '%0' having no arguments but returning pointer or reference to writable object");
        const clang::DiagnosticBuilder builder = m_diagnosticsEngine.Report(call->getLocStart(), id);
        builder << method->getQualifiedNameAsString();
        builder.setForceEmit();
    }

    return true;
}

bool NonStaticPrivateMethodsAstVisitor::looksLikeSingletonAccessor(const clang::CXXMethodDecl &method)
{
    clang::ast_matchers::cxxMethodDecl(clang::ast_matchers::isPrivate(), !clang::ast_matchers::isStatic());
    return !method.isStatic() && method.getDeclContext()->getAccessSpecifier() == clang::AS_private && method.isUserProvided();
}

bool NonStaticPrivateMethodsAstVisitor::isFromMainFile(const clang::Decl &decl)
{
    const auto &sourceManager = decl.getASTContext().getSourceManager();

    return sourceManager.isInMainFile(decl.getLocation());
}
