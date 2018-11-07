clipp - command line interfaces for modern C++
===========================================================

[![Linux build status](https://travis-ci.org/muellan/clipp.svg?branch=master)](https://travis-ci.org/muellan/clipp) [![MSVC build status](https://ci.appveyor.com/api/projects/status/ci29ngpfks980i7g?svg=true)](https://ci.appveyor.com/project/muellan/clipp)

Easy to use, powerful and expressive command line argument handling for C++11/14/17 contained in a **single header file**. 

- options, options+value(s), positional values, positional commands, nested alternatives, decision trees, joinable flags, custom value filters, ...
- documentation generation (usage lines, man pages); error handling 
- lots of examples; large set of tests

- ### [Quick Reference Table](#quick-reference)
- ### [Overview (short examples)](#overview)
- ### [Detailed Examples](#examples)
- #### [Why yet another library for parsing command line arguments?](#motivation) / [Design goals](#design-goals)
- #### [Requirements / Compilers](#requirements)



## Quick Intro

### Simple Use Case — Simple Setup!
Consider this command line interface:
```man
SYNOPSIS
    convert <input file> [-r] [-o <output format>] [-utf16]

OPTIONS
    -r, --recursive  convert files recursively
    -utf16           use UTF-16 encoding
```
Here is the code that defines the positional value ```input file``` and the three options ```-r```, ```-o``` and ```-utf16```. If parsing fails, the above default man page-like snippet will be printed to stdout.
```cpp
#include <iostream>
#include "clipp.h"
using namespace clipp; using std::cout; using std::string;

int main(int argc, char* argv[]) { 
    bool rec = false, utf16 = false;
    string infile = "", fmt = "csv";

    auto cli = (
        value("input file", infile),
        option("-r", "--recursive").set(rec).doc("convert files recursively"),
        option("-o") & value("output format", fmt),
        option("-utf16").set(utf16).doc("use UTF-16 encoding")
    );

    if(!parse(argc, argv, cli)) cout << make_man_page(cli, argv[0]);
    // ...
}
```



### A More Complex Example:
```man
SYNOPSIS
    finder make <wordfile> -dict <dictionary> [--progress] [-v]
    finder find <infile>... -dict <dictionary> [-o <outfile>] [-split|-nosplit] [-v] 
    finder help [-v]

OPTIONS
    --progress, -p           show progress                       
    -o, --output <outfile>   write to file instead of stdout
    -split, -nosplit         (do not) split output
    -v, --version            show version
```
This CLI has three alternative commands (```make```, ```find```, ```help```), some positional value-arguments (```<wordfile>```, ```<infile>```) of which one is repeatable, a required flag with value-argument (```-dict <dictionary>```), an option with value-argument (```-o <outfile>```), one option with two alternatives (```-split```, ```-nosplit```) and two conventional options (```-v```, ```--progress```).

Here is the code that defines the interface, generates the man page snippet above *and* handles the parsing result:
```cpp
using namespace clipp; using std::cout; using std::string;

//variables storing the parsing result; initialized with their default values
enum class mode {make, find, help};
mode selected = mode::help;
std::vector<string> input;
string dict, out;
bool split = false, progr = false;

auto dictionary = required("-dict") & value("dictionary", dict);

auto makeMode = (
    command("make").set(selected,mode::make),
    values("wordfile", input),
    dictionary,
    option("--progress", "-p").set(progr) % "show progress" );

auto findMode = (
    command("find").set(selected,mode::find),
    values("infile", input),
    dictionary,
    (option("-o", "--output") & value("outfile", out)) % "write to file instead of stdout",
    ( option("-split"  ).set(split,true) |
      option("-nosplit").set(split,false) ) % "(do not) split output" );

auto cli = (
    (makeMode | findMode | command("help").set(selected,mode::help) ),
    option("-v", "--version").call([]{cout << "version 1.0\n\n";}).doc("show version")  );

if(parse(argc, argv, cli)) {
    switch(selected) {
        case mode::make: /* ... */ break;
        case mode::find: /* ... */ break;
        case mode::help: cout << make_man_page(cli, "finder"); break;
    }
} else {
     cout << usage_lines(cli, "finder") << '\n';
}
```




## Quick Reference

Below are a few examples that should give you an idea for how clipp works.
Consider this basic setup with a few variables that we want to set using
command line arguments:
```cpp
int main(int argc, char* argv[]) { 
    using namespace clipp;

    // define some variables
    bool a = false, b = false;
    int n = 0, k = 0;
    double x = 0.0, y = 0.0;
    std::vector<int> ids;

    auto cli = ( /* CODE DEFINING COMMAND LINE INTERFACE GOES HERE */ );

    parse(argc, argv, cli);    //excludes argv[0]

    std::cout << usage_lines(cli, "exe") << '\n';
}
```

| Interface (`usage_lines`)  | Code (content of `cli` parentheses )
| -------------------------- | ------------------------------------
| ` exe [-a] `               | ` option("-a", "--all").set(a)`
| ` exe [--all] `            | ` option("--all", "-a", "--ALL").set(a)`
| ` exe [-a] [-b] `          | ` option("-a").set(a), option("-b").set(b)`
| ` exe -a `                 | ` required("-a").set(a)`
| ` exe [-a] -b `            | ` option("-a").set(a), required("-b").set(b)`
| ` exe [-n <times>] `       | ` option("-n", "--iter") & value("times", n) `
| ` exe [-n [<times>]] `     | ` option("-n", "--iter") & opt_value("times", n) `
| ` exe -n <times>  `        | ` required("-n", "--iter") & value("times", n) `
| ` exe -n [<times>] `       | ` required("-n", "--iter") & opt_value("times", n) `
| ` exe [-c <x> <y>]`        | ` option("-c") & value("x", x) & value("y", y)`
| ` exe -c <x> <y> `         | ` required("-c") & value("x", x) & value("y", y)`
| ` exe -c <x> [<y>] `       | ` required("-c") & value("x", x) & opt_value("y", y)`
| ` exe [-l <lines>...] `    | ` option("-l") & values("lines", ids) `
| ` exe [-l [<lines>...]] `  | ` option("-l") & opt_values("lines", ids) `
| ` exe [-l <lines>]... `    | ` repeatable( option("-l") & value("lines", ids) ) `
| ` exe -l <lines>... `      | ` required("-l") & values("lines", ids) `
| ` exe -l [<lines>...] `    | ` required("-l") & opt_values("lines", ids) `
| ` exe (-l <lines>)... `    | ` repeatable( required("-l") & value("lines", ids) ) `
| ` exe fetch [-a] `         | ` command("fetch").set(k,1), option("-a").set(a) `
| ` exe init \| fetch [-a] ` | ` command("init").set(k,0) \| (command("fetch").set(k,1), option("-a").set(a)) `
| ` exe [-a\|-b]  `          | ` option("-a").set(a) \| option("-b").set(b) `
| ` exe [-m a\|b] `          | ` option("-m") & (required("a").set(b) \| required("b").set(b)) `




## Overview

See the [examples](#examples) section for detailed explanations of each topic.

Namespace qualifiers are omitted from all examples for better readability. All entities are defined in ```namespace clipp```.


#### Basic Setup
```cpp
int main(int argc, char* argv[]) { 
    using namespace clipp;

    auto cli = ( /* CODE DEFINING COMMAND LINE INTERFACE GOES HERE */ );
    parse(argc, argv, cli);    //excludes argv[0]

    //if you want to include argv[0]
    //parse(argv, argv+argc, cli);
}
```

There are two kinds of building blocks for command line interfaces: parameters and groups. Convieniently named factory functions produce parameters or groups with the desired settings applied.
#### Parameters ([flag strings](#flag-strings), [commands](#commands), [positional values](#required-positional-values), [required flags](#required-flags), [repeatable parameters](#repeatable-parameters))
```cpp
bool a = false, f = false;
string s; vector<string> vs;
auto cli = (                             // matches  required  positional  repeatable
    command("push"),                     // exactly      yes       yes         no
    required("-f", "--file").set(f),     // exactly      yes       no          no
    required("-a", "--all", "-A").set(a),  // exactly      no        no          no
                                                  
    value("file", s),                    // any arg      yes       yes         no
    values("file", vs),                  // any arg      yes       yes         yes
    opt_value("file", s),                // any arg      no        yes         no
    opt_values("file", vs),              // any arg      no        yes         yes
    
    //"catch all" parameter - useful for error handling
    any_other(vs),                       // any arg      no        no          yes
    //catches arguments that fulfill a predicate and aren't matched by other parameters
    any(predicate, vs)                   // predicate    no        no          yes
);
```
The functions above are convenience factories:
```cpp
bool f = true; string s;
auto v1 = values("file", s);
// is equivalent to:
auto v2 = parameter{match::nonempty}.label("file").blocking(true).repeatable(true).set(s);

auto r1 = required("-f", "--file").set(f);
// is equivalent to:
auto r2 = parameter{"-f", "--file"}.required(true).set(f);
```
 - a required parameter has to match at least one command line argument 
 - a repeatable parameter can match any number of arguments
 - non-positional (=non-blocking) parameters can match arguments in any order
 - a positional (blocking) parameter defines a "stop point", i.e., until it matches all parameters following it are not allowed to match; once it matched, all parameters preceding it (wihtin the current group) will become unreachable 

##### [Flags + Values](#options-with-values)
If you want parameters to be matched in sequence, you can tie them together using either ```operator &``` or the grouping function ```in_sequence```:

```cpp
int n = 1; string s; vector<int> ls;
auto cli = (
    //option with required value
    option("-n", "--repeat") & value("times", n),

    //required flag with optional value
    required("--file") & opt_value("name", s),
    
    //option with exactly two values
    option("-p", "--pos") & value("x") & value("y"),

    //same as before                   v            v
    in_sequence( option("-p", "--pos") , value("x") , value("y") ),
    
    //option with at least one value (and optionally more)
    option("-l") & values("lines", ls)
);
```

##### [Filtering Value Parameters](#value-filters)
Value parameters use a filter function to test if they are allowed to match an argument string. The default filter ```match::nonempty``` that is used by ```value```, ```values```, ```opt_value``` and ```opt_values``` will match any non-empty argument string. 
You can either supply other filter functions/function objects as first argument of ```value```, ```values```, etc. or use one of these built-in shorthand factory functions covering the most common cases:
```cpp
string name; double r = 0.0; int n = 0;
auto cli = (
    value("user", name),   // matches any non-empty string
    word("user", name),    // matches any non-empty alphanumeric string
    number("ratio", r),    // matches string representations of numbers
    integer("times", n)    // matches string representations of integers
);
```
Analogous to ```value```, ```opt_value```, etc. there are also functions for ```words```, ```opt_word```, etc.

##### Value Parameters With [Custom Filters](#custom-value-filters)
```cpp
auto is_char = [](const string& arg) { return arg.size() == 1 && std::isalpha(arg[0]); };

char c = ' ';
                             // matches       required  positional  repeatable
value(is_char, "c", c);      // one character  yes       yes         no
```


#### Groups
 - [group](#grouping) mutually compatible parameters with parentheses and commas:
   ```cpp
   auto cli = ( option("-a"), option("-b"), option("-c") );
   ```

 - group mutually exclusive parameters as [alternatives](#alternatives) using ```operator |``` or ```one_of```:
   ```cpp
   auto cli1 = ( value("input_file") | command("list") | command("flush") );

   auto cli2 = one_of( value("input_file") , command("list") , command("flush") );
   ```

 - group parameters so that they must be matched in sequence using ```operator &``` or ```in_sequence```:
   ```cpp
   double x = 0, y = 0, z = 0;
   auto cli1 = ( option("-pos") & value("X",x) & value("Y",y) & value("Z",z) );
   
   auto cli2 = in_sequence( option("-pos") , value("X",x) , value("Y",y) , value("Z",z) );
   ```
   Note that surrounding groups are not affected by this, so that ```-a``` and ```-b``` can be matched in any order while ```-b``` and the value ```X``` must match in sequence:
   ```cpp
   bool a = false, b = false; int x = 0;
   auto cli = (  option("-a").set(a),  option("-b").set(b) & value("X",x)  );
   ```

 - groups can be nested and combined to form arbitrarily complex interfaces (see [here](#nested-alternatives) and [here](#complex-nestings)):
   ```cpp
   auto cli = ( command("push") | ( command("pull"), option("-f", "--force") )  );
   ```

 - groups can be repeatable as well:
   ```cpp
   auto cli1 = repeatable( command("flip") | command("flop") );
   ```

 - force common prefixes on a group of flags:
   ```cpp
   int x = 0;
   auto cli1 = with_prefix("-", option("a"), option("b") & value("x",x), ... );
                             // =>     -a           -b     ^unaffected^

   auto cli2 = with_prefix_short_long("-", "--", option("a", "all"), option("b"), ... );
                                                  // => -a  --all           -b
   ```

 - force common suffixes on a group of flags:
   ```cpp
   int x = 0;
   auto cli1 = with_suffix("=", option("a") & value("x",x), ... );
                             // =>      a=    ^unaffected^

   auto cli2 = with_suffix_short_long(":", ":=", option("a", "all"), option("b"), ... );
                                                  // =>  a:   all:=          b:
   ```

 - make a group of flags [joinable](#joinable-flags):
   ```cpp
   auto cli1 = joinable( option("-a"), option("-b"));  //will match "-a", "-b", "-ab", "-ba"

   //works also with arbitrary common prefixes:
   auto cli2 = joinable( option("--xA0"), option("--xB1"));  //will also match "--xA0B1" or "--xB1A0"
   ```


#### Interfacing With Your Code
The easiest way to connect the command line interface to the rest of your code is to bind object values or function (object) calls to parameters (see also [here](#actions)):  
```cpp
bool b = false; int i = 5; int m = 0; string x; ifstream fs;
auto cli = ( 
    option("-b").set(b),                      // "-b" detected -> set b to true
    option("-m").set(m,2),                    // "-m" detected -> set m to 2
    option("-x") & value("X", x),             // set x's value from arg string 
    option("-i") & opt_value("i", i),         // set i's value from arg string  
    option("-v").call( []{ cout << "v"; } ),  // call function (object) / lambda
    option("-v")( []{ cout << "v"; } ),       // same as previous line
    option("-f") & value("file").call([&](string f){ fs.open(f); })
);
```

In production code one would probably use a settings class:
```cpp
struct settings { bool x = false; /* ... */ };

settings cmdline_settings(int argc, char* argv[]) {
    settings s;
    auto cli = ( option("-x").set(s.x), /* ... */ );
    parse(argc, argv, cli);
    return s;
}
```
Note that the target must either be:
 - a fundamental type (```int, long int, float, double, ...```)
 - a type that is convertible from ```const char*```
 - a callable entity: function, function object / lambda
   that either has an empty parameter list or exactly one parameter that is
   convertible from ```const char*```


#### Generating Documentation ([see also here](#documentation-generation))
Docstrings for groups and for parameters can either be set with the member function ```doc``` or with ```operator %```:
```cpp
auto cli = (
    (   option("x").set(x).doc("sets X"),
        option("y").set(y) % "sets Y"         
    ),
    "documented group 1:" % (  
        option("-g").set(g).doc("activates G"),   
        option("-h").set(h) % "activates H"   
    ),
    (   option("-i").set(i) % "activates I",   
        option("-j").set(j) % "activates J" 
    ).doc("documented group 2:")
);
```

Usage Lines:
```cpp
cout << usage_lines(cli, "progname") << '\n';

//with formatting options
auto fmt = doc_formatting{}
           .first_column(3)
           .last_column(79);
cout << usage_lines(cli, "progname", fmt) << '\n';
```

Detailed Documentation:
```cpp
cout << documentation(cli) << '\n';

//with formatting options
auto fmt = doc_formatting{}
           .first_column(7)
           .doc_column(15)
           .last_column(99);
cout << documentation(cli, fmt) << '\n';
```

Man Pages:
```cpp
auto cli = ( /*CODE DEFINING COMMAND LINE INTERFACE GOES HERE*/ );
cout << make_man_page(cli, "progname") << '\n';

//with formatting options
auto fmt = doc_formatting{}
           .first_column(7)
           .doc_column(15)
           .last_column(99);
cout << make_man_page(cli, "progname", fmt) << '\n'; 
```


#### (Error) Event Handlers ([see here](#error-handling), [and here](#per-parameter-parsing-report))
Each parameter can have event handler functions attached to it. These are invoked once for each argument that is mapped to the parameter (or once per missing event):
```cpp
string file = "default.txt";
auto param = required("-nof").set(file,"") | 
             required("-f") & value("file", file) 
                              // on 2nd, 3rd, 4th,... match (would be an error in this case)
                              .if_repeated( [] { /* ... */ } )    
                              // if required value-param was missing
                              .if_missing( [] { /* ... */ } )    
                              // if unreachable, e.g. no flag "-f" before filename
                              .if_blocked( [] { /* ... */ } )    
                              // if match is in conflict with other alternative "-nof"
                              .if_conflicted( [] { /* ... */ } );   
```
The handler functions can also take an int, which is set to the argument index at which the event occurred first:
```cpp
string file = "default.txt";
auto param = required("-nof").set(file,"") | 
             required("-f") & value("file", file) 
                              .if_repeated  ( [] (int argIdx) { /* ... */ } )
                              .if_missing   ( [] (int argIdx) { /* ... */ } )
                              .if_blocked   ( [] (int argIdx) { /* ... */ } )
                              .if_conflicted( [] (int argIdx) { /* ... */ } );
```


#### Special Cases
If we give ```-f -b``` or ```-b -f -a``` as command line arguments for the following CLI, an error will be reported, since the value after ```-f``` is not optional: 
```cpp
auto cli = (  option("-a"),  option("-f") & value("filename"),  option("-b")  );
```
This behavior is fine for most use cases. 
But what if we want our program to take any string as a filename, because our filenames might also collide with flag names? We can make the value parameter [greedy](#greedy-parameters) with ```operator !```. This way, the next string after ```-f``` will always be matched with highest priority as soon as ```-f``` was given:
```cpp
auto cli = (  option("-a"),  option("-f") & !value("filename"),   option("-b")  );
                                        //  ^~~~~~ 
```
Be **very careful** with greedy parameters! 



#### Parsing Result Analysis
```cpp
auto cli = ( /* your interface here */ );
auto res = parse(argc, argv, cli);

if(res.any_error()) { /* ... */ }

//aggregated errors
if(res.unmapped_args_count()) { /* ... */ }
if(res.any_bad_repeat()) { /* ... */ }
if(res.any_blocked())    { /* ... */ }
if(res.any_conflict())   { /* ... */ }

for(const auto& m : res.missing()) { 
    cout << "missing " << m.param() << " after index " << m.after_index() << '\n';
}

//per-argument mapping
for(const auto& m : res) {
    cout << m.index() << ": " << m.arg() << " -> " << m.param();
    cout << " repeat #" << m.repeat();
    if(m.blocked()) cout << " blocked";
    if(m.conflict()) cout << " conflict";
    cout << '\n';
}
```

#### Writing Your Own Convenience Factories
Sometimes it can make your CLI code more expressive and increase maintainability, if you create your own factory functions for making parameters:
```cpp
//value that can only connect to one object with automatic default value documentation
template<class Target>
clipp::parameter
documented_value(const std::string& name, Target& tgt, const std::string& docstr) {
    using std::to_string;
    return clipp::value(name,tgt).doc(docstr + "(default: " + to_string(tgt) + ")");
}
```
```cpp
//value that only matches strings without prefix '-'
template<class Target, class... Targets>
clipp::parameter
nodash_value(std::string label, Target&& tgt, Targets&&... tgts) {
    return clipp::value(clipp::match::prefix_not{"-"}, std::move(label), 
               std::forward<Target>(tgt), std::forward<Targets>(tgts)...);
}
```




## Examples

Note that namespace qualifiers are omitted from all examples for better readability. 
The repository folder "examples" contains code for most of the following examples.

- [options](#options)
- [coding styles](#coding-styles)
- [flag strings](#flag-strings)
- [groups](#grouping)
- [positional values](#required-positional-values)
- [options with values](#options-with-values)
- [options with multiple values](#options-with-multiple-values)
- [required flags](#required-flags)
- [repeatable parameters](#repeatable-parameters)
- [actions](#actions)
- [joinable flags](#joinable-flags)
- [alternatives](#alternatives)
- [commands](#commands)
- [nested alternatives](#nested-alternatives)
- [complex nestings](#complex-nestings)
- [example from docopt](#an-example-from-docopt)
- [value filters](#value-filters)
- [greedy parameters](#greedy-parameters)
- [generalized joinable parameters](#generalized-joinable-parameters)
- [custom value filters](#custom-value-filters)
- [sanity checks](#sanity-checks)
- [basic error handling](#basic-error-handling)
- [parsing](#parsing)
- [documentation generation](#documentation-generation)
- [documentation filtering](#documentation-filtering)



### Options
```man
SYNOPSIS
       switch [-a] [-b] [-c] [--hi]

OPTIONS
       -a          activates a
       -b          activates b
       -c, --noc   deactivates c
       --hi        says hi
```

```cpp
bool a = false, b = false, c = true; //target variables

auto cli = ( 
    option("-a").set(a)                  % "activates a",
    option("-b").set(b)                  % "activates b",
    option("-c", "--noc").set(c,false)   % "deactivates c",
    option("--hi")([]{cout << "hi!\n";}) % "says hi");

if(parse(argc, argv, cli)) 
    cout << "a=" << a << "\nb=" << b << "\nc=" << c << '\n';
else 
    cout << make_man_page(cli, "switch");
```
This will set ```a``` to true, if ```-a``` is found, ```b``` to true, if ```-b``` is found, ```c``` to false, if ```-c``` or ```--noc``` are found and prints "hi" if ```--hi``` is found in ```argv```. In case of parsing errors a man page will be printed.



### Coding Styles
If you like it more verbose use ```set``` to set variables, ```call``` to call functions and ```doc``` for docstrings. The sequence of member function calls doesn't matter. 
```cpp
auto cli = ( 
    option("-b").set(b).doc("activates b"),
    option("-c", "--noc").set(c,false).doc("deactivates c"),
    option("--hi").call([]{cout << "hi!\n";}).doc("says hi") );
```

#### You can also use ```operator >>``` and ```operator <<``` to define actions
```cpp
auto cli = ( 
    option("-b")          % "activates b"   >> b,             
    option("-c", "--noc") % "deactivates c" >> set(c,false),
    option("--hi")        % "says hi"       >> []{cout << "hi!\n";} );
```

```cpp
auto cli = ( 
    option("-b")          % "activates b"   >> b,             
    option("-c", "--noc") % "deactivates c" >> set(c,false),
    option("--hi")        % "says hi"       >> []{cout << "hi!\n";} );
```
```cpp
auto cli = ( 
    b                    << option("-b")          % "activates b",
    set(c,false)         << option("-c", "--noc") % "deactivates c",
    []{cout << "hi!\n";} << option("--hi")        % "says hi" );

```
```cpp
auto cli = ( 
    "activates b"   % option("-b")          >> b,
    "deactivates c" % option("-c", "--noc") >> set(c,false),
    "says hi"       % option("--hi")        >> []{cout << "hi!\n";} );
```
Note that ```%``` has a higher precedence than ```<<``` and ```>>``` which means that you either have to keep the docstrings closer to the command line parameters than the actions or use parentheses.

You should also have a look at [actions](#actions) for more details.


#### Step-by-step configuration of parameters:
```cpp
int n = 1;

auto optN = parameter{"-n", "-N", "--iterations", "--repeats"}.required(true);

auto valN = parameter{match::any}
    .label("times")
    .set(n)
    .call([](string s) { if(!str::represents_number(s)) throw runtime_error{"invalid value for 'times'"}; })
    .if_missing([]{ cout << "value 'times' not found!\n"; })
    .doc("number of iterations (default = " + std::to_string(n) + ")");

auto cli = group{};
cli.push_back(std::move(optN));
cli.push_back(std::move(valN));

// or:
auto cli = group{std::move(optN), std::move(valN)};
```



### Flag Strings
There are no limitations regarding formatting and you can have an arbitrary number of flags per command line parameter.
```cpp
bool onetwo = false;
auto myopt = option("-1", "-2", "1", "2", ":1", ":2", "--no1", "--no2").set(onetwo);
             //     ^----- will match any one of these strings ------^
``` 

#### Same prefix for all flags
```cpp
bool a = false, b = false;

auto cli = with_prefix("-", 
    option("a").set(a),     // -a
    option("b").set(b)      // -b
);
```

#### Same prefix for all flags: single vs. multiple character(s)
Single-letter flags will get the first prefix, flags with more than one letter will get the second one.
```cpp
bool a = false, b = false;

auto cli = with_prefixes_short_long("-", "--",
    option("a", "all").set(a),      // -a, --all
    option("b", "bottom").set(b)    // -b, --bottom
);
```

#### Same suffix for all flags
```cpp
bool a = false, b = false;

auto cli = with_suffix(":", 
    option("a").set(a),     // a:
    option("b").set(b)      // b:
);
```

#### Same suffix for all flags: single vs. multiple character(s)
Single-letter flags will get the first suffix (empty in this example), flags with more than one letter will get the second one.
```cpp
int a = 0, b = 0;

auto cli = with_suffixes_short_long("", "=",
    option("a", "all") & value("A", a),      // -a, --all=
    option("b", "bottom") & value("B", b)    // -b, --bottom=
);
```

#### Make Sure No Flag Occurs As Prefix Of Another Flag
```cpp
auto cli = ( /* your command line interface here */ );
assert(cli.flags_are_prefix_free());
```
Note that identical flags will not trigger an error.


### Grouping
Groups can be nested (see [here](#nested-alternatives)) and have their own documentation string. 
The statement ```auto cli = ( ... );``` creates a group, if there are more than two parameters/groups declared inside the parentheses.

```man
SYNOPSIS
        myprogram [x] [y] [a] [b] [-c] [-d] [-e] [-f]

OPTIONS
        x      sets X
        y      sets Y

        documented group 1:
        a      activates A
        b      activates B

        documented group 2:
        -c     activates C
        -d     activates D

        -e, -f activates E or F
```

```cpp
bool x = false, y = false, a = false, b = false;
bool g = false, h = false, e = false, f = false;

auto cli = (
    (   option("x").set(x) % "sets X",  //simple group 
        option("y").set(y) % "sets Y"
    ),
    (   option("a").set(a) % "activates A",   
        option("b").set(b) % "activates B" 
    ) % "documented group 1:"           //docstring after group
    ,
    "documented group 2:" % (           //docstring before group
        option("-g").set(g) % "activates G",   
        option("-h").set(h) % "activates H" 
    ),
    "activates E or F" % (              
        option("-e").set(e),        //no docstrings inside group
        option("-f").set(f)
    )
);

cout << make_man_page(cli, "myprogram");
```

The above example is in fact shorthand for this:
```cpp
group cli{
    group{   
        parameter{"x"}.set(x).doc("sets X"),
        parameter{"y"}.set(y).doc("sets Y")
    },
    group{
        parameter{"a"}.set(a).doc("activates A"),   
        parameter{"b"}.set(b).doc("activates B")
    }.doc("documented group 1:")
    ,
    group{
        parameter{"-g"}.set(g).doc("activates G"),   
        parameter{"-h"}.set(h).doc("activates H")
    }.doc("documented group 2:")
    ,
    group{
        parameter{"-e"}.set(e), 
        parameter{"-f"}.set(f)
    }.doc("activates E or F")
};

cout << make_man_page(cli, "myprogram");
```

You can of course also fill groups one-by-one:
```cpp
group cli;
cli.push_back(option("x").sets(x).doc("sets X"));
//...
```



### Required Positional Values
```man
SYNOPSIS
        myprogram <infile> <outfile> [-s]
   
OPTIONS
        infile        input filename
        outfile       output filename
        -s, --split   split files
```

```cpp
string ifile, ofile;
bool split = false; 
auto cli = (
    value("infile", ifile)             % "input filename",
    value("outfile", ofile)            % "output filename",
    option("-s", "--split").set(split) % "split files" );
```

#### Alternative Value Mapping Style
```cpp
auto cli = (
     value("infile")         % "input filename"  >> ifile,
     value("outfile")        % "output filename" >> ofile,
     option("-s", "--split") % "split files"     >> split  );
```
See [here](#coding-styles) for more on possible mapping styles.



### Options With Values 
Parameters can be sequenced using operator ```&``` or the function ```in_sequence```. Sequenced parameters can only be matched one after the other. This mechanism can be used to attach a value parameter to an option. 

```man
SYNOPSIS
       simplify [-n <count>] [-r <ratio>] [-m [<lines=5>]] 

OPTIONS
       -n, --count <count>     number of iterations
       -r, --ratio <ratio>     compression ratio
       -m <lines=5>            merge lines (default: 5)
```

```cpp
int n = 0;
bool domerge = false;
long m = 5;
auto print_ratio = [](const char* r) { cout << "using ratio of " << r << '\n'; };

auto cli = ( 
    (option("-n", "--count") & value("count", n))           % "number of iterations",
    (option("-r", "--ratio") & value("ratio", print_ratio)) % "compression ratio",
    (option("-m").set(domerge) & opt_value("lines=5", m))   % "merge lines (default: 5)"
);
```


#### Alternative Value Mapping Styles
```cpp
auto cli = ( 
    (option("-n", "--count") & value("count") >> n                 ) % "number of iterations",
    (option("-r", "--ratio") & value("ratio") >> print_ratio       ) % "compression ratio",
    (option("-m"           ) & opt_value("lines=5") >> m >> domerge) % "merge lines (default: 5)" 
);
```
```cpp
auto cli = ( 
    (option("-n", "--count") & value("count").set(n))         % "number of iterations",
    (option("-r", "--ratio") & value("ratio")(print_ratio))   % "compression ratio",
    (option("-m").set(domerge) & opt_value("lines=5").set(m)) % "merge lines (default: 5)"
);
```
See [here](#coding-styles) for more on coding styles.



### Options With Multiple Values 
Parameters can be sequenced using operator ```&``` or the function ```in_sequence```. Sequenced parameters can only be matched one after the other. This mechanism can be used to attach multiple values to an option.

```man
SYNOPSIS
       transform <geometry file> [-translate <x> <y> <z>] [-rotate <azimuth> <polar>]
```

```cpp
string infile;
bool tr = false, rot = false;
double x = 0, y = 0, z = 0;
double phi = 0, theta = 0;

auto cli = (
    value("geometry file", infile),
    option("-translate").set(tr) & value("x", x) & value("y", y) & value("z", z),
    option("-rotate").set(rot) & value("azimuth", phi) & value("polar", theta)
);
```

Note that the following interface definition is equivalent to the above. Since ```value``` is positional we can list it with ```,```, but we have to make sure that the groups of values will only be matched after the options, hence the ```&```.
```cpp
auto cli = (
    value("geometry file", infile),
    option("-translate").set(tr) & ( value("x", x), value("y", y), value("z", z) ),
    option("-rotate").set(rot)   & ( value("azimuth", phi) , value("polar", theta) )
);
```



### Required Flags
Required flags are usually used together with non-optional values. Note that ```-i``` and ```-o``` are not positional in the following example, i.e., the relative order in which command line arguments for ```-i```, ```-o``` and ```-r``` are provided is irrelevant.

```man
SYNOPSIS
        myprogram [-r] -i <input dir> -o <output dir>

OPTIONS
        -r, --recursive
                search in subdirectories
        -i, --in <input dir>
                path to input directory
        -o, --out <output dir>
                path to output directory
```

```cpp
bool recurse = false;
string inpath, outpath;

auto cli = (
    option("-r", "--recursive").set(recurse)                 % "search in subdirectories",
    (required("-i", "--in" ) & value("input dir", inpath))   % "path to input directory",
    (required("-o", "--out") & value("output dir", outpath)) % "path to output directory"
);
```



### Repeatable Parameters
```man
SYNOPSIS
        simplify <file>... [-c] [-i <line>...]
   
OPTIONS
        <file>...               input files
        -c, --compress          compress results             
        -i, --ignore <line>...  lines to be ignored
```

```cpp
vector<string> files;
vector<int> lines;
bool zip = false;
auto cli = (
    values("file", files)                                % "input files",
    option("-c", "--compress").set(zip)                  % "compress results",
    (option("-i", "--ignore") & integers("line", lines)) % "lines to be ignored"
);
```

The call ```values("v")``` is shorthand for ```value("v").repeatable(true)```.

Note, that the  value parameter ```line``` is repeatable, but the flag ```--ignore``` is not. So 
something like
```
    simplify file1 file2 --ignore 1 2 --ignore 3 4 -c
```
is taken to be an error.
However, it is possible if you make the group of ```--ignore``` and ```line``` itself repeatable:

#### Repeatable Groups of Options with Repeatable Values
```man
SYNOPSIS
        simplify <file>... [-c] [-i <line>...]...
   
OPTIONS
        <file>...               input files
        -c, --compress          compress results             
        -i, --ignore <line>...  lines to be ignored
```

```cpp
vector<string> files;
vector<int> lines;
bool zip = false;
auto cli = (
    values("file", files)                            % "input files",
    option("-c", "--compress").set(zip)              % "compress results",
    repeatable(  // <-----
        option("-i", "--ignore") & integers("line", lines)
    )                                                % "lines to be ignored"
);
```
Now both the option ```--ignore``` *and* the value parameter ```value``` are repeatable. In all of the following examples ```lines``` will be set to ```{1,2,3,4}``` and ```c``` will be set to ```true```:
```
    simplify file1 file2 -c --ignore 1 2 3 4 
    simplify file1 file2 --ignore 1 2 3 4 -c
    simplify file1 file2 --ignore 1 -c --ignore 2 3 4
    simplify file1 file2 --ignore 1 2 --ignore 3 4 -c
    simplify file1 file2 --ignore 1 --ignore 2 -c --ignore 3 --ignore 4
    simplify file1 file2 -c --ignore1 --ignore2 --ignore3 --ignore4
```



### Actions
Actions are executed if a parameter matched an argument string in the command line arguments list. Actions are defined using the following member functions or operators:

- ```parameter::call(f)``` or ```parameter::operator () (f)```: call a callable entity ```f``` (function, lambda, custom function object) for each one of the matched argument strings. If ```f``` accepts exactly one parameter that is convertible from ```const char*```, the command line argument is passed to it. If the parameter list is empty, it is simply called without argument.

- ```parameter::set(target, value)```: assign a fixed value to a target object; note that the assignment ```target = value;``` must be a valid statement

- ```parameter::set(target)```:
    - A ```bool``` target is set to true if the flag/value is present and **left unchanged otherwise**.
    - A target object of fundamental type ```T``` (```int```, ```long```, ```float```, ```double```, ...) will be assigned the result of converting the argument string to type ```T```.
    - Targets of type ```std::vector<T>``` are appended with a value for each matched argument string. Note that ```T``` must either be (explicitly) convertible from ```const char*``` or a fundamental type.

- ```operator <<``` or ```operator >>``` assign arg strings to lvalues or call callable entities. Which kind of action will be performed is automatically determined through overload resolution.


#### Predefined Functions 
```cpp
int x = 0;                   // equivalent to:     
option("-x")(set(x))         // option("-x").set(x) 
option("-x")(set(x,2))       // option("-x").set(x,2) 
option("-x")(increment(x))   // option("-x")([&]{++x;})
option("-x")(decrement(x))   // option("-x")([&]{--x;})

bool b = false;              // equivalent to:
option("-b")(flip(b))        // option("-x")([&]{b = !b;})
```


#### Some Examples
```cpp
bool a = false, b = false;
int i = 1, n = 0, m = 0;
float x = 0.0f;

auto cli = (                           //INFORMAL description
    option("-a").set(a),               //if(found("-a")) a = true;
    option("-b") >> b,                 //if(found("-b")) b = true;
    option("--toggle").call(flip(b)),  //if(found("--toggle")) flip(b);

    value("n").set(n),                 //n = std::atoi(arg);
    option("-i") & value("#",i),       //if(found("-i arg")) i = std::atoi(arg);
    option("-1").set(m,1),             //if(found("-1")) m = 1;
    option("-2").set(m,2),             //if(found("-2")) m = 2;

    //if(found("-z")) call_lambda_with_arg("-z");
    option("-z").call([](const char* s) { cout << s; }),

    //using 'operator()' instead of 'call'
    //if(found("bob")) call_lambda_with_arg("bob");
    option("bob")([](const std::string& s) { cout << s; }),

    //for_each_occurence("-x arg", call_lambda_with_arg(arg));
    repeatable( option("-x") & value("X")([&](const char* s) { x = std::atof(s); }) ),

    option("--all") >> []()              { cout << "found --all\n"; }
                    >> [](const char* s) { cout << "found flag " << s << '\n'; };
);
```



### Joinable Flags
```man
SYNOPSIS
        edit <file> [-rbs] ([:vim] [:st3] [:atom] [:emacs])

OPTIONS
        -r      open read-only
        -b      use backup file
        -s      use swap file

        :vim, :st3, :atom, :emacs
                editor(s) to use; multiple possible        
```

```cpp
std::string file;
bool readonly = false, usebackup = false, useswap = false;
enum class editor {vim, sublime3, atom, emacs};
std::vector<editor> editors;
auto add = [&](editor e){ return [&]{ editors.push_back(e); }; };

auto cli = (
    value("file", file),
    joinable(
        option("-r").set(readonly)  % "open read-only",
        option("-b").set(usebackup) % "use backup file",
        option("-s").set(useswap)   % "use swap file"
    ),
    joinable(
        option(":vim")    >> add(editor::vim),
        option(":st3")    >> add(editor::sublime3),
        option(":atom")   >> add(editor::atom),
        option(":emacs")  >> add(editor::emacs)
    ) % "editor(s) to use; multiple possible"
);
```
- Flags can be joined regardless of their length (second group in the example). 
- If the flags have a common prefix (```-``` or ```:``` in the example) it must be given at least
  once as leading prefix in the command line argument.
- Allowed args for the first group are:
  ```-r```, ```-b```, ```-s```, ```-rb```, ```-br```, ```-rs```, ```-sr```, 
  ```-sb```, ```-bs```, ```-rbs```, ```-rsb```, ...
- Allowed args for the second group are:
  ```:vim```, ```:vim:atom```, ```:emacs:st3```, ```:vimatom```, ...

#### More Examples

| joinable flags           | valid args                                                                         |
| ------------------------ | ---------------------                                                              |
| ```a```, ```b```         | ```ab```, ```ba```, ```a```, ```b```                                               |
| ```-a```, ```-b```       | ```-ab```, ```-ba```, ```-a```, ```-b```, ```-a-b```, ```-b-a```                   |
| ```--a```, ```--b```     | ```--ab```, ```--ba```, ```--a```, ```--b```, ```--a--b```, ```--b--a```           |
| ```,a```, ```,b```       | ```,ab```, ```,ba```, ```,a```, ```,b```, ```,a,b```, ```,b,a```                   |
| ```Xab```, ```Xcd```     | ```Xabcd```, ```Xcdab```, ```XabXcd```, ```XcdXab```, ```Xab```, ```Xcd```         |
| ```x:ab```, ```x:cd```   | ```x:abcd```, ```x:cdab```, ```x:abx:cd```, ```x:cdx:ab```, ```x:ab```, ```x:cd``` |



### Alternatives
```man
SYNOPSIS
        find <file>... -s <expr> [any|all]

OPTIONS
        <file>...  input filenames
        -s <expr>  string to look for
        any        report as soon as any matches
        all        report only if all match
```

```cpp
vector<string> files;
string expr;
bool ifany = false, ifall = false;

auto cli = (
    values("file", files)                  % "input filenames",
    (required("-s") & value("expr", expr)) % "string to look for",
    option("any").set(ifany)               % "report as soon as any matches" |
    option("all").set(ifall)               % "report only if all match"
);
```

If you like it more verbose you can use the function ```one_of``` instead of ```operator |```:
```cpp
auto cli = (
    values("file", files)                  % "input filenames",
    (required("-s") & value("expr", expr)) % "string to look for",
    one_of( option("any").set(ifany)       % "report as soon as any matches",
            option("all").set(ifall)       % "report only if all match" )
);
```


#### gcc-style switches
```man
SYNOPSIS
        format [-o <output file>] [-falign|-fnoalign)]

OPTIONS
        -o, --out <file>
                output filename

        -falign, -fnoalign 
                control alignment
```

```cpp
string outfile = "a.out";
bool align = false;
     
auto cli = (
    (option("-o", "--out") & value("output file", outfile)) % "output filename",
    ( option("-falign"  ).set(align,true) |
      option("-fnoalign").set(align,false) )                % "control alignment"
);
```
    
Note, that the documentation string is attached to the group of parameters for better readability.


#### non-redundant prefix specification
```cpp
//has the same meaning as the code above
string outfile = "a.out";
bool align = false;

auto cli = (
    (option("-o", "--out") & value("output file", outfile)) % "output filename",
    with_prefix("-f", option("align"  ).set(align,true) |
                      option("noalign").set(align,false) )  % "control alignment"
);
```


#### merge alternatives with common prefixes in documentation
```man
Usage:   format [-o <output file>] [-f(align|noalign)]
```
```cpp
auto fmt = doc_formatting{}.merge_alternative_flags_with_common_prefix(true);
cout << usage_lines(cli, "format", fmt) << '\n';
```



### Commands 
**= positional, required flags**
```man
SYNOPSIS
        make_doc new <filename> [-e <enc>]

OPTIONS
        -e, --encoding  'utf8' or 'cp1252', default is UTF-8
    
```

```cpp
std::string fname;
std::string enc = "utf8";

auto cli = (
    command("new"),
    value("filename", fname),
    option("-e", "--encoding") & value("enc", enc).doc("'utf8' or 'cp1252', default is " + enc)
);
```



### Nested Alternatives
```man
SYNOPSIS
        image-find help
        image-find build (new|add) <file>... [-v] [-b [<size=1024>]] [--init|--no-init]
        image-find query <infile> -o <outfile> [-f <format>]

OPTIONS
        -v, --verbose
                print detailed report

        -b, --buffer [<size=1024>]
                sets buffer size in KiByte

        --init, --no-init 
                do or don't initialize

        -f, --out-format <format>
                determine output format
```

Value handling actions are omitted; see examples/nested_alternatives.cpp for a fully functional demo.
```cpp
auto cli = (
    command("help") 
    | ( command("build"),
        ( command("new") | command("add")),                  
        values("file"),
        option("-v", "--verbose")                           % "print detailed report",
        (option("-b", "--buffer") & opt_value("size=1024")) % "sets buffer size in KiByte",
        ( option("--init") | option("--no-init") )          % "do or don't initialize"
    ) 
    | ( command("query"),
        value("infile"),
        required("-o", "--out") & value("outfile"),
        (option("-f", "--out-format") & value("format"))  % "determine output format"
    )
);
```

Note: 
```
doc_formatting::split_alternatives(bool)             //default: true
doc_formatting::alternatives_min_split_size(int)     //default: 3
```
control if the usage is split up into several lines if any group inside an alternative exceeds a given minimum size.



### Complex Nestings
The combination of blocking parameters, alternatives and grouping makes it possible to define interfaces with decision trees/DAGs of arbitrary complexity.
```man
SYNOPSIS
    complex_nesting [-v] [-i] (copy|move) [--all] [--replace] [-f] <files>... [-r] [-h]
    complex_nesting [-v] [-i] compare (date|content) [-b] [-q] <files>... [-r] [-h]
    complex_nesting [-v] [-i] merge (diff|patch) -o <outdir> [--show-conflicts] <files>... [-r] [-h]
    complex_nesting [-v] [-i] merge content [--git-style] [-m <marker>] -o <outdir> [--show-conflicts] <files>... [-r] [-h]
    complex_nesting [-v] [-i] list <files>... [-r] [-h]

OPTIONS
    user interface options:
        -v, --verbose      show detailed output
        -i, --interactive  use interactive mode

    copy mode:
        --all              copy all
        --replace          replace existing files
        -f, --force        don't ask for confirmation

    compare mode:
        -b, --binary       compare files byte by byte
        -q, --quick        use heuristics for faster comparison

    merge mode:
        diff               merge using diff
        patch              merge using patch
        content            merge based on content

        content based merge options:
            --git-style    emulate git's merge behavior
            <marker>       merge marker symbol

        <outdir>           target directory for merge result
        --show-conflicts   show merge conflicts during run

    mode-independent options:
        <files>...         input files
        -r, --recursive    descend into subdirectories
        -h, --help         show help
```

Actions and target variables are omitted in the code. 
```cpp
auto copyMode = "copy mode:" % (
    command("copy") | command("move"),
    option("--all")         % "copy all",
    option("--replace")     % "replace existing files",
    option("-f", "--force") % "don't ask for confirmation"
);

auto compareMode = "compare mode:" % (
    command("compare"),
    (command("date") | command("content")),
    option("-b", "--binary") % "compare files byte by byte",
    option("-q", "--quick")  % "use heuristics for faster comparison"
);

auto mergeAlgo = (
    command("diff")  % "merge using diff"  |
    command("patch") % "merge using patch" |
    (   command("content") % "merge based on content",
        "content based merge options:" % (
          option("--git-style") % "emulate git's merge behavior",
          option("-m", "--marker") & value("marker") % "merge marker symbol"
        )
    )
);

auto mergeMode = "merge mode:" % (
    command("merge"),
    mergeAlgo,
    required("-o") & value("outdir") % "target directory for merge result",
    option("--show-conflicts")       % "show merge conflicts during run"
);

auto firstOpt = "user interface options:" % (
    option("-v", "--verbose")     % "show detailed output",
    option("-i", "--interactive") % "use interactive mode"
);
auto lastOpt = "mode-independent options:" % (
    values("files")             % "input files",
    option("-r", "--recursive") % "descend into subdirectories",
    option("-h", "--help")      % "show help"
);

auto cli = (
    firstOpt,
    copyMode | compareMode | mergeMode | command("list"),
    lastOpt
);

if(parse(argc, argv, cli)) {
     // program logic...
} else {
    auto fmt = doc_formatting{}.doc_column(31);
    cout << make_man_page(cli, argv[0], fmt) << '\n';
}
```

You could of course write down everything as one big expression (docstrings are omitted)...:
```cpp
auto cli = (
    option("-v", "--verbose"),
    option("-i", "--interactive"),
    (
        (   (command("copy") | command("move")),
            option("--all"), option("--replace"),
            option("-f", "--force") 
        )
        | ( command("compare"),
            (command("date") | command("content")),
            option("-b", "--binary"), option("-q", "--quick") 
        ) 
        | ( command("merge"),
            (
                ( command("content"),
                  option("--git-style"),
                  option("-m", "--marker") & value("marker")
                )
                | command("diff")
                | command("patch")
            ),
            required("-o") & value("outdir"),
            option("--show-conflicts")
        ) 
        | command("list")
    ),
    values("files"),
    option("-r", "--recursive"),
    option("-h", "--help")
);
 ```
...but it is probably more readable and maintainable if you break up the CLI definition into logical parts.

Note: 
```
doc_formatting::split_alternatives(bool)             //default: true
doc_formatting::alternatives_min_split_size(int)     //default: 3
```
control whether the usage is split up into several lines if any group inside an alternative  exceeds a given minimum size.



### An Example From [docopt]
```man
Naval Fate.

Usage:
  naval_fate ship new <name>...
  naval_fate ship <name> move <x> <y> [--speed= <kn>]
  naval_fate ship shoot <x> <y>
  naval_fate mine (set|remove) <x> <y> [--moored|--drifting]
  naval_fate -h | --help
  naval_fate --version

Options:
  --speed= <kn>  Speed in knots [default: 10].
  --moored       Moored (anchored) mine.
  --drifting     Drifting mine.
  -h, --help     Show this screen.
  --version      Show version.
```

This code defines the command line interface, handles the parsing result and
generates the above man page snippet.
```cpp
int x = 0, y = 0;
float speed = 0.0f;
bool drift = true;
vector<string> names;
enum class mode { none, help, shipnew, shipmove, shipshoot, mineset, minerem};
mode selected = mode::none;

//define command line interface
auto coordinates = ( value("x", x), value("y", y) );

auto shipnew  = ( command("new").set(selected,mode::shipnew),
                  values("name", names) );

auto shipmove = (
    value("name", names),
    command("move").set(selected,mode::shipmove), coordinates,
    option("--speed=") & value("kn",speed) % "Speed in knots [default: 10]");

auto shipshoot = ( command("shoot").set(selected,mode::shipshoot),
                   coordinates );

auto mines = ( 
    command("mine"),
    (command("set"   ).set(selected,mode::mineset) | 
     command("remove").set(selected,mode::minerem) ),
    coordinates,
    (option("--moored"  ).set(drift,false) % "Moored (anchored) mine." | 
     option("--drifting").set(drift,true)  % "Drifting mine."          )
);

auto navalcli = (
    ( command("ship"), ( shipnew | shipmove | shipshoot ) )
    | mines,
    | command("-h", "--help").set(selected,mode::help)     % "Show this screen."
    | command("--version")([]{ cout << "version 1.0\n"; }) % "Show version."
);

parse(argc, argv, navalcli);

//handle results
switch(m) {
    case mode::none: 
        break;
    case mode::help: {
        auto fmt = doc_formatting{}
            .first_column(2).doc_column(16)
            .max_flags_per_param_in_usage(4);

        cout << "Naval Fate.\n\nUsage:\n"
             << usage_lines(navalcli, "naval_fate", fmt)
             << "\n\nOptions:\n"
             << documentation(navalcli, fmt) << '\n';
        }
        break;
    }
    //... 
}
```




### Value Filters
If a parameter doesn't have flags, i.e. it is a value-parameter, a filter function will be used to test if it matches an argument string. The default filter is ```clipp::match::nonempty``` which will match any non-empty argument string. 
If you want more control over what is matched, you can use some other predefined filters or you can write your own ones (see [here](#custom-value-filters)).

```man
Usage:   exec [-n <times>] [-l <line>...] [-b <ratio>] [-f <term>]
```

```cpp
int n = 1;
std::vector<int> lines;
double r = 1.0;
string term, name;
auto cli = (
    option("-n", "--repeat") & integer("times", n),
    option("-l", "--line")   & integers("#", lines),
    option("-r", "--ratio)   & number("ratio", r),
    option("-f", "--find")   & word("term", term)
);
```

#### Predefined Filtering Value-Parameters
```cpp
auto cli = (
    value("x"),    //non-empty string
    word("x"),     //alphanumeric string
    number("x"),   //string representing integer or floating point number
    integer("x")   //string representing integral number
);
```
Note that there are additional functions for
 - optional parameters: ```opt_value```, ```opt_word```, ...
 - repeatable parameters: ```values```, ```words```, ...
 - repeatable, optional parameters: ```opt_values```, ```opt_words```, ...


#### Using Filters Explicitly
Two kinds of filters are supported right now that can be passed as first argument of ```value```, ```values```, ```opt_value``` or ```opt_values``` as well as argument of the constructor ```parameter::parameter(Filter&&)```
 - Predicates ```(const string&) -> bool``` 
   which should return true if and only if an argument is an exact match.

 - Substring matchers ```(const string&) -> subrange```
   which in case of a match also indicate the position and length of the matched substring within a command line argument.

```cpp
string s;
value( match::length{1,5}, "str", s);

//or using the parameter class directly
auto p = parameter{ match::length{1,5} }
         .positional(true).required(true)
         .label("str").set(s);
```
There are a couple of predefined filters in ```namespace clipp::match```, but you can of course write your own ones (see [here](#custom-value-filters)).

Here is another example that makes sure we don't catch any value starting with "-" as a filename:
```cpp
auto cli = (  
    option("-a")  
    option("-f") & value(match::prefix_not("-"), "filename"),
    option("-b")  
);
```

```cpp
namespace clipp {
namespace match {

  //simple predicates
  bool none (const string&);
  bool any (const string&);
  bool nonempty (const string&);
  bool alphabetic (const string&);
  bool alphanumeric (const string&);

  //filters with settings and substring matching
  class numbers {
      explicit numbers(char decimalPoint = '.', char digitSeparator = ',', char exponentSeparator = 'e');
      subrange operator () (const string& arg);
  };

  class integers {
      explicit integers(char digitSeparator = ',');
      subrange operator () (const string& arg);
  };

  class substring {
      explicit substring(const string& str);
      subrange operator () (const string& arg);
  };

  class prefix {
      explicit prefix(const string& prefix);
      subrange operator () (const string& arg);
  };

  class prefix_not {
      explicit prefix_not(const string& prefix);
      subrange operator () (const string& arg);
  };

  class length {
      explicit length(size_t exact);
      explicit length(size_t min, size_t max);
      subrange operator () (const string& arg);
  };

} }
```



### Greedy Parameters

By default, the parser tries to identify a command line argument (in that order) as
 - a single flag
 - a concatenation of multiple, _joinable_ flags (in any order)
 - a concatenation of a _joinable_ flag sequence in the order defined in the CLI code
 - a single value parameter
 - a concatenation of a _joinable_ flag/value sequence in the order defined in the CLI code
 - a concatenation of _joinable_ flags & values in no particular order

If no match was found, the parser tries the same list again without any restrictions imposed by blocking (positional) parameters, conflicting alternatives, etc. If this leads to any match, an error will be reported. This way, _potential_, but illegal matches can be found and, e.g., conflicting alternatives can be reported.

Consider this CLI:
```cpp
auto cli = (  option("-a"),  option("-f") & value("filename"),  option("-b")  );
```
If we give ```-f -b``` or ```-b -f -a``` as command line arguments, an error will be reported, since the value after ```-f``` is not optional. 

This behavior is fine for most use cases. 
But what if we want our program to take any string as a filename, because our filenames might also collide with flag names? We can make the ```filename``` value parameter greedy, so that the next string after ```-f``` will always be matched with highest priority as soon as ```-f``` was given:
```cpp
auto cli = (  option("-a"),  option("-f") & greedy(value("filename")),  option("-b")  );
```
or using ```operator !```:
```cpp
auto cli = (  option("-a"),  option("-f") & !value("filename"),   option("-b")  );
```

Now, every string coming after an ```-f``` will be used as filename. 

If we don't want just *any* kind of match accepted, but still retain a higher priority for a value parameter, we could use a [value filter](#value-filters):
```cpp
auto cli = (  
    (   command("A"),
        option("-f") & !value(match::prefix_not("-"), "filename"),
        option("-b")  
    ) |
    (   command("B"),
        option("-x")  
    )
);
```
This way, the command line arguments ```A -f B``` will set the filename to "B" and produce no conflict error between the alternative commands ```A``` and ```B``` but ```A -f -b``` will still give an error.

Note, that there is an inherent decision problem: either we want the ```filename``` value to match no matter what, or we won't get proper error handling if someone forgets to specify a filename and gives ```A -f -b``` Also, there might be interfaces where we really want to catch something like ```A -f B``` as a command conflict.




### Generalized Joinable Parameters

Not only flags, but arbitrary combinations of flags and values can be made joinable. This feature is especially powerful if combined with repeatable groups. 
Important: in order for an argument to be matched by such expressions, no parameter requirements must be violated during the matching. Also, no partial argument matches are allowed.

#### Example 1: Counting Letters
```man
Usage:   counter [a|b]...
```

```cpp
int as = 0, bs = 0;

auto cli = joinable( repeatable(
        option("a").call([&]{++as;}) |
        option("b").call([&]{++bs;})
    ) );

if(parse(argc, argv, cli))     
    cout << "as: " << as << "\nbs: " << bs << '\n';
else 
    cout << "Usage:\n" << usage_lines(cli, argv[0]) << '\n';
```

Valid input includes:
```
$ ./counter a                               
$ ./counter b
$ ./counter ab
$ ./counter abba
$ ./counter a b baba
$ ./counter a babba abab abbbbba b a ba a
    ...
```

#### Example 2: Listing Numbers
```man
Usage:   numbers ([,] [<number>])...
```

```cpp
std::vector<double> nums;

auto cli = joinable(repeatable( option(",") , opt_number("number", nums) ));

if(parse(argc, argv, cli)) {
    cout << "numbers:\n";
    for(auto n : nums) cout << n << '\n';
} else {
    cout << "Usage:\n" << usage_lines(cli, argv[0]) << '\n';
}
```

Valid input includes:
```
$ ./numbers 1
$ ./numbers 1 2 3
$ ./numbers 1 , 2
$ ./numbers 1 , 2 , 3
$ ./numbers 1, 2, 3
$ ./numbers 1 ,2 ,3
$ ./numbers 1,2
$ ./numbers 1,2,3
$ ./numbers 1.1 , 2
$ ./numbers 1,2.3,4.5
$ ./numbers 1,2,3 4.2 5,6 2 7.1,8.23,9 
```

**Warning:** Be careful with joinable and repeatable parameters! The resulting command line interface might be a lot less intuitive to use than you think. It can also be hard to get the "grammar" of complex parsing expressions right.
The following definition for example, contains a subtle pitfall:
```cpp
auto cli = joinable(repeatable( option(",") , number("number", nums) ));
//                                            ^^^ non-optional
```
This will not match arguments like ```"1,"```. This is, because, if the repeat group is 'hit' by any of its child parameters, all non-optional parameters must also match within the current 'repeat cycle'. So, if the parser hits the ```","``` it expects to find a number arg as well, because it is blocking (positional) and required. Only after seeing this number can it enter the next repeat cycle. Thus, the argument will not be matched, since joined matches are only valid if no error occured. Making the number optional solves the problem.


### Custom Value Filters
Two kinds of filters are supported right now that can be passed as first argument of ```value```, ```values```, ```opt_value``` or ```opt_values``` as well as argument of the constructor ```parameter::parameter(Filter&&)```:

 - Predicates ```(const string&) -> bool``` 
   which should return true if and only if an argument is an exact match.

 - Substring matchers ```(const string&) -> subrange```
   which in case of a match also indicate the position and length of the matched substring within a command line argument.


#### Simple Predicate Example
 ```man
Usage:   annotate auto | (label <character>)
```

```cpp
auto is_char = [](const string& arg) { return arg.size() == 1 && std::isalpha(arg[0]); };

char lbl = ' ';
auto cli = (  command("auto") | ( command("label"), value(is_char, "character", lbl) )  );
```


#### Substring Matcher Example

Let's write a program that takes strings and lists all tag names (```<tag>```) contained in them:
```man
Usage:   tagnames <string>...
```

```cpp
//custom filter
auto tag_name = [] (const string& arg) {
    if(arg.size() < 3) return subrange{}; //too short
    auto i = arg.find("<");
    if(i == string::npos) return subrange{}; //no tag start found
    auto j = arg.find(">", i+1);
    if(j == string::npos) return subrange{}; //didn't find end of tag
    return subrange{i,j-i+1}; //partial match {start, length}
};

std::set<string> tags;
auto cli = joinable(
    values(tag_name, "string", 
           [&](const string& arg){ if(arg[1] != '/') tags.insert(arg);})
);

if(parse(argc, argv, cli)) {
    cout << "tag names:\n";
    for(const auto& t : tags) cout << t << '\n';
} else {
    cout << "Usage:\n" << usage_lines(cli, "tagnames") << '\n';
}
```

```
$ ./tagnames "<cee><d><e></e></d></cee>" "<a><bo></bo></a>"
tag names:
<a>
<bo>
<cee>
<d>
<e>
```



### Sanity Checks

Check, if no flag occurs as prefix of any other flag (identical flags will be ignored):
```cpp
auto cli = ( /* command line interface definition */);

assert( cli.flags_are_prefix_free() );
```

Check common prefix of all flags, like for example "-" (or "/" on Windows):
```cpp
auto cli = ( /* command line interface definition */);

assert( cli.common_flag_prefix() == "-" );
```



### Basic Error Handling

Each parameter can have error handler functions/lambdas/function objects for different fail cases attached to it:
 - ```if_repeated``` is raised each time an argument is mapped to a parameter regardless of that parameter's repeatability setting
 - ```if_missing``` is raised if a required parameter has no argument associated with it
 - ```if_conflicted``` is raised if two or more arguments are mapped to more than one parameter of a group of alternatives
 - ```if_blocked``` is raised if an argument can only be mapped to a parameter that was not reachable at the time (e.g. because a positional value was expected before that parameter or the parameter was in a non-active alternative branch)

#### Example:
```man
Usage:   send <file> -t <target>... [--http|--ftp]
```

```cpp
string filename;
vector<string> targets;
vector<string> wrong;
bool http = true;

auto istarget = match::prefix_not("-");

auto cli = (
    value("file", filename)
        .if_missing([]{ cout << "You need to provide a source filename!\n"; } )
        .if_repeated([](int idx){ cout << "Only one source file allowed! (index " << idx << ")\n"; } )
    ,
    required("-t") & values(istarget, "target", targets)
        .if_missing([]{ cout << "You need to provide at least one target filename!\n"; } )
        .if_blocked([]{ cout << "Target names must not be given before the source file name!\n"; })
    ,
    option("--http").set(http,true) | 
    option("--ftp").set(http,false) % "protocol, default is http"
        .if_conflicted([]{ cout << "You can only use one protocol at a time!\n"; } )
    ,
    any_other(wrong)
);

if(parse(argc, argv, cli) && wrong.empty()) {
    cout << "OK\n";
    /* ... */
} else {
    for(const auto& arg : wrong) cout << "'" << arg << "' is not a valid argument\n";
    cout << "Usage:" << usage_lines(cli,argv[0]) << '\n';
}
```
An error handler can either have an empty parameter list or take an ```int``` which is set to the command line argument index where the error occured first.

The catch-all parameter made by ```any_other``` is used to catch command line arguments that are not supported.

The value parameter ```target``` will only match command line arguments that do not begin with ```"-"```, so that wrongly spelled options cannot be parsed as ```target``` value.


### Parsing
```cpp
auto cli = ( 
    command("make"),
    value("file")           % "name of file to make",
    option("-f", "--force") % "overwrite existing file"
);

//excludes argv[0]
parse(argc, argv, cli);

//if you want to include argv[0]
parse(argv, argv+argc, cli);

parse({"make", "out.txt"}, cli);

auto args = std::vector<std::string> {"make", "out.txt", "-f"};
parse(args, cli);
```

The parse functions return an object of ```parsing_result``` which can be used for detailed analysis and will (explicitly) convert to false if any error occured during parsing. 
```cpp
auto result = parse(argc, argv, cli);

auto doc_label = [](const parameter& p) {
    if(!p.flags().empty()) return p.flags().front();
    if(!p.label().empty()) return p.label();
    return doc_string{"<?>"};
};

cout << "args -> parameter mapping:\n";
for(const auto& m : result) {
    os << "#" << m.index() << " " << m.arg() << " -> ";
    auto p = m.param();
    if(p) {
        os << doc_label(*p) << " \t";
        if(m.repeat() > 0) {
            os << (m.bad_repeat() ? "[bad repeat " : "[repeat ")
               <<  m.repeat() << "]";
        }
        if(m.blocked())  os << " [blocked]";
        if(m.conflict()) os << " [conflict]";
        os << '\n';
    }
    else {
        os << " [unmapped]\n";
    }
}

cout << "missing parameters:\n";
for(const auto& m : result.missing()) {
    auto p = m.param();
    if(p) {
        os << doc_label(*p) << " \t";
        os << " [missing after " << m.after_index() << "]\n";
    }
}

```



### Documentation Generation
Generate usage lines and documentation from parameters:
```cpp
auto cli = ( /* command line interface definition */ );

//used default formatting
cout << "Usage:\n" << usage_lines(cli, "progname")
     << "\nOptions:\n" << documentation(cli) << '\n';
```
... or generate an entire man page in one go:
```cpp
auto cli = ( /* command line interface definition */ );

cout << make_man_page(cli, "progname")
        .prepend_section("DESCRIPTION", "    The best thing since sliced bread.")
        .append_section("LICENSE", "    GPL3");
```

#### Example
```man
DESCRIPTION
    Builds a database of words from text files.

SYNOPSIS
    worddb help
    worddb build (new|add) <file>
    worddb query -i <infile> [-p]
    worddb info space
    worddb info statistics (words|chars)
    worddb remove (any|all) <regex>
    worddb modify [-c] [-u] [-m <size>]

OPTIONS
    build commands
        new                  make new database
        add                  append to existing database

    query settings
        <infile>             input file
        -p, --pretty-print   human friendly output

    database info modes
        space                detailed memory occupation analysis

        statistics analysis
            words            word frequency table
            chars            character frequency table

    remove mode
        <regex>              regular expression filter

    modification operations
        -c, --compress       compress database in-memory
        -u, --unique         keep only unique entries
        -m, --memlimit       max. size in RAM

LICENSE
    GPL3
 ```

The full code:
```cpp
auto cli = (
    command("help") | 
    ( command("build"),
        "build commands" %
        (   command("new")  % "make new database"
          | command("add")  % "append to existing database"
        ),
        value("file")
    ) | 
    ( command("query"),
        "query settings" %
        (   required("-i", "--input") & value("infile") % "input file",
            option("-p", "--pretty-print") % "human friendly output")
    ) | 
    ( command("info"),
        "database info modes" % (
            command("space") % "detailed memory occupation analysis" |
            (
                command("statistics"),
                "statistics analysis" % (
                    command("words") % "word frequency table" |
                    command("chars") % "character frequency table"
                )
            )
        )
    ) | 
    "remove mode" % (
        command("remove"),
        "modify" % ( command("any") | command("all") ),
        value("regex") % "regular expression filter"
    ) | 
    ( command("modify"),
        "modification operations" % (
            option("-c", "--compress") % "compress database in-memory",
            option("-u", "--unique")   % "keep only unique entries",
            option("-m", "--memlimit") % "max. size in RAM" & value("size")
        )
    )
);

auto fmt = doc_formatting{} .first_column(4) .doc_column(28) .last_column(80);

cout << make_man_page(cli, "worddb", fmt)
    .prepend_section("DESCRIPTION", "    Builds a database of words from text files.")
    .append_section("LICENSE", "    GPL3") << '\n';
```


#### Formatting Options
```cpp
//all formatting options (with their default values)
auto fmt = doc_formatting{}
    .first_column(8)                           //left border column for text body
    .doc_column(20)                            //column where parameter docstring starts
    .last_column(100)                          //right border column for text body
    .indent_size(4)                            //indent of documentation lines for children of a documented group
    .line_spacing(0)                           //number of empty lines after single documentation lines
    .paragraph_spacing(1)                      //number of empty lines before and after paragraphs
    .flag_separator(", ")                      //between flags of the same parameter
    .param_separator(" ")                      //between parameters 
    .group_separator(" ")                      //between groups (in usage)
    .alternative_param_separator("|")          //between alternative flags 
    .alternative_group_separator(" | ")        //between alternative groups 
    .surround_group("(", ")")                  //surround groups with these 
    .surround_alternatives("(", ")")           //surround group of alternatives with these
    .surround_alternative_flags("", "")        //surround alternative flags with these
    .surround_joinable("(", ")")               //surround group of joinable flags with these
    .surround_optional("[", "]")               //surround optional parameters with these
    .surround_repeat("", "...")                //surround repeatable parameters with these
    .surround_value("<", ">")                  //surround values with these
    .empty_label("")                           //used if parameter has no flags and no label
    .max_flags_per_param_in_usage(1)           //max. # of flags per parameter in usage
    .max_flags_per_param_in_doc(32)            //max. # of flags per parameter in detailed documentation
    .split_alternatives(true)                  //split usage into several lines for large alternatives
    .alternatives_min_split_size(3)            //min. # of parameters for separate usage line
    .merge_alternative_flags_with_common_prefix(false)  //-ab(cdxy|xy) instead of -abcdxy|-abxy
    .merge_joinable_flags_with_common_prefix(true)     //-abc instead of -a -b -c
    .ignore_newline_chars(false)               //ignore '\n' in docstrings
    ;

cout << "Usage:\n" << usage_lines(cli, "progname", fmt)
     << "\nOptions:\n" << documentation(cli, fmt) << '\n';

//or generate entire man page in one go
cout << make_man_page(cli, "progname", fmt)
        .prepend_section("DESCRIPTION", "This program lets you format text.")
        .append_section("LICENSE", "GPLv3");
```



### Documentation Filtering

The documentation generator class ```documentation``` can also take an additional third constructor argument that allows to filter parameters according to their properties.

```cpp
int main(int argc, char* argv[]) {
    auto cli = (
        value("input file"),
        option("-r", "--recursive").set(rec).doc("convert files recursively"),
        option("-o") & value("output format", fmt),
        option("-utf16").set(utf16).doc("use UTF-16 encoding")
    );

    auto fmt = doc_formatting{}.doc_column(28);

    auto filter = param_filter{}.prefix("--");

    cout << "Usage:\n" << usage_lines(cli, argv[0]) << "\n\n"
         << "Parameters:\n" << documentation(cli, fmt, filter) << '\n';}
```

Which results in:
```man
Usage:
        convert <input file> [-r] [-o <output format>] [-utf16]

Parameters:
        -r, --recursive    convert files recursively
```

#### Parameter Filters

Any function/lambda that maps a parameter to a bool can be used as filter 
predicate. CLIPP also comes with a default parameter filter class:
```cpp
//all param_filter options (with their default values)
auto filter = param_filter{}
    .prefix("")               //only parameters with given prefix
    .required(tri::either)    //required parameters 
    .blocking(tri::either)    //blocking/positional parameters
    .repeatable(tri::either)  //repeatable parameters
    .has_doc(tri::yes)        //parameters with/without docstrings
    ;
```
which uses a dedicated tristate type:
```cpp
namespace clipp {
   enum class tri { no, yes, either };
}
```


## Motivation
Well, I didn't find a library that makes building simple command line interfaces simple, yet can also be used to build complex CLIs. I really don't want to write 20 lines of boilerplate just to have 3 simple command line options. I also don't want to drag along monstrosities like boost or Qt for that. Over time, *clipp* evolved into a domain specific language (in pure C++) that I hope can at least somewhat approach the readability of [docopt] but can leverage the benefits (toolability, etc.) of C++'s type system.


#### Other libraries (Boost, POCO, Qt, adishavit/Argh, Taywee/args ... or 'getopt')
 - often involve a lot of boilerplate (also for very simple use cases) 
 - Some libs might be okay in terms of usability, but don't let you build complex interfaces
   with nested alternatives, mixed commands & options, positional values, more than 2 flags per option, etc.
 - I really like ad-hoc parsers like [Argh](https://github.com/adishavit/argh) for their simplicity, but they don't generate usage / man pages and don't allow complex interfaces with error checking.
 - Most libs make it really hard to figure out the resulting command line interface by looking at the code (OK, you be the judge if I did a better job at this...).
 - Some libs come with a ton of dependencies (Qt, Boost). I want a single header file!
 - Some libs require the separation of code related to one command/option. I find this harder to maintain than having everything related to one option in one place.
 - Sometimes flag strings of commands/options have to be repeated several times over.
 - Many libs come with restrictions regarding flag names, formatting, joinability, etc.


#### What about [docopt]?
I like the basic idea, but I don't like redundancy in code, especially if it involves repeating string literals. Docopt generates a command line argument parser from a "man page docstring". After parsing, you have to query a dictionary to get values or check for option presence. That means you either have to mention the same flag names twice (in the docstring *and* in the query) or you have to use string variables which makes the docstring hard to read and kind of defeats the purpose of docopt. Furthermore, the code tends to be littered with string-to-whatever conversions.

I also wanted the ability to keep *everything* related to one option/command/value together in the code which I found very useful for programs with lots of command line options. Docopt doesn't let you do that since the interface definition and the code performing any actions is always separated (unless you give up on the "beautiful doc page" idea at the heart of docopt and build your input string to the parser generator piece-by-piece).




## Design Goals
 - minimize boilerplate 
 - simple code for simple use cases
 - good code readability (as far as C++ allows for it)
 - avoid ambiguities 
 - eliminate repetitions
 - ability to keep code related to one option/command/value together 
 - support many different command line interface conventions
 - support different coding styles, conventions & tastes
 - value semantics wherever possible
 - do not break clipp's general interface and conventions in the future




## Requirements
  - requires a mostly C++11 conforming compiler

### Compilers, clipp compiles with
  - g++ 5.3.0, g++ 5.4.1, g++ 6, g++ 7
  - clang++ 3.8, clang++ 3.9, clang++ 4.0, clang++ 5.0
  - MSVC 14.11.25503 (compiler 19.11.25547)




## Feedback
I would be delighted to hear from anyone who uses *clipp* in their project(s). If you find bugs or design flaws or want to suggest improvements please open an issue or submit a pull request.




## Development
*clipp* is still very young, so I probably won't add new features any time soon and rather:
 - fix bugs 
 - improve test cases and coverage
 - add more code documentation 
 - clean up the code / make it more understandable
 - add the ability to use other string classes 


[docopt]: http://docopt.org

