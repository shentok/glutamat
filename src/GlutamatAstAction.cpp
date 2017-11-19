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

llvm::cl::opt<GlutamatAstConsumer::Level> GlutamatAstAction::s_level(
        "level",
        llvm::cl::desc("Set the analysis level:"),
        llvm::cl::values(
            clEnumValN(GlutamatAstConsumer::Level::Evil, "evil", "Find calls to <<Singleton::getInstance()>> inside Singleton's own non-static methods"),
            clEnumValN(GlutamatAstConsumer::Level::Bad, "bad", "Find calls to <<Singleton::getInstance()>> inside Singleton's own methods"),
            clEnumValN(GlutamatAstConsumer::Level::All, "all", "Find definitions and usages of singletons (default)")),
        llvm::cl::init(GlutamatAstConsumer::Level::All));


GlutamatAstAction::GlutamatAstAction() :
    clang::PluginASTAction()
{}

std::unique_ptr<clang::ASTConsumer> GlutamatAstAction::CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef)
{
    if (!compiler.getLangOpts().CPlusPlus) {
        return std::unique_ptr<clang::ASTConsumer>(new clang::ASTConsumer()); // dummy consumer
    }

    return std::unique_ptr<clang::ASTConsumer>(new GlutamatAstConsumer(compiler, s_level));
}

bool GlutamatAstAction::ParseArgs(const clang::CompilerInstance &, const std::vector<std::string> &arguments)
{
    std::vector<const char *> argumentPointers;

    {
        // make llvm::cl::ParseCommandLineOptions happy
        argumentPointers.push_back(plugin_name);
        for (auto & argument : arguments) {
            argumentPointers.push_back(argument.c_str());
        }
    }

    llvm::cl::ParseCommandLineOptions(argumentPointers.size(), &argumentPointers.front());

    return true;
}
