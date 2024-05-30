#pragma once

#include "clipp.h"

namespace clipp {

using wmatch_predicate = std::function<bool(const arg_wstring&)>;
using  wmatch_function = std::function<subrange(const arg_wstring&)>;
using       wparameter = tparameter<wchar_t>;
using           wgroup = tgroup<wchar_t>;
using  wparsing_result = tparsing_result<wchar_t>;
using   wdocumentation = tdocumentation<wchar_t>;
using  wdoc_formatting = tdoc_formatting<wchar_t>;
using     wusage_lines = tusage_lines<wchar_t>;
using    wparam_filter = tparam_filter<wchar_t>;

namespace match {

inline bool          wany(const arg_wstring& s) { return tany<wchar_t>(s); }
inline bool         wnone(const arg_wstring& s) { return tnone<wchar_t>(s); }
inline bool     wnonempty(const arg_wstring& s) { return tnonempty<wchar_t>(s); }
inline bool walphanumeric(const arg_wstring& s) { return talphanumeric<wchar_t>(s); }
inline bool   walphabetic(const arg_wstring& s) { return talphabetic<wchar_t>(s); }

using           wnone_of = tnone_of<wchar_t>;
using           wnumbers = tnumbers<wchar_t>;
using          wintegers = tintegers<wchar_t>;
using wpositive_integers = tpositive_integers<wchar_t>;
using         wsubstring = tsubstring<wchar_t>;
using            wprefix = tprefix<wchar_t>;
using        wprefix_not = tprefix_not<wchar_t>;
using          wnoprefix = prefix_not;

} // namespace match

namespace detail {

template<class T> inline T&  operator % (doc_wstring docstr, ttoken<wchar_t, T>&  p) { return p.doc(std::move(docstr)); }
template<class T> inline T&& operator % (doc_wstring docstr, ttoken<wchar_t, T>&& p) { return std::move(p.doc(std::move(docstr))); }
template<class T> inline T&  operator % (ttoken<wchar_t, T>&  p, doc_wstring docstr) { return p.doc(std::move(docstr)); }
template<class T> inline T&& operator % (ttoken<wchar_t, T>&& p, doc_wstring docstr) { return std::move(p.doc(std::move(docstr))); }

} // namespace detail

/*************************************************************************//**
 *
 * Rapid helpers
 *
 *****************************************************************************/

template<class String, class... Strings>
inline wparameter
wcommand(String&& flag, Strings&&... flags)
{
    return tcommand<wchar_t>(std::forward<String>(flag), std::forward<Strings>(flags)...);
}

template<class String, class... Strings>
inline wparameter
wrequired(String&& flag, Strings&&... flags)
{
    return trequired<wchar_t>(std::forward<String>(flag), std::forward<Strings>(flags)...);
}

template<class String, class... Strings>
inline wparameter
woption(String&& flag, Strings&&... flags)
{
    return toption<wchar_t>(std::forward<String>(flag), std::forward<Strings>(flags)...);
}

template<class... Targets>
inline wparameter
wvalue(const doc_wstring& label, Targets&&... tgts)
{
    return tvalue<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets, typename Enabled = OverloadEnabled<Filter, wchar_t>>
inline wparameter
wvalue(Filter&& filter, const doc_wstring& label, Targets&&... tgts)
{
    return wparameter{std::forward<Filter>(filter)}
        .label(std::move(label))
        .target(std::forward<Targets>(tgts)...)
        .required(true).blocking(true).repeatable(false);
}

template<class... Targets>
inline wparameter
wvalues(const doc_wstring& label, Targets&&... tgts)
{
    return tvalues<wchar_t>(label, std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets, typename Enabled = OverloadEnabled<Filter, wchar_t>>
inline wparameter
wvalues(Filter&& filter, const doc_wstring& label, Targets&&... tgts)
{
    return wparameter{std::forward<Filter>(filter)}
        .label(label)
        .target(std::forward<Targets>(tgts)...)
        .required(true).blocking(true).repeatable(true);

}

template<class... Targets>
inline wparameter
wopt_value(const doc_wstring& label, Targets&&... tgts)
{
    return topt_value<wchar_t>(label, std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets, typename Enabled = OverloadEnabled<Filter, wchar_t>>
inline wparameter
wopt_value(Filter&& filter, const doc_wstring& label, Targets&&... tgts)
{
    return wparameter{std::forward<Filter>(filter)}
        .label(label)
        .target(std::forward<Targets>(tgts)...)
        .required(false).blocking(false).repeatable(false);
}

template<class... Targets>
inline wparameter
wopt_values(const doc_wstring& label, Targets&&... tgts)
{
    return topt_values<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets, typename Enabled = OverloadEnabled<Filter, wchar_t>>
inline wparameter
wopt_values(Filter&& filter, const doc_wstring& label, Targets&&... tgts)
{
    return wparameter{std::forward<Filter>(filter)}
        .label(label)
        .target(std::forward<Targets>(tgts)...)
        .required(false).blocking(false).repeatable(true);
}

template<class... Targets>
inline wparameter
wword(const doc_wstring& label, Targets&&... tgts)
{
    return tword<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wwords(const doc_wstring& label, Targets&&... tgts)
{
    return twords<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wopt_word(const doc_wstring& label, Targets&&... tgts)
{
    return topt_word<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wopt_words(const doc_wstring& label, Targets&&... tgts)
{
    return topt_words<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wnumber(const doc_wstring& label, Targets&&... tgts)
{
    return tnumber<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wnumbers(const doc_wstring& label, Targets&&... tgts)
{
    return tnumbers<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wopt_number(const doc_wstring& label, Targets&&... tgts)
{
    return topt_number<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wopt_numbers(const doc_wstring& label, Targets&&... tgts)
{
    return topt_numbers<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
winteger(const doc_wstring& label, Targets&&... tgts)
{
    return tinteger<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wintegers(const doc_wstring& label, Targets&&... tgts)
{
    return tintegers<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wopt_integer(const doc_wstring& label, Targets&&... tgts)
{
    return topt_integer<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wopt_integers(const doc_wstring& label, Targets&&... tgts)
{
    return topt_integers<wchar_t, Targets...>(label, std::forward<Targets>(tgts)...);
}

template<class... Targets>
inline wparameter
wany_other(Targets&&... tgts)
{
    return tany_other<wchar_t, Targets...>(std::forward<Targets>(tgts)...);
}

template<class Filter, class... Targets>
inline wparameter
wany(Filter&& filter, Targets&&... tgts)
{
    return tany<wchar_t, Filter, Targets...>(std::forward<Filter>(filter), std::forward<Targets>(tgts)...);
}

/*************************************************************************//**
 *
 * Rapid prefix
 *
 *****************************************************************************/

inline tgroup<wchar_t>&
wwith_prefix(const arg_wstring& prefix, tgroup<wchar_t>& g)
{
    return twith_prefix<wchar_t>(prefix, g);
}

inline tgroup<wchar_t>&&
wwith_prefix(const arg_wstring& prefix, tgroup<wchar_t>&& g)
{
    return std::move(twith_prefix<wchar_t>(prefix, std::move(g)));
}

template<class Param, class... Params>
inline tgroup<wchar_t>
wwith_prefix(const arg_wstring& prefix, Param&& param, Params&&... params)
{
    return twith_prefix(prefix, tgroup<wchar_t>{std::forward<Param>(param),
                                      std::forward<Params>(params)...});
}

/*************************************************************************//**
 *
 * Rapid prefix short/long
 *
 *****************************************************************************/

inline wparameter&
wwith_prefixes_short_long(const arg_wstring& shortpfx, const arg_wstring& longpfx, wparameter& p)
{
    return twith_prefixes_short_long<wchar_t>(shortpfx, longpfx, p);
}

inline wparameter&&
wwith_prefixes_short_long(const arg_wstring& shortpfx, const arg_wstring& longpfx, wparameter&& p)
{
    return std::move(twith_prefixes_short_long<wchar_t>(shortpfx, longpfx, p));
}

inline wgroup&
wwith_prefixes_short_long(const arg_wstring& shortFlagPrefix,
                          const arg_wstring& longFlagPrefix,
                          wgroup& g)
{
    return twith_prefixes_short_long<wchar_t>(shortFlagPrefix, longFlagPrefix, g);
}

inline wgroup&&
wwith_prefixes_short_long(const arg_wstring& shortFlagPrefix,
                          const arg_wstring& longFlagPrefix,
                          wgroup&& g)
{
    return std::move(twith_prefixes_short_long<wchar_t>(shortFlagPrefix, longFlagPrefix, g));
}

template<class Param, class... Params>
inline wgroup
wwith_prefixes_short_long(const arg_wstring& shortFlagPrefix,
                          const arg_wstring& longFlagPrefix,
                          Param&& param, Params&&... params)
{
    return twith_prefixes_short_long<wchar_t, Param, Params...>(shortFlagPrefix, longFlagPrefix,
                                     std::forward<Param>(param),
                                     std::forward<Params>(params)...);
}

/*************************************************************************//**
 *
 * Rapid sufix
 *
 *****************************************************************************/
inline wparameter&
wwith_suffix(const arg_wstring& suffix, wparameter& p)
{
    return twith_suffix<wchar_t>(suffix, p);
}

inline wparameter&&
wwith_suffix(const arg_wstring& suffix, wparameter&& p)
{
    twith_suffix<wchar_t>(suffix, p);
    return std::move(p);
}

inline wgroup&
wwith_suffix(const arg_wstring& suffix, wgroup& g)
{
    return twith_suffix<wchar_t>(suffix, g);
}

inline wgroup&&
wwith_suffix(const arg_wstring& suffix, wgroup&& g)
{
    twith_suffix<wchar_t>(suffix, g);
    return std::move(g);
}

template<class Param, class... Params>
inline wgroup
wwith_suffix(arg_wstring suffix, Param&& param, Params&&... params)
{
    return twith_suffix<wchar_t, Param, Params...>(suffix,
                                                std::forward<Param>(param),
                                                std::forward<Params>(params)...);
}

/*************************************************************************//**
 *
 * Rapid sufix short/long
 *
 *****************************************************************************/

inline wparameter&
wwith_suffixes_short_long(const arg_wstring& shortsfx, const arg_wstring& longsfx,
                          wparameter& p)
{
    return twith_suffixes_short_long<wchar_t>(shortsfx, longsfx, p);
}

inline wparameter&&
wwith_suffixes_short_long(const arg_wstring& shortsfx, const arg_wstring& longsfx,
                          wparameter&& p)
{
    twith_suffixes_short_long<wchar_t>(shortsfx, longsfx, p);
    return std::move(p);
}

inline wgroup&
wwith_suffixes_short_long(const arg_wstring& shortFlagSuffix,
                          const arg_wstring& longFlagSuffix,
                          wgroup& g)
{
    return twith_suffixes_short_long<wchar_t>(shortFlagSuffix, longFlagSuffix, g);
}

inline wgroup&&
wwith_suffixes_short_long(const arg_wstring& shortFlagSuffix,
                          const arg_wstring& longFlagSuffix,
                          wgroup&& g)
{
    twith_suffixes_short_long<wchar_t>(shortFlagSuffix, longFlagSuffix, g);
    return std::move(g);
}

template<class Param, class... Params>
inline wgroup
wwith_suffixes_short_long(const arg_wstring& shortFlagSuffix,
                          const arg_wstring& longFlagSuffix,
                          Param&& param, Params&&... params)
{
    return twith_suffixes_short_long<wchar_t, Param, Params...>(shortFlagSuffix, longFlagSuffix,
                                                             std::forward<Param>(param),
                                                             std::forward<Params>(params)...);
}

/*************************************************************************//**
 *
 * Rapid accumulators
 *
 *****************************************************************************/

inline wgroup wjoinable(wgroup g)
{
    return tjoinable<wchar_t>(g);
}

template<class... Params>
inline wgroup
wjoinable(wparameter param, Params... params)
{
    return tjoinable<wchar_t, Params...>(std::move(param), std::move(params)...); 
}

template<class P2, class... Ps>
inline wgroup
wjoinable(wgroup p1, P2 p2, Ps... ps)
{
    return tjoinable<wchar_t, P2, Ps...>(std::move(p1), std::move(p2), std::move(ps)...);
}

template<class Param, class... Params>
inline wgroup
wjoinable(doc_wstring docstr, Param param, Params... params)
{
    return tjoinable<wchar_t, Param, Params...>(std::move(docstr), std::move(param), std::move(params)...);
}

inline wparameter 
wrepeatable(wparameter p)
{
    return trepeatable<wchar_t>(p); 
}

inline wgroup
wrepeatable(wgroup g)
{
    return trepeatable<wchar_t>(g);
}

template<class P2, class... Ps>
inline wgroup
wrepeatable(wparameter p1, P2 p2, Ps... ps)
{
    return trepeatable<wchar_t, P2, Ps...>(std::move(p1), std::move(p2), std::move(ps)...);
}

template<class P2, class... Ps>
inline wgroup
wrepeatable(wgroup p1, P2 p2, Ps... ps)
{
    return trepeatable<wchar_t, P2, Ps...>(std::move(p1), std::move(p2), std::move(ps)...);
}

template<class Param, class... Params>
inline wgroup
win_sequence(Param param, Params... params)
{
    return tin_sequence<wchar_t, Param, Params...>(std::move(param), std::move(params)...);
}

inline man_page<wchar_t>
wmake_man_page(const wgroup& cli,
               doc_wstring progname = {},
               const tdoc_formatting<wchar_t>& fmt = tdoc_formatting<wchar_t>{})
{
    return tmake_man_page<wchar_t>(cli, std::move(progname), fmt);
}

} // namespace clipp
