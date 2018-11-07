/*****************************************************************************
 *
 * demo program - part of CLIPP (command line interfaces for modern C++)
 *
 * released under MIT license
 *
 * (c) 2017-2018 André Müller; foss@andremueller-online.de
 *
 *****************************************************************************/

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <clipp.h>


//-------------------------------------------------------------------
enum class mode {
    none, train, validate, classify
};

struct settings {
    mode selected = mode::none;
    std::string imageFile;
    std::string labelFile;
    std::string modelFile = "out.mdl";
    std::size_t batchSize = 128;
    std::size_t threads = 0;
    std::size_t inputLimit = 0;
    std::vector<std::string> inputFiles;
};


//-------------------------------------------------------------------
settings configuration(int argc, char* argv[])
{
    using namespace clipp;

    settings s;

    std::vector<std::string> unrecognized;

    auto isfilename = clipp::match::prefix_not("-");

    auto inputOptions = (
        required("-i", "-I", "--img") & !value(isfilename, "image file", s.imageFile),
        required("-l", "-L", "--lbl") & !value(isfilename, "label file", s.labelFile)
    );

    auto trainMode = (
        command("train", "t", "T").set(s.selected,mode::train)
            .if_conflicted([]{std::cerr << "conflicting modes\n"; }),

        inputOptions,

        (option("-n") & integer("limit", s.inputLimit))
            % "limit number of input images",

        (option("-o", "-O", "--out") & !value("model file", s.modelFile))
            % "write model to specific file; default: 'out.mdl'",

        (option("-b", "--batch-size") & integer("batch size", s.batchSize)),

        (option("-p") & integer("threads", s.threads))
            % "number of threads to use; default: optimum for machine"
    );

    auto validationMode = (
        command("validate", "v", "V").set(s.selected,mode::validate),
        !value(isfilename, "model", s.modelFile),
        inputOptions
    );

    auto classificationMode = (
        command("classify", "c", "C").set(s.selected,mode::classify),
        !value(isfilename, "model", s.modelFile),
        !values(isfilename, "images", s.inputFiles)
    );

    auto cli = (
        trainMode | validationMode | classificationMode,
        any_other(unrecognized)
    );

    auto res = parse(argc, argv, cli);

    debug::print(std::cout, res);

    if(!res || !unrecognized.empty()) {
        std::string msg = "Wrong command line arguments!\n";

        if(s.selected == mode::none) {
            msg += "Please select a mode!\n";
        }
        else {
            for(const auto& m : res.missing()) {
                if(!m.param()->flags().empty()) {
                    msg += "Missing option: " + m.param()->flags().front() + '\n';
                }
                else if(!m.param()->label().empty()) {
                    msg += "Missing value: " + m.param()->label() + '\n';
                }
            }

            for(const auto& arg : unrecognized) {
                msg += "Argument not recognized: " + arg + '\n';
            }
        }

        auto fmt = doc_formatting{}.first_column(8).doc_column(16);
        //.max_flags_per_param_in_usage(3).surround_alternative_flags("(", ")");

        msg += "\nUsage:\n" + usage_lines(cli, argv[0], fmt).str() + '\n';
        msg += "\nOptions:\n" + documentation(cli, fmt).str() + '\n';

        throw std::invalid_argument{msg};
    }

    return s;
}



//-------------------------------------------------------------------
int main(int argc, char* argv[])
{
    try {
        auto conf = configuration(argc, argv);
        std::cout << "SUCCESS\n";
    }
    catch(std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
    }
}
