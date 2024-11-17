#include "stdafx.h"
#include "Animator.h"

void Animator::AddEvent(const std::string& id, int frame, std::function<void()> action)
{
	std::pair<std::string, int> key(id, frame);
	auto find = events.find(key);
	if (find == events.end())
	{
		events.insert({ key, {id,frame } });
	}
	events[key].actions.push_back(action);
}

void Animator::Update(float dt)
{
	//���� �÷������� �ƴ϶�� ������Ʈ ��������
	if (!isPlaying)
		return;
	//���� �ð��� ��ŸŸ���� ����
	accumTime += dt;
	//���� �ð��� ������ ��ȯ ���ݺ��� ���ٸ� ������Ʈ ���� ����
	if (accumTime < frameDuration)
		return;	
	
	auto find = events.find({ currentClip->id, currentFrame });
	if (find != events.end())
	{
		auto& ev = *find;
		for (auto& func : ev.second.actions)
		{
			if (func)
				func();
		}
	}

	if (!isReverse)
	{
		//���� �������� ���� ���������� �ٲٰ�, �����ð��� 0���� �ʱ�ȭ
		currentFrame++;
		accumTime = 0.f;
		//���� ���� �������� ������ ������ �̶��
		if (currentFrame == totalFrame)
		{
			if (!playQueue.empty())
			{
				std::string clipId = playQueue.front();
				Play(clipId, false);
				playQueue.pop();
				return;
			}
			
			//���� Ÿ���� �˻��ؼ�, ������ �����ӿ��� ���߰ų�, ó�� ���������� ������
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = totalFrame - 1;
				break;
			case AnimationLoopTypes::Loop:
				currentFrame = 0;
				break;
			case AnimationLoopTypes::PingPong:
				currentFrame = totalFrame - 1;
				isReverse = true;
				break;
			}
		}
	}
	else
	{
		currentFrame--;
		accumTime = 0.f;
		if (currentFrame <= 0)
		{
			if (!playQueue.empty())
			{
				std::string clipId = playQueue.front();
				Play(clipId, false);
				playQueue.pop();
				return;
			}
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = 0;
				break;
			case AnimationLoopTypes::Loop:
				currentFrame = totalFrame - 1;
				break;
			case AnimationLoopTypes::PingPong:
				currentFrame = 0;
				isReverse = false;
				break;
			}
		}
	}
	//�������� �����Ѵ�.
	SetFrame(currentClip->frames[currentFrame]);


}

void Animator::Play(const std::string& clipId, bool clearQueue)
{
	Play(&ANI_CLIP_MGR.Get(clipId),clearQueue);
}

void Animator::Play(AnimationClip* clip, bool clearQueue)
{
	if (clearQueue)
	{
		while (!playQueue.empty())
			playQueue.pop();
	}

	//�÷��������� �ٲٱ�
	isPlaying = true;

	//���� Ŭ���� �Ű����� Ŭ������ �ٲٰ�
	currentClip = clip;
	//ù ���������� �ٲٰ�
	currentFrame = 0;
	//�� ������ ���� ���� Ŭ���� ������� �ٲ۴�
	totalFrame = currentClip->frames.size();

	//������ ��ȯ �ð��� �����ϰ�
	frameDuration = 1.f / currentClip->fps;

	//���� �ð��� 0���� �����ϰ�
	accumTime = 0.f;

	//�������� ���� Ŭ���� ���� ������(���� 0) ���� �����Ѵ�.
	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::PlayQueue(const std::string& clipId)
{
	playQueue.push(clipId);
}

void Animator::Stop()
{
	isPlaying = false;
}

void Animator::SetFrame(const AnimationFrame& frame)
{
	sprite->setTexture(TEXTURE_MGR.Get(frame.texId));
	sprite->setTextureRect(frame.texCoord);
}

void Animator::SetSpeed(float speed)
{
	if (speed < 0.f)
	{
		isReverse = true;
		currentFrame = totalFrame - 1;
	}
	else
		isReverse = false;

	frameDuration /= std::fabs(speed);
}
