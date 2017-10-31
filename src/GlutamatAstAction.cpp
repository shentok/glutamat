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

#include "GlutamatAstAction.h"

#include "GlutamatAstConsumer.h"

#include <clang/Frontend/CompilerInstance.h>

#include "llvm/Support/CommandLine.h"

const char *const GlutamatAstAction::plugin_name = "glutamat";

GlutamatAstAction::GlutamatAstAction() :
    clang::PluginASTAction()
{}

std::unique_ptr<clang::ASTConsumer> GlutamatAstAction::CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef)
{
    if (!compiler.getLangOpts().CPlusPlus) {
        return std::unique_ptr<clang::ASTConsumer>(new clang::ASTConsumer()); // dummy consumer
    }

    return std::unique_ptr<clang::ASTConsumer>(new GlutamatAstConsumer(compiler));
}

bool GlutamatAstAction::ParseArgs(const clang::CompilerInstance &, const std::vector<std::string> &)
{
    return true;
}
