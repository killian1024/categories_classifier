/* classifier
 * Copyright (C) 2018 Killian Poulaud.
 *
 * This file is part of classifier.
 *
 * classifier is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * classifier is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

#include <speed/speed.hpp>

#include "program.hpp"


int main(int argc, char* argv[])
{
    spdap::arg_parser ap("classifier");
    ap.add_help_text("Options:");
    ap.add_key_value_arg({"--categories-files", "-cf"}, "Categories files names.",
                         {spdap::avt_t::STRING}, 1u, ~0u);
    ap.add_help_arg({"--help"}, "Display this help and exit.");
    ap.add_gplv3_version_arg({"--version"}, "Output version information and exit", "1.0", "2018",
                             "Killian Poulaud");
    ap.add_foreign_arg("SOURCE-DIR", "Source directory", "", {spdap::avt_t::RWX_DIR});
    ap.add_foreign_arg("DESTINATION-DIR", "Destination directory", "", {spdap::avt_t::RWX_DIR});
    ap.parse_args((unsigned int)argc, argv);
    
    classifier::program prog(
            ap.get_front_arg_value_as<std::filesystem::path>("SOURCE-DIR"),
            ap.get_front_arg_value_as<std::filesystem::path>("DESTINATION-DIR"),
            ap.get_arg_values_as<std::string>("--categories-files", {".categories.json"})
    );
    
    return prog.execute();
}
