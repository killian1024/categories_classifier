//
// Created by Killian on 13/07/18.
//

#ifndef CATEGORIES_CLASSIFIER_PROGRAM_HPP
#define CATEGORIES_CLASSIFIER_PROGRAM_HPP

#include <filesystem>
#include <string>
#include <unordered_set>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>


namespace categories_classifier {


class program
{
public:
    program(
            std::filesystem::path src_pth,
            std::filesystem::path dest_pth,
            std::unordered_set<std::string> categories_fles_nmes
    );
    
    int execute();

private:
    bool execute_directory(const std::filesystem::path& cur_dir_pth) const;
    
    bool parse_categories_file(
            const std::filesystem::path& cur_dir_pth,
            const std::filesystem::path& categories_fle
    ) const;
    
    bool make_symlink(
            const std::filesystem::path& cur_dir_pth,
            std::filesystem::path cur_dest_pth,
            boost::property_tree::ptree& nod
    ) const;

private:
    std::filesystem::path src_pth_;
    
    std::filesystem::path dest_pth_;
    
    std::unordered_set<std::string> categories_fles_nmes_;
};


}


#endif
