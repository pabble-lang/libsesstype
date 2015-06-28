/**
 * \file sesstype/role.h
 * \brief A participant of a protocol or session.
 */
#ifndef SESSTYPE__ROLE_H__
#define SESSTYPE__ROLE_H__

#ifdef __cplusplus
#include <string>
#include <vector>
#endif

#ifdef __cplusplus
namespace sesstype {
#endif

#ifdef __cplusplus
namespace utils {
class RoleVisitor;
} // namespace utils

/**
 * \brief Role (participant) of a protocol or session.
 */
class Role {
  public:
    /// \brief Role constructor with "default" as name.
    Role();

    /// \brief Role constructor.
    Role(std::string name);

    /// \brief Role copy constructor.
    Role(const Role &role);

    /// \brief Role destructor.
    virtual ~Role();

    /// \brief clone a Role.
    virtual Role *clone() const;

    /// \returns name of Role.
    std::string name() const;

    /// \param[in] name Sets role name to name.
    void set_name(std::string name);

    /// \brief Check if this Role matches another Role.
    /// \returns true if this Role is another Role.
    virtual bool matches(Role *other) const;

    /// \brief <tt>accept</tt> method for utils::RoleVisitor.
    virtual void accept(utils::RoleVisitor &v);


  private:
    std::string name_;
};
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
typedef Role st_role;
#else
typedef struct Role st_role;
#endif

/// \brief Create a Role.
/// \param[in] name of Role.
/// \returns newly allocated Role.
st_role *st_role_init(const char *name);

/// \brief Create a copy of a Role.
/// \param[in] name of Role.
/// \returns pointer to newly allocated copy of Role.
st_role *st_role_copy(const st_role *const role);

/// \brief Get a name of the Role.
/// \param[in] role pointer.
/// \returns string name of Role.
const char *st_role_name(st_role *const role);

/// \brief Set the name of the Role.
/// \param[in,out] role pointer.
/// \param[in] name to use.
/// \returns the modified Role.
st_role *st_role_set_name(st_role *const role, const char *name);

/// \brief Free a previously allocated Role.
/// \param[in] role pointer.
void st_role_free(st_role *role);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace sesstype
#endif

#endif//SESSTYPE__ROLE_H__
