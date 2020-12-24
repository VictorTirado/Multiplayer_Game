#pragma once

// TODO(you): World state replication lab session

class ModuleNetworkingClient;
class ReplicationManagerClient
{
public:
	void Read(const InputMemoryStream& packet, ModuleNetworkingClient* client);
};