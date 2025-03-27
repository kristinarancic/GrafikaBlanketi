
// Kol2013View.cpp : implementation of the CKol2013View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2013.h"
#endif

#include "Kol2013Doc.h"
#include "Kol2013View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2013View

IMPLEMENT_DYNCREATE(CKol2013View, CView)

BEGIN_MESSAGE_MAP(CKol2013View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2013View construction/destruction

CKol2013View::CKol2013View() noexcept
{
	// TODO: add construction code here

	bark = new DImage;
	bark->Load(CString("res/bark.jpg"));
	sky = new DImage;
	sky->Load(CString("res/sky.jpg"));
}

CKol2013View::~CKol2013View()
{
	delete bark;
	delete sky;
}

BOOL CKol2013View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2013View drawing

void CKol2013View::Translate(CDC* pDC, double x, double y, bool rightMultiply)
{
	XFORM form;
	form.eM11 = 1;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = 1;
	form.eDx = x;
	form.eDy = y;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CKol2013View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form;
	form.eM11 = cos(angle);
	form.eM12 = sin(angle);
	form.eM21 = -sin(angle);
	form.eM22 = cos(angle);
	form.eDx = 0;
	form.eDy = 0;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CKol2013View::DrawBlade(CDC* pDC, int size)
{
	CPen* pen = new CPen(PS_SOLID, 1, RGB(200, 200, 200));
	CBrush* brushLight = new CBrush(RGB(200, 200, 200));
	CBrush* brushDark = new CBrush(RGB(150, 150, 150));

	CPen* oldPen = pDC->SelectObject(pen);
	CBrush* oldBrush = pDC->SelectObject(brushLight);

	CPoint ptsLight[3] = { CPoint(0,0), CPoint(size, -size), CPoint(4 * size, 0) };
	pDC->Polygon(ptsLight, 3);

	CPoint ptsDark[3] = { CPoint(0,0), CPoint(4 * size, 0), CPoint(size, size) };
	pDC->SelectObject(brushDark);
	pDC->Polygon(ptsDark, 3);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	CFont font;
	COLORREF bkColor = pDC->SetBkMode(TRANSPARENT);
	COLORREF textColor = pDC->SetTextColor(RGB(255, 255, 255));
	font.CreateFont(0.7 * size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Monotype Corsiva"));

	CFont* oldFont = pDC->SelectObject(&font);

	pDC->TextOut(size-1, -0.7 * size / 2-1, CString("Shuricane"), 9);
	pDC->SetTextColor(RGB(40, 40, 40));

	pDC->TextOut(size, -0.7 * size / 2, CString("Shuricane"), 9);

	pDC->SetBkColor(bkColor);
	pDC->SetTextColor(textColor);

	pDC->SelectObject(&oldFont);
}

void CKol2013View::DrawStar(CDC* pDC, int size)
{
	DrawBlade(pDC, size);

	Rotate(pDC, 3.14 / 2);
	DrawBlade(pDC, size);

	Rotate(pDC, 3.14 / 2);
	DrawBlade(pDC, size);

	Rotate(pDC, 3.14 / 2);
	DrawBlade(pDC, size);

	Rotate(pDC, 3.14 / 2);
}

void CKol2013View::DrawScene(CDC* pDC, CRect rect)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	int gm = pDC->SetGraphicsMode(GM_ADVANCED);

	sky->Draw(pDC, CRect(0, 0, sky->Width(), sky->Height()), rect);

	bark->Draw(pDC, CRect(0, 0, bark->Width(), bark->Height()), CRect(rect.Width()- bark->Width(), 0, rect.Width(), rect.Height()));

	Translate(pDC, 100, 200);
	Translate(pDC, x, 0);
	Rotate(pDC, rot * 3.14 / 180);
	DrawStar(pDC, starRadius);

	pDC->SetWorldTransform(&form);
	pDC->SetGraphicsMode(gm);
}

void CKol2013View::OnDraw(CDC* pDC)
{
	CKol2013Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	CBitmap* oldBmp = memDC->SelectObject(&bmp);

	DrawScene(memDC, rect);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	memDC->DeleteDC();
	delete memDC;
}


// CKol2013View printing

BOOL CKol2013View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2013View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2013View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2013View diagnostics

#ifdef _DEBUG
void CKol2013View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2013View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2013Doc* CKol2013View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2013Doc)));
	return (CKol2013Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2013View message handlers


BOOL CKol2013View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CKol2013View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CRect rect;
	GetClientRect(&rect);

	if (nChar == 'W')
	{
		if (x <= rect.Width() - bark->Width())
		{
			x += 20;
			rot += 10;
		}
		
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
