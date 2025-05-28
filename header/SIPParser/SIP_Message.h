#pragma once

#include <string>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;




// from.name_address.uri.user_info
class Ds_SIP_Message
{
public:
  Ds_SIP_Message(const std::string & str)
  {
    parse(str);
  }


  class DataStruct
  {
  public:
    virtual void parse(const std::string& str) = 0;
    virtual std::string to_str() = 0;
  };

  class Ds_name_address : public DataStruct
  {
  public:
    std::string display_name;
    std::string uri;
    std::string to_str(){
      return display_name+uri;
    }
    void parse(const std::string& str)
    {

    }
  };

  class Ds_from : public DataStruct
  {
  public:
    std::string name_address;
    std::string tag;
    std::string generics;

    std::string to_str() override{
      std::string answer = "From: " + name_address;
      if(!tag.empty()){
        answer+=";tag="+tag;
      }
      if(!generics.empty()){
        answer+=generics;
      }
      return answer+"\r\n";
    }

    void parse(const std::string& str) override
    {
      // Регулярное выражение для разбора поля From в SIP
      static const boost::regex from_pattern(
          // Общий формат: From: [display-name] <sip:user@host>;tag=xxx[;params]
          "^From:\\s*"                             // Начало и ": "
          "(?:(\"[^\"]*\")|([^<;]+))?\\s*"         // Отображаемое имя (в кавычках или без)
          "(?:<([^>]*)>)?"                         // URI в угловых скобках
          "(?:;tag=([^;\\s]+))?"                   // Параметр tag
          "(.*)$",                                 // Остальные параметры
          boost::regex::icase);

      boost::smatch matches;
      if (boost::regex_search(str, matches, from_pattern))
      {
        // Разбираем компоненты
        std::string quoted_name = matches[1].str();  // Имя в кавычках
        std::string unquoted_name = matches[2].str(); // Имя без кавычек
        std::string uri = matches[3].str();
        this->tag = matches[4].str();
        this->generics = matches[5].str();

        // Очищаем имя от лишних пробелов
        if (!quoted_name.empty()) {
          quoted_name = quoted_name.substr(1, quoted_name.length() - 2); // Удаляем кавычки
        }
        boost::trim(unquoted_name);

        std::string display_name = !quoted_name.empty() ? quoted_name : unquoted_name;

        if (!display_name.empty()) {
          this->name_address = display_name + " <" + uri + ">";
        }
        else {
          this->name_address = str; // Если ничего не распарсилось
        }
      }
      else {
        // Если не соответствует формату, сохраняем как есть
        this->name_address = str;
      }
    }
  };

  class Ds_to : public Ds_from{
  public:
    std::string to_str() override{
      std::string answer = "To: " + name_address;
      if(!tag.empty()){
        answer+=";tag="+tag;
      }
      if(!generics.empty()){
        answer+=generics;
      }
      return answer+"\r\n";
    }
    void parse(const std::string& str) override
    {
      // Регулярное выражение для разбора поля From в SIP
      static const boost::regex from_pattern(
          // Общий формат: From: [display-name] <sip:user@host>;tag=xxx[;params]
          "^To:\\s*"                             // Начало и ": "
          "(?:(\"[^\"]*\")|([^<;]+))?\\s*"         // Отображаемое имя (в кавычках или без)
          "(?:<([^>]*)>)?"                         // URI в угловых скобках
          "(?:;tag=([^;\\s]+))?"                   // Параметр tag
          "(.*)$",                                 // Остальные параметры
          boost::regex::icase);

      boost::smatch matches;
      if (boost::regex_search(str, matches, from_pattern))
      {
        // Разбираем компоненты
        std::string quoted_name = matches[1].str();  // Имя в кавычках
        std::string unquoted_name = matches[2].str(); // Имя без кавычек
        std::string uri = matches[3].str();
        this->tag = matches[4].str();
        this->generics = matches[5].str();

        // Очищаем имя от лишних пробелов
        if (!quoted_name.empty()) {
          quoted_name = quoted_name.substr(1, quoted_name.length() - 2); // Удаляем кавычки
        }
        boost::trim(unquoted_name);

        std::string display_name = !quoted_name.empty() ? quoted_name : unquoted_name;

        if (!display_name.empty()) {
          this->name_address = display_name + " <" + uri + ">";
        }
        else {
          this->name_address = str; // Если ничего не распарсилось
        }
      }
      else {
        // Если не соответствует формату, сохраняем как есть
        this->name_address = str;
      }
    }
  };

  void parse(const std::string& str)
  {
    // Регулярное выражение для поиска поля From и разделения сообщения
    static const boost::regex from_splitter(
        "(.*?)"                          // Часть до From (нежадное совпадение)
        "(From:[^\r\n]*)"                // Само поле From
        "\r\n"
        "(To:[^\r\n]*)"
        "\r\n"
        "(.*?)",       // Часть после From
        boost::regex::icase);

    boost::smatch matches;
    if (boost::regex_match(str, matches, from_splitter)) {
      begin = matches[1].str();  // Часть до From
      from.parse(matches[2].str());  // Поле From
      to.parse(matches[3].str());  // Поле From
      end = matches[4].str();   // Часть после From
    }
  }

  std::string to_str()
  {
    return begin+from.to_str()+to.to_str()+end;
  }


public:
  std::string begin;
  Ds_from from;
  Ds_to to;
  std::string end;

};