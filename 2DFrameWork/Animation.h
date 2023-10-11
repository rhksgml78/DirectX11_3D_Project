#pragma once
class Animation
{
public:
	UINT					frameMax;
	UINT					boneMax;
	Matrix**				arrFrameBone;//	�����Ӱ���* ������ ( local Inverse * ani)
	float					tickPerSecond;
	string					file;


	Animation();
	~Animation();
	void LoadFile(string file);
	void SaveFile(string file);
};
enum class AnimationState
{
	STOP,//Pause
	LOOP,
	ONCE_LAST,
	ONCE_FIRST,
};

class Animations
{
	struct Animator
	{
		float frameWeight = 0.0f;
		UINT  currentFrame = 0;
		UINT  nextFrame = 1;
		UINT  animIdx = 0;
		AnimationState animState = AnimationState::STOP;
	};
	void AnimatorUpdate(Animator& Animator);
public:
	Animations();
	~Animations();
	void Update();

	//���� �ִϸ��̼ǿ��� ���� �ִϸ��̼����� �Ѿ �ִϸ�����
	Animator							currentAnimator;
	Animator							nextAnimator;

	int									animNum;
	int									currentAnim = 0;
	int									lastAnim = 0;
	int									playnum;

	bool								isChanging;
	float								blendtime;
	float								Changedtime;
	float								aniScale = 1.0f;
	vector<shared_ptr<Animation>>		playList;
	map<string, shared_ptr<Animation>>	animList;
	Matrix	GetFrameBone(int boneIndex);
	void	ChangeAnimation(AnimationState state, UINT idx, float blendtime = 0.5f);
	void	RenderDetail();
	float   GetPlayTime();// 0ó�� ~ 1 ��
	int		GetPlayNum() { return playnum; }
};