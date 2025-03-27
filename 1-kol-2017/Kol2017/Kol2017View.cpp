
// Kol2017View.cpp : implementation of the CKol2017View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2017.h"
#endif

#include "Kol2017Doc.h"
#include "Kol2017View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2017View

IMPLEMENT_DYNCREATE(CKol2017View, CView)

BEGIN_MESSAGE_MAP(CKol2017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol2017View construction/destruction

CKol2017View::CKol2017View() noexcept
{
	// TODO: add construction code here

	body = new DImage();
	body->Load(CString("res/Body.png"));
	leg1 = new DImage();
	leg1->Load(CString("res/Leg1.png"));
	leg2 = new DImage();
	leg2->Load(CString("res/Leg2.png"));
	leg3 = new DImage();
	leg3->Load(CString("res/Leg3.png"));
	back2 = new DImage();
	back2->Load(CString("res/Back2.jpg"));

	alfa = new float[8];
	alfa[0] = 0 * PI / 180;
	alfa[1] = 10 * PI / 180;
	alfa[2] = 20 * PI / 180;
	alfa[3] = 10 * PI / 180;
	alfa[4] = 0 * PI / 180;
	alfa[5] = -10 * PI / 180;
	alfa[6] = -20 * PI / 180;
	alfa[7] = -10 * PI / 180;
}

CKol2017View::~CKol2017View()
{
	if (body)
		delete body;
	body = NULL;

	if (leg1)
		delete leg1;
	leg1 = NULL;

	if (leg2)
		delete leg2;
	leg2 = NULL;

	if (leg3)
		delete leg3;
	leg3 = NULL;

	if (back2)
		delete back2;
	back2 = NULL;
}

BOOL CKol2017View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2017View drawing

void CKol2017View::LoadIdentity(CDC* pDC)
{
	XFORM form;
	form.eM11 = 1.0;
	form.eM12 = 0.0;
	form.eM21 = 0.0;
	form.eM22 = 1.0;
	form.eDx = 0.0;
	form.eDy = 0.0;

	pDC->SetWorldTransform(&form);
}

void CKol2017View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CKol2017View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void CKol2017View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form;
	form.eM11 = sX;
	form.eM12 = 0.0;
	form.eM21 = 0.0;
	form.eM22 = sY;
	form.eDx = 0.0;
	form.eDy = 0.0;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CKol2017View::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, dx, dy);
	Translate(pDC, 28, 28);
	Rotate(pDC, alpha);
	Translate(pDC, -28, -28);

	DrawTransparent(pDC, leg1);

	Translate(pDC, 0, leg1->Height());
	Translate(pDC, 0, -28 * 2);

	Translate(pDC, 28, 28);
	Rotate(pDC, -alpha);
	Translate(pDC, -28, -28);

	DrawTransparent(pDC, leg2);

	Translate(pDC, -leg3->Width() / 4, leg2->Height());
	Translate(pDC, 0, -29);

	DrawTransparent(pDC, leg3);

	pDC->SetWorldTransform(&oldForm);
}

void CKol2017View::DrawTransparent(CDC* pDC, DImage* image)
{
	int w = image->Width();
	int h = image->Height();
	CRect rect(0, 0, w, h);

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* imageBmp = new CBitmap();
	imageBmp->CreateCompatibleBitmap(pDC, w, h);
	CBitmap* maskBmp = new CBitmap();
	maskBmp->CreateBitmap(w, h, 1, 1, NULL);

	CBitmap* oldSrc = srcDC->SelectObject(imageBmp);
	CBitmap* oldDst = dstDC->SelectObject(maskBmp);

	image->Draw(srcDC, rect, rect);// ovde se u imageBmp kopira image, i sve je to u srcDC

	COLORREF bkColor = srcDC->GetPixel(0, 0);
	srcDC->SetBkColor(bkColor);
	dstDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCCOPY);//imamo masku u dstDC

	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->SetTextColor(RGB(255, 255, 255));

	srcDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCPAINT);// ovde je zavrsena slika, figura postavljena na odgovarajucu pozadinu kojas e vec nalazila u pDC

	delete imageBmp;
	delete maskBmp;

	srcDC->DeleteDC();
	dstDC->DeleteDC();
	delete srcDC;
	delete dstDC;
}

void CKol2017View::DrawScene(CDC* pDC, CRect rect)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	int gm = pDC->SetGraphicsMode(GM_ADVANCED);

	//pDC->SetBkColor(RGB(255, 255, 255));
	//CBrush* bk = new CBrush(RGB(255, 255, 255));

	back2->Draw(pDC, CRect(0, 0, back2->Width(), back2->Height()), rect);

	Scale(pDC, scale, scale);

	//pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 100 + x, 100);//pocetak tela

	DrawTransparent(pDC, body);

	DrawLeg(pDC, alfa[(i)%8], 52, 168);//zadnja noga
	DrawLeg(pDC, -alfa[(i) % 8], 52, 168);//zadnja noga

	DrawLeg(pDC, alfa[(i +3) % 8], 270, 168);//prednja noga
	DrawLeg(pDC, -alfa[(i + 3) % 8], 270, 168);//prednja noga

	pDC->SetWorldTransform(&oldForm);
	pDC->SetGraphicsMode(gm);
}

void CKol2017View::OnDraw(CDC* pDC)
{
	CKol2017Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(&rect);

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* oldBmp = pMemDC->SelectObject(&bmp);

	DrawScene(pMemDC, rect);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pMemDC, 0, 0, SRCCOPY);

	pMemDC->SelectObject(&oldBmp);

	pMemDC->DeleteDC();
}


// CKol2017View printing

BOOL CKol2017View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2017View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2017View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2017View diagnostics

#ifdef _DEBUG
void CKol2017View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2017View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2017Doc* CKol2017View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2017Doc)));
	return (CKol2017Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2017View message handlers


void CKol2017View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == VK_RIGHT)
	{
		i = (i + 1) % 8;
		x += 20;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		i = (i + 8 - 1) % 8;
		x -= 20;
		Invalidate();
	}
	if (nChar == VK_UP)
	{
		scale += 0.1;
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		scale -= 0.1;
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		scale -= 0.1;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol2017View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}
