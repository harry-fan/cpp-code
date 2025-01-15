//
// Created by Administrator on 2023/8/9.
//

#ifndef DEVELOP_NLOHMANNUTIL_H
#define DEVELOP_NLOHMANNUTIL_H

#include "json.hpp"
#include <string>
#include <vector>
#include "pan/PanException.h"

namespace jsonUtil {
    template<class T>
    static T getValue(const nlohmann::json &params, const std::string &key) {
        if (params.template find(key) != params.end()) {
            return params[key].template get<T>();
        }
        throwException(ptars::Code_Gs_Param_Invalid, "参数json的key不存在:" << key);
    }

    template<class T>
    static std::vector<T> getVecJson(const nlohmann::json &params, const std::string &key) {
        if (params.template find(key) == params.end()) {
            return std::vector<T>();
        }
        if (!params[key].is_array()) {
            return std::vector<T>();
        }
        std::vector<T> result;
        for (std::size_t sdx = 0; sdx < params[key].size(); ++sdx) {
            result.push_back(params[key].at(sdx).get<T>());
        }
        return result;
    }
}

#endif //DEVELOP_NLOHMANNUTIL_H
