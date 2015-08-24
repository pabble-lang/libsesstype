/**
 * \file sesstype/parameterised/role.h
 * \brief A (parameterised) participant of a protocol or session.
 */
#ifndef SESSTYPE__PARAMETERISED__ROLE_H__
#define SESSTYPE__PARAMETERISED__ROLE_H__

#include "sesstype/role.h"

#include "sesstype/parameterised/expr.h"
#include "sesstype/parameterised/expr/rng.h"

#ifdef __cplusplus
namespace sesstype {
namespace parameterised {
namespace util {
class RoleVisitor;
} // namespace util
} // namespace sesstype
} // namespace sesstype
#endif

#ifdef __cplusplus
namespace sesstype {
namespace parameterised {
#endif

#ifdef __cplusplus
/**
 * \brief Parameterised Role (participant) of a protocol or session.
 */
class Role : public sesstype::Role {
    std::vector<Expr *> param_;

  public:
    /// \brief Role constructor with "default" as name.
    Role() : sesstype::Role(), param_() { }

    /// \brief Role constructor.
    Role(std::string name) : sesstype::Role(name), param_() { }

    /// \brief Role copy constructor.
    Role(const Role &role) : sesstype::Role(role), param_()
    {
        for (auto param : role.param_) {
            param_.push_back(param->clone());
        }
    }

    /// \brief Role destructor.
    ~Role() override
    {
        for (auto param : param_) {
            delete param;
        }
    }

    /// \brief clone a Role
    Role *clone() const override
    {
        return new Role(*this);
    }

    /// \returns Number of dimensions in the parameterised Role.
    unsigned int num_dimens() const
    {
        return param_.size();
    }

    /// \param[in] param Adds parameter as a new dimension to the Role.
    void add_param(Expr *param)
    {
        param_.push_back(param);
    }

    /// \param[in] idx Dimension index of parameterised Role.
    /// \returns expression at dimension idx.
    /// \exception std::out_of_range if dimension idx does not exist.
    Expr *operator[](std::size_t idx) const
    {
        return param_.at(idx);
    }

    /// \brief Check if this Role is/is in another Role.
    /// \returns true if this Role is/is in another Role.
    virtual bool matches(sesstype::Role *other) const
    {
        bool matching = true;
        if (auto other_param = dynamic_cast<Role *>(other)) {
            // 0. Check if current role is endpoint role (both dimen=0)
            // 1. Check if current role is a member of endpoint role (both dimen>0)
            //    current role is NOT multi and endpoint role is multi
            // 2. Check if current role is subset of endpoint role (both dimen>0)
            //    Note: endpoint role is always the full set (multi)
            //    So if name matches this is always a subset (multi)
            matching &= (name() == other_param->name()); // Name matches
            matching &= (num_dimens() == other_param->num_dimens()); // Dimen
            return matching;
        }
        return false;
    }

    /// This subsumes accept in base class (but RoleVisitor is not a subclass)
    virtual void accept(util::RoleVisitor &v);

    friend std::ostream &operator<<(std::ostream &os, Role &role);

  private:
    virtual void accept(sesstype::util::RoleVisitor &v) override { /* hidden */ }
};
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
typedef Role st_param_role;
#else
typedef struct Role st_param_role;
#endif

/// \brief Get the total dimension of a Role.
/// \param[in] role pointer.
/// \returns the dimension of the Role.
unsigned int st_role_num_dimen(st_param_role *const role);

/// \brief Get the idx'th dimension parameter of a Role.
/// \param[in] role pointer.
/// \returns the idx'th dimension parameter of the Role.
st_expr *st_role_get_param(st_param_role *const role, unsigned int idx);

/// \brief Add a new parameter to the Role as a new dimension.
/// \param[in] role pointer.
/// \param[in] param Expr.
/// \returns the modified Role.
st_param_role *st_role_add_param(st_param_role *const role, st_expr *param);

void st_param_role_free(st_param_role *role);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
} // namespace parameterised
} // namespace sesstype
#endif

#endif//SESSTYPE__PARAMETERISED__ROLE_H__
