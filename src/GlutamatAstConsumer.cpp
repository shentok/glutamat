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

#include "GlutamatAstConsumer.h"

#include "SingletonAstVisitor.h"

#include <clang/AST/ASTContext.h>
#include <clang/Frontend/CompilerInstance.h>

GlutamatAstConsumer::GlutamatAstConsumer(const clang::CompilerInstance &compiler) :
    clang::ASTConsumer(),
    m_diagnosticsEngine(compiler.getDiagnostics())
{
}

void GlutamatAstConsumer::HandleTranslationUnit(clang::ASTContext &astContext)
{
    SingletonAstVisitor visitor(m_diagnosticsEngine, astContext.getSourceManager());
    visitor.TraverseDecl(astContext.getTranslationUnitDecl());
}
