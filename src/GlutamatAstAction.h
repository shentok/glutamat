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

#include <clang/Frontend/FrontendAction.h>

#include "GlutamatAstConsumer.h"


// The const analyser plugin... Implements the neccessary interface
// to be a plugin. Parse command line arguments and dispatch the
// real work to other classes.
class GlutamatAstAction : public clang::PluginASTAction
{
public:
    GlutamatAstAction();

    GlutamatAstAction(GlutamatAstAction const &) = delete;
    GlutamatAstAction & operator=(GlutamatAstAction const &) = delete;

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef) override;

    bool ParseArgs(const clang::CompilerInstance &, const std::vector<std::string> &arguments) override;

    static const char *const plugin_name;

private:
    static llvm::cl::opt<GlutamatAstConsumer::Level> s_level;
};
