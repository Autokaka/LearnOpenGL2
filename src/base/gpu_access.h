/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/23.
 */

#pragma once

#include <string_view>
#include <vector>

#include "macros.h"

template <typename GLObject>
class GPUAccess {
 public:
  class Command final : public std::enable_shared_from_this<Command> {
   public:
    using SharedCommand = std::shared_ptr<Command>;
    using Callback = std::function<void(const GLObject&)>;

    MAKE_SHARED_CONSTRUCTOR(Command, Create);

    const std::string_view name;
    const Callback callback;

   private:
    explicit Command(const std::string_view& name, const Callback& callback) : name(name), callback(callback) {}

    DISALLOW_COPY_ASSIGN_AND_MOVE(Command);
  };
  using SharedCommand = typename Command::SharedCommand;
  using CommandList = std::vector<SharedCommand>;

  virtual ~GPUAccess() = default;
  virtual GLObject MakeGLObject() = 0;

 protected:
  GLObject gl_object_;
  virtual void SubmitCommands() {
    for (const auto& command : GetCommandList()) {
      command->callback(gl_object_);
    }
    ClearCommands();
  }
  const CommandList& GetCommandList() const { return command_list_; }
  void AppendCommand(const std::string_view& name, const typename Command::Callback& callback) {
    command_list_.emplace_back(Command::Create(name, callback));
  }
  void ReplaceLastCommand(const std::string_view& name, const typename Command::Callback& callback) {
    for (auto&& it = command_list_.rbegin(); it != command_list_.rend(); ++it) {
      if (name == it->get()->name) {
        *it = Command::Create(name, callback);
        return;
      }
    }
    AppendCommand(name, callback);
  }
  void ClearCommands() { command_list_.clear(); }

 private:
  CommandList command_list_{};
};
