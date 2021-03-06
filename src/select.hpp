#ifndef SQLLIB_SELECT_HPP
#define SQLLIB_SELECT_HPP

#include "defines.hpp"
#include "query.hpp"

#include <string>
#include <sstream>
#include <memory>

SQLLIB_NS

template<typename...>
class TableSetImpl;

template<typename TableTuple, int... Ns>
class SelectImpl : public Query, public std::enable_shared_from_this<SelectImpl<TableTuple, Ns...>> {
public:
    using CallbackType = typename SelectCallbackType<TableTuple, TList<>, Ns...>::type;

    virtual ~SelectImpl();

    auto callback(CallbackType func);
    void execute(std::shared_ptr<Connection> connection) override;

protected:
    TableTuple tableTuple;
    CallbackType callbackFunc;

    SelectImpl(TableTuple tableTuple);
};

template<typename TableTuple, int... Ns>
class Select : public SelectImpl<TableTuple, Ns...> {
public:
    using Ptr = std::shared_ptr<Select<TableTuple, Ns...>>;

    template<int Index, int FieldID>
    auto select();

    template<typename Where>
    auto where();
    
    std::string sql() override;

private:
    template<typename...>
    friend class TableSetImpl;

    template<typename, int...>
    friend class Select;

    Select(TableTuple tableTuple);
};

template<typename TableTuple, typename Where, int... Ns>
class SelectWhere : public SelectImpl<TableTuple, Ns...> {
public:
    using Ptr = std::shared_ptr<SelectWhere<TableTuple, Where, Ns...>>;

    std::string sql() override;

private:
    template<typename...>
    friend class TableSetImpl;

    template<typename, int...>
    friend class Select;

    SelectWhere(TableTuple tableTuple);
};

SQLLIB_NS_END

#include "tpp/select.tpp"

#endif
