#include <iostream>

#include <sesstype/msg.h>
#include <sesstype/node.h>
#include <sesstype/role.h>
#include <sesstype/node/interaction.h>

namespace sesstype {

st_node *st_mk_interaction_node_init()
{
    return new InteractionNode();
}

st_node *st_mk_interaction_node(st_msg *msg)
{
    return new InteractionNode(msg);
}

st_node *st_interaction_node_set_msgsig(st_node *node, st_msg *msgsig)
{
    if (InteractionNode *interaction = dynamic_cast<InteractionNode *>(node)) {
        interaction->set_msgsig(msgsig);
    }
    return node;
}

st_msg *st_interaction_node_get_msgsig(st_node *node)
{
    if (InteractionNode *interaction = dynamic_cast<InteractionNode *>(node)) {
        return interaction->msgsig();
    } else {
        std::cerr << __FILE__ << ":" << __LINE__ << ": "
                  << __FUNCTION__ << ": node is not an Interaction.\n";
        return nullptr;
    }
}

st_node *st_interaction_node_set_from(st_node *node, st_role *from)
{
    if (InteractionNode *interaction = dynamic_cast<InteractionNode *>(node)) {
        interaction->set_from(from);
    } else {
        std::cerr << __FILE__ << ":" << __LINE__ << ": "
                  << __FUNCTION__ << ": node is not an Interaction.\n";
    }
    return node;
}
Role *st_interaction_node_get_from(st_node *node)
{
    if (InteractionNode *interaction = dynamic_cast<InteractionNode *>(node)) {
        return interaction->from();
    } else {
        std::cerr << __FILE__ << ":" << __LINE__ << ": "
                  << __FUNCTION__ << ": node is not an Interaction.\n";
        return nullptr;
    }
}

st_node *st_interaction_node_add_to(st_node *node, st_role *to)
{
    if (InteractionNode *interaction = dynamic_cast<InteractionNode *>(node)) {
        interaction->add_to(to);
    } else {
        std::cerr << __FILE__ << ":" << __LINE__ << ": "
                  << __FUNCTION__ << ": node is not an Interaction.\n";
    }
    return node;
}

unsigned int st_interaction_node_num_tos(st_node *node)
{
    if (InteractionNode *interaction = dynamic_cast<InteractionNode *>(node)) {
        return interaction->num_tos();
    } else {
        std::cerr << __FILE__ << ":" << __LINE__ << ": "
                  << __FUNCTION__ << ": node is not an Interaction.\n";
        return 0;
    }
}

Role *st_interaction_node_get_to(st_node *node, unsigned int index)
{
    if (InteractionNode *interaction = dynamic_cast<InteractionNode *>(node)) {
        return interaction->to(index);
    } else {
        std::cerr << __FILE__ << ":" << __LINE__ << ": "
                  << __FUNCTION__ << ": node is not an Interaction.\n";
        return nullptr;
    }
}

} // namespace sesstype
