#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "hash_table.hpp"
#include "graph.hpp"
#include "siphash.hpp"
#include "list.hpp"
#include <string>
#include <iostream>
#include <functional>

namespace vishnyakov
{
  class CommandHandler
  {
  public:
    using CommandFunc = std::function< void(std::istream&, std::ostream&) >;

    CommandHandler();
    ~CommandHandler() = default;

    void add_graph(const std::string& name, const Graph& graph);
    bool has_graph(const std::string& name) const;
    Graph& get_graph(const std::string& name);
    const Graph& get_graph(const std::string& name) const;

    void execute(const std::string& command_line, std::ostream& out);

  private:
    HashTable< std::string, Graph, SipHash, std::equal_to< std::string > > graphs_;
    HashTable< std::string, CommandFunc, SipHash, std::equal_to< std::string > > commands_;

    void cmd_graphs(std::istream& in, std::ostream& out);
    void cmd_vertexes(std::istream& in, std::ostream& out);
    void cmd_outbound(std::istream& in, std::ostream& out);
    void cmd_inbound(std::istream& in, std::ostream& out);
    void cmd_bind(std::istream& in, std::ostream& out);
    void cmd_cut(std::istream& in, std::ostream& out);
    void cmd_create(std::istream& in, std::ostream& out);
    void cmd_merge(std::istream& in, std::ostream& out);
    void cmd_extract(std::istream& in, std::ostream& out);
  };
}

#endif

