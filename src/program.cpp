//
// Created by Killian on 14/07/18.
//

#include <speed/speed.hpp>

#include "program.hpp"


namespace categories_classifier {


program::program(
        std::filesystem::path src_pth,
        std::filesystem::path dest_pth,
        std::unordered_set<std::string> categories_fles_nmes
)
        : src_pth_(std::move(src_pth))
        , dest_pth_(std::move(dest_pth))
        , categories_fles_nmes_(std::move(categories_fles_nmes))
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


bool program::execute_directory(const std::filesystem::path& cur_dir_pth) const
{
    bool sucss = true;
    
    try
    {
        for (auto& x : std::filesystem::directory_iterator(cur_dir_pth))
        {
            if (std::filesystem::is_regular_file(x))
            {
                if (categories_fles_nmes_.count(x.path().filename()) > 0)
                {
                    std::cout << spdios::set_light_blue_text
                              << "Parsing categories file " << x.path().filename()
                              << " in " << cur_dir_pth;
                    
                    if (!parse_categories_file(cur_dir_pth, x.path()))
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
            else if (std::filesystem::is_directory(x) || std::filesystem::is_symlink(x))
            {
                if (!execute_directory(x.path()))
                {
                    sucss = false;
                }
            }
        }
        
        return sucss;
    }
    catch (const std::filesystem::filesystem_error& fe)
    {
        std::cerr << spdios::set_light_red_text
                  << "Error executing in directory: " << cur_dir_pth
                  << spdios::newl;
    
        return false;
    }
}


bool program::parse_categories_file(
        const std::filesystem::path& cur_dir_pth,
        const std::filesystem::path& categories_fle
) const
{
    namespace pt = boost::property_tree;
    
    try
    {
        pt::ptree root;
        std::filesystem::path cur_dest_pth;
        bool sucss = true;
    
        pt::read_json(categories_fle, root);
        
        for (auto& x : root)
        {
            cur_dest_pth = dest_pth_;
            cur_dest_pth /= x.first;
            
            if (!std::filesystem::exists(cur_dest_pth))
            {
                if (!spdsys::mkdir(cur_dest_pth.c_str()))
                {
                    sucss = false;
                    continue;
                }
            }

            if (!x.second.data().empty())
            {
                if (!make_symlink(cur_dir_pth, cur_dest_pth, x.second))
                {
                    sucss = false;
                    continue;
                }
            }
            else
            {
                for (auto& y : x.second)
                {
                    if (!make_symlink(cur_dir_pth, cur_dest_pth, y.second))
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
        const std::filesystem::path& cur_dir_pth,
        std::filesystem::path cur_dest_pth,
        boost::property_tree::ptree& nod
) const
{
    if (nod.data() == "false")
    {
        return true;
    }
    
    if (nod.data() != "true")
    {
        cur_dest_pth /= nod.data();
    }
    
    if (!std::filesystem::exists(cur_dest_pth))
    {
        if (!spdsys::mkdir(cur_dest_pth.c_str()))
        {
            return false;
        }
    }
    
    cur_dest_pth /= cur_dir_pth.filename();
    if (std::filesystem::exists(cur_dest_pth))
    {
        if (remove(cur_dest_pth.c_str()) != 0)
        {
            return false;
        }
    }
    
    return spdsys::symlink(cur_dir_pth.c_str(), cur_dest_pth.c_str());
}


}
