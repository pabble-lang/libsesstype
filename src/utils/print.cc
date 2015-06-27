/**
 * \file print.cc
 * \brief Printer.
 */
#include <cstring>
#include <iostream>
#include <ostream>
#include <sstream>

#include <sesstype/node.h>
#include <sesstype/role.h>
#include <sesstype/utils.h>

namespace sesstype {
namespace utils {

Printer::Printer()
    : os_(std::cout), indent_lvl_(0), indent_str_("  "), line_count_(1)
{
}

Printer::Printer(std::ostream &os)
    : os_(os), indent_lvl_(0), indent_str_("  "), line_count_(1)
{
}

void Printer::prefix()
{
    os_ << line_count_++ << ": ";
    if (indent_lvl_ > 1) {
        os_ << '|';
        for (int i=1; i<indent_lvl_; i++) {
            os_ << indent_str_;
        }
    }
}

void Printer::reset_line_num()
{
    indent_lvl_ = 0;
    line_count_ = 1;
}

// Nodes ---------------------------------------------------------------------

void Printer::visit(InteractionNode *node)
{
    prefix();
    os_ << "interaction { from: ";
    if (node->from()) {
        node->from()->accept(*this);
    } else {
        os_ << "(empty)";
    }
    os_ << ", to(" << node->num_to() << "): [";
    if (node->num_to() > 0) {
        node->to()->accept(*this);
    } else {
        os_ << "(empty)";
    }
    os_ << (node->num_to()>1 ? ".. ]" : "]")
        << ", msgsig: " << node->msgsig()->label()
        << "(" << node->msgsig()->num_payload() << ") } @" << node << "\n";
}

void Printer::visit(BlockNode *node)
{
    indent_lvl_++;
    for (auto it=node->child_begin(); it!=node->child_end(); it++) {
        (*it)->accept(*this);
    }
    indent_lvl_--;
}

void Printer::visit(RecurNode *node)
{
    prefix();
    os_ << "recur " << "{ label: " << node->label() << " }";
    os_ << " children: " << node->num_child() << " @" << node << "\n";

    node->BlockNode::accept(*this);
}

void Printer::visit(ContinueNode *node)
{
    prefix();
    os_ << "cont { label: " << node->label() << " } @" << node << "\n";
}

void Printer::visit(ChoiceNode *node)
{
    prefix();
    os_  << " choice { at: " << node->at()->name() << " }";
    os_ << " children: " << node->num_child() << " @" << node << "\n";

    node->BlockNode::accept(*this);
}

// Roles ---------------------------------------------------------------------

void Printer::visit(Role *role)
{
    os_ << role->name();
}

} // namespace utils

extern "C" {

void st_node_print(st_node *node)
{
    sesstype::utils::Printer p;
    node->accept(p);
}

} // extern "C"

} // namespace sesstype
