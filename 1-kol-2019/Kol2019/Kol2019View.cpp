
// Kol2019View.cpp : implementation of the CKol2019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2019.h"
#endif

#include "Kol2019Doc.h"
#include "Kol2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2019View

IMPLEMENT_DYNCREATE(CKol2019View, CView)

BEGIN_MESSAGE_MAP(CKol2019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2019View construction/destruction

CKol2019View::CKol2019View() noexcept
{
	// TODO: add construction code here

	background = new DImage();
	background->Load(CString("res/background.jpg"));
	body1 = new DImage();
	body1->Load(CString("res/body1.png"));
	arm1 = new DImage();
	arm1->Load(CString("res/arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("res/arm2.png"));
	leg1 = new DImage();
	leg1->Load(CString("res/leg1.png"));
	leg2 = new DImage();
	leg2->Load(CString("res/leg2.png"));


}

CKol2019View::~CKol2019View()
{
	delete background;
	delete body1;
	delete arm1;
	delete arm2;
	delete leg1;
	delete leg2;
}

BOOL CKol2019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2019View drawing

void CKol2019View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form;
	form.eM11 = 1.0;
	form.eM12 = 0.0;
	form.eM21 = 0.0;
	form.eM22 = 1.0;
	form.eDx = dX;
	form.eDy = dY;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CKol2019View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void CKol2019View::DrawTransparent(CDC* pDC, DImage* image)
{
	int w = image->Width();
	int h = image->Height();

	CRect rect(0, 0, w, h);

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();

	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* imageBmp = new CBitmap();
	CBitmap* maskBmp = new CBitmap();

	imageBmp->CreateCompatibleBitmap(pDC, w, h);
	maskBmp->CreateBitmap(w, h, 1, 1, NULL);

	CBitmap* oldSrc = srcDC->SelectObject(imageBmp);
	CBitmap* oldDst = dstDC->SelectObject(maskBmp);

	image->Draw(srcDC, rect, rect);

	COLORREF colorbk = srcDC->GetPixel(0, 0);
	srcDC->SetBkColor(colorbk);
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

void CKol2019View::DrawTransformer(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	int gm = pDC->SetGraphicsMode(GM_ADVANCED);
	
	//DrawArm1(pDC);

	//DrawLeg1(pDC);

	//DrawBody1(pDC);

	Translate(pDC, 200, 200);
	DrawLeg1(pDC);

	Translate(pDC, leg1->Width()-54, -5);
	DrawBody1(pDC);

	Translate(pDC, body1->Width()-150, 55);
	DrawArm1(pDC);
	
	pDC->SetWorldTransform(&oldForm);
	pDC->SetGraphicsMode(gm);
}

void CKol2019View::DrawBackground(CDC* pDC, CRect rc)
{
	background->Draw(pDC, CRect(0, 0, background->Width(), background->Height()), rc);
}

void CKol2019View::DrawArm1(CDC* pDC)
{
	XFORM form;

	Translate(pDC, 34, 31);
	Rotate(pDC, rotarm1*PI/180);
	Translate(pDC, -34, -31);

	Translate(pDC, 210, 102);
	Rotate(pDC, rotarm1*PI/180);
	Translate(pDC, -210, -102);

	pDC->GetWorldTransform(&form);

	Translate(pDC, 188, 40);
	DrawArm2(pDC);
	Translate(pDC, -188, -40);

	pDC->SetWorldTransform(&form);

	DrawTransparent(pDC, arm1);
}

void CKol2019View::DrawArm2(CDC* pDC)
{
	Translate(pDC, 23, 61);
	Rotate(pDC, -PI);
	Rotate(pDC, rotarm2 * PI / 180);
	Translate(pDC, -23, -61);

	DrawTransparent(pDC, arm2);
}

void CKol2019View::DrawLeg1(CDC* pDC)
{
	XFORM form;

	Translate(pDC, 30, 125);
	Rotate(pDC, rotleg1 * PI / 180);
	Translate(pDC, -30, -125);

	Translate(pDC, 237, 125);
	Rotate(pDC, rotleg1 * PI / 180);
	Translate(pDC, -237, -125);

	pDC->GetWorldTransform(&form);

	Translate(pDC, -6, 66);
	DrawLeg2(pDC);
	Translate(pDC, 6, -66);

	pDC->SetWorldTransform(&form);

	DrawTransparent(pDC, leg1);
}

void CKol2019View::DrawLeg2(CDC* pDC)
{
	Translate(pDC, 35, 60);
	Rotate(pDC, rotleg2 * PI / 180);
	Translate(pDC, -35, -60);

	DrawTransparent(pDC, leg2);
}

void CKol2019View::DrawBody1(CDC* pDC)
{
	Translate(pDC, 26, 133);
	Rotate(pDC, rotbody * PI / 180);
	Translate(pDC, -26, -133);

	DrawTransparent(pDC, body1);
}

void CKol2019View::OnDraw(CDC* pDC)
{
	CKol2019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CRect rect;
	GetClientRect(&rect);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	CBitmap* oldBmp = memDC->SelectObject(&bmp);

	DrawBackground(memDC, rect);

	DrawTransformer(memDC);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	memDC->SelectObject(oldBmp);
	memDC->DeleteDC();
	delete memDC;
}


// CKol2019View printing

BOOL CKol2019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2019View diagnostics

#ifdef _DEBUG
void CKol2019View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2019Doc* CKol2019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2019Doc)));
	return (CKol2019Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2019View message handlers


BOOL CKol2019View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CKol2019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == 'Q')
	{
		rotarm1 += 20;
		rotleg1 +=20;
		Invalidate();
	}
	if (nChar == 'A')
	{
		rotarm1 -= 20;
		rotleg1 -=20;
		Invalidate();
	}
	if (nChar == 'T')
	{
		rotarm2 += 20;
		Invalidate();
	}
	if (nChar == 'G')
	{
		rotarm2 -= 20;
		Invalidate();
	}
	if (nChar == 'R')
	{
		rotleg2 += 20;
		Invalidate();
	}
	if (nChar == 'F')
	{
		rotleg2 -= 20;
		Invalidate();
	}
	if (nChar == 'W')
	{
		rotbody += 20;
		Invalidate();
	}
	if (nChar == 'S')
	{
		rotbody -= 20;
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		rotleg1 += 20;
		rotleg2 += 20;
		rotarm1 += 20;
		rotarm2 += 20;
		rotbody += 20;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
