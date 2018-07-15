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

//
// Created by Killian on 13/07/18.
//

#ifndef CATEGORIES_CLASSIFIER_PROGRAM_HPP
#define CATEGORIES_CLASSIFIER_PROGRAM_HPP

#include <filesystem>
#include <string>
#include <vector>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>


namespace classifier {


class program
{
public:
    program(
            std::filesystem::path src_pth,
            std::filesystem::path dest_pth,
            std::vector<std::string> catg_fles_nmes
    );
    
    int execute();

private:
    bool execute_directory(const std::filesystem::path& cur_dir_pth) const;
    
    bool parse_categories_file(
            const std::filesystem::path& cur_dir_pth,
            const std::filesystem::path& catg_fle_pth
    ) const;
    
    bool make_symlink(
            const std::filesystem::path& cur_dir_pth,
            std::filesystem::path cur_dest_pth,
            boost::property_tree::ptree& nod
    ) const;

private:
    std::filesystem::path src_pth_;
    
    std::filesystem::path dest_pth_;
    
    std::vector<std::string> catg_fles_nmes_;
};


}


#endif
