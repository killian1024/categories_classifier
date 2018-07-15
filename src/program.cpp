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
// Created by Killian on 14/07/18.
//

#include <speed/speed.hpp>

#include "program.hpp"


namespace classifier {


program::program(
        std::filesystem::path src_pth,
        std::filesystem::path dest_pth,
        std::vector<std::string> catg_fles_nmes
)
        : src_pth_(std::move(src_pth))
        , dest_pth_(std::move(dest_pth))
        , catg_fles_nmes_(std::move(catg_fles_nmes))
{
    char buf[PATH_MAX + 1] = {0};
    
    if (src_pth_.is_relative())
    {
        if (realpath(src_pth_.c_str(), buf) != nullptr)
        {
            src_pth_ = buf;
        }
    }
    if (dest_pth_.is_relative())
    {
        if (realpath(dest_pth_.c_str(), buf) != nullptr)
        {
            dest_pth_ = buf;
        }
    }
}


int program::execute()
{
    return execute_directory(src_pth_) ? 0 : -1;
}


bool program::execute_directory(const std::filesystem::path& dir_pth) const
{
    std::filesystem::path catg_pth;
    bool sucss = true;
    
    catg_pth = dir_pth;
    catg_pth /= ".";
    
    for (auto& x : catg_fles_nmes_)
    {
        catg_pth.replace_filename(x);
        
        if (std::filesystem::is_regular_file(catg_pth))
        {
            std::cout << spdios::set_light_blue_text
                      << "Parsing categories file " << catg_pth;
    
            if (!parse_categories_file(dir_pth, catg_pth))
            {
                sucss = false;
                std::cout << spdios::set_light_red_text << " [fail]" << spdios::newl;
            }
            else
            {
                std::cout << spdios::set_light_green_text << " [ok]" << spdios::newl;
            }
        }
    }
    
    try
    {
        for (auto& x : std::filesystem::directory_iterator(dir_pth))
        {
            if (std::filesystem::is_directory(x) || std::filesystem::is_symlink(x))
            {
                if (!execute_directory(x.path()))
                {
                    sucss = false;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& fe)
    {
        std::cerr << spdios::set_light_red_text
                  << "Error executing in directory: " << dir_pth
                  << spdios::newl;
    
        return false;
    }
    
    return sucss;
}


bool program::parse_categories_file(
        const std::filesystem::path& dir_pth,
        const std::filesystem::path& catg_pth
) const
{
    namespace pt = boost::property_tree;
    
    try
    {
        pt::ptree root;
        std::filesystem::path dest_pth_bldr;
        bool sucss = true;
    
        pt::read_json(catg_pth, root);
    
        dest_pth_bldr = dest_pth_;
        dest_pth_bldr /= ".";
        
        for (auto& x : root)
        {
            dest_pth_bldr.replace_filename(x.first);
            
            if (!std::filesystem::exists(dest_pth_bldr))
            {
                if (!spdsys::mkdir(dest_pth_bldr.c_str()))
                {
                    sucss = false;
                    continue;
                }
            }

            if (!x.second.data().empty())
            {
                if (!make_symlink(dir_pth, dest_pth_bldr, x.second))
                {
                    sucss = false;
                    continue;
                }
            }
            else
            {
                for (auto& y : x.second)
                {
                    if (!make_symlink(dir_pth, dest_pth_bldr, y.second))
                    {
                        sucss = false;
                        continue;
                    }
                }
            }
        }
        
        return sucss;
    }
    catch (const pt::json_parser::json_parser_error& jpe)
    {
        return false;
    }
}


bool program::make_symlink(
        const std::filesystem::path& dir_pth,
        const std::filesystem::path& dest_pth,
        boost::property_tree::ptree& nod
) const
{
    std::filesystem::path dest_pth_bldr = dest_pth;
    
    if (nod.data() == "false")
    {
        return true;
    }
    
    if (nod.data() != "true")
    {
        dest_pth_bldr /= nod.data();
    }
    
    if (!std::filesystem::exists(dest_pth_bldr))
    {
        if (!spdsys::mkdir(dest_pth_bldr.c_str()))
        {
            return false;
        }
    }
    
    dest_pth_bldr /= dir_pth.filename();
    if (std::filesystem::exists(dest_pth_bldr))
    {
        if (remove(dest_pth_bldr.c_str()) != 0)
        {
            return false;
        }
    }
    
    return spdsys::symlink(dir_pth.c_str(), dest_pth_bldr.c_str());
}


}
