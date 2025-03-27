
// Kol2019View.h : interface of the CKol2019View class
//
#include "DImage.h"

#pragma once

#define PI 3.14


class CKol2019View : public CView
{
protected: // create from serialization only
	CKol2019View() noexcept;
	DECLARE_DYNCREATE(CKol2019View)

// Attributes
public:
	CKol2019Doc* GetDocument() const;

protected:
	DImage* background;
	DImage* arm1;
	DImage* arm2;
	DImage* body1;
	DImage* leg1;
	DImage* leg2;
	float rotarm1 = 0;
	float rotarm2 = 0;
	float rotleg1 = 0;
	float rotleg2 = 0;
	float rotbody = 0;
	//double rot2arm1 = 0;

// Operations
public:

// Overrides
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply=false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply =false);
	void DrawTransparent(CDC* pDC, DImage* image);
	void DrawTransformer(CDC* pDC);
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody1(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2019View();
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

#ifndef _DEBUG  // debug version in Kol2019View.cpp
inline CKol2019Doc* CKol2019View::GetDocument() const
   { return reinterpret_cast<CKol2019Doc*>(m_pDocument); }
#endif

