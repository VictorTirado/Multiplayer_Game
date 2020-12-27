#include "Networks.h"
// TODO(you): World state replication lab session

#include "ReplicationManagerClient.h"

void ReplicationManagerClient::Read(const InputMemoryStream& packet, ModuleNetworkingClient* client)
{
	int size;
	packet >> size;

	for (int i = 0; i < size; ++i)
	{
		uint32 networkId; packet >> networkId;
		ReplicationAction action; packet >> action;

		switch (action)
		{
		case ReplicationAction::None:
			break;
		case ReplicationAction::Create:
		{
			if (networkId != -1)
			{
				GameObject* newGo = Instantiate();

				if (newGo)
				{
					newGo->sprite = App->modRender->addSprite(newGo);
					packet >> newGo->position.x;
					packet >> newGo->position.y;
					newGo->finalPos = newGo->initialPos = newGo->position;

					packet >> newGo->angle;
					newGo->finalAngle = newGo->initialAngle = newGo->angle;

					packet >> newGo->sprite->order;

					packet >> newGo->sprite->pivot.x;
					packet >> newGo->sprite->pivot.y;

					packet >> newGo->size.x;
					packet >> newGo->size.y;

					packet >> newGo->sprite->color.r;
					packet >> newGo->sprite->color.g;
					packet >> newGo->sprite->color.b;
					packet >> newGo->sprite->color.a;

					


					packet >> newGo->sprite->textureType;

					switch (newGo->sprite->textureType)
					{
					case TextureType::Spacecraft1:
						newGo->sprite->texture = App->modResources->spacecraft1;
						break;
					case TextureType::Spacecraft2:
						newGo->sprite->texture = App->modResources->spacecraft2;
						break;
					case TextureType::Spacecraft3:
						newGo->sprite->texture = App->modResources->spacecraft3;
						break;
					case TextureType::Laser:
						newGo->sprite->texture = App->modResources->laser;
						newGo->clip = App->modResources->audioClipLaser;
						newGo->playsound = true;
						//packet >> newGo->clip->bitsPerSample;
						//packet >> newGo->clip->channelCount;
						//packet >> newGo->clip->sampleCount;
						//////packet >> newGo->clip->samples;
						//packet >> newGo->clip->samplingRate;
						break;
					case TextureType::Explosion:
						newGo->sprite->texture = App->modResources->explosion1;
						newGo->animation = App->modRender->addAnimation(newGo);
						newGo->animation->clip = App->modResources->explosionClip;
						newGo->clip = App->modResources->audioClipExplosion;
						break;
					default:
						break;
					}

					packet >> newGo->tag;

					App->modLinkingContext->registerNetworkGameObjectWithNetworkId(newGo, networkId);
				}
			}
		}
		break;
		case ReplicationAction::Update:
		{
			if (networkId != -1)
			{
				GameObject* go = App->modLinkingContext->getNetworkGameObject(networkId);

				if (go)
				{
					packet >> go->position.x;
					packet >> go->position.y;
					if (go->playsound == true)
					{
						App->modSound->playAudioClip(go->clip);
						go->playsound = false;
					}
					
					packet >> go->angle;
				}
			}
		}
		break;
		case ReplicationAction::Destroy:
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject(networkId);

			if (go)
			{
				App->modLinkingContext->unregisterNetworkGameObject(go);

				Destroy(go);
			}
		}
		break;
		case ReplicationAction::Notification:
		{
			uint32 data = 0u;
			packet >> data;

			client->SetInputDataFront(data);
		}
		break;
		default:
			break;
		}
	}
}