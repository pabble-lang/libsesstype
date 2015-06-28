/**
 * \file parameterised/simplify.cc
 * \brief Expression simplifier.
 */
#include <iostream>
#include <ostream>

#include <sesstype/parameterised/expr.h>
#include <sesstype/utils.h>

namespace sesstype {
namespace utils {

void Simplifier::visit(parameterised::VarExpr *expr)
{
}

void Simplifier::visit(parameterised::ValExpr *expr)
{
}

void Simplifier::visit(parameterised::AddExpr *expr)
{
}

void Simplifier::visit(parameterised::SubExpr *expr)
{
}

void Simplifier::visit(parameterised::MulExpr *expr)
{
}

void Simplifier::visit(parameterised::DivExpr *expr)
{
}

void Simplifier::visit(parameterised::ModExpr *expr)
{
}

void Simplifier::visit(parameterised::ShlExpr *expr)
{
}

void Simplifier::visit(parameterised::ShrExpr *expr)
{
}

void Simplifier::visit(parameterised::SeqExpr *expr)
{
}

void Simplifier::visit(parameterised::RngExpr *expr)
{
}

} // namespace utils

} // namespace sesstype