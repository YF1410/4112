#include "RouteSearch.h"
#include"LibMath.h"
#include"Collision.h"
#include"ModelData.h"

using namespace MelLib;

//#ifdef _DEBUG
//std::vector<AStarNode*>AStarNode::pNodes;
//
//MelLib::AStarNode::AStarNode()
//{
//	pNodes.push_back(this);
//
//	//�p�x�ݒ�߂�ǂ�����{�b�N�X�ɂ��Ă�
//	nodeModel.Create(ModelData::Get(ShapeType3D::BOX), nullptr);
//	//nodeModel.SetScale(size)
//}
//
//MelLib::AStarNode::~AStarNode()
//{
//	for (int i = 0, size = pNodes.size(); i < size; i++) 
//	{
//		if (pNodes[i] == this) 
//		{
//			pNodes.erase(pNodes.begin() + i);
//			break;
//		}
//	}
//}
//
//void MelLib::AStarNode::Draw()
//{
//	nodeModel.Draw();
//}
//
//void MelLib::AStarNode::AllDraw()
//{
//	for(auto& p : pNodes)
//	{
//		p->Draw();
//	}
//}

//#endif // _DEBUG


#pragma region �ŒZ�o�H
//
//void RouteSearch::SetAStarNodePosition
//(
//	const Vector2& leftUpPos,
//	const Vector2& rightDownPos,
//	const UINT& nodeNumX,
//	const UINT& nodeNumY,
//	std::vector< std::vector<AStarNode>>& nodes,
//	const bool upPlus
//)
//{
//	nodes.clear();
//
//	//�}�X�̃T�C�Y�����߂�
//	Vector2 size = rightDownPos - leftUpPos;
//	size.x = abs(size.x);
//	size.y = abs(size.y);
//	if (nodeNumX >= 1) size.x /= nodeNumX - 1;
//	if (nodeNumY >= 1) size.y /= nodeNumY - 1;
//
//	//�m�[�h���T�C�Y
//	nodes.resize(nodeNumY, std::vector<AStarNode>(nodeNumX));
//
//	//�m�[�h�̍��W���Z�b�g���Ă���
//	Vector2 addPos = 0;
//	for (int y = 0; y < nodeNumY; y++)
//	{
//		for (int x = 0; x < nodeNumX; x++)
//		{
//			if (upPlus)addPos = Vector2(size.x * x, size.y * -y);
//			else addPos = Vector2(size.x * x, size.y * y);
//			nodes[y][x].position = leftUpPos + addPos;
//		}
//	}
//
//}
//
//void RouteSearch::SetAStarNodeHitObjectNodeFlag
//(
//	const std::vector<Vector2>& hitObjectsPos,
//	const std::vector<Vector2>& hitObjectsSize,
//	std::vector< std::vector<AStarNode>>& nodes
//)
//{
//	//�T�C�Y�����߂�
//	Vector2 nodeSize = 0;
//	nodeSize.x = abs(nodes[0][0].position.x - nodes[0][1].position.x);
//
//	float smallPos = 0.0f;
//	float bigPos = 0.0f;
//	if (nodes[0][0].position.y >= nodes[1][0].position.y)
//	{
//		bigPos = nodes[0][0].position.y;
//		smallPos = nodes[1][0].position.y;
//	}
//	else
//	{
//		bigPos = nodes[1][0].position.y;
//		smallPos = nodes[0][0].position.y;
//	}
//	nodeSize.y = abs(smallPos - bigPos);
//
//	//����
//	auto nodesXNum = nodes[0].size();
//	auto nodesYNum = nodes.size();
//	auto hitObjectNum = hitObjectsPos.size();
//
//	//�������Ă邩�m�F
//	for (int y = 0; y < nodesYNum; y++)
//	{
//		for (int x = 0; x < nodesXNum; x++)
//		{
//
//			for (int i = 0; i < hitObjectNum; i++)
//			{
//				nodes[y][x].hitObjectNode =
//					LibMath::RectAndRectCollision
//					(
//						nodes[y][x].position - nodeSize / 2,
//						nodeSize,
//						hitObjectsPos[i] - hitObjectsSize[i] / 2,
//						hitObjectsSize[i]
//					);
//
//				//�Ԃ����Ă���break
//				if (nodes[y][x].hitObjectNode)
//				{
//					break;
//				}
//
//			}
//		}
//	}
//}
//
//bool RouteSearch::GetAStarCalcResult
//(
//	const Vector2& startPos,
//	const Vector2& endPos,
//	std::vector< std::vector<AStarNode>>& nodes,
//	std::vector<Vector2>& routeVector
//)
//{
//
//	//��邱��
//	//�u���b�N�ɗאڂ��Ă�}�X����A�����u���b�N�ɗאڂ��Ă�}�X�ֈړ����Ȃ��悤�ɂ���
//
//
//	//���Z�b�g
//	for (auto& n1 : nodes)
//	{
//		for (auto& n2 : n1)
//		{
//			n2.closeFlag = false;
//			n2.openFlag = false;
//			n2.previousNode = nullptr;
//			n2.calcNum = UINT_MAX;
//			n2.closeIndex = INT_MAX;
//		}
//	}
//
//	//�X�^�[�g�n�_�Ɉ�ԋ߂��m�[�h�̋������i�[����ϐ�
//	float startMinDistance = FLT_MAX;
//	//�S�[���n�_�Ɉ�ԋ߂��m�[�h�̋������i�[����ϐ�
//	float endMinDistance = FLT_MAX;
//
//	auto nodeXArrayNum = nodes[0].size();
//	auto nodeYArrayNum = nodes.size();
//
//	//�X�^�[�g�̓Y����
//	int startNodeIndexX = 0;
//	int startNodeIndexY = 0;
//	bool trueStartNodeHitFlag = false;
//
//	//�m�[�h�̔z��̃S�[���n�_�̏ꏊ�������Y����
//	int endNodeIndexX = 0;
//	int endNodeIndexY = 0;
//	bool trueEndNodeHitFlag = false;
//
//	//�ꎞ�I��false�ɂ����Ƃ��ɖ߂�����
//	auto ReturnHitObjectNode = [&]()
//	{
//		if (trueStartNodeHitFlag)nodes[startNodeIndexY][startNodeIndexX].hitObjectNode = true;
//		if (trueEndNodeHitFlag)nodes[endNodeIndexY][endNodeIndexX].hitObjectNode = true;
//
//	};
//
//	for (int y = 0; y < nodeYArrayNum; y++)
//	{
//		for (int x = 0; x < nodeXArrayNum; x++)
//		{
//			float distance = 0.0f;
//			distance = LibMath::CalcDistance2D(nodes[y][x].position, startPos);
//
//			if (distance <= startMinDistance)
//			{
//				startMinDistance = distance;
//				startNodeIndexX = x;
//				startNodeIndexY = y;
//
//			}
//
//			distance = LibMath::CalcDistance2D(nodes[y][x].position, endPos);
//			if (distance <= endMinDistance)
//			{
//				endMinDistance = distance;
//				endNodeIndexX = x;
//				endNodeIndexY = y;
//
//
//			}
//
//			//�C���f�b�N�X���
//			nodes[y][x].indexX = x;
//			nodes[y][x].indexY = y;
//		}
//	}
//
//	if (nodes[startNodeIndexY][startNodeIndexX].hitObjectNode)
//	{
//		//�ꎞ�I��false
//		nodes[startNodeIndexY][startNodeIndexX].hitObjectNode = false;
//		trueStartNodeHitFlag = true;
//	}
//
//	if (nodes[endNodeIndexY][endNodeIndexX].hitObjectNode)
//	{
//		//�ꎞ�I��false
//		nodes[endNodeIndexY][endNodeIndexX].hitObjectNode = false;
//		trueEndNodeHitFlag = true;
//	}
//
//	//�S�[���̃m�[�h�܂ł̋��������߂郉���_��
//	auto CalcNodeDistance = []
//	(
//		const int startX,
//		const int startY,
//		const int endX,
//		const int endY
//		)
//	{
//		//X�̍�
//		int startXToEndXDiff = abs(startX - endX);
//		//Y�̍�
//		int startYToEndYDiff = abs(startY - endY);
//
//		//X�̍���Y�̍��̂ǂ��炪�傫�������߂�B�΂߈ړ�����̏ꍇ�A�傫���ق����ŒZ����
//		if (startXToEndXDiff >= startYToEndYDiff)return startXToEndXDiff;
//		return startYToEndYDiff;
//
//	};
//
//	//�X�^�[�g�̃m�[�h�̃C���f�b�N�X����
//	int startToEndDis = CalcNodeDistance(startNodeIndexX, startNodeIndexY, endNodeIndexX, endNodeIndexY);
//	nodes[startNodeIndexY][startNodeIndexX].calcNum = startToEndDis + nodes[startNodeIndexY][startNodeIndexX].cost;
//
//	std::vector<AStarNode*>openNodes(1, &nodes[startNodeIndexY][startNodeIndexX]);
//	std::vector<AStarNode*>closeNodes;
//
//
//	AStarNode* endNode;
//
//
//	//�X�e�b�v�̌v�Z�͎���̃m�[�h���ׂ�Ƃ��ɂ��?
//
//	while (1)
//	{
//		//�T���I�����ǂ���
//		bool checkEnd = false;
//
//		//�S�[���ɂ��ǂ蒅���Ȃ��ꍇ
//		if (openNodes.size() == 0)
//		{
//			ReturnHitObjectNode();
//			return false;
//		}
//
//		//���ёւ�
//		std::sort
//		(
//			openNodes.begin(),
//			openNodes.end(),
//			[]
//		(
//			const AStarNode* node1,
//			const AStarNode* node2
//			)
//		{
//			int num1 = node1->calcNum;
//			int num2 = node2->calcNum;
//
//			return num1 > num2;
//		});
//
//		//calcNum����ԏ��Ȃ�����擾
//		AStarNode* mainNode = nullptr;
//		int minCalcNum = openNodes[openNodes.size() - 1]->calcNum;
//		float minDis = FLT_MAX;
//		int mainNodeOpenIndex = 0;
//		for (int i = openNodes.size() - 1;; i--)
//		{
//			float dis = LibMath::CalcDistance2D(openNodes[i]->position, nodes[startNodeIndexY][startNodeIndexX].position);
//			if (minDis > dis)
//			{
//				minDis = dis;
//				mainNode = openNodes[i];
//				mainNodeOpenIndex = i;
//			}
//			if (openNodes[i]->calcNum != minCalcNum || i == 0)break;
//		}
//
//
//
//		//�I�[�v���ɒǉ�����m�[�h���i�[����z��
//		std::vector<AStarNode*>openPushBackNode;
//
//		for (int y = -1; y < 2; y++)
//		{
//			int indexY = mainNode->indexY + y;
//
//			//Y�͈͊O�w��h�~
//			if (indexY <= -1 || indexY >= nodeYArrayNum)continue;
//
//			for (int x = -1; x < 2; x++)
//			{
//				int indexX = mainNode->indexX + x;
//
//				//X�͈͊O�w��h�~
//				if (indexX <= -1 || indexX >= nodeXArrayNum)continue;
//
//				//������h�~
//				if (indexX == mainNode->indexX && indexY == mainNode->indexY)continue;
//
//				AStarNode* checkNode = &nodes[indexY][indexX];
//
//				//�I�u�W�F�N�g�ɏd�Ȃ��Ă邩�ǂ���
//				if (checkNode->hitObjectNode)
//				{
//					checkNode->closeFlag = true;
//					continue;
//				}
//
//
//				//calcNum = �X�e�b�v�� + �S�[���܂ł̋��� + �R�X�g
//				int calcNum = mainNode->stepNum + 1
//					+ CalcNodeDistance(indexX, indexY, endNodeIndexX, endNodeIndexY)
//					+ checkNode->cost;
//
//				//�����𖞂���������
//				if (checkNode->openFlag)
//				{
//					if (calcNum < checkNode->calcNum)
//					{
//						checkNode->calcNum = calcNum;
//						checkNode->previousNode = mainNode;
//						checkNode->stepNum = mainNode->stepNum + 1;
//					}
//
//					continue;
//				}
//
//				//�����𖞂�������Aclose����open�Ɉړ�&���
//				if (checkNode->closeFlag)
//				{
//					if (calcNum < checkNode->calcNum)
//					{
//						//close����폜
//						closeNodes.erase(closeNodes.begin() + checkNode->closeIndex);
//						checkNode->closeFlag = false;
//						checkNode->closeIndex = INT_MAX;
//
//						//open�ɒǉ�
//						openNodes.push_back(checkNode);
//						checkNode->openFlag = true;
//
//						checkNode->calcNum = calcNum;
//						checkNode->previousNode = mainNode;
//						checkNode->stepNum = mainNode->stepNum + 1;
//					}
//					continue;
//				}
//
//
//				checkNode->calcNum = calcNum;
//				checkNode->previousNode = mainNode;
//				checkNode->stepNum = mainNode->stepNum + 1;
//
//				if (checkNode->indexX == endNodeIndexX && checkNode->indexY == endNodeIndexY)
//				{
//					checkEnd = true;
//					endNode = checkNode;
//				}
//
//
//				openPushBackNode.push_back(checkNode);
//				checkNode->openFlag = true;
//			}
//		}
//
//		if (checkEnd)break;
//
//		closeNodes.push_back(mainNode);
//		mainNode->closeFlag = true;
//		mainNode->closeIndex = closeNodes.size() - 1;
//
//		//���������������
//		openNodes[openNodes.size() - 1]->openFlag = false;
//		openNodes.erase(openNodes.begin() + mainNodeOpenIndex);
//
//
//		for (auto& n : openPushBackNode)
//		{
//			openNodes.push_back(n);
//		}
//		openPushBackNode.clear();
//	}
//
//	AStarNode* currentNode = endNode;
//	std::vector<Vector2>routeNodeVectors;
//	while (1)
//	{
//		if (!currentNode)break;
//		routeNodeVectors.push_back(currentNode->position);
//		currentNode = currentNode->previousNode;
//	}
//	std::reverse(routeNodeVectors.begin(), routeNodeVectors.end());
//
//	//�x�N�g�������߂�
//	routeVector.clear();
//	auto routeVectorSize = routeNodeVectors.size() - 1;
//	routeVector.resize(routeVectorSize);
//	for (int i = 0; i < routeVectorSize; i++)
//	{
//		routeVector[i] = Vector2::Normalize(routeNodeVectors[i + 1] - routeNodeVectors[i]);
//	}
//
//	ReturnHitObjectNode();
//	return true;
//}


#pragma endregion

#pragma region �V�o�H�T��

void MelLib::RouteSearch::SetNodePosition
(
	const Vector3& leftDownNearPos,
	const Vector3& rightUpFarPos,
	const Value3<UINT>& nodeNum,
	std::vector<std::vector<std::vector<AStarNode>>>& nodes
)
{
	// �x�e��ɂ�邱��
	// �͈͂��ǂ�����0�ɂ����Ƃ��ɂ�size��0����Ȃ��Ă����ƃZ�b�g����
	// �܂��́A�͈̓Z�b�g���Ă�1����������W��0�ɂ���


	nodes.clear();
	nodes.resize(nodeNum.v3, std::vector< std::vector<AStarNode>>(nodeNum.v2, std::vector<AStarNode>(nodeNum.v1)));


	for (int z = 0; z < nodes.size(); z++)
	{

		for (int y = 0; y < nodes[z].size(); y++)
		{

			for (int x = 0; x < nodes[z][y].size(); x++)
			{
				//�T�C�Y�v�Z
				nodes[z][y][x].size = (rightUpFarPos - leftDownNearPos).Abs();
				nodes[z][y][x].size.x /= nodeNum.v1;
				nodes[z][y][x].size.y /= nodeNum.v2;
				nodes[z][y][x].size.z /= nodeNum.v3;

				//���W�v�Z
				nodes[z][y][x].position = leftDownNearPos + nodes[z][y][x].size * Vector3(x, y, z);
				if (nodeNum.v1 == 1)nodes[z][y][x].position.x = 0;
				if (nodeNum.v2 == 1)nodes[z][y][x].position.y = 0;
				if (nodeNum.v3 == 1)nodes[z][y][x].position.z = 0;
			}
		}
	}
}

void MelLib::RouteSearch::SetHitObjectFlag
(
	const std::vector<BoxData>& hitObjectsBoxData,
	std::vector<std::vector<std::vector<AStarNode>>>& nodes
)
{
	for (auto& nZ : nodes)
	{
		for (auto& nY : nZ)
		{
			for (auto& nX : nY)
			{
				for (int i = 0; i < hitObjectsBoxData.size(); i++)
				{
					BoxData nodeBoxData;
					nodeBoxData.SetPosition(nX.position);
					nodeBoxData.SetSize(nX.size);

					if (Collision::BoxAndBox(nodeBoxData, nullptr, hitObjectsBoxData[i], nullptr))
					{
						nX.hitObjectNode = true;
						break;
					}
				}
			}
		}
	}

}

void MelLib::RouteSearch::SetCost(const std::vector<BoxData>& hitObjectsBoxData, const std::vector<UINT>& costs, std::vector<std::vector<std::vector<AStarNode>>>& nodes)
{
	if (hitObjectsBoxData.size() != costs.size())return;
	for (auto& nZ : nodes)
	{
		for (auto& nY : nZ)
		{
			for (auto& nX : nY)
			{
				for (int i = 0; i < hitObjectsBoxData.size(); i++)
				{
					BoxData nodeBoxData;
					nodeBoxData.SetPosition(nX.position);
					nodeBoxData.SetSize(nX.size);

					if (Collision::BoxAndBox(nodeBoxData, nullptr, hitObjectsBoxData[i], nullptr))
					{
						nX.cost += costs[i];
					}
				}
			}
		}
	}
}

bool MelLib::RouteSearch::CheckStraightMove(const Vector3& startPos, const Vector3& endPos, std::vector<std::vector<std::vector<AStarNode>>>& nodes, const float straightDis, Vector3& startToEnd)
{
	bool straightMove = true;
	Segment3DData segment;
	segment.SetPosition(MelLib::Value2<MelLib::Vector3>(startPos, endPos));

	for (const auto& nZ : nodes)
	{
		for (const auto& nY : nZ)
		{
			for (const auto& nX : nY)
			{
				BoxData box;
				box.SetPosition(nX.position);
				box.SetSize(nX.size);

				if(nX.position == MelLib::Vector3(0,0,8))
				{
					int z = 0;
				}

				// ������Ȃ������玟��
				if (!Collision::BoxAndSegment3D(box, segment))continue;

				// �i�s�s�\�m�[�h�������璼�i�s�Ȃ̂Ŋm�F�I��
				if (nX.hitObjectNode)
				{
					straightMove = false;
					break;
				}

				// �ʂ̃m�[�h�Ɗm�F
				for (const auto& nZ2 : nodes)
				{
					for (const auto& nY2 : nZ2)
					{
						for (const auto& nX2 : nY2)
						{
							// �q�b�g�I�u�W�F�N�g����Ȃ������玟��
							if (!nX2.hitObjectNode)continue;

							if ((nX.position - nX2.position).Length() < straightDis)
							{
								straightMove = false;
								break;
							}
						}
						if (!straightMove)break;
					}
					if (!straightMove)break;
				}
				if (!straightMove)break;
			}
			if (!straightMove)break;
		}
		if (!straightMove)break;
	}

	// �\�������璼�i���ď����I��
	if (straightMove)
	{
		startToEnd = (endPos - startPos).Normalize();
		return true;
	}
}

bool MelLib::RouteSearch::CalcRoute
(
	const Vector3& startPos,
	const Vector3& endPos,
	std::vector<std::vector<std::vector<AStarNode>>>& nodes,
	std::vector<Vector3>* routeVector,
	std::vector<Vector3>* routeNodePos
)
{

	//�X�^�[�g�n�_�Ɉ�ԋ߂��m�[�h�̋������i�[����ϐ�
	float startMinDistance = FLT_MAX;
	//�S�[���n�_�Ɉ�ԋ߂��m�[�h�̋������i�[����ϐ�
	float endMinDistance = FLT_MAX;

	auto nodeXArrayNum = nodes[0][0].size();
	auto nodeYArrayNum = nodes[0].size();
	auto nodeZArrayNum = nodes.size();

	//�X�^�[�g�̓Y����
	int startNodeIndexX = 0;
	int startNodeIndexY = 0;
	int startNodeIndexZ = 0;
	bool trueStartNodeHitFlag = false;

	//�m�[�h�̔z��̃S�[���n�_�̏ꏊ�������Y����
	int endNodeIndexX = 0;
	int endNodeIndexY = 0;
	int endNodeIndexZ = 0;
	bool trueEndNodeHitFlag = false;


	struct AStarCalcData
	{
		//�v�Z����(�X�e�b�v + ���� + �R�X�g)
		UINT calcNum = UINT_MAX;

		//�z��̃C���f�b�N�X
		int indexX = INT_MAX;
		int indexY = INT_MAX;
		int indexZ = INT_MAX;

		//close�z��̃C���f�b�N�X
		int closeIndex = INT_MAX;

		//�X�^�[�g����̋���
		int stepNum = 0;

		AStarNode* pNode = nullptr;
		//�O�̃m�[�h�̃f�[�^
		AStarCalcData* previousNode = nullptr;

		bool openFlag = false;
		bool closeFlag = false;

	};
	std::vector<std::vector<std::vector<AStarCalcData>>>calcData
	(nodeZArrayNum, std::vector<std::vector<AStarCalcData>>(nodeYArrayNum, std::vector<AStarCalcData>(nodeXArrayNum)));

	//�m�[�h�Z�b�g
	for (int z = 0; z < calcData.size(); z++)
	{

		for (int y = 0; y < calcData[z].size(); y++)
		{
			for (int x = 0; x < calcData[z][y].size(); x++)
			{
				calcData[z][y][x].pNode = &nodes[z][y][x];
			}
		}
	}


	//�ꎞ�I��false�ɂ����Ƃ��ɖ߂�����
	auto ReturnHitObjectNode = [&]()
	{
		if (trueStartNodeHitFlag)nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].hitObjectNode = true;
		if (trueEndNodeHitFlag)nodes[endNodeIndexZ][endNodeIndexY][endNodeIndexX].hitObjectNode = true;

	};

	for (int z = 0; z < nodeZArrayNum; z++)
	{
		for (int y = 0; y < nodeYArrayNum; y++)
		{
			for (int x = 0; x < nodeXArrayNum; x++)
			{
				float distance = 0.0f;
				distance = LibMath::CalcDistance3D(nodes[z][y][x].position, startPos);

				if (distance <= startMinDistance)
				{
					startMinDistance = distance;
					startNodeIndexX = x;
					startNodeIndexY = y;
					startNodeIndexZ = z;

				}

				distance = LibMath::CalcDistance3D(nodes[z][y][x].position, endPos);
				if (distance <= endMinDistance)
				{
					endMinDistance = distance;
					endNodeIndexX = x;
					endNodeIndexY = y;
					endNodeIndexZ = z;


				}

				//�C���f�b�N�X���
				calcData[z][y][x].indexX = x;
				calcData[z][y][x].indexY = y;
				calcData[z][y][x].indexZ = z;
			}
		}
	}

	if (nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].hitObjectNode)
	{
		//�ꎞ�I��false
		nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].hitObjectNode = false;
		trueStartNodeHitFlag = true;
	}

	if (nodes[endNodeIndexZ][endNodeIndexY][endNodeIndexX].hitObjectNode)
	{
		//�ꎞ�I��false
		nodes[endNodeIndexZ][endNodeIndexY][endNodeIndexX].hitObjectNode = false;
		trueEndNodeHitFlag = true;
	}

	//�S�[���̃m�[�h�܂ł̋��������߂郉���_��
	auto CalcNodeDistance = []
	(
		const int startX,
		const int startY,
		const int startZ,
		const int endX,
		const int endY,
		const int endZ
		)
	{
		//X�̍�
		int startXToEndXDiff = abs(startX - endX);
		//Y�̍�
		int startYToEndYDiff = abs(startY - endY);
		int startZToEndZDiff = abs(startZ - endZ);

		//X�̍���Y�̍��̂ǂ��炪�傫�������߂�B�΂߈ړ�����̏ꍇ�A�傫���ق����ŒZ����
		if (startXToEndXDiff > startYToEndYDiff)
		{
			if (startXToEndXDiff > startZToEndZDiff)return startXToEndXDiff;
			else return startZToEndZDiff;
		}
		else
		{
			if (startYToEndYDiff > startZToEndZDiff)return startYToEndYDiff;
			else return startZToEndZDiff;
		}


	};

	//�X�^�[�g�̃m�[�h�̃C���f�b�N�X����
	int startToEndDis = CalcNodeDistance(startNodeIndexX, startNodeIndexY, startNodeIndexZ, endNodeIndexX, endNodeIndexY, endNodeIndexZ);
	calcData[startNodeIndexZ][startNodeIndexY][startNodeIndexX].calcNum =
		startToEndDis + nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].cost;

	std::vector<AStarCalcData*>openNodes(1, &calcData[startNodeIndexZ][startNodeIndexY][startNodeIndexX]);
	std::vector<AStarCalcData*>closeNodes;


	AStarCalcData* endNode = nullptr;


	//�X�e�b�v�̌v�Z�͎���̃m�[�h���ׂ�Ƃ��ɂ��?


	while (1)
	{
		//�T���I�����ǂ���
		bool checkEnd = false;

		//�S�[���ɂ��ǂ蒅���Ȃ��ꍇ
		if (openNodes.size() == 0)
		{
			ReturnHitObjectNode();
			return false;
		}

		//���ёւ�
		std::sort
		(
			openNodes.begin(),
			openNodes.end(),
			[]
		(
			const AStarCalcData* node1,
			const AStarCalcData* node2
			)
		{
			int num1 = node1->calcNum;
			int num2 = node2->calcNum;

			return num1 > num2;
		});

		//calcNum����ԏ��Ȃ�����擾
		AStarCalcData* mainNode = nullptr;
		int minCalcNum = openNodes[openNodes.size() - 1]->calcNum;
		float minDis = FLT_MAX;
		int mainNodeOpenIndex = 0;
		for (int i = openNodes.size() - 1;; i--)
		{
			//float dis = LibMath::CalcDistance3D(openNodes[i]->pNode->position, nodes[startNodeIndexZ][startNodeIndexY][startNodeIndexX].position);
			float dis = LibMath::CalcDistance3D(openNodes[i]->pNode->position, nodes[endNodeIndexZ][endNodeIndexY][endNodeIndexX].position);
			if (minDis > dis)
			{
				minDis = dis;
				mainNode = openNodes[i];
				mainNodeOpenIndex = i;
			}
			if (openNodes[i]->calcNum != minCalcNum || i == 0)break;
		}

		//�I�[�v���ɒǉ�����m�[�h���i�[����z��
		std::vector<AStarCalcData*>openPushBackNode;

		for (int z = -1; z < 2; z++)
		{
			int indexZ = mainNode->indexZ + z;
			if (indexZ <= -1 || indexZ >= nodeZArrayNum)continue;

			for (int y = -1; y < 2; y++)
			{
				int indexY = mainNode->indexY + y;

				//Y�͈͊O�w��h�~
				if (indexY <= -1 || indexY >= nodeYArrayNum)continue;

				for (int x = -1; x < 2; x++)
				{
					int indexX = mainNode->indexX + x;

					//X�͈͊O�w��h�~
					if (indexX <= -1 || indexX >= nodeXArrayNum)continue;

					//������h�~
					if (indexX == mainNode->indexX && indexY == mainNode->indexY && indexZ == mainNode->indexZ)continue;

					AStarCalcData* checkNode = &calcData[indexZ][indexY][indexX];

					//�I�u�W�F�N�g�ɏd�Ȃ��Ă��玟�̃m�[�h�ց@
					if (checkNode->pNode->hitObjectNode)
					{
						checkNode->closeFlag = true;
						continue;
					}


					//calcNum = �X�e�b�v�� + �S�[���܂ł̋��� + �R�X�g
					int calcNum = mainNode->stepNum + 1
						+ CalcNodeDistance(indexX, indexY, indexZ, endNodeIndexX, endNodeIndexY, endNodeIndexZ)
						+ checkNode->pNode->cost;

					//�����𖞂���������
					if (checkNode->openFlag)
					{
						if (calcNum < checkNode->calcNum)
						{
							checkNode->calcNum = calcNum;
							checkNode->previousNode = mainNode;
							checkNode->stepNum = mainNode->stepNum + 1;
						}

						continue;
					}

					//�����𖞂�������Aclose����open�Ɉړ�&���
					if (checkNode->closeFlag)
					{
						if (calcNum < checkNode->calcNum)
						{
							//close����폜
							closeNodes.erase(closeNodes.begin() + checkNode->closeIndex);
							checkNode->closeFlag = false;
							checkNode->closeIndex = INT_MAX;

							//open�ɒǉ�
							openNodes.push_back(checkNode);
							checkNode->openFlag = true;

							checkNode->calcNum = calcNum;
							checkNode->previousNode = mainNode;
							checkNode->stepNum = mainNode->stepNum + 1;
						}
						continue;
					}


					checkNode->calcNum = calcNum;
					checkNode->previousNode = mainNode;
					checkNode->stepNum = mainNode->stepNum + 1;

					if (checkNode->indexX == endNodeIndexX
						&& checkNode->indexY == endNodeIndexY
						&& checkNode->indexZ == endNodeIndexZ)
					{
						checkEnd = true;
						endNode = checkNode;
					}


					openPushBackNode.push_back(checkNode);
					checkNode->openFlag = true;
				}
			}
		}

		if (checkEnd)break;

		closeNodes.push_back(mainNode);
		mainNode->closeFlag = true;
		mainNode->closeIndex = closeNodes.size() - 1;

		//���������������
		openNodes[openNodes.size() - 1]->openFlag = false;
		openNodes.erase(openNodes.begin() + mainNodeOpenIndex);


		for (auto& n : openPushBackNode)
		{
			openNodes.push_back(n);
		}
		openPushBackNode.clear();
	}

	AStarCalcData* currentNode = endNode;
	std::vector<Vector3>routeNodePositions;
	while (1)
	{
		if (!currentNode)break;

		routeNodePositions.push_back(currentNode->pNode->position);

		currentNode = currentNode->previousNode;
	}
	std::reverse(routeNodePositions.begin(), routeNodePositions.end());

	if (routeNodePos)
	{
		routeNodePos->clear();
		*routeNodePos = routeNodePositions;
	}

	//�x�N�g�������߂�
	if (routeVector)
	{
		std::vector<Vector3>& refRouteVector = *routeVector;
		refRouteVector.clear();
		size_t routeVectorSize = routeNodePositions.size() - 1;
		refRouteVector.resize(routeVectorSize);
		for (size_t i = 0; i < routeVectorSize; i++)
		{
			refRouteVector[i] = Vector3::Normalize(routeNodePositions[i + 1] - routeNodePositions[i]);
		}
	}

	ReturnHitObjectNode();
	return true;
}

#pragma endregion

