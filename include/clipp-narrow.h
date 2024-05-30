#pragma once

#include "clipp.h"

namespace clipp {

using match_predicate = std::function<bool(const arg_string&)>;
using  match_function = std::function<subrange(const arg_string&)>;
using       parameter = tparameter<char>;
using           group = tgroup<char>;
using  parsing_result = tparsing_result<char>;
using   documentation = tdocumentation<char>;
using  doc_formatting = tdoc_formatting<char>;
using     usage_lines = tusage_lines<char>;
using    param_filter = tparam_filter<char>;

namespace match {

inline bool          any(const arg_string& s) { return tany<char>(s); }
inline bool         none(const arg_string& s) { return tnone<char>(s); }
inline bool     nonempty(const arg_string& s) { return tnonempty<char>(s); }
inline bool alphanumeric(const arg_string& s) { return talphanumeric<char>(s); }
inline bool   alphabetic(const arg_string& s) { return talphabetic<char>(s); }

using           none_of = tnone_of<char>;
using           numbers = tnumbers<char>;
using          integers = tintegers<char>;
using positive_integers = tpositive_integers<char>;
using         substring = tsubstring<char>;
using            prefix = tprefix<char>;
using        prefix_not = tprefix_not<char>;
using          noprefix = prefix_not;

} // namespace match

namespace detail {

template<class T> inline T&  operator % (doc_string docstr, ttoken<char, T>&  p) { return p.doc(std::move(docstr)); }
template<class T> inline T&& operator % (doc_string docstr, ttoken<char, T>&& p) { return std::move(p.doc(std::move(docstr))); }
template<class T> inline T&  operator % (ttoken<char, T>&  p, doc_string docstr) { return p.doc(std::move(docstr)); }
template<class T> inline T&& operator % (ttoken<char, T>&& p, doc_string docstr) { return std::move(p.doc(std::move(docstr))); }

} // namespace detail

/*************************************************************************//**
 *
 * Rapid helpers
 *
 *****************************************************************************/

template<class String, class... Strings>
inline parameter
command(String&& flag, Strings&&... flags)
{
    return tcommand<char>(std::forward<String>(flag), std::forward<Strings>(flags)...);
}

template<class String, class... Strings>
inline parameter
required(String&& flag, Strings&&... flags)
{
    return trequired<char>(std::forward<String>(flag), std::forward<Strings>(flags)...);
}

template<class String, class... Strings>
inline parameter
option(String&& flag, Strings&&... flags)
{
    return toption<char>(std::forward<String>(flag), std::forward<Strings>(flags)...);
}

template<class... Targets>
inline parameter
value(const doc_string& label, Targets&&... tgts)
{
    return tvalue<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets, typename Enabled = OverloadEnabled<Filter, char>>
inline parameter
value(Filter&& filter, const doc_string& label, Targets&&... tgts)
{
    return parameter{std::forward<Filter>(filter)}
        .label(std::move(label))
        .target(std::forward<Targets>(tgts)...)
        .required(true).blocking(true).repeatable(false);
}

template<class... Targets>
inline parameter
values(const doc_string& label, Targets&&... tgts)
{
    return tvalues<char>(label, std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets, typename Enabled = OverloadEnabled<Filter, char>>
inline parameter
values(Filter&& filter, const doc_string& label, Targets&&... tgts)
{
    return parameter{std::forward<Filter>(filter)}
        .label(label)
        .target(std::forward<Targets>(tgts)...)
        .required(true).blocking(true).repeatable(true);

}

template<class... Targets>
inline parameter
opt_value(const doc_string& label, Targets&&... tgts)
{
    return topt_value<char>(label, std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets, typename Enabled = OverloadEnabled<Filter, char>>
inline parameter
opt_value(Filter&& filter, const doc_string& label, Targets&&... tgts)
{
    return parameter{std::forward<Filter>(filter)}
        .label(label)
        .target(std::forward<Targets>(tgts)...)
        .required(false).blocking(false).repeatable(false);
}

template<class... Targets>
inline parameter
opt_values(const doc_string& label, Targets&&... tgts)
{
    return topt_values<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets, typename Enabled = OverloadEnabled<Filter, char>>
inline parameter
opt_values(Filter&& filter, const doc_string& label, Targets&&... tgts)
{
    return parameter{std::forward<Filter>(filter)}
        .label(label)
        .target(std::forward<Targets>(tgts)...)
        .required(false).blocking(false).repeatable(true);
}

template<class... Targets>
inline parameter
word(const doc_string& label, Targets&&... tgts)
{
    return tword<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
words(const doc_string& label, Targets&&... tgts)
{
    return twords<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
opt_word(const doc_string& label, Targets&&... tgts)
{
    return topt_word<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
opt_words(const doc_string& label, Targets&&... tgts)
{
    return topt_words<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
number(const doc_string& label, Targets&&... tgts)
{
    return tnumber<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
numbers(const doc_string& label, Targets&&... tgts)
{
    return tnumbers<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
opt_number(const doc_string& label, Targets&&... tgts)
{
    return topt_number<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
opt_numbers(const doc_string& label, Targets&&... tgts)
{
    return topt_numbers<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
integer(const doc_string& label, Targets&&... tgts)
{
    return tinteger<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
integers(const doc_string& label, Targets&&... tgts)
{
    return tintegers<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
opt_integer(const doc_string& label, Targets&&... tgts)
{
    return topt_integer<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
opt_integers(const doc_string& label, Targets&&... tgts)
{
    return topt_integers<char, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline parameter
any_other(Targets&&... tgts)
{
    return tany_other<char, Targets...>(std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets>
inline parameter
any(Filter&& filter, Targets&&... tgts)
{
    return tany<char, Filter, Targets...>(std::forward<Filter>(filter), std::forward<Targets>(tgts)...);
}

/*************************************************************************//**
 *
 * Rapid prefix
 *
 *****************************************************************************/

inline tgroup<char>&
with_prefix(const arg_string& prefix, tgroup<char>& g)
{
    return twith_prefix<char>(prefix, g);
}

inline tgroup<char>&&
with_prefix(const arg_string& prefix, tgroup<char>&& g)
{
    return std::move(twith_prefix<char>(prefix, std::move(g)));
}

template<class Param, class... Params>
inline tgroup<char>
with_prefix(const arg_string& prefix, Param&& param, Params&&... params)
{
    return twith_prefix(prefix, tgroup<char>{std::forward<Param>(param),
                                      std::forward<Params>(params)...});
}

/*************************************************************************//**
 *
 * Rapid prefix short/long
 *
 *****************************************************************************/

inline parameter&
with_prefixes_short_long(const arg_string& shortpfx, const arg_string& longpfx, parameter& p)
{
    return twith_prefixes_short_long<char>(shortpfx, longpfx, p);
}

inline parameter&&
with_prefixes_short_long(const arg_string& shortpfx, const arg_string& longpfx, parameter&& p)
{
    return std::move(twith_prefixes_short_long<char>(shortpfx, longpfx, p));
}

inline group&
with_prefixes_short_long(const arg_string& shortFlagPrefix,
                         const arg_string& longFlagPrefix,
                         group& g)
{
    return twith_prefixes_short_long<char>(shortFlagPrefix, longFlagPrefix, g);
}

inline group&&
with_prefixes_short_long(const arg_string& shortFlagPrefix,
                         const arg_string& longFlagPrefix,
                         group&& g)
{
    return std::move(twith_prefixes_short_long<char>(shortFlagPrefix, longFlagPrefix, g));
}

template<class Param, class... Params>
inline group
with_prefixes_short_long(const arg_string& shortFlagPrefix,
                         const arg_string& longFlagPrefix,
                         Param&& param, Params&&... params)
{
    return twith_prefixes_short_long<char, Param, Params...>(shortFlagPrefix, longFlagPrefix,
                                     std::forward<Param>(param),
                                     std::forward<Params>(params)...);
}

/*************************************************************************//**
 *
 * Rapid sufix
 *
 *****************************************************************************/
inline parameter&
with_suffix(const arg_string& suffix, parameter& p)
{
    return twith_suffix<char>(suffix, p);
}

inline parameter&&
with_suffix(const arg_string& suffix, parameter&& p)
{
    twith_suffix<char>(suffix, p);
    return std::move(p);
}

inline group&
with_suffix(const arg_string& suffix, group& g)
{
    return twith_suffix<char>(suffix, g);
}

inline group&&
with_suffix(const arg_string& suffix, group&& g)
{
    twith_suffix<char>(suffix, g);
    return std::move(g);
}

template<class Param, class... Params>
inline group
with_suffix(arg_string suffix, Param&& param, Params&&... params)
{
    return twith_suffix<char, Param, Params...>(suffix,
                                                std::forward<Param>(param),
                                                std::forward<Params>(params)...);
}

/*************************************************************************//**
 *
 * Rapid sufix short/long
 *
 *****************************************************************************/

inline parameter&
with_suffixes_short_long(const arg_string& shortsfx, const arg_string& longsfx,
                         parameter& p)
{
    return twith_suffixes_short_long<char>(shortsfx, longsfx, p);
}

inline parameter&&
with_suffixes_short_long(const arg_string& shortsfx, const arg_string& longsfx,
                         parameter&& p)
{
    twith_suffixes_short_long<char>(shortsfx, longsfx, p);
    return std::move(p);
}

inline group&
with_suffixes_short_long(const arg_string& shortFlagSuffix,
                         const arg_string& longFlagSuffix,
                         group& g)
{
    return twith_suffixes_short_long<char>(shortFlagSuffix, longFlagSuffix, g);
}

inline group&&
with_suffixes_short_long(const arg_string& shortFlagSuffix,
                         const arg_string& longFlagSuffix,
                         group&& g)
{
    twith_suffixes_short_long<char>(shortFlagSuffix, longFlagSuffix, g);
    return std::move(g);
}

template<class Param, class... Params>
inline group
with_suffixes_short_long(const arg_string& shortFlagSuffix,
                         const arg_string& longFlagSuffix,
                         Param&& param, Params&&... params)
{
    return twith_suffixes_short_long<char, Param, Params...>(shortFlagSuffix, longFlagSuffix,
                                                             std::forward<Param>(param),
                                                             std::forward<Params>(params)...);
}

/*************************************************************************//**
 *
 * Rapid accumulators
 *
 *****************************************************************************/

inline group joinable(group g)
{
    return tjoinable<char>(g);
}

template<class... Params>
inline group
joinable(parameter param, Params... params)
{
    return tjoinable<char, Params...>(std::move(param), std::move(params)...); 
}

template<class P2, class... Ps>
inline group
joinable(group p1, P2 p2, Ps... ps)
{
    return tjoinable<char, P2, Ps...>(std::move(p1), std::move(p2), std::move(ps)...);
}

template<class Param, class... Params>
inline group
joinable(doc_string docstr, Param param, Params... params)
{
    return tjoinable<char, Param, Params...>(std::move(docstr), std::move(param), std::move(params)...);
}

inline parameter 
repeatable(parameter p)
{
    return trepeatable<char>(p); 
}

inline group
repeatable(group g)
{
    return trepeatable<char>(g);
}

template<class P2, class... Ps>
inline group
repeatable(parameter p1, P2 p2, Ps... ps)
{
    return trepeatable<char, P2, Ps...>(std::move(p1), std::move(p2), std::move(ps)...);
}

template<class P2, class... Ps>
inline group
repeatable(group p1, P2 p2, Ps... ps)
{
    return trepeatable<char, P2, Ps...>(std::move(p1), std::move(p2), std::move(ps)...);
}

template<class Param, class... Params>
inline group
in_sequence(Param param, Params... params)
{
    return tin_sequence<char, Param, Params...>(std::move(param), std::move(params)...);
}

inline man_page<char>
make_man_page(const group& cli,
              doc_string progname = {},
              const tdoc_formatting<char>& fmt = tdoc_formatting<char>{})
{
    return tmake_man_page<char>(cli, std::move(progname), fmt);
}

} // namespace clipp
