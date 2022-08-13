#ifndef SERVER__SERVER_POOL_H
#define SERVER__SERVER_POOL_H
#include <unordered_map>
#include <string>
#include <server/server.h>
#include <fmt/chrono.h>

namespace GTServer {
    class server_pool {
    public:
        server_pool() {
            fmt::print("initializing server_pool\n");
        }
        ~server_pool() = default;

        bool initialize_enet() {
            if (enet_initialize() != 0) {
                fmt::print("failed to initialize enet service\n");
                return false;
            }
            return true;
        }
        void deinitialize_enet() {
            enet_deinitialize();
        }

        std::unordered_map<uint8_t, server*> get_servers() {
            return m_servers;
        }
        server* start_instance() {
            uint8_t instanceId = static_cast<uint8_t>(m_servers.size());
            auto svr = new server(++instanceId, m_address, m_port++, m_max_peers);
            m_servers.insert_or_assign(instanceId, svr);
            return svr;
        }
        void stop_instance(const uint8_t& instanceId) {
            fmt::print("shutting down instanceId: {} - {}\n", instanceId, std::chrono::system_clock::now());
            if(auto it = m_servers.find(instanceId); it != m_servers.end()) {
                it->second->stop();
                m_servers.erase(instanceId);
                return;
            }
            fmt::print("failed to shutdown instanceId: {}\n", instanceId);
            return;
        }

        void set_user_id(const int& uid) {
            user_id = uid;
        }
        [[nodiscard]] int get_user_id(bool increase = true) {
            return increase ? ++user_id : user_id;
        }
    private:
        std::string m_address{ "0.0.0.0" };
        uint16_t m_port{ 17091 };
        size_t m_max_peers{ 0xFF };

        int user_id{ 0 };
        std::unordered_map<uint8_t, server*> m_servers{};
    };
}
#endif // SERVER__SERVER_POOL_H