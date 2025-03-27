#include "DImage.h"
// Kol2017View.h : interface of the CKol2017View class
//
#define PI 3.14151

#pragma once


class CKol2017View : public CView
{
protected: // create from serialization only
	CKol2017View() noexcept;
	DECLARE_DYNCREATE(CKol2017View)

// Attributes
public:
	CKol2017Doc* GetDocument() const;

protected:
	DImage* body;
	DImage* leg1;
	DImage* leg2;
	DImage* leg3;
	DImage* back2;
	//double beta=0;
	int x = 0;
	int i = 0;
	float* alfa;
	float scale = 1;
	int xMin = 0;
	int yMin = 0;

// Operations
public:

// Overrides
public:
	void LoadIdentity(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);
	void DrawTransparent(CDC* pDC, DImage* image);
	void DrawScene(CDC* pDC, CRect rect);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2017View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in Kol2017View.cpp
inline CKol2017Doc* CKol2017View::GetDocument() const
   { return reinterpret_cast<CKol2017Doc*>(m_pDocument); }
#endif

