/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/23.
 */

#pragma once

#include <string_view>
#include <vector>

#include "base/macros.h"
#include "scoped_gl_object.h"

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
    explicit Command(std::string_view name, const Callback& callback)
        : name(name), callback(callback) {}

    DISALLOW_COPY_ASSIGN_AND_MOVE(Command);
  };
  using SharedCommand = typename Command::SharedCommand;
  using CommandList = std::vector<SharedCommand>;

  virtual ~GPUAccess() = default;
  virtual GLObject CreateGLObject() = 0;
  virtual void SubmitCommands(const GLObject& gl_object) final {
    for (const auto& command : GetCommandList()) {
      command->callback(gl_object);
    }
    ClearCommands();
  }

 protected:
  const CommandList& GetCommandList() const { return command_list_; }
  void AppendCommand(std::string_view name,
                     const typename Command::Callback& callback) {
    command_list_.emplace_back(Command::Create(name, callback));
  }
  void ReplaceLastCommand(std::string_view name,
                          const typename Command::Callback& callback) {
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
