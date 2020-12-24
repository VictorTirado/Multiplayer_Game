#include "Networks.h"

// TODO(you): World state replication lab session

#include "ReplicationManagerServer.h"

using namespace std;

void ReplicationManagerServer::create(uint32 networkId)
{
	commandList.push_back(new ReplicationCommand(ReplicationAction::Create, networkId));
}

void ReplicationManagerServer::update(uint32 networkId)
{
	commandList.push_back(new ReplicationCommand(ReplicationAction::Update, networkId));
}

void ReplicationManagerServer::destroy(uint32 networkId)
{
	commandList.push_back(new ReplicationCommand(ReplicationAction::Destroy, networkId));
}

void ReplicationManagerServer::notification(uint32 networkId, uint32 data)
{
	commandList.push_back(new ReplicationCommand(ReplicationAction::Notification, networkId, data));
}



void ReplicationManagerServer::write(OutputMemoryStream& packet)
{
	int size = commandList.size();
	packet << size;

	for (list<ReplicationCommand*>::iterator it = commandList.begin(); it != commandList.end(); ++it)
	{
		GameObject* go = App->modLinkingContext->getNetworkGameObject((*it)->networkId);

		switch ((*it)->action)
		{
		case ReplicationAction::None:
			break;
		case ReplicationAction::Create:
			if (go)
			{
				packet << (*it)->networkId;
				packet << (*it)->action;

				packet << go->position.x;
				packet << go->position.y;

				packet << go->angle;

				packet << go->sprite->order;

				packet << go->sprite->pivot.x;
				packet << go->sprite->pivot.y;

				packet << go->size.x;
				packet << go->size.y;

				packet << go->sprite->color.r;
				packet << go->sprite->color.g;
				packet << go->sprite->color.b;
				packet << go->sprite->color.a;

				packet << go->sprite->textureType;

				packet << go->tag;
			}
			else
			{
				packet << -1;
				packet << (*it)->action;
			}
			break;
		case ReplicationAction::Update:
			if (go)
			{
				packet << (*it)->networkId;
				packet << (*it)->action;

				packet << go->position.x;
				packet << go->position.y;

				packet << go->angle;
			}
			else
			{
				packet << -1;
				packet << (*it)->action;
			}
			break;
		case ReplicationAction::Destroy:

			packet << (*it)->networkId;
			packet << (*it)->action;

			break;
		case ReplicationAction::Notification:

			packet << (*it)->networkId;
			packet << (*it)->action;
			packet << (*it)->data;

			break;
		default:
			break;
		}

		delete* it;
		*it = nullptr;
	}

	commandList.clear();
}