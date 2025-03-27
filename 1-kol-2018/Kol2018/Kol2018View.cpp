
// Kol2018View.cpp : implementation of the CKol2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2018.h"
#endif

#include "Kol2018Doc.h"
#include "Kol2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2018View

IMPLEMENT_DYNCREATE(CKol2018View, CView)

BEGIN_MESSAGE_MAP(CKol2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2018View construction/destruction

CKol2018View::CKol2018View() noexcept
{
	// TODO: add construction code here

	arm1 = new DImage();
	arm1->Load(CString("res/arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("res/arm2.png"));
	bager = new DImage();
	bager->Load(CString("res/bager.png"));
	pozadina = new DImage();
	pozadina->Load(CString("res/pozadina.png"));

	mf = GetEnhMetaFile(L"res/viljuska.emf");

	ENHMETAHEADER header;
	GetEnhMetaFileHeader(mf, sizeof(ENHMETAHEADER), &header);

	widthFajl = header.rclBounds.right - header.rclBounds.left;
	heightFajl = header.rclBounds.bottom - header.rclBounds.top;

}

CKol2018View::~CKol2018View()
{
	delete arm1;
	delete arm2;
	delete bager;
	delete pozadina;

	DeleteEnhMetaFile(mf);
}

BOOL CKol2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2018View drawing

void CKol2018View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CKol2018View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void CKol2018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form;
	form.eM11 = sX;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = sY;
	form.eDx = 0;
	form.eDy = 0;

	DWORD mode = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;

	pDC->ModifyWorldTransform(&form, mode);
}

void CKol2018View::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	int w = pozadina->Width();
	int h = pozadina->Height();

	int wr = rect.Width();
	int hr = rect.Height();

	XFORM form;
	pDC->GetWorldTransform(&form);

	//Translate(pDC, wr / 2, hr / 2);
	pozadina->Draw(pDC, CRect(0, 0, w, h), rect);
	//pozadina->Draw(pDC, CRect(0, 0, w, h), CRect(-w / 2, -rect.top - h / 2, w / 2, rect.bottom));

	pDC->SetWorldTransform(&form);
}

void CKol2018View::DrawTransparent(CDC* pDC, DImage* image)
{
	int w = image->Width();
	int h = image->Height();

	CRect rect(0, 0, w, h);

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* imageBmp = new CBitmap();
	imageBmp->CreateCompatibleBitmap(pDC, w, h);
	CBitmap* maskBmp = new CBitmap();
	maskBmp->CreateBitmap(w, h, 1, 1, NULL);

	CBitmap* oldSrc = srcDC->SelectObject(imageBmp);
	CBitmap* oldDst = dstDC->SelectObject(maskBmp);

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

void CKol2018View::DrawBody(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	Translate(pDC, rect.right - 350, rect.bottom -300);
	DrawTransparent(pDC, bager);
	//Translate(pDC, 0, 3 * bager->Height() / 4);
	Translate(pDC, 0, bager->Height() / 3);
}

void CKol2018View::DrawArm1(CDC* pDC)
{
	//Translate(pDC, 2, -arm1->Height());
	Translate(pDC, 2, -arm1->Height()+bager->Height()-55);
	Rotate(pDC, -PI / 2);
	Translate(pDC, 58, 61);
	Rotate(pDC, rotArm1 * PI / 180);
	Translate(pDC, -58, -61);
	DrawTransparent(pDC, arm1);
	Rotate(pDC, PI / 2);
}

void CKol2018View::DrawArm2(CDC* pDC)
{
	Translate(pDC, 98, -arm1->Width() + 80);
	Rotate(pDC, PI);
	Translate(pDC, 36, 40);
	Rotate(pDC, rotArm2 * PI / 180);
	Translate(pDC, -36, -40);

	DrawTransparent(pDC, arm2);
	Rotate(pDC, -PI);
}

void CKol2018View::DrawFork(CDC* pDC)
{
	CRect rect(0, 0, widthFajl, heightFajl);
	Translate(pDC, -270, -40);
	Scale(pDC, 2.5, 2.5);
	Rotate(pDC, PI / 2);
	Translate(pDC, -13, -18);

	Translate(pDC, 14, 20);
	Rotate(pDC, rotFork * PI / 180);
	Translate(pDC, -14, -20);
	pDC->PlayMetaFile(mf, rect);
	Rotate(pDC, -PI / 2);
}

void CKol2018View::DrawExcavator(CDC* pDC)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	int gm = pDC->SetGraphicsMode(GM_ADVANCED);

	Translate(pDC, x, 0);

	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);

	pDC->SetWorldTransform(&form);
	pDC->SetGraphicsMode(gm);
}

void CKol2018View::OnDraw(CDC* pDC)
{
	CKol2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC;
	memDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	CBitmap* oldBmp = memDC->SelectObject(&bmp);

	DrawBackground(memDC);
	DrawExcavator(memDC);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	memDC->SelectObject(oldBmp);

	memDC->DeleteDC();
	delete memDC;
}


// CKol2018View printing

BOOL CKol2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2018View diagnostics

#ifdef _DEBUG
void CKol2018View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2018Doc* CKol2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2018Doc)));
	return (CKol2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2018View message handlers


BOOL CKol2018View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CKol2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == '1')
	{
		rotArm1 += 10;
	}
	if (nChar == '2')
	{
		rotArm1 -= 10;
	}
	if (nChar == '3')
	{
		rotArm2 += 10;
	}
	if (nChar == '4')
	{
		rotArm2 -= 10;
	}
	if (nChar == '5')
	{
		rotFork += 10;
	}
	if (nChar == '6')
	{
		rotFork -= 10;
	}
	if (nChar == VK_RIGHT)
	{
		x += 10;
	}
	if (nChar == VK_LEFT)
	{
		x -= 10;
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
