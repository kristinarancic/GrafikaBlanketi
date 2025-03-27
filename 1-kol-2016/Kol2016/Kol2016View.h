
// Kol2016View.h : interface of the CKol2016View class
//

#pragma once
#include "DImage.h"

class CKol2016View : public CView
{
protected: // create from serialization only
	CKol2016View() noexcept;
	DECLARE_DYNCREATE(CKol2016View)

// Attributes
public:
	CKol2016Doc* GetDocument() const;
	DImage* felt2;
	DImage* wood;
	float rotStick = 0;
	int x = 0;
	int t = 0;

// Operations
public:

// Overrides
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void DrawTransparent(CDC* pDC, DImage* image);
	void DrawBorder(CDC* pDC, CRect rect, int w);
	void DrawTable(CDC* pDC, CRect rect);
	void DrawHoles(CDC* pDC, CRect rect, int size);
	void DrawStick(CDC* pDC, int w);
	void DrawBall(CDC* pDC, int w);
	void DrawScene(CDC* pDC, CRect rect);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2016View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Kol2016View.cpp
inline CKol2016Doc* CKol2016View::GetDocument() const
   { return reinterpret_cast<CKol2016Doc*>(m_pDocument); }
#endif

