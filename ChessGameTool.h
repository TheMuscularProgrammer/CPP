#pragma once
#include <iostream>
#include "ToolBase.h"

enum class toolType {
	checker=0,
	amazon=1,
	pawn=2, // ���� �� ��� ����� ���� �������
	steeple=3, // ���� �������
	bishop=4, // �� ��������
	knight=5, // ��� ����� ������� ���� ��� ����� ������ ���
	queen=6, // ���� �������� ��������
	king=7 // ��� ��� ��� ��� �����
};


class ChessGameTool : public ToolBase
{
protected:
	toolType m_tType;
public:
	// Ctor // Dtor
	ChessGameTool(int row, int col, belongToPlayer pl, GameBase* game, toolType tType);
	ChessGameTool(GameBase* game, toolType tType);
	//ChessGameTool();

	~ChessGameTool();
	void addToPosArr(Position* arr, int& size, int r, int c) const;

	virtual Position* getRisks(int& size)const;
	toolType getToolType() { return m_tType; }

private:
	ToolBase** getOpponentTools(int& size) const;
};

