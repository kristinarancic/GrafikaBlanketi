
// Kol2016View.cpp : implementation of the CKol2016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2016.h"
#endif

#include "Kol2016Doc.h"
#include "Kol2016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2016View

IMPLEMENT_DYNCREATE(CKol2016View, CView)

BEGIN_MESSAGE_MAP(CKol2016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2016View construction/destruction

CKol2016View::CKol2016View() noexcept
{
	// TODO: add construction code here
	felt2 = new DImage();
	felt2->Load(CString("res/felt2.jpg"));
	wood = new DImage();
	wood->Load(CString("res/wood.jpg"));


}

CKol2016View::~CKol2016View()
{
	delete felt2;
	delete wood;
}

BOOL CKol2016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2016View drawing

void CKol2016View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form;
	form.eM11 = 1;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = 1;
	form.eDx = dX;
	form.eDy = dY;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CKol2016View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void CKol2016View::DrawTransparent(CDC* pDC, DImage* image)
{
	int w = image->Width();
	int h = image->Height();

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();

	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* imageBmp = new CBitmap();
	CBitmap* maskBmp = new CBitmap();

	imageBmp->CreateCompatibleBitmap(srcDC, w, h);
	maskBmp->CreateBitmap(w, h, 1, 1, NULL);

	CBitmap* oldSrc = srcDC->SelectObject(imageBmp);
	CBitmap* oldDst = dstDC->SelectObject(maskBmp);

	CRect rect(0, 0, w, h);

	image->Draw(srcDC, rect, rect);

	COLORREF color = srcDC->GetPixel(0, 0);
	srcDC->SetBkColor(color);
	dstDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCCOPY);

	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCPAINT);

	delete imageBmp;
	delete maskBmp;

	srcDC->DeleteDC();
	dstDC->DeleteDC();

	delete srcDC;
	delete dstDC;
}

void CKol2016View::DrawBorder(CDC* pDC, CRect rect, int w)
{
	//wood->Draw(pDC, CRect(0, 0, wood->Width(), wood->Height()), rect);
	wood->Draw(pDC, rect, rect);
}

void CKol2016View::DrawTable(CDC* pDC, CRect rect)
{
	CRgn rgn;
	rgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	pDC->SelectObject(&rgn);
	int w = felt2->Width();
	int h = felt2->Height();
	int i = rect.left;
	while (i <= rect.right)
	{
		int j = rect.top;
		while (j <= rect.bottom)
		{
			felt2->Draw(pDC, CRect(0, 0, w, h), CRect(i, j, i + w, j + h));
			j+=h;
		}
		i+=w;
	}
	pDC->SelectClipRgn(NULL);
}

void CKol2016View::DrawHoles(CDC* pDC, CRect rect, int size)
{
	CBrush* brush = new CBrush(RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(brush);
	
	pDC->Ellipse(size/2, size/2, size/2 +size, size / 2 + size);//gore levo
	pDC->Ellipse(size / 2, rect.Height() - size - size / 2, size / 2 + size, rect.Height() - size / 2);//donje levo
	
	pDC->Ellipse(rect.Width() / 2 - size/2, size / 2, rect.Width() / 2 + size / 2, size / 2 + size);//gore srednja
	pDC->Ellipse(rect.Width() / 2 - size / 2, rect.Height() - size - size / 2, rect.Width() / 2 + size / 2, rect.Height() - size / 2);//dole srednja
	
	pDC->Ellipse(rect.Width() - size / 2 - size, size / 2, rect.Width() - size / 2, size / 2 + size);//gore desno
	pDC->Ellipse(rect.Width() - size / 2 - size, rect.Height() - size - size / 2, rect.Width() - size / 2, rect.Height() - size / 2);//dole desno

	pDC->SelectObject(oldBrush);
}

void CKol2016View::DrawStick(CDC* pDC, int w)
{
	CPen* pen = new CPen(PS_SOLID, 1, RGB(64, 32, 0));
	CPen* oldPen = pDC->SelectObject(pen);

	CBrush* brushOker = new CBrush(RGB(237, 181, 37));
	CBrush* brushBrown = new CBrush(RGB(64, 32, 0));
	CBrush* oldBrush = pDC->SelectObject(brushBrown);

	CPoint pts[4] = {CPoint(0,0), CPoint(w / 3, 0), CPoint(w / 3, w / 67), CPoint(0, w / 50)};

	pDC->Polygon(pts, 4);

	pDC->SelectObject(brushOker);
	CPoint pts1[4] = { CPoint(w / 3, 0), CPoint(w, 0), CPoint(w, w / 100), CPoint(w / 3, w / 67) };
	pDC->Polygon(pts1, 4);

	CPen* pen2 = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	pDC->SelectObject(pen2);

	pDC->MoveTo(0, 2);
	pDC->LineTo(w, 2);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CKol2016View::DrawBall(CDC* pDC, int w)
{
	CPen* pen = new CPen(PS_SOLID, 2, RGB(80, 0, 0));
	CBrush* brushRed = new CBrush(RGB(128, 0, 0));

	CPen* oldPen = pDC->SelectObject(pen);
	CBrush* oldBrush = pDC->SelectObject(brushRed);

	pDC->Ellipse(0, 0, w, w);

	CPen* pen2 = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	CBrush* brushWhite = new CBrush(RGB(255, 255, 255));

	pDC->SelectObject(pen2);
	pDC->SelectObject(brushWhite);

	pDC->Ellipse(w / 2 + 2, w / 2 - 2, w / 2 + 2 + w / 3, w / 2 - 2 + w / 3);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CKol2016View::DrawScene(CDC* pDC, CRect rect)
{
	XFORM oldForm, form;
	pDC->GetWorldTransform(&oldForm);

	int gm = pDC->SetGraphicsMode(GM_ADVANCED);

	DrawBorder(pDC, rect, 50);
	DrawTable(pDC, CRect(40, 40, rect.Width() - 40, rect.Height() - 40));
	DrawHoles(pDC, rect, 40);

	Translate(pDC, 850, 350);
	DrawBall(pDC, 30);

	Translate(pDC, 15, 15);
	Rotate(pDC, -3.14 / 8);
	Rotate(pDC, rotStick * 3.14 / 180);
	Translate(pDC, -15, -15);
	pDC->GetWorldTransform(&form);
	Translate(pDC, -50, 0);
	Translate(pDC, x, 0);
	Translate(pDC, -700, 10);
	DrawStick(pDC, 700);

	pDC->SetWorldTransform(&oldForm);
	pDC->SetGraphicsMode(gm);
}

void CKol2016View::OnDraw(CDC* pDC)
{
	CKol2016Doc* pDoc = GetDocument();
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

	//DrawBorder(memDC, rect, 50);
	//DrawTable(memDC, CRect(40, 40, rect.Width() - 40, rect.Height() - 40));
	DrawScene(memDC, rect);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	memDC->DeleteDC();
	delete memDC;
}


// CKol2016View printing

BOOL CKol2016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2016View diagnostics

#ifdef _DEBUG
void CKol2016View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2016Doc* CKol2016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2016Doc)));
	return (CKol2016Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2016View message handlers


BOOL CKol2016View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CKol2016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_RIGHT)
	{
		rotStick += 20;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		rotStick -= 20;
		Invalidate();
	}
	if (nChar == VK_UP)
	{
		
		x += 20;
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		x -= 20;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
