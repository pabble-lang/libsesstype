/**
 * \file test/projection.cc
 * \brief Tests for sesstype::utils::Projection.
 */

#include "gtest/gtest.h"

#include <iostream>
#include <string>

#include "sesstype/role.h"
#include "sesstype/util.h"

#include "sesstype/node.h"
#include "sesstype/node/interaction.h"
#include "sesstype/node/recur.h"
#include "sesstype/node/continue.h"

namespace sesstype {
namespace tests {

class ProjectionTest : public ::testing::Test {
  protected:
    ProjectionTest() {}
};

/**
 * \test Basic usage of Role.
 */
TEST_F(ProjectionTest, BasicProjection)
{
    auto *root = new sesstype::BlockNode();

    // Interaction 1.
    auto *ALICE = new sesstype::Role("Alice");
    auto *BOB = new sesstype::Role("Bob");
    auto *CAROL = new sesstype::Role("Carol");

    auto *interact_node = new sesstype::InteractionNode(new MsgSig("First"));
    interact_node->set_sndr(ALICE);
    interact_node->add_rcvr(BOB);
    interact_node->add_rcvr(CAROL);
    root->append_child(interact_node);

    // Interaction 2.
    auto *interact2_node = new sesstype::InteractionNode(new MsgSig("Second"));
    interact2_node->set_sndr(ALICE);
    interact2_node->add_rcvr(CAROL);
    root->append_child(interact2_node);

    // Recur 1.
    auto *recur1_node = new sesstype::RecurNode("Rec0");
    root->append_child(recur1_node);

    // Recur 2.
    auto *recur2_node = new sesstype::RecurNode("Rec1");
    recur1_node->append_child(recur2_node);

    // Interaction 3.
    auto *MALLORY = new sesstype::Role("Mallory");
    auto *msgsig1 = new sesstype::MsgSig("Label");
    auto *interact3_node = new sesstype::InteractionNode();
    interact3_node->set_sndr(ALICE);
    interact3_node->add_rcvr(MALLORY);
    interact3_node->set_msg(msgsig1);
    recur2_node->append_child(interact3_node);

    // Continue 1.
    auto *cont1_node = new sesstype::ContinueNode("Rec0");
    recur1_node->append_child(cont1_node);

    // Interaction 4.
    auto *interact4_node = new sesstype::InteractionNode(new MsgSig("Third"));
    interact4_node->set_sndr(BOB);
    interact4_node->add_rcvr(MALLORY);
    interact4_node->add_rcvr(ALICE);

    root->append_child(interact4_node);

    util::Print print_visitor;
    root->accept(print_visitor);

    // Project endpoint.
    sesstype::util::Projection projection_bob(BOB);
    root->accept(projection_bob);
    sesstype::Node *endpoint_root = projection_bob.get_root();

    EXPECT_EQ(endpoint_root->type(), ST_NODE_ROOT);
    BlockNode *ep_bob_root = dynamic_cast<BlockNode *>(endpoint_root);
    EXPECT_EQ(ep_bob_root->num_children(), 3);

    EXPECT_EQ(ep_bob_root->child(0)->type(), ST_NODE_SENDRECV);
    InteractionNode *ep_bob_interact0 = dynamic_cast<InteractionNode *>(ep_bob_root->child(0));
    EXPECT_EQ(ep_bob_interact0->msg()->label(), "First");
    EXPECT_EQ(ep_bob_interact0->rcvr()->name(), "Alice");

    EXPECT_EQ(ep_bob_root->child(1)->type(), ST_NODE_RECUR);
    RecurNode *ep_bob_recur0 = dynamic_cast<RecurNode *>(ep_bob_root->child(1));
    EXPECT_EQ(ep_bob_recur0->label(), "Rec0");
    EXPECT_EQ(ep_bob_recur0->num_children(), 2);

    EXPECT_EQ(ep_bob_recur0->child(0)->type(), ST_NODE_RECUR);
    RecurNode *ep_bob_recur1 = dynamic_cast<RecurNode *>(ep_bob_recur0->child(0));
    EXPECT_EQ(ep_bob_recur1->label(), "Rec1");
    EXPECT_EQ(ep_bob_recur1->num_children(), 0);

    EXPECT_EQ(ep_bob_recur0->child(1)->type(), ST_NODE_CONTINUE);
    ContinueNode *ep_bob_interact1 = dynamic_cast<ContinueNode *>(ep_bob_recur0->child(1));
    EXPECT_EQ(ep_bob_interact1->label(), "Rec0");

    EXPECT_EQ(ep_bob_root->child(2)->type(), ST_NODE_SENDRECV);
    InteractionNode *ep_bob_interact2 = dynamic_cast<InteractionNode *>(ep_bob_root->child(2));
    EXPECT_EQ(ep_bob_interact2->msg()->label(), "Third");
    EXPECT_EQ(ep_bob_interact2->rcvr()->name(), "Mallory");
}

} // namespace tests
} // namespace sesstype

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
