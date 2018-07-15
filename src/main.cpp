#include <speed/speed.hpp>

#include "program.hpp"


int main(int argc, char* argv[])
{
    spdap::arg_parser ap("categories_classifier");
    ap.add_help_text("Options:");
    ap.add_key_value_arg({"--categories-files", "-cf"}, "Categories files names.",
                         {spdap::avt_t::STRING}, 1u, ~0u);
    ap.add_help_arg({"--help"}, "Display this help and exit.");
    ap.add_gplv3_version_arg({"--version"}, "Output version information and exit", "1.0.0", "2018",
                             "Killian Poulaud");
    ap.add_foreign_arg("SOURCE-DIR", "Source directory", "",
                       {spdap::avt_t::R_DIR | spdap::avt_t::W_DIR | spdap::avt_t::X_DIR});
    ap.add_foreign_arg("DESTINATION-DIR", "Destination directory", "",
                       {spdap::avt_t::R_DIR | spdap::avt_t::W_DIR | spdap::avt_t::X_DIR});
    ap.parse_args((unsigned int)argc, argv);
    
    std::unordered_set<std::string> categories_fles_nmes;
    for (auto& x : ap.get_arg_values("--categories-files"))
    {
        categories_fles_nmes.insert(x.as<std::string>());
    }
    if (categories_fles_nmes.empty())
    {
        categories_fles_nmes.insert(".categories.json");
    }
    
    categories_classifier::program prog(
            ap.get_front_arg_value_as<std::filesystem::path>("SOURCE-DIR"),
            ap.get_front_arg_value_as<std::filesystem::path>("DESTINATION-DIR"),
            std::move(categories_fles_nmes)
    );
    
    return prog.execute();
}