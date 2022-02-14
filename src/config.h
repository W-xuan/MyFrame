#ifndef __MYFRAME_CONFIG_H__
#define __MYFRAME_CONFIG_H__

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <map>

namespace MyFrame {

// 配置变量的基类
class ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;

    // 构造函数
    ConfigVarBase(const std::string& name, const std::string& description = "")
        :m_name(name)
        ,m_description(description) {
    }

    // 析构函数
    virtual ~ConfigVarBase() {}
    
    // 返回配置参数名称
    const std::string& getName() const { return m_name; }

    //返回配置参数的描述
    const std::string& getDescription() const { return m_description; }

    // 转成字符串
    virtual std::string toString() = 0;

    // 从字符串初始化值
    virtual bool fromString(const std::string& val) = 0;

    // 返回配置参数值的类型名称
    virtual std::string getTypeName() const = 0;
protected:
    std::string m_name;
    std::string m_description;
};

template<class T>
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;

    ConfigVar(const std::string& name
            ,const T& default_value
            ,const std::string& description = "")
        :ConfigVarBase(name, description)
        ,m_val(default_value) {
    }

    std::string toString() override {
        try {
            return boost::lexical_cast<std::string>(m_val);
        } catch (std::exception& e) {
            MYFRAME_LOG_ERROR(MYFRAME_LOG_ROOT()) << "ConfigVar::tiString exception"
                << e.what() << " convert: " << typeid(m_val).name() << " to string";
        }
        return "";
    }

    bool fromString(const std::string& val) override {
        try {
            m_val = boost::lexical_cast<T>(val);
        } catch (std::exception& e) {
            MYFRAME_LOG_ERROR(MYFRAME_LOG_ROOT()) << "ConfigVar::tiString exception"
                << e.what() << " convert: string to " << typeid(m_val).name() << " to string";
        
        }
        return false;
    }
private:
    T m_val;
};

class Config {
    typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
            const T& default_value, const std::string& description = "") {
        auto tmp = Lookup<T>{name};
        if (tmp) {
            MYFRAME_LOG_INFO(MYFRAME_LOG_ROOT()) << "Lookup name=" << name << "exists";
            return tmp;
        }

        if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678")
                != std::string::npos) {
            MYFRAME_LOG_ERROR(MYFRAME_LOG_ROOT()) << "Lookup name invalid " << name;
            throw std::invalid_argument(name);
        }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>)
    }

    //查找配置参数,返回配置参数名为name的配置参数
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
        auto it = GetDatas().find(name);
        if(it == GetDatas().end()) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
    }

};

}
#endif