/**
 * \file sesstype/session.h
 * \brief A session or a session of an interaction, also known in the
 * literature as global type.
 */
#ifndef SESSTYPE__SESSION_H__
#define SESSTYPE__SESSION_H__

#ifdef __cplusplus
#include <string>
#include <iterator>
#include <unordered_map>
#endif

#include <sesstype/import.h>
#include <sesstype/node.h>
#include <sesstype/role.h>

#ifdef __cplusplus
namespace sesstype {
#endif

#define ST_TYPE_GLOBAL 1
#define ST_TYPE_LOCAL  2

#ifdef __cplusplus
/**
 * \brief Session (st_tree) class encapsulates a single session.
 *
 * The Session class is a container for
 *  - root Node representing the body of Session
 *  - Metadata about the Session, including Roles in the Session
 */
template <class BaseNode, class RoleType>
class SessionTmpl {
    std::string name_;
    int type_;
    RoleType *me_; // Localised role (only used in endpoint session)
    BaseNode *root_;
    std::unordered_map<std::string, RoleType *> roles_;

  public:
    using RoleContainer = std::unordered_map<std::string, RoleType *>;

    /// Session constructor with "default" as Session name.
    SessionTmpl()
        : name_("default"), type_(ST_TYPE_GLOBAL), me_(), root_(0), roles_() { }

    /// Session constructor.
    /// \param[in] name Session name.
    SessionTmpl(std::string name)
        : name_(name), type_(ST_TYPE_GLOBAL), me_(), root_(0), roles_() { }

    /// Session destructor.
    virtual ~SessionTmpl()
    {
        {
            for (auto role_pair : roles_) {
                delete role_pair.second;
            }
            if (root_ != NULL) {
                delete root_;
            }
        }
    }

    /// \returns name of Session.
    std::string name() const
    {
        return name_;
    }

    /// \param[in] root Node of Session body.
    void set_root(BaseNode *root)
    {
        if (root_ != NULL) {
            delete root_;
        }
        root_ = root;
    }

    /// \returns root Node of Session body.
    BaseNode *root() const
    {
        return root_;
    }

    /// \returns endpoint of current Session.
    RoleType *endpoint() const
    {
        return me_;
    }

    /// \param[in] role Sets endpoint role to endpoint.
    void set_endpoint(RoleType *endpoint)
    {
        me_ = endpoint;
        type_ = ST_TYPE_LOCAL;
    }

    /// Perform endpoint projection of Session with respect to endpoint Role.
    /// \param[in] role to project for.
    void project(Role *role)
    {
//        util::Projection projection_visitor(role);
//        root_->accept(projection_visitor);
    }

    /// \returns Session type (global or local).
    int type() const
    {
        return type_;
    }

    /// Add a Role as a participant of the Session.
    /// \param[in] role to add to this Session.
    void add_role(RoleType *role)
    {
        roles_.insert(std::pair<std::string, RoleType *>(role->name(), role));
    }

    /// Test if Role is a participant of Session.
    /// \param[in] role to look for.
    /// \returns true if role is a Role in the Session.
    bool has_role(RoleType *role) const
    {
        return (roles_.find(role->name()) != roles_.end());
    }

    /// Test if Role is a participant of Session.
    /// \param[in] name of role to look for.
    /// \returns true if role is a Role in the Session.
    bool has_role(std::string name) const
    {
        return (roles_.find(name) != roles_.end());
    }

    /// Retrieve a Role from the Session by name.
    /// \param[in] name of the Role.
    /// \returns Role named name.
    /// \exception std::out_of_range if not found.
    RoleType *role(std::string name) const
    {
        return roles_.at(name);
    }

    unsigned int num_roles() const
    {
        return roles_.size();
    }

    typename RoleContainer::const_iterator role_begin() const
    {
        return roles_.begin();
    }

    typename RoleContainer::const_iterator role_end() const
    {
        return roles_.end();
    }
};

using Session = SessionTmpl<Node, Role>;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
typedef Session st_tree;
#else
typedef struct Session st_tree;
#endif

/// \param[in] name of the session.
/// \returns pointer to session object allocated dynamically.
st_tree *st_tree_mk_init(const char *name);

/// \param[in,out] tree pointer to session object.
/// \param[in] role of the local session.
/// \returns pointer to modified session object.
st_tree *st_tree_add_role(st_tree *tree, st_role *role);

/// \param[in,out] tree pointer to session object.
/// \parma[in] root to use.
/// \returns pointer to modified session object.
st_tree *st_tree_set_root(st_tree *tree, st_node *root);

/// \param[in,out] tree pointer to session object.
/// \returns pointer to root (body of session).
st_node *st_tree_get_root(st_tree *tree);

/// \param[in,out] session object to destroy.
void st_tree_free(st_tree *tree);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace sesstype
#endif

#endif//SESSTYPE__SESSION_H__
